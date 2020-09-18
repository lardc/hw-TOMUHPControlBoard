// Header
#include "Measurement.h"
// Includes
#include "LowLevel.h"
#include "Board.h"
#include "Global.h"
#include "DataTable.h"
#include "SysConfig.h"
#include "InitConfig.h"
#include "Interrupts.h"
#include "Delay.h"

// Macro
//
#define ABS(a)	(((a) < 0) ? -(a) : (a))
//
#define MEASURE_POINTS_NUMBER		20		// Количество точек при единичном измерении
//
#define I_MAX_START_INDEX			75		// Начальный адрес в массиве тока для определения максимального значения
#define I_MAX_STOP_INDEX			85		// Конечный адрес в массиве тока для определения максимального значения
//
#define ITTERATIONS_OF_AVERAGING	3		// Количество иттераций усреднения

// Variables
volatile uint16_t LOGIC_OutputPulseRaw[PULSE_ARR_MAX_LENGTH];
Int16U MEASURE_TurnDelayResultBuffer[TIME_ARR_MAX_LENGTH];
Int16U MEASURE_TurnOnResultBuffer[TIME_ARR_MAX_LENGTH];

// Forward functions
uint16_t MEASURE_UrefToDAC(uint16_t Voltage);
void MEASURE_AveragingData(Int16U *Array, Int16U *MeanValue, Int16U AllowedSpread);
void MEASURE_FineTuneTdelTon(uint16_t* TurnDelay, uint16_t* TurnOn);

// Functions
//
uint16_t MEASURE_UrefToDAC(uint16_t Voltage)
{
	uint32_t result = (uint32_t)Voltage * DAC_RESOLUTION / DAC_REF_MV;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

void MEASURE_SetUref10(uint16_t Voltage)
{
	uint16_t Data = MEASURE_UrefToDAC(Voltage) | DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_DAC, RISE_Edge);
}
//---------------------

void MEASURE_SetUref90(uint16_t Voltage)
{
	uint16_t Data = MEASURE_UrefToDAC(Voltage) & (~DAC_CHANNEL_B);
	LL_WriteDACx(Data, GPIO_CS_DAC, RISE_Edge);
}
//---------------------

void MEASURE_ConvertRawArray(volatile uint16_t* RawArray, volatile uint16_t* OutputArray, uint16_t DataLength)
{
	uint16_t i, Imax = 0;
	float tmp = 0;

	float Offset = (float)((int16_t)DataTable[REG_I_DUT_OFFSET]);
	float K = (float)DataTable[REG_I_DUT_GAIN] / 1000;

	float ShuntRes = (float)DataTable[REG_DUT_CURRENT_SHUNT] / 10;

	float P0 = (float)((int16_t)DataTable[REG_P0_I_DUT]);
	float P1 = (float)DataTable[REG_P1_I_DUT] / 1000;
	float P2 = (float)((int16_t)DataTable[REG_P2_I_DUT]) / 1e6;

	for (i = 0; i < DataLength; ++i)
	{
		tmp = ((float)RawArray[i] - Offset) * ADC_REF_MV / ADC_RESOLUTION * K;
		tmp = tmp / ShuntRes;
		tmp = tmp * tmp * P2 + tmp * P1 + P0;
		OutputArray[i] = (tmp > 0) ? (uint16_t)tmp : 0;

		// Определение максимума тока
		if((i >= I_MAX_START_INDEX) && (i < I_MAX_STOP_INDEX))
			Imax += OutputArray[i];
	}

	DataTable[REG_MEAS_CURRENT_VALUE] = Imax / (I_MAX_STOP_INDEX - I_MAX_START_INDEX);
}
//---------------------

bool MEASURE_CheckAnodeCurrent()
{
	uint16_t AnodeCurrent = 0, AnodeCurrentRaw = 0;
	float AlowedError = 0;

	// Запуск процесса оцифровки тока
	DMA_ChannelReload(DMA_ADC_DUT_I_CHANNEL, MEASURE_POINTS_NUMBER);
	DMA_ChannelEnable(DMA_ADC_DUT_I_CHANNEL, true);
	TIM_Start(TIM6);

	DELAY_US(7);

	DMA_ChannelReload(DMA_ADC_DUT_I_CHANNEL, PULSE_ARR_MAX_LENGTH);
	DMA_ChannelEnable(DMA_ADC_DUT_I_CHANNEL, true);

	// Усреднение результата измерения
	for(int i = 0; i < MEASURE_POINTS_NUMBER; i++)
		AnodeCurrentRaw += LOGIC_OutputPulseRaw[i];
	AnodeCurrentRaw = (float)AnodeCurrentRaw / MEASURE_POINTS_NUMBER;

	MEASURE_ConvertRawArray(&AnodeCurrentRaw, &AnodeCurrent, 1);
	AlowedError = (1 - (float)AnodeCurrent / CachedMeasurementSettings.AnodeCurrent) * 100;

	if(AlowedError <= DataTable[REG_ID_THRESHOLD])
		return true;
	else
		return false;
}
//---------------------

void MEASURE_TurnOnMeasurement()
{
	uint32_t DataRaw;
	uint16_t TurnOn, TurnDelay;

	DataRaw = LL_HSTimers_Read();

	TurnDelay = ((DataRaw >> 12) & 0x0FF0) | ((DataRaw >> 28) & 0x000F);
	TurnDelay = TurnDelay * COUNTER_CLOCK_PERIOD_NS;

	if(TurnDelay < DataTable[REG_MEAS_TIME_LOW])
		TurnDelay = 0;

	TurnOn = (DataRaw & 0x00FF) | ((DataRaw >> 16) & 0x0F00);
	TurnOn = TurnOn * COUNTER_CLOCK_PERIOD_NS;

	if(TurnOn < DataTable[REG_MEAS_TIME_LOW])
		TurnOn = 0;

	if(TurnDelay && TurnOn)
		MEASURE_FineTuneTdelTon(&TurnDelay, &TurnOn);

	MEASURE_TurnDelayResultBuffer[CONTROL_AverageCounter] = TurnDelay;
	MEASURE_TurnOnResultBuffer[CONTROL_AverageCounter] = TurnOn;

	// Сохранение результата в endpoint
	CONTROL_Values_TurnDelay[CONTROL_AverageCounter] = TurnDelay;
	CONTROL_Values_TurnOn[CONTROL_AverageCounter] = TurnOn;
}
//-----------------------------------------------

void MEASURE_FineTuneTdelTon(uint16_t* TurnDelay, uint16_t* TurnOn)
{
	float Tdel_P2, Tdel_P1, Ton_P2, Ton_P1;
	int16_t Tdel_P0, Ton_P0;
	uint16_t T;

	switch(DataTable[REG_ANODE_VOLTAGE])
	{
		case TOU_600V:
		{
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_600V_P2]) / 1000000;
			Tdel_P1 = (float)DataTable[REG_T_DEL_600V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_600V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_600V_P2]) / 1000000;
			Ton_P1 = (float)DataTable[REG_T_ON_600V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_600V_P0];
		}
		break;

		case TOU_1000V:
		{
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_1000V_P2]) / 1000000;
			Tdel_P1 = (float)DataTable[REG_T_DEL_1000V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_1000V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_1000V_P2]) / 1000000;
			Ton_P1 = (float)DataTable[REG_T_ON_1000V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_1000V_P0];
		}
		break;

		case TOU_1500V:
		{
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_1500V_P2]) / 1000000;
			Tdel_P1 = (float)DataTable[REG_T_DEL_1500V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_1500V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_1500V_P2]) / 1000000;
			Ton_P1 = (float)DataTable[REG_T_ON_1500V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_1500V_P0];
		}
		break;

		default:
		{
			Tdel_P2 = 0;
			Tdel_P1 = 0;
			Tdel_P0 = 0;

			Ton_P2 = 0;
			Ton_P1 = 0;
			Ton_P0 = 0;
		}
			break;
	}


	T = *TurnDelay;
	*TurnDelay = T * T * Tdel_P2 + T * Tdel_P1 + Tdel_P0;

	T = *TurnOn;
	*TurnOn = T * T * Ton_P2 + T * Ton_P1 + Ton_P0;
}
//-----------------------------------------------


void MEASURE_TurnOnAveragingProcess()
{
	Int16U TonAverage = 0, TonDelayAverage = 0;

	for(int i = 0; i < ITTERATIONS_OF_AVERAGING; i++)
	{
		MEASURE_AveragingData(&MEASURE_TurnDelayResultBuffer[0], &TonDelayAverage, DataTable[REG_AVERAGE_ALLOWED_SPREAD]);
		MEASURE_AveragingData(&MEASURE_TurnOnResultBuffer[0], &TonAverage, DataTable[REG_AVERAGE_ALLOWED_SPREAD]);
	}

	DataTable[REG_MEAS_TIME_DELAY] = TonDelayAverage;
	DataTable[REG_MEAS_TIME_ON] = TonAverage;
}
//-----------------------------------------------

void MEASURE_AveragingData(Int16U *Array, Int16U *MeanValue, Int16U AllowedSpread)
{
	float Result = 0;
	Int16U AverageCounter = 0;

	if(*MeanValue == 0)
	{
		for(int i = 0; i < DataTable[REG_AVERAGE_NUM]; i++)
			Result += *(Array + i);

		*MeanValue = (Int16U)(Result / DataTable[REG_AVERAGE_NUM]);
	}
	else
	{
		for(int i = 0; i < DataTable[REG_AVERAGE_NUM]; i++)
		{
			if(((*MeanValue + AllowedSpread) >= *(Array + i)) && ((*MeanValue - AllowedSpread) <= *(Array + i)))
			{
				Result += *(Array + i);
				AverageCounter++;
			}
		}

		*MeanValue = (Int16U)(Result / AverageCounter);
	}
}
//-----------------------------------------------

void MEASURE_AnodeCurrentTune(AnodeVoltageEnum AnodeVoltage, float *AnodeCurrent)
{
	float P2, P1, P0, I;

	switch(AnodeVoltage)
	{
		case TOU_600V:
			P2 = ((float) ((int16_t)DataTable[REG_I_DUT_600V_P2])) / 1000000;
			P1 = ((float) DataTable[REG_I_DUT_600V_P1]) / 1000;
			P0 = (int16_t) DataTable[REG_I_DUT_600V_P0];
			break;

		case TOU_1000V:
			P2 = ((float) ((int16_t)DataTable[REG_I_DUT_1000V_P2])) / 1000000;
			P1 = ((float) DataTable[REG_I_DUT_1000V_P1]) / 1000;
			P0 = (int16_t) DataTable[REG_I_DUT_1000V_P0];
			break;

		case TOU_1500V:
			P2 = ((float) ((int16_t)DataTable[REG_I_DUT_1500V_P2])) / 1000000;
			P1 = ((float) DataTable[REG_I_DUT_1500V_P1]) / 1000;
			P0 = (int16_t) DataTable[REG_I_DUT_1500V_P0];
			break;

		default:
			return;
	}

	I = *AnodeCurrent;

	I = I * I * P2 + I * P1 + P0;

	*AnodeCurrent = I / 10;
}
//-----------------------------------------------

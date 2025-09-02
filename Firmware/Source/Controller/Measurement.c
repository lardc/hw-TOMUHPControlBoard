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
#include "FirCoefficients.h"

// Definitions
#define MEASURE_POINTS_NUMBER		20		// Количество точек при единичном измерении
#define I_MAX_AVERAGE_POINTS		10		// Количество точек усреднения амплитуды тока

// Variables
volatile uint16_t LOGIC_OutputPulseRaw[PULSE_ARR_MAX_LENGTH];
Int16U MEASURE_TurnDelayResultBuffer[TIME_ARR_MAX_LENGTH];
Int16U MEASURE_TurnOnResultBuffer[TIME_ARR_MAX_LENGTH];

// Forward functions
uint16_t MEASURE_UrefToDAC(uint16_t Voltage);
Int16U MEASURE_AverageData(pInt16U Data, Int16U DataCount);
void MEASURE_FineTuneTdelTon(uint16_t* TurnDelay, uint16_t* TurnOn);

// Functions
//
uint16_t MEASURE_UrefToDAC(uint16_t Voltage)
{
	float Uref = DataTable[REG_OVERRIDE_DAC_REF] ? DataTable[REG_OVERRIDE_DAC_REF] : DAC_REF_MV;
	uint32_t result = (uint32_t)Voltage * DAC_RESOLUTION / Uref;
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
	uint16_t i, Imax = 0, ImaxArrayIndex = 0;
	float tmp = 0;
	float Uref = DataTable[REG_OVERRIDE_ADC_REF] ? DataTable[REG_OVERRIDE_ADC_REF] : ADC_REF_MV;

	float Offset = (float)((int16_t)DataTable[REG_I_DUT_OFFSET]);
	float K = (float)DataTable[REG_I_DUT_GAIN] / 1000;

	float ShuntRes = (float)DataTable[REG_DUT_CURRENT_SHUNT] / 10;

	float P0 = (float)((int16_t)DataTable[REG_P0_I_DUT]);
	float P1 = (float)DataTable[REG_P1_I_DUT] / 1000;
	float P2 = (float)((int16_t)DataTable[REG_P2_I_DUT]) / 1e6f;

	if(DataLength == 1)
	{
		tmp = ((float)RawArray[0] - Offset) * Uref / ADC_RESOLUTION * K;
		tmp = tmp / ShuntRes;
		tmp = tmp * tmp * P2 + tmp * P1 + P0;
		RawArray[0] = (tmp > 0) ? (uint16_t)tmp : 0;
		OutputArray[0] = RawArray[0];
		return;
	}
	else
	{
		for(i = I_DUT_PRE_SKIP_POINTS; i < DataLength; ++i)
		{
			tmp = ((float)RawArray[i] - Offset) * Uref / ADC_RESOLUTION * K;
			tmp = tmp / ShuntRes;
			tmp = tmp * tmp * P2 + tmp * P1 + P0;
			RawArray[i] = (tmp > 0) ? (uint16_t)tmp : 0;

			// Фильтрация тока
			OutputArray[i] = 0;

			for(int j = 0; j < FIR_LENGTH - 1; j++)
			{
				if((i - j) >= 0)
					OutputArray[i] += RawArray[i - j] * FIR_Coefficients[j];
			}

			// Определение индекса массива с максимальным значением тока
			if(OutputArray[i] > Imax)
			{
				Imax = OutputArray[i];
				ImaxArrayIndex = i;
			}
		}
	}

	Imax = 0;

	// Усреднение амплитуды тока
	for(int i = (ImaxArrayIndex - I_MAX_AVERAGE_POINTS / 2); i < (ImaxArrayIndex + I_MAX_AVERAGE_POINTS / 2); i++)
		Imax += OutputArray[i];

	DataTable[REG_MEAS_CURRENT_VALUE] = Imax / I_MAX_AVERAGE_POINTS;
}
//---------------------

bool MEASURE_CheckAnodeCurrent()
{
	// Запуск процесса оцифровки тока
	DMA_ChannelReload(DMA_ADC_DUT_I_CHANNEL, MEASURE_POINTS_NUMBER);
	DMA_ChannelEnable(DMA_ADC_DUT_I_CHANNEL, true);

	TIM_Start(TIM6);
	DELAY_US(7);
	// Гарантируетя остановка, если вызов раньше прерывания DMA
	TIM_Stop(TIM6);

	DMA_ChannelReload(DMA_ADC_DUT_I_CHANNEL, PULSE_ARR_MAX_LENGTH);
	DMA_ChannelEnable(DMA_ADC_DUT_I_CHANNEL, true);

	// Усреднение результата измерения
	uint32_t AnodeCurrentRaw = 0;
	for(int i = 0; i < MEASURE_POINTS_NUMBER; i++)
		AnodeCurrentRaw += LOGIC_OutputPulseRaw[i];
	uint16_t AnodeCurrentRaw16b = AnodeCurrentRaw / MEASURE_POINTS_NUMBER;

	uint16_t AnodeCurrent = 0;
	MEASURE_ConvertRawArray(&AnodeCurrentRaw16b, &AnodeCurrent, 1);

	DataTable[REG_DIAG_ANODE_SHORT] = AnodeCurrent;

	return (DataTable[REG_SHORT_CALIBRATE_FLAG] == 0) && (AnodeCurrent * 10 > DataTable[REG_SHORT_THRESHOLD]);
}
//---------------------

void MEASURE_TurnOnMeasurement()
{
	uint32_t DataRaw;
	uint16_t TurnOn, TurnDelay;

	DataRaw = LL_HSTimers_Read();

	// Сохранение отладочной информации
	DataTable[REG_DIAG_RAW_SHIFT_REG] = DataRaw & 0xFFF;
	DataTable[REG_DIAG_RAW_SHIFT_REG_2] = DataRaw >> 12;

	// Наложена защитная 10битная маска
	TurnDelay = (DataRaw >> 12) & 0x3FF;
	TurnDelay = TurnDelay * COUNTER_CLOCK_PERIOD_NS;

	if(TurnDelay < DataTable[REG_MEAS_TIME_LOW])
		TurnDelay = 0;

	TurnOn = DataRaw & 0xFFF;
	TurnOn = TurnOn * COUNTER_CLOCK_PERIOD_NS;

	if(TurnOn < DataTable[REG_MEAS_TIME_LOW])
		TurnOn = 0;

	if(TurnDelay && TurnOn)
		MEASURE_FineTuneTdelTon(&TurnDelay, &TurnOn);

	// Сохранение результата в endpoint
	CONTROL_Values_TurnDelay[CONTROL_Values_TurnCounter] = TurnDelay;
	CONTROL_Values_TurnOn[CONTROL_Values_TurnCounter] = TurnOn;
	CONTROL_Values_TurnCounter++;
}
//-----------------------------------------------

void MEASURE_FineTuneTdelTon(uint16_t* TurnDelay, uint16_t* TurnOn)
{
	float Tdel_P2 = 0, Tdel_P1 = 1, Tdel_P0 = 0;
	float Ton_P2 = 0, Ton_P1 = 1, Ton_P0 = 0;
	float T;

	switch(DataTable[REG_ANODE_VOLTAGE])
	{
		case TOU_600V:
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_600V_P2]) / 1e6f;
			Tdel_P1 = (float)DataTable[REG_T_DEL_600V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_600V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_600V_P2]) / 1e6f;
			Ton_P1 = (float)DataTable[REG_T_ON_600V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_600V_P0];
			break;

		case TOU_1000V:
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_1000V_P2]) / 1e6f;
			Tdel_P1 = (float)DataTable[REG_T_DEL_1000V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_1000V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_1000V_P2]) / 1e6f;
			Ton_P1 = (float)DataTable[REG_T_ON_1000V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_1000V_P0];
			break;

		case TOU_1500V:
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_1500V_P2]) / 1e6f;
			Tdel_P1 = (float)DataTable[REG_T_DEL_1500V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_1500V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_1500V_P2]) / 1e6f;
			Ton_P1 = (float)DataTable[REG_T_ON_1500V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_1500V_P0];
			break;

		default:
			break;
	}

	T = *TurnDelay;
	T = T * T * Tdel_P2 + T * Tdel_P1 + Tdel_P0;
	*TurnDelay = (T > 0) ? ((uint16_t)T) : 0;

	T = *TurnOn;
	T = T * T * Ton_P2 + T * Ton_P1 + Ton_P0;
	*TurnOn = (T > 0) ? ((uint16_t)T) : 0;
}
//-----------------------------------------------

Int16U MEASURE_AverageData(pInt16U Data, Int16U DataCount)
{
	Int32U Avg = 0;
	for(int i = 0; i < DataCount; i++)
		Avg += Data[i];
	return Avg / DataCount;
}
//-----------------------------------------------

void MEASURE_TurnOnAveragingProcess()
{
	DataTable[REG_MEAS_TIME_DELAY] = MEASURE_AverageData((pInt16U)CONTROL_Values_TurnDelay, CONTROL_Values_TurnCounter);
	DataTable[REG_MEAS_TIME_ON] = MEASURE_AverageData((pInt16U)CONTROL_Values_TurnOn, CONTROL_Values_TurnCounter);
}
//-----------------------------------------------

float MEASURE_AnodeCurrentTune(AnodeVoltageEnum AnodeVoltage, float AnodeCurrent)
{
	float P2, P1, P0;

	switch(AnodeVoltage)
	{
		case TOU_600V:
			P2 = ((float)((int16_t)DataTable[REG_I_DUT_600V_P2])) / 1e6f;
			P1 = ((float)DataTable[REG_I_DUT_600V_P1]) / 1000;
			P0 = (int16_t)DataTable[REG_I_DUT_600V_P0];
			break;

		case TOU_1000V:
			P2 = ((float)((int16_t)DataTable[REG_I_DUT_1000V_P2])) / 1e6f;
			P1 = ((float)DataTable[REG_I_DUT_1000V_P1]) / 1000;
			P0 = (int16_t)DataTable[REG_I_DUT_1000V_P0];
			break;

		case TOU_1500V:
			P2 = ((float)((int16_t)DataTable[REG_I_DUT_1500V_P2])) / 1e6f;
			P1 = ((float)DataTable[REG_I_DUT_1500V_P1]) / 1000;
			P0 = (int16_t)DataTable[REG_I_DUT_1500V_P0];
			break;

		default:
			return 0;
	}

	return AnodeCurrent * AnodeCurrent * P2 + AnodeCurrent * P1 + P0;
}
//-----------------------------------------------

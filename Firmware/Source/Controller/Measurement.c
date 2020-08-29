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

// Variables
volatile uint16_t LOGIC_OutputPulseRaw[PULSE_ARR_MAX_LENGTH];

// Forward functions
uint16_t MEASURE_UrefToDAC(uint16_t Voltage);

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

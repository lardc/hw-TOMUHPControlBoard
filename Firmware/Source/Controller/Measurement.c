// Header
#include "Measurement.h"
// Includes
#include "LowLevel.h"
#include "Board.h"
#include "Global.h"
#include "DataTable.h"
#include "SysConfig.h"

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
	LL_WriteDACx(Data, GPIO_CS_DAC);
}
//---------------------

void MEASURE_SetUref90(uint16_t Voltage)
{
	uint16_t Data = MEASURE_UrefToDAC(Voltage) & (~DAC_CHANNEL_B);
	LL_WriteDACx(Data, GPIO_CS_DAC);
}
//---------------------

void MEASURE_ConvertRawArray(uint16_t* RawArray, uint16_t* OutputArray, uint16_t DataLength)
{
	uint16_t i;
	float tmp;

	float Offset = (float)((int16_t)DataTable[REG_DUT_CURRENT_OFFSET]);
	float K = (float)DataTable[REG_DUT_CURRENT_GAIN] / 1000;

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
	}
}
//---------------------

uint16_t MEASURE_ReadCurrent()
{
	uint16_t raw, result;

	raw = ADC_Measure(ADC1, ADC1_CURRENT_CHANNEL);
	MEASURE_ConvertRawArray(&raw, &result, 1);

	return result;
}
//---------------------

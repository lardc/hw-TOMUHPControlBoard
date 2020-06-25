// Header
#include "Measurement.h"
// Includes
#include "LowLevel.h"
#include "Board.h"
#include "Global.h"

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

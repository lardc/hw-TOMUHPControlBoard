// Header
#include "Measurement.h"
// Includes
#include "LowLevel.h"
#include "Board.h"

// Functions
//
void MEASURE_SetUref10(uint16_t Data)
{
	Data |= DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_DAC);
}
//---------------------

void MEASURE_SetUref90(uint16_t Data)
{
	Data &= ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_DAC);
}
//---------------------

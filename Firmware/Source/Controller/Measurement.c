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

void MEASURE_Set_Uref90(uint16_t Data)
{
	Data &= ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_DAC);
}
//---------------------

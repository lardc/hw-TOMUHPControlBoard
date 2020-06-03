// Header
#include "GateDriver.h"
// Includes
#include "Board.h"
#include "LowLevel.h"

// Functions
void GateDriver_Sync(bool State)
{
	GPIO_SetState(GPIO_SYNC_GD, State);
}
//---------------------

void GateDriver_SetCurrent(uint16_t Data)
{
	Data &= ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2);
}
//---------------------

void GateDriver_SetCompThreshold(uint16_t Data)
{
	Data |= DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2);
}
//---------------------

void GateDriver_SetFallRate(uint16_t Data)
{
	Data &= ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD1);
}
//---------------------

void GateDriver_SetRiseRate(uint16_t Data)
{
	Data |= DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD1);
}
//---------------------

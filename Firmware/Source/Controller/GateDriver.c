// Header
#include "GateDriver.h"
// Includes
#include "Board.h"
#include "LowLevel.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Forward functions
uint16_t MEASURE_IrefToDAC(uint16_t Current);

// Functions
void GateDriver_Sync(bool State)
{
	GPIO_SetState(GPIO_SYNC_GD, State);
}
//---------------------

uint16_t MEASURE_IrefToDAC(uint16_t Current)
{
	uint32_t result = (uint32_t)Current * DataTable[REG_GD_CURRENT_SHUNT] * DAC_RESOLUTION / DAC_REF_MV / 1000;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

void GateDriver_SetCurrent(uint16_t Data)
{
	Data &= ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2);
}
//---------------------

void GateDriver_SetCompThreshold(uint16_t Current)
{
	uint16_t Data = MEASURE_IrefToDAC(Current) | DAC_CHANNEL_B;
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

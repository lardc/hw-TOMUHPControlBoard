// Header
#include "GateDriver.h"
// Includes
#include "Board.h"
#include "LowLevel.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Forward functions
uint16_t MEASURE_ItoDAC(float GateCurrent);
uint16_t MEASURE_IrefToDAC(float GateCurrent);

// Functions
void GateDriver_Sync(bool State)
{
	GPIO_SetState(GPIO_SYNC_GD, State);
}
//---------------------

uint16_t MEASURE_ItoDAC(float GateCurrent)
{
	float K = (float)DataTable[REG_GD_I_SET_K] / 1000;
	float Offset = (float)((int16_t)DataTable[REG_GD_I_SET_OFFSET]);

	uint32_t result = GateCurrent * K + Offset;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

void GateDriver_SetCurrent(float GateCurrent)
{
	uint16_t Data = MEASURE_ItoDAC(GateCurrent) & ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2);
}
//---------------------

uint16_t MEASURE_IrefToDAC(float GateCurrentThreshold)
{
	uint32_t result = (uint32_t)GateCurrentThreshold * DataTable[REG_GD_CURRENT_SHUNT] * DAC_RESOLUTION / DAC_REF_MV / 1000;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

void GateDriver_SetCompThreshold(float GateCurrentThreshold)
{
	uint16_t Data = MEASURE_IrefToDAC(GateCurrentThreshold) | DAC_CHANNEL_B;
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

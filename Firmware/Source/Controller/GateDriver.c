// Header
#include "GateDriver.h"
// Includes
#include "Board.h"
#include "LowLevel.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Forward functions
uint16_t GateDriver_ItoDAC(float GateCurrent);
uint16_t GateDriver_IrefToDAC(float GateCurrent);
uint16_t GateDriver_IrateToDAC(float CurrentRate, uint16_t KRegister);

// Functions
void GateDriver_Sync(bool State)
{
	GPIO_SetState(GPIO_SYNC_GD, State);
}
//---------------------

uint16_t GateDriver_ItoDAC(float GateCurrent)
{
	float K = (float)DataTable[REG_GD_I_SET_K];
	float Offset = (float)((int16_t)DataTable[REG_GD_I_SET_OFFSET]);
	
	uint32_t result = GateCurrent * K + Offset;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

uint16_t GateDriver_IrefToDAC(float GateCurrentThreshold)
{
	float ShuntRes = (float)DataTable[REG_GD_CURRENT_SHUNT] / (1000 * 10);
	uint32_t result = (uint32_t)GateCurrentThreshold * ShuntRes * DAC_RESOLUTION / DAC_REF_MV;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

uint16_t GateDriver_IrateToDAC(float CurrentRate, uint16_t KRegister)
{
	float K = (float)DataTable[KRegister];
	
	uint32_t result = CurrentRate * K;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

void GateDriver_SetCurrent(float GateCurrent)
{
	uint16_t Data = GateDriver_ItoDAC(GateCurrent) & ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2, RISE_Edge);
	DataTable[REG_DBG] = Data;
}
//---------------------

void GateDriver_SetCompThreshold(float GateCurrentThreshold)
{
	uint16_t Data = GateDriver_IrefToDAC(GateCurrentThreshold) | DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2, RISE_Edge);
}
//---------------------

void GateDriver_SetFallRate(float GateCurrentFallRate)
{
	uint16_t Data = GateDriver_IrateToDAC(GateCurrentFallRate, REG_GD_FALL_RATE_TO_DAC) & ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD1, RISE_Edge);
}
//---------------------

void GateDriver_SetRiseRate(float GateCurrentRiseRate)
{
	uint16_t Data = GateDriver_IrateToDAC(GateCurrentRiseRate, REG_GD_RISE_RATE_TO_DAC) | DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD1, RISE_Edge);
}
//---------------------

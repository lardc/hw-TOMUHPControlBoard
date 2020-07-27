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
uint16_t GateDriver_IrateToDAC(float CurrentRate, uint16_t K, int16_t Offset);

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

uint16_t GateDriver_IrateToDAC(float CurrentRate, uint16_t K, int16_t Offset)
{
	uint32_t result = CurrentRate * K + Offset;
	return (result > DAC_RESOLUTION) ? DAC_RESOLUTION : result;
}
//---------------------

void GateDriver_SetCurrent(float GateCurrent)
{
	uint16_t Data = GateDriver_ItoDAC(GateCurrent) & ~DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2, RISE_Edge);
}
//---------------------

void GateDriver_SetCompThreshold(float GateCurrentThreshold)
{
	uint16_t Data = GateDriver_IrefToDAC(GateCurrentThreshold) | DAC_CHANNEL_B;
	LL_WriteDACx(Data, GPIO_CS_GD2, RISE_Edge);
}
//---------------------

void GateDriver_SetFallRate(MeasurementSettings *Settings)
{
	float FrontTime, FrontTimeMin, GateCurrentFallRate;
	uint16_t Data;

	FrontTime = Settings->GateCurrent / Settings->GateCurrentFallRate;
	FrontTimeMin = (float)DataTable[REG_GD_FRONT_TIME_MIN] / 10;

	if(FrontTime < FrontTimeMin)
		GateCurrentFallRate = FrontTimeMin * Settings->GateCurrent;
	else
		GateCurrentFallRate = Settings->GateCurrentFallRate;

	Data = GateDriver_IrateToDAC(GateCurrentFallRate, DataTable[REG_GD_I_FALL_RATE_K], DataTable[REG_GD_I_FALL_RATE_OFFSET]);
	LL_WriteDACx((Data & ~DAC_CHANNEL_B), GPIO_CS_GD1, RISE_Edge);
}
//---------------------

void GateDriver_SetRiseRate(MeasurementSettings *Settings)
{
	float FrontTime, FrontTimeMin, GateCurrentRiseRate;
	uint16_t Data;

	FrontTime = Settings->GateCurrent / Settings->GateCurrentRiseRate;
	FrontTimeMin = (float)DataTable[REG_GD_FRONT_TIME_MIN] / 10;

	if(FrontTime < FrontTimeMin)
		GateCurrentRiseRate = FrontTimeMin * Settings->GateCurrent;
	else
		GateCurrentRiseRate = Settings->GateCurrentRiseRate;

	Data = GateDriver_IrateToDAC(GateCurrentRiseRate, DataTable[REG_GD_I_RISE_RATE_K], DataTable[REG_GD_I_RISE_RATE_OFFSET]);
	LL_WriteDACx((Data | DAC_CHANNEL_B), GPIO_CS_GD1, RISE_Edge);
}
//---------------------

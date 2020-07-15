// Header
#include "LowLevel.h"

// Include
#include "Delay.h"

// Functions
//
void LL_ToggleLED()
{
	GPIO_Toggle(GPIO_LED);
}
//-----------------------------

void LL_ExternalLED(bool State)
{
	GPIO_SetState(GPIO_IND, State);
}
//-----------------------------

void LL_UnitFan(bool State)
{
	GPIO_SetState(GPIO_FAN, State);
}
//-----------------------------

void LL_PsBoard_PowerOutput(bool State)
{
	GPIO_SetState(GPIO_PS_EN, !State);
}
//-----------------------------

void LL_PsBoard_PowerInput(bool State)
{
	GPIO_SetState(GPIO_RELAY, State);
}
//-----------------------------

void LL_HSTimers_Reset()
{
	GPIO_SetState(GPIO_M_RESET, false);
	DELAY_US(1);
	GPIO_SetState(GPIO_M_RESET, true);
}
//-----------------------------

uint32_t LL_HSTimers_Read()
{
	uint32_t Data;

	SPI_SetSyncPolarity(SPI1, RISE_Edge);

	GPIO_SetState(GPIO_LOAD, false);
	DELAY_US(1);
	GPIO_SetState(GPIO_LOAD, true);

	GPIO_SetState(GPIO_CS, false);
	Data = SPI_ReadByte(SPI1) << 16;
	Data |= SPI_ReadByte(SPI1);
	GPIO_SetState(GPIO_CS, true);

	return (Data);
}
//---------------------------

void LL_GateLatchReset()
{
	GPIO_SetState(GPIO_TRIG_RST, false);
	DELAY_US(1);
	GPIO_SetState(GPIO_TRIG_RST, true);
}
//-----------------------------

void LL_SyncOscilloscope(bool State)
{
	GPIO_SetState(GPIO_SYNC, State);
}
//-----------------------------

void LL_SyncTOCU(bool State)
{
	GPIO_SetState(GPIO_SNC_TOCU, State);
}
//-----------------------------

bool LL_IsOverflow90()
{
	return GPIO_GetState(GPIO_OVERFLOW90);
}
//-----------------------------

bool LL_IsOverflow10()
{
	return GPIO_GetState(GPIO_OVERFLOW10);
}
//-----------------------------

void LL_WriteDACx(uint16_t Data, GPIO_PortPinSetting CS_SYNC, SPI_SyncPolarityEnum Polarity)
{
	SPI_SetSyncPolarity(SPI2, Polarity);

	GPIO_SetState(CS_SYNC, false);
	SPI_WriteByte(SPI2, Data);
	GPIO_SetState(CS_SYNC, true);
	DELAY_US(1);

	GPIO_SetState(GPIO_LDAC, false);
	DELAY_US(1);
	GPIO_SetState(GPIO_LDAC, true);
	DELAY_US(1);
}
//---------------------

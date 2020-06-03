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

void LL_MonitorSafetyInput(bool State)
{
	GPIO_SetState(GPIO_SFTY_EN, State);
}
//-----------------------------

bool LL_IsSafetyTrig()
{
	return GPIO_GetState(GPIO_SFTY);
}
//-----------------------------

void LL_HSTimers_Reset(bool State)
{
	GPIO_SetState(GPIO_M_RESET, !State);
}
//-----------------------------

uint32_t LL_HSTimers_Read()
{
	uint32_t Data;

	GPIO_SetState(GPIO_LOAD, false);
	DELAY_US(1);
	GPIO_SetState(GPIO_LOAD, true);

	GPIO_SetState(GPIO_CS, false);
	Data = SPI_ReadByte(SPI1);
	Data |= (uint32_t)SPI_ReadByte(SPI1) << 8;
	Data |= (uint32_t)SPI_ReadByte(SPI1) << 16;
	GPIO_SetState(GPIO_CS, true);

	return Data;
}
//---------------------------

void LL_GateLatchReset(bool State)
{
	GPIO_SetState(GPIO_TRIG_RST, !State);
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

void LL_InternalCommutation(bool State)
{
	GPIO_SetState(GPIO_RLC, State);
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

bool LL_IsPressureTrig()
{
	return GPIO_GetState(GPIO_PRESSURE);
}
//-----------------------------

void LL_OutputRegister_Enable(bool State)
{
	GPIO_SetState(GPIO_SREG_OE, State);
}
//-----------------------------

void LL_OutputRegister_Write(uint16_t Data)
{
	GPIO_SetState(GPIO_SREG_CS, false);
	SPI_WriteByte(SPI2, Data);
	GPIO_SetState(GPIO_SREG_CS, true);
}
//---------------------

void LL_WriteDACx(uint16_t Data, GPIO_PortPinSetting CS_SYNC)
{
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

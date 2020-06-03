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

void LL_HSTimers_Load(bool State)
{
	GPIO_SetState(GPIO_LOAD, State);
}
//-----------------------------

void LL_HSTimers_CS(bool State)
{
	GPIO_SetState(GPIO_CS, State);
}
//-----------------------------

void LL_GateLatch(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_7) : GPIO_Bit_Rst(GPIOB, Pin_7);
}
//-----------------------------

// Osc SYNC
void LL_OscSync(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_8) : GPIO_Bit_Rst(GPIOA, Pin_8);
}
//-----------------------------

// TOCU SYNC
void LL_TOCU_Sync(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_2) : GPIO_Bit_Rst(GPIOB, Pin_2);
}
//-----------------------------

// Internal commutation
void LL_INT_Commutation(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_0) : GPIO_Bit_Rst(GPIOB, Pin_0);
}
//-----------------------------

// Overflow90
bool LL_IsOverflow90()
{
	return GPIO_Read_Bit(GPIOB, Pin_4);
}
//-----------------------------

// Overflow10
bool LL_IsOverflow10()
{
	return GPIO_Read_Bit(GPIOB, Pin_8);
}
//-----------------------------

// Pressure
bool LL_IsPressure()
{
	return GPIO_Read_Bit(GPIOB, Pin_10);
}
//-----------------------------

// Output reg CS
void LL_OutReg_CS(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_5) : GPIO_Bit_Rst(GPIOB, Pin_5);
}
//-----------------------------

// Output reg OE
void LL_OutReg_OE(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_11) : GPIO_Bit_Rst(GPIOB, Pin_11);
}
//-----------------------------

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

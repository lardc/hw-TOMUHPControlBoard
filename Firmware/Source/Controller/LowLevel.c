// Header
#include "LowLevel.h"

// Include
#include "Board.h"

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

void LL_GD_Sync(bool State)
{
	GPIO_SetState(GPIO_SYNC_GD, State);
}
//-----------------------------

void LL_GD_DAC_CS_SYNC1(bool State)
{
	GPIO_SetState(GPIO_CS_GD1, State);
}
//-----------------------------

void LL_GD_DAC_CS_SYNC2(bool State)
{
	GPIO_SetState(GPIO_CS_GD2, State);
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

// DAC SYNC
void LL_DAC_CS_SYNC(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_9) : GPIO_Bit_Rst(GPIOB, Pin_9);
}
//-----------------------------

// LDAC line
void LL_DAC_LDAC(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_14) : GPIO_Bit_Rst(GPIOB, Pin_14);
}
//-----------------------------














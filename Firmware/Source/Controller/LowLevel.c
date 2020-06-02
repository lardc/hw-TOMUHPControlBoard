// Header
#include "LowLevel.h"

// Functions
//
// LED on board
bool LL_IsBlinkLED()
{
	return GPIO_Read_Bit(GPIOC, Pin_13);
}
//-----------------------------

void LL_BlinkLED(bool State)
{
	State ? GPIO_Bit_Set(GPIOC, Pin_13) : GPIO_Bit_Rst(GPIOC, Pin_13);
}
//-----------------------------

// External LED
void LL_ExternalLED(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_7) : GPIO_Bit_Rst(GPIOA, Pin_7);
}
//-----------------------------

// Unit cooler
void LL_ExternalFan(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_0) : GPIO_Bit_Rst(GPIOA, Pin_0);
}
//-----------------------------

// GateDriver PsBoard control
void LL_GD_PsBoard(bool State)
{
	State ? GPIO_Bit_Set(GPIOC, Pin_15) : GPIO_Bit_Rst(GPIOC, Pin_15);
}
//-----------------------------

// GateDriver sync line
void LL_GD_Sync(bool State)
{
	State ? GPIO_Bit_Set(GPIOC, Pin_14) : GPIO_Bit_Rst(GPIOC, Pin_14);
}
//-----------------------------

// GateDriver SYNC1 line
void LL_GD_DAC_CS_SYNC1(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_12) : GPIO_Bit_Rst(GPIOB, Pin_12);
}
//-----------------------------

// GateDriver SYNC2 line
void LL_GD_DAC_CS_SYNC2(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_6) : GPIO_Bit_Rst(GPIOB, Pin_6);
}
//-----------------------------

// Safety system control
void LL_SFTY_ActiveState(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_15) : GPIO_Bit_Rst(GPIOA, Pin_15);
}
//-----------------------------

// Safety signal
bool LL_IsSafety()
{
	return GPIO_Read_Bit(GPIOB, Pin_3);
}
//-----------------------------

// M_RESET timers
void LL_HSTimers_Reset(bool State)
{
	(!State) ? GPIO_Bit_Set(GPIOA, Pin_2) : GPIO_Bit_Rst(GPIOA, Pin_2);
}
//-----------------------------

// LOAD timers
void LL_HSTimers_Load(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_3) : GPIO_Bit_Rst(GPIOA, Pin_3);
}
//-----------------------------

// CS timers
void LL_HSTimers_CS(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_4) : GPIO_Bit_Rst(GPIOA, Pin_4);
}
//-----------------------------

// Relay control
void LL_RelayControl(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_1) : GPIO_Bit_Rst(GPIOB, Pin_1);
}
//-----------------------------

// Gate Comparator Latch Control
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














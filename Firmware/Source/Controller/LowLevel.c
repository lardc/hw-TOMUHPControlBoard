// Header
#include "LowLevel.h"

// Functions
//
// LED on board
bool LL_IsBlinkLED()
{
	return GPIO_Read_Bit(GPIOB, Pin_12);
}
//-----------------------------

void LL_BlinkLED(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_12) : GPIO_Bit_Rst(GPIOB, Pin_12);
}
//-----------------------------

// External LED
void LL_ExternalLED(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_4) : GPIO_Bit_Rst(GPIOA, Pin_4);
}
//-----------------------------

// M_RESET timers
void LL_TimersReset(bool State)
{
	(!State) ? GPIO_Bit_Set(GPIOA, Pin_2) : GPIO_Bit_Rst(GPIOA, Pin_2);
}
//-----------------------------

// Unit cooler
void LL_ExternalFan(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_7) : GPIO_Bit_Rst(GPIOA, Pin_7);
}
//-----------------------------

// External SYNC
void LL_ExternalSync(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_8) : GPIO_Bit_Rst(GPIOA, Pin_8);
}
//-----------------------------

// Gate control
void LL_GateControl(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_5) : GPIO_Bit_Rst(GPIOB, Pin_5);
}
//-----------------------------

// Relay control
void LL_RelayControl(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_6) : GPIO_Bit_Rst(GPIOB, Pin_6);
}
//-----------------------------

// Gate Comparator Latch Control
void LL_GateLatch(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_2) : GPIO_Bit_Rst(GPIOB, Pin_2);
}
//-----------------------------

// Порты Serial shift registers
//
// DATA
bool LL_IsShiftRegData()
{
	return GPIO_Read_Bit(GPIOA, Pin_6);
}
//-----------------------------

// LOAD
void LL_ShiftRegLoad(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_3) : GPIO_Bit_Rst(GPIOA, Pin_3);
}
//-----------------------------

// CLK
void LL_ShiftRegClk(bool State)
{
	State ? GPIO_Bit_Set(GPIOA, Pin_5) : GPIO_Bit_Rst(GPIOA, Pin_5);
}
//-----------------------------

// CS
void LL_ShiftRegCS(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_0) : GPIO_Bit_Rst(GPIOB, Pin_0);
}
//-----------------------------

// Порты Serial to TOCU
//
// Enable
void LL_TOCUSerialEnable(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_10) : GPIO_Bit_Rst(GPIOB, Pin_10);
}
//-----------------------------

// SRCK
void LL_TOCUSerialSRCK(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_13) : GPIO_Bit_Rst(GPIOB, Pin_13);
}
//-----------------------------

// RCK
void LL_TOCUSerialRCK(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_14) : GPIO_Bit_Rst(GPIOB, Pin_14);
}
//-----------------------------

// DATA
void LL_TOCUSerialData(bool State)
{
	State ? GPIO_Bit_Set(GPIOB, Pin_15) : GPIO_Bit_Rst(GPIOB, Pin_15);
}
//-----------------------------

#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"
#include "Board.h"

// Defines
#define DAC_CHANNEL_B		0x8000

// Functions
void LL_ToggleLED();
void LL_ExternalLED(bool State);
void LL_UnitFan(bool State);
void LL_PsBoard_PowerOutput(bool State);
void LL_PsBoard_PowerInput(bool State);
void LL_MonitorSafetyInput(bool State);
bool LL_IsSafetyTrig();
void LL_HSTimers_Reset(bool State);
void LL_HSTimers_Load(bool State);
void LL_HSTimers_CS(bool State);
void LL_GateLatch(bool State);
void LL_OscSync(bool State);
void LL_TOCU_Sync(bool State);
void LL_INT_Commutation(bool State);
bool LL_IsOverflow90();
bool LL_IsOverflow10();
bool LL_IsPressure();
void LL_OutReg_CS(bool State);
void LL_OutReg_OE(bool State);
void LL_WriteDACx(uint16_t Data, GPIO_PortPinSetting CS_SYNC);

#endif //__LOWLEVEL_H

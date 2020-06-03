#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Functions
void LL_ToggleLED();
void LL_ExternalLED(bool State);
void LL_UnitFan(bool State);
void LL_PsBoard_PowerOutput(bool State);
void LL_PsBoard_PowerInput(bool State);
void LL_GD_Sync(bool State);
void LL_GD_DAC_CS_SYNC1(bool State);
void LL_GD_DAC_CS_SYNC2(bool State);
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
void LL_DAC_CS_SYNC(bool State);
void LL_DAC_LDAC(bool State);

#endif //__LOWLEVEL_H

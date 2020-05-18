#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "ZwGPIO.h"


// Functions
//
bool LL_IsBlinkLED();
void LL_BlinkLED(bool State);
void LL_ExternalLED(bool State);
void LL_TimersReset(bool State);
void LL_ExternalFan(bool State);
void LL_ExternalSync(bool State);
void LL_GateControl(bool State);
void LL_RelayControl(bool State);
void LL_GateLatch(bool State);
bool LL_IsShiftRegData();
void LL_ShiftRegLoad(bool State);
void LL_ShiftRegClk(bool State);
void LL_ShiftRegCS(bool State);
void LL_TOCUSerialEnable(bool State);
void LL_TOCUSerialSRCK(bool State);
void LL_TOCUSerialRCK(bool State);
void LL_TOCUSerialData(bool State);

#endif //__LOWLEVEL_H

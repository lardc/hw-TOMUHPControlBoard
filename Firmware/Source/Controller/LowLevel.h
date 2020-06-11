#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"
#include "Board.h"

// Defines
#define DAC_CHANNEL_B		BIT15

// Functions
void LL_ToggleLED();
void LL_ExternalLED(bool State);
void LL_UnitFan(bool State);
void LL_PsBoard_PowerOutput(bool State);
void LL_PsBoard_PowerInput(bool State);
void LL_HSTimers_Reset();
uint32_t LL_HSTimers_Read();
void LL_GateLatchReset();
void LL_SyncOscilloscope(bool State);
void LL_SyncTOCU(bool State);
bool LL_IsOverflow90();
bool LL_IsOverflow10();
void LL_WriteDACx(uint16_t Data, GPIO_PortPinSetting CS_SYNC);

#endif //__LOWLEVEL_H

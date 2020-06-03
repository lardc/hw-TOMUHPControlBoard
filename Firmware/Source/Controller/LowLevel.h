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
uint32_t LL_HSTimers_Read();
void LL_GateLatchReset(bool State);
void LL_SyncOscilloscope(bool State);
void LL_SyncTOCU(bool State);
void LL_InternalCommutation(bool State);
bool LL_IsOverflow90();
bool LL_IsOverflow10();
bool LL_IsPressureTrig();
void LL_OutputRegister_Enable(bool State);
void LL_OutputRegister_Write(uint16_t Data);
void LL_WriteDACx(uint16_t Data, GPIO_PortPinSetting CS_SYNC);

#endif //__LOWLEVEL_H

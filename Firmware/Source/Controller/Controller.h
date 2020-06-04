#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"

// Definitions
typedef enum __DeviceState
{
	DS_None = 0,
	DS_Fault = 1,
	DS_Disabled = 2,
	DS_Ready = 3,
	DS_InProcess = 4
} DeviceState;
typedef enum __SubState
{
	SS_None = 0,
	SS_PowerOn,
	SS_StopProcess = 1,
	SS_WaitVoltage = 2,
	SS_VoltageReady = 3,
	SS_WaitContactor = 4
} SubState;

// Variables
extern volatile Int64U CONTROL_TimeCounter;

// Functions
void CONTROL_Init();
void CONTROL_Idle();

#endif // __CONTROLLER_H

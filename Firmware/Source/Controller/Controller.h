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

// Variables
extern volatile Int64U CONTROL_TimeCounter;

// Functions
void CONTROL_Init();
void CONTROL_Idle();

#endif // __CONTROLLER_H

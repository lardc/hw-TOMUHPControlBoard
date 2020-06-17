#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"

// Types
typedef enum __AnodeVoltage
{
	TOU_500V = 500,
	TOU_1000V = 1000,
	TOU_1500V = 1500
} AnodeVoltageEnum;
typedef struct __MeasurementSettings
{
	AnodeVoltageEnum AnodeVoltage;
	float AnodeCurrent;
	float GateCurrent;
	float GateCurrentRiseRate;
	float GateCurrentFallRate;
} MeasurementSettings;

// Variables
extern volatile Int64U CONTROL_TimeCounter;

// Functions
void CONTROL_Init();
void CONTROL_Idle();

#endif // __CONTROLLER_H

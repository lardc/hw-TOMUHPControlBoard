#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
#include "Measurement.h"

// Defines
//
#define EP_SIZE			(PULSE_ARR_MAX_LENGTH / 2)
//

// Types
typedef enum __AnodeVoltage
{
	TOU_600V = 600,
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
extern volatile Int16U CONTROL_Values_Current[PULSE_ARR_MAX_LENGTH];
extern volatile Int16U CONTROL_Values_Counter;
extern MeasurementSettings CachedMeasurementSettings;
extern Int16U CONTROL_AverageCounter;

// Functions
void CONTROL_Init();
void CONTROL_Idle();
void CONTROL_UnitFan();

#endif // __CONTROLLER_H

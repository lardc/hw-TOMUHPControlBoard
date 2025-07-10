#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
#include "Measurement.h"

// Defines
#define EP_SIZE			(PULSE_ARR_MAX_LENGTH / 2)

// Types
typedef struct __MeasurementSettings
{
	AnodeVoltageEnum AnodeVoltage;
	float CorrectedVoltage;
	float AnodeCurrent;
	float GateCurrent;
	float GateCurrentRiseRate;
	float GateCurrentFallRate;
} MeasurementSettings, *pMeasurementSettings;

// Variables
extern volatile Int64U CONTROL_TimeCounter;
extern volatile Int16U CONTROL_Values_Current[PULSE_ARR_MAX_LENGTH];
extern volatile Int16U CONTROL_Values_TurnDelay[TIME_ARR_MAX_LENGTH];
extern volatile Int16U CONTROL_Values_TurnOn[TIME_ARR_MAX_LENGTH];
extern volatile Int16U CONTROL_Values_CurrentCounter;
extern volatile Int16U CONTROL_Values_TurnCounter;
extern MeasurementSettings CachedMeasurementSettings;
extern Int16U CONTROL_ExtInfoCounter;

// EP
extern Int16U CONTROL_ExtInfoData[];

// Functions
void CONTROL_Init();
void CONTROL_Idle();
void CONTROL_UnitFan();

#endif // __CONTROLLER_H

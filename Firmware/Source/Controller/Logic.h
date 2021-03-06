#ifndef __LOGIC_H
#define __LOGIC_H

#include "stdinc.h"
#include "BCCIMHighLevel.h"
#include "Controller.h"

// Functions
bool LOGIC_ReadSlavesState();
bool LOGIC_WriteSlavesConfig();
bool LOGIC_CallCommandForSlaves(uint16_t Command);
bool LOGIC_AreSlavesInStateX(uint16_t State);
bool LOGIC_IsSlaveInFaultOrDisabled(uint16_t Fault, uint16_t Disabled);
void LOGIC_AssignVItoSlaves(AnodeVoltageEnum AnodeVoltage, float AnodeCurrent);
bool LOGIC_IsAnodeVRegCorrect();
bool LOGIC_GetSafetyState();
bool LOGIC_GetPressureState();
void LOGIC_ConfigVoltageComparators(AnodeVoltageEnum AnodeVoltage);
MeasurementSettings LOGIC_CacheMeasurementSettings();

#endif // __LOGIC_H

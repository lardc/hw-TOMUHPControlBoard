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
void LOGIC_AssignVoltageAndCurrentToSlaves(AnodeVoltage Voltage, uint16_t Current);

#endif // __LOGIC_H

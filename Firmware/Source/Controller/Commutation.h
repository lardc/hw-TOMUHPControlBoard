#ifndef __COMMUTATION_H
#define __COMMUTATION_H

#include "stdinc.h"
#include "Controller.h"

// Functions
void COMM_InternalCommutation(bool State);

bool COMM_IsPressureTrig();
bool COMM_IsSafetyTrig();
void COMM_EnableSafetyInput(bool State);

void COMM_TOSURaw(uint16_t Data);
void COMM_TOSU(AnodeVoltageEnum AnodeVoltage);

void COMM_PotSwitch(bool State);

#endif // __COMMUTATION_H

#ifndef __COMMUTATION_H
#define __COMMUTATION_H

#include "stdinc.h"
#include "Controller.h"

// Functions
void COMM_InternalCommutation(bool State);

bool COMM_IsPressureTrig();
bool COMM_IsSafetyTrig();

void COMM_TOSURaw(uint16_t Data);
void COMM_TOSU(AnodeVoltage Voltage);

#endif // __COMMUTATION_H

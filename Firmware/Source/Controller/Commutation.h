#ifndef __COMMUTATION_H
#define __COMMUTATION_H

#include "stdinc.h"

// Functions
void COMM_InternalCommutation(bool State);

bool COMM_IsPressureTrig();
bool COMM_IsSafetyTrig();

void COMM_TOSURaw(uint16_t Data);
void COMM_TOSU500();
void COMM_TOSU1000();
void COMM_TOSU1500();

#endif // __COMMUTATION_H

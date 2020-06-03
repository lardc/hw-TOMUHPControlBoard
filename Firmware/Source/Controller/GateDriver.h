#ifndef __GATEDRIVER_H
#define __GATEDRIVER_H

// Includes
#include "stdinc.h"

// Functions prototypes
void GateDriver_Sync(bool State);
void GateDriver_SetCurrent(uint16_t Data);
void GateDriver_SetCompThreshold(uint16_t Data);
void GateDriver_SetFallRate(uint16_t Data);
void GateDriver_SetRiseRate(uint16_t Data);

#endif // __GATEDRIVER_H

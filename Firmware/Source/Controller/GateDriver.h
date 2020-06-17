#ifndef __GATEDRIVER_H
#define __GATEDRIVER_H

// Includes
#include "stdinc.h"

// Functions prototypes
void GateDriver_Sync(bool State);
void GateDriver_SetCurrent(float GateCurrent);
void GateDriver_SetCompThreshold(float GateCurrentThreshold);
void GateDriver_SetFallRate(float GateCurrentFallRate);
void GateDriver_SetRiseRate(float GateCurrentRiseRate);

#endif // __GATEDRIVER_H

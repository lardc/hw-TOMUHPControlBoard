#ifndef __GATEDRIVER_H
#define __GATEDRIVER_H

// Includes
#include "stdinc.h"
#include "Controller.h"

// Functions prototypes
void GateDriver_Sync(bool State);
void GateDriver_SetCurrent(float GateCurrent);
void GateDriver_SetCompThreshold(float GateCurrentThreshold);
void GateDriver_SetFallRate(MeasurementSettings *Settings);
void GateDriver_SetRiseRate(MeasurementSettings *Settings);

#endif // __GATEDRIVER_H

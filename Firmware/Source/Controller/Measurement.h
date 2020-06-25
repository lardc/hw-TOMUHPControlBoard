#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

#include "stdinc.h"

// Definitions
#define PULSE_ARR_MAX_LENGTH	1000

// Variables
extern volatile uint16_t LOGIC_OutputPulseRaw[];

// Functions
void MEASURE_SetUref10(uint16_t Voltage);
void MEASURE_SetUref90(uint16_t Voltage);

#endif // __MEASUREMENT_H

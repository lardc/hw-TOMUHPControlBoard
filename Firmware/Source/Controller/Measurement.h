#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

// Includes
#include "stdinc.h"

// Definitions
#define PULSE_ARR_MAX_LENGTH	300

// Variables
extern volatile uint16_t LOGIC_OutputPulseRaw[PULSE_ARR_MAX_LENGTH];

// Functions
void MEASURE_SetUref10(uint16_t Voltage);
void MEASURE_SetUref90(uint16_t Voltage);

void MEASURE_ConvertRawArray(volatile uint16_t* RawArray, volatile uint16_t* OutputArray, uint16_t DataLength);
bool MEASURE_CheckAnodeCurrent();

#endif // __MEASUREMENT_H

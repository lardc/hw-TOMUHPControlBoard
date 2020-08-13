// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

//Definitions
//
#define NO						0
#define YES						1

#define DUT_VOLTAGE_MIN			500		// ¬
#define DUT_VOLTAGE_MAX			1500	// ¬
#define DUT_VOLTAGE_DEF			500		// ¬

#define DUT_CURRENT_MIN			100		// ј x10
#define DUT_CURRENT_MAX			1000	// ј x10
#define DUT_CURRENT_DEF			100		// ј x10

#define GATE_CURRENT_MIN		10		// A x10
#define GATE_CURRENT_MAX		200		// A x10
#define GATE_CURRENT_DEF		10		// A x10

#define GATE_FRONT_MIN			0		// мкс х10
#define GATE_FRONT_MAX			20		// мкс х10
#define GATE_FRONT_DEF			10		// мкс х10

#define GATE_I_RATE_MIN			1		// ј/мкс х10
#define GATE_I_RATE_MAX			100		// ј/мкс х10
#define GATE_I_RATE_DEF			10		// ј/мкс х10

#define GATE_TIME_CHRG_MIN		100		// мс
#define GATE_TIME_CHRG_MAX		1000	// мс
#define GATE_TIME_CHRG_DEF		500		// мс

#define CURRENT_SHUNT_MIN		1		// мќм х10
#define CURRENT_SHUNT_MAX		10000	// мќм х10
#define CURRENT_SHUNT_DEF		1000	// мќм х10

#define GAIN_COEFFICIENT_MIN	1
#define GAIN_COEFFICIENT_MAX	50000
#define GAIN_COEFFICIENT_DEF	1000

#define TOCU_CHRG_TIMEOUT_MIN	1000	// мс
#define TOCU_CHRG_TIMEOUT_MAX	60000	// мс
#define TOCU_CHRG_TIMEOUT_DEF	10000	// мс

#define TOCU_RES_PER_BIT_MIN	1		// ќм
#define TOCU_RES_PER_BIT_MAX	10000	// ќм
#define TOCU_RES_PER_BIT_DEF	1000	// ќм

#define DAC_VOLTAGE_MIN			0		// м¬
#define DAC_VOLTAGE_MAX			5000	// м¬
#define DAC_VOLTAGE_DEF			100		// м¬

#define ID_THRESHOLD_MIN		0		// %
#define ID_THRESHOLD_MAX		80		// %
#define ID_THRESHOLD_DEF		50		// %

#define MEAS_TIME_LOW_MIN		0		// нс
#define MEAS_TIME_LOW_MAX		1000	// нс
#define MEAS_TIME_LOW_DEF		500		// нс

// Types
//
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

// Variables
//
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];


#endif // __CONSTRAINTS_H

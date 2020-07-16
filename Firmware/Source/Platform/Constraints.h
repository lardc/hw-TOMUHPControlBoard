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

#define DUT_VOLTAGE_MIN			500		// Â
#define DUT_VOLTAGE_MAX			1500	// Â
#define DUT_VOLTAGE_DEF			500		// Â

#define DUT_CURRENT_MIN			100		// À x10
#define DUT_CURRENT_MAX			1000	// À x10
#define DUT_CURRENT_DEF			100		// À x10

#define GATE_CURRENT_MIN		10		// A x10
#define GATE_CURRENT_MAX		200		// A x10
#define GATE_CURRENT_DEF		10		// A x10

#define CURRENT_SHUNT_MIN		1		// ìÎì õ10
#define CURRENT_SHUNT_MAX		10000	// ìÎì õ10
#define CURRENT_SHUNT_DEF		1000	// ìÎì õ10

#define GAIN_COEFFICIENT_MIN	1
#define GAIN_COEFFICIENT_MAX	50000
#define GAIN_COEFFICIENT_DEF	1000

#define TOCU_CHRG_TIMEOUT_MIN	1000	// ìñ
#define TOCU_CHRG_TIMEOUT_MAX	60000	// ìñ
#define TOCU_CHRG_TIMEOUT_DEF	10000	// ìñ

#define TOCU_RES_PER_BIT_MIN	1		// Îì
#define TOCU_RES_PER_BIT_MAX	10000	// Îì
#define TOCU_RES_PER_BIT_DEF	1000	// Îì

#define DAC_VOLTAGE_MIN			0		// ìÂ
#define DAC_VOLTAGE_MAX			5000	// ìÂ
#define DAC_VOLTAGE_DEF			100		// ìÂ

#define GATE_I_RATE_MIN			1		// À/ìêñ õ10
#define GATE_I_RATE_MAX			100		// À/ìêñ õ10
#define GATE_I_RATE_DEF			10		// À/ìêñ õ10

#define ID_THRESHOLD_MIN		0		// %
#define ID_THRESHOLD_MAX		80		// %
#define ID_THRESHOLD_DEF		50		// %

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

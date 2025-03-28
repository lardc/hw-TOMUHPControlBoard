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

#define DUT_VOLTAGE_MIN			600		// В
#define DUT_VOLTAGE_MAX			1500	// В
#define DUT_VOLTAGE_DEF			600		// В

#define DUT_CURRENT_MIN			125		// А
#define DUT_CURRENT_MAX			2500	// А
#define DUT_CURRENT_DEF			125		// А

#define GATE_CURRENT_MIN		1000	// мA
#define GATE_CURRENT_MAX		20000	// мA
#define GATE_CURRENT_DEF		1000	// мA

#define GATE_FRONT_MIN			0		// мкс х10
#define GATE_FRONT_MAX			20		// мкс х10
#define GATE_FRONT_DEF			10		// мкс х10

#define GATE_I_RATE_MIN			1000	// мА/мкс
#define GATE_I_RATE_MAX			10000	// мА/мкс
#define GATE_I_RATE_DEF			5000	// мА/мкс

#define GATE_TIME_CHRG_MIN		100		// мс
#define GATE_TIME_CHRG_MAX		1000	// мс
#define GATE_TIME_CHRG_DEF		500		// мс

#define CURRENT_SHUNT_MIN		1		// мОм х10
#define CURRENT_SHUNT_MAX		10000	// мОм х10
#define CURRENT_SHUNT_DEF		1000	// мОм х10

#define GAIN_COEFFICIENT_MIN	1
#define GAIN_COEFFICIENT_MAX	50000
#define GAIN_COEFFICIENT_DEF	1000

#define TOCU_CHRG_TIMEOUT_MIN	1000	// мс
#define TOCU_CHRG_TIMEOUT_MAX	60000	// мс
#define TOCU_CHRG_TIMEOUT_DEF	10000	// мс

#define TOCU_RES_PER_BIT_MIN	1		// Ом
#define TOCU_RES_PER_BIT_MAX	10000	// Ом
#define TOCU_RES_PER_BIT_DEF	1000	// Ом

#define DAC_VOLTAGE_MIN			0		// мВ
#define DAC_VOLTAGE_MAX			5000	// мВ
#define DAC_VOLTAGE_DEF			100		// мВ

#define ID_THRESHOLD_MIN		0		// %
#define ID_THRESHOLD_MAX		80		// %
#define ID_THRESHOLD_DEF		50		// %

#define MEAS_TIME_LOW_MIN		0		// нс
#define MEAS_TIME_LOW_MAX		1000	// нс
#define MEAS_TIME_LOW_DEF		500		// нс

#define AVERAGE_NUM_MIN			1		//
#define AVERAGE_NUM_MAX			20		//
#define AVERAGE_NUM_DEF			10		//

#define ALLOWED_SPREAD_MIN		0		// нс
#define ALLOWED_SPREAD_MAX		500		// нс
#define ALLOWED_SPREAD_DEF		100		// нс

#define AVERAGE_PERIOD_MIN		300		// мс
#define AVERAGE_PERIOD_MAX		2000	// мс
#define AVERAGE_PERIOD_DEF		500		// мс

#define AFTER_MEASURE_DELAY_MIN	1000	// мс
#define AFTER_MEASURE_DELAY_MAX	10000	// мс
#define AFTER_MEASURE_DELAY_DEF	5000	// мс

#define REF_VOLTAGE_MIN			1000	// мв
#define REF_VOLTAGE_MAX			5000	// мв
#define REF_VOLTAGE_DEF			0		// мв
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

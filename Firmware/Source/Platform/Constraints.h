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

#define DUT_VOLTAGE_MIN			500		// �
#define DUT_VOLTAGE_MAX			1500	// �
#define DUT_VOLTAGE_DEF			500		// �

#define DUT_CURRENT_MIN			100		// � x10
#define DUT_CURRENT_MAX			1000	// � x10
#define DUT_CURRENT_DEF			100		// � x10

#define GATE_CURRENT_MIN		10		// A x10
#define GATE_CURRENT_MAX		200		// A x10
#define GATE_CURRENT_DEF		10		// A x10

#define GATE_FRONT_MIN			0		// ��� �10
#define GATE_FRONT_MAX			20		// ��� �10
#define GATE_FRONT_DEF			10		// ��� �10

#define GATE_I_RATE_MIN			1		// �/��� �10
#define GATE_I_RATE_MAX			100		// �/��� �10
#define GATE_I_RATE_DEF			10		// �/��� �10

#define GATE_TIME_CHRG_MIN		100		// ��
#define GATE_TIME_CHRG_MAX		1000	// ��
#define GATE_TIME_CHRG_DEF		500		// ��

#define CURRENT_SHUNT_MIN		1		// ��� �10
#define CURRENT_SHUNT_MAX		10000	// ��� �10
#define CURRENT_SHUNT_DEF		1000	// ��� �10

#define GAIN_COEFFICIENT_MIN	1
#define GAIN_COEFFICIENT_MAX	50000
#define GAIN_COEFFICIENT_DEF	1000

#define TOCU_CHRG_TIMEOUT_MIN	1000	// ��
#define TOCU_CHRG_TIMEOUT_MAX	60000	// ��
#define TOCU_CHRG_TIMEOUT_DEF	10000	// ��

#define TOCU_RES_PER_BIT_MIN	1		// ��
#define TOCU_RES_PER_BIT_MAX	10000	// ��
#define TOCU_RES_PER_BIT_DEF	1000	// ��

#define DAC_VOLTAGE_MIN			0		// ��
#define DAC_VOLTAGE_MAX			5000	// ��
#define DAC_VOLTAGE_DEF			100		// ��

#define ID_THRESHOLD_MIN		0		// %
#define ID_THRESHOLD_MAX		80		// %
#define ID_THRESHOLD_DEF		50		// %

#define MEAS_TIME_LOW_MIN		0		// ��
#define MEAS_TIME_LOW_MAX		1000	// ��
#define MEAS_TIME_LOW_DEF		500		// ��

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

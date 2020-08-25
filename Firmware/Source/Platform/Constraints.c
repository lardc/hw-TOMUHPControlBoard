// -----------------------------------------
// Global definitions
// ----------------------------------------

// Header
#include "Constraints.h"

// Constants
//
const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE] =
{
		{CURRENT_SHUNT_MIN, CURRENT_SHUNT_MAX, CURRENT_SHUNT_DEF},					// 0
		{CURRENT_SHUNT_MIN, CURRENT_SHUNT_MAX, CURRENT_SHUNT_DEF},					// 1
		{TOCU_CHRG_TIMEOUT_MIN, TOCU_CHRG_TIMEOUT_MAX, TOCU_CHRG_TIMEOUT_DEF},		// 2
		{TOCU_RES_PER_BIT_MIN, TOCU_RES_PER_BIT_MAX, TOCU_RES_PER_BIT_DEF},			// 3
		{ID_THRESHOLD_MIN, ID_THRESHOLD_MAX, ID_THRESHOLD_DEF},						// 4
		{MEAS_TIME_LOW_MIN, MEAS_TIME_LOW_MAX, MEAS_TIME_LOW_DEF},					// 5
		{0, INT16U_MAX, 0},															// 6
		{0, INT16U_MAX, 0},															// 7
		{GATE_I_RATE_MIN, GATE_I_RATE_MAX, GATE_I_RATE_DEF},						// 8
		{GATE_FRONT_MIN, GATE_FRONT_MAX, GATE_FRONT_DEF},							// 9
		{GATE_TIME_CHRG_MIN, GATE_TIME_CHRG_MAX, GATE_TIME_CHRG_DEF},				// 10
		{NO, YES, NO},																// 11
		{NO, YES, NO},																// 12
		{NO, YES, NO},																// 13
		{NO, YES, NO},																// 14
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 15
		{0, INT16U_MAX, 0},															// 16
		{0, INT16U_MAX, 0},															// 17
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 18
		{0, INT16U_MAX, 0},															// 19
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 20
		{0, INT16U_MAX, 0},															// 21
		{0, INT16U_MAX, 0},															// 22
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 23
		{0, INT16U_MAX, 0},															// 24
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 25
		{0, INT16U_MAX, 0},															// 26
		{0, INT16U_MAX, 0},															// 27
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 28
		{0, INT16U_MAX, 0},															// 29
		{DAC_VOLTAGE_MIN, DAC_VOLTAGE_MAX, DAC_VOLTAGE_DEF},						// 30
		{DAC_VOLTAGE_MIN, DAC_VOLTAGE_MAX, DAC_VOLTAGE_DEF},						// 31
		{DAC_VOLTAGE_MIN, DAC_VOLTAGE_MAX, DAC_VOLTAGE_DEF},						// 32
		{DAC_VOLTAGE_MIN, DAC_VOLTAGE_MAX, DAC_VOLTAGE_DEF},						// 33
		{DAC_VOLTAGE_MIN, DAC_VOLTAGE_MAX, DAC_VOLTAGE_DEF},						// 34
		{DAC_VOLTAGE_MIN, DAC_VOLTAGE_MAX, DAC_VOLTAGE_DEF},						// 35
		{0, INT16U_MAX, 0},															// 36
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 37
		{0, INT16U_MAX, 0},															// 38
		{0, INT16U_MAX, 0},															// 39
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 40
		{0, INT16U_MAX, 0},															// 41
		{0, INT16U_MAX, 0},															// 42
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 43
		{0, INT16U_MAX, 0},															// 44
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 45
		{0, INT16U_MAX, 0},															// 46
		{0, INT16U_MAX, 0},															// 47
		{GAIN_COEFFICIENT_MIN, GAIN_COEFFICIENT_MAX, GAIN_COEFFICIENT_DEF},			// 48
		{0, INT16U_MAX, 0},															// 49
		{0, 0, 0},																	// 50
		{0, 0, 0},																	// 51
		{0, 0, 0},																	// 52
		{0, 0, 0},																	// 53
		{0, 0, 0},																	// 54
		{0, 0, 0},																	// 55
		{0, 0, 0},																	// 56
		{0, 0, 0},																	// 57
		{0, 0, 0},																	// 58
		{0, 0, 0},																	// 59
		{0, 0, 0},																	// 60
		{0, 0, 0},																	// 61
		{0, 0, 0},																	// 62
		{0, 0, 0},															 		// 63
		{0, 0, 0},																	// 64
		{0, 0, 0},																	// 65
		{0, 0, 0},																	// 66
		{0, 0, 0}, 																	// 67
		{0, 0, 0},																	// 68
		{0, 0, 0},																	// 69
		{0, 0, 0},																	// 70
		{0, 0, 0},																 	// 71
	{0, 0, 0},																	// 72
	{0, 0, 0},																 	// 73
	{0, 0, 0},																	// 74
	{0, 0, 0},																	// 75
	{0, 0, 0},																	// 76
	{0, 0, 0},																	// 77
	{0, 0, 0},																	// 78
	{0, 0, 0},																	// 79
	{0, 0, 0},																	// 80
	{0, 0, 0},																 	// 81
	{0, 0, 0},																 	// 82
	{0, 0, 0},																	// 83
	{0, 0, 0},																 	// 84
	{0, 0, 0},																	// 85
	{0, 0, 0},																	// 86
	{0, 0, 0},																	// 87
	{0, 0, 0},																	// 88
	{0, 0, 0},																	// 89
	{0, 0, 0},																	// 90
	{0, 0, 0},																	// 91
	{0, 0, 0},																	// 92
	{0, 0, 0},																	// 93
	{0, 0, 0},																	// 94
	{0, 0, 0},																	// 95
	{0, 0, 0},																	// 96
	{0, 0, 0},																	// 97
	{0, 0, 0},																	// 98
	{0, 0, 0},																	// 99
	{0, 0, 0},																	// 100
	{0, 0, 0},																	// 101
	{0, 0, 0},																	// 102
	{0, 0, 0},																	// 103
	{0, 0, 0},																	// 104
	{0, 0, 0},																	// 105
	{0, 0, 0},																	// 106
	{0, 0, 0},																	// 107
	{0, 0, 0},																	// 108
	{0, 0, 0},																	// 109
	{0, 0, 0},																	// 110
	{0, 0, 0},																	// 111
	{0, 0, 0},																	// 112
	{0, 0, 0},																	// 113
	{0, 0, 0},																	// 114
	{0, 0, 0},																	// 115
	{0, 0, 0},																	// 116
	{0, 0, 0},																	// 117
	{0, 0, 0},																	// 118
	{0, 0, 0},																	// 119
	{0, 0, 0},																	// 120
	{0, 0, 0},																	// 121
	{0, 0, 0},																	// 122
	{0, 0, 0},																	// 123
	{0, 0, 0},																	// 124
	{0, 0, 0},																	// 125
	{0, 0, 0},																	// 126
	{0, 0, 0}																	// 127
};

const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START] =
{
	{DUT_VOLTAGE_MIN, DUT_VOLTAGE_MAX, DUT_VOLTAGE_DEF},			 			// 128
	{DUT_CURRENT_MIN, DUT_CURRENT_MAX, DUT_CURRENT_DEF},						// 129
	{GATE_CURRENT_MIN, GATE_CURRENT_MAX, GATE_CURRENT_DEF},					 	// 130
	{GATE_I_RATE_MIN, GATE_I_RATE_MAX, GATE_I_RATE_DEF},					 	// 131
	{0, 0, 0},																	// 132
	{0, 0, 0},																	// 133
	{0, 0, 0},																	// 134
	{0, 0, 0},																	// 135
	{0, 0, 0},																	// 136
	{0, 0, 0},																	// 137
	{0, 0, 0},																	// 138
	{0, 0, 0},																	// 139
	{0, 0, 0},																	// 140
	{0, 0, 0},													 				// 141
	{0, 0, 0},																	// 142
	{0, 0, 0},																	// 143
	{0, 0, 0},																	// 144
	{0, 0, 0},																	// 145
	{0, 0, 0},																	// 146
	{0, 0, 0},																	// 147
	{0, 0, 0},																	// 148
	{0, 0, 0},																	// 149
	{0, 0, 0},																 	// 150
	{0, 0, 0},																	// 151
	{0, 0, 0},																	// 152
	{0, 0, 0},																 	// 153
	{0, 0, 0},																	// 154
	{0, 0, 0},																	// 155
	{0, 0, 0},																	// 156
	{0, 0, 0},																	// 157
	{0, 0, 0},																	// 158
	{0, 0, 0},																	// 159
	{0, 0, 0},																	// 160
	{0, 0, 0},																	// 161
	{0, 0, 0},																	// 162
	{0, 0, 0},																	// 163
	{0, 0, 0},																	// 164
	{0, 0, 0},																	// 165
	{0, 0, 0},																	// 166
	{0, 0, 0},																	// 167
	{0, 0, 0},																	// 168
	{0, 0, 0},																	// 169
	{0, 0, 0},																	// 170
	{0, 0, 0},																	// 171
	{0, 0, 0},																	// 172
	{0, 0, 0},																	// 173
	{0, 0, 0},																	// 174
	{0, 0, 0},																	// 175
	{0, 0, 0},																	// 176
	{0, 0, 0},																	// 177
	{0, 0, 0},																	// 178
	{0, 0, 0},																	// 179
	{0, INT16U_MAX, 0},															// 180
	{0, INT16U_MAX, 0},															// 181
	{0, INT16U_MAX, 0},															// 182
	{0, INT16U_MAX, 0},															// 183
	{0, 0, 0},																	// 184
	{0, 0, 0},																	// 185
	{0, 0, 0},																	// 186
	{0, 0, 0},																	// 187
	{0, 0, 0},																	// 188
	{0, 0, 0},																	// 189
	{0, INT16U_MAX, 0},															// 190
	{0, 0, 0}																	// 191
};

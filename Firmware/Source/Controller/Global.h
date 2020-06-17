// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __GLOBAL_H
#define __GLOBAL_H

// Include
#include "stdinc.h"
#include "SysConfig.h"

#define	SCCI_TIMEOUT_TICKS		1000			// ������� ���������� SCCI (��)
#define EP_WRITE_COUNT			0				// ���������� �������� ��� ������
#define EP_COUNT				0				// ���������� �������� ��� ������
#define ENABLE_LOCKING			FALSE			// ������ �������

#define SLAVE_UPDATE_PERIOD		100				// ������ ������ ������-����� (��)
#define PRESSURE_FAULT_DELAY	100				// �������� ������������ ������ �� �������� (��)
#define LED_BLINK_PERIOD		500				// ������ �������� ���������� �� ����� (��)

#define GATE_CURRENT_THRESHOLD	0.1f			// ����� ������������ ����������� ���� (%)

#define ADC_RESOLUTION			4095
#define ADC_REF_MV				3000			// ������� ���������� ��� (��)

#define DAC_RESOLUTION			4095
#define DAC_REF_MV				5000			// ������� ���������� ��� (��)

#endif //  __GLOBAL_H

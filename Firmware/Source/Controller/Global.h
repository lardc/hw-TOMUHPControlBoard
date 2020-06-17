// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __GLOBAL_H
#define __GLOBAL_H

// Include
#include "stdinc.h"
#include "SysConfig.h"

#define	SCCI_TIMEOUT_TICKS		1000			// Таймаут протоколоа SCCI (мс)
#define EP_WRITE_COUNT			0				// Количество массивов для записи
#define EP_COUNT				0				// Количество массивов для чтения
#define ENABLE_LOCKING			FALSE			// Защита паролем

#define SLAVE_UPDATE_PERIOD		100				// Период опроса блоков-рабов (мс)
#define PRESSURE_FAULT_DELAY	100				// Задержка срабатывания аварии по давлению (мс)
#define LED_BLINK_PERIOD		500				// Период моргания светодиода на плате (мс)

#define GATE_CURRENT_THRESHOLD	0.1f			// Порог срабатывания компоратора тока (%)

#define ADC_RESOLUTION			4095
#define ADC_REF_MV				3000			// Опорное напряжение АЦП (мВ)

#define DAC_RESOLUTION			4095
#define DAC_REF_MV				5000			// Опорное напряжение ЦАП (мВ)

#endif //  __GLOBAL_H

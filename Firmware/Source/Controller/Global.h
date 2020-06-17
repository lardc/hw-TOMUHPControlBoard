// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __GLOBAL_H
#define __GLOBAL_H


// Include
#include "stdinc.h"
#include "SysConfig.h"


// Definitions
// 
// Global miscellaneous parameters
#define	SCCI_TIMEOUT_TICKS		1000			// in ms
//
#define EP_WRITE_COUNT			0				// Количество массивов для записи
#define EP_COUNT				0				// Количество массивов для чтения
// Password to unlock non-volatile area for write
#define ENABLE_LOCKING			FALSE
// -----------------------------------------------

// Временные параметры
#define T_SLAVE_UPDATE_PERIOD	100

#define LED_BLINK_TIME			500				// Период моргания светодиода на плате [ms]
#define DELAY_CONTACTOR			500				// Задержка включения контактора [мс]
#define	T_CHARGE_DELAY			60000			// Время ожидания заряда [мс]
#define	T_CHARGE_DELAY_SHORT	15000			// Время ожидания заряда в процессе измерения [мс]
#define	T_OSCILLATOR			10				// Время одного такта ВЧ генератора [нс]
#define T_TIMES_DELTA			100				// Время включения должно отличаться от времени задержки включения
												// как минимум на указанное время [нс]
#define T_IDLE_SEND_TOCU		200				// В состоянии простоя отправлять команду сброса в TOCU с периодом [мс]
//
// Параметры вентилятора
#define FAN_ACTIVE_TIME			30000			// Время работы вентилятора (мс)
#define FAN_SLEEP_TIME			180000			// Время повторного запуска вентилятора (мс)

// Параметры измерения заряда батареи
#define V_BAT_THRESHOLD_MIN		(299.0f)		// Нижний порог заряда батареи [В]
#define V_BAT_THRESHOLD_MAX		(302.0f)		// Верхний порог заряда батареи [В]

// Параметры контура тока
#define CURRENT_MIN_THRESHOLD	(50.0f)			// Минимальный порог тока (А)
#define CURRENT_BIT				(9.77f)			// Ток с одного мосфета [А] (токовый младший бит)
//
#define CURRENT_MAX_DISTORTION	(0.1f)			// Максимальное отклонение тока от уставки +-10%


#define GATE_CURRENT_THRESHOLD	0.1f


// Параметры оцифровки
#define ADC_RESOLUTION			4095
#define ADC_REF_MV				3000			// Опорное напряжение АЦП, мВ

#define DAC_RESOLUTION			4095
#define DAC_REF_MV				5000			// Опорное напряжение ЦАП, мВ
// -----------------------------------------------


#endif //  __GLOBAL_H

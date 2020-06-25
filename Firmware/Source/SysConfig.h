#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#include "stdinc.h"

// Flash loader options
#define BOOT_LOADER_VARIABLE			(*((volatile uint32_t *)0x20000000))
#define BOOT_LOADER_REQUEST				0x12345678
#define BOOT_LOADER_MAIN_PR_ADDR		0x08008000
//-----------------------------------------------

// System clock
#define SYSCLK							70000000	// Тактовая частота системной шины процессора
#define QUARTZ_FREQUENCY				20000000	// Частота кварца
// ----------------------------------------------

// USART
#define USART_BAUDRATE					115200		// Скорость USART
#define USART_FIFOlen					32			// Длина FIFO USART
// ----------------------------------------------

// Timers
#define TIMER3_uS						1000		// Период работы таймера общего назначения
#define TIMER6_uS						0.5f		// Период работы таймера тактирования АЦП
// ----------------------------------------------

// SPI
#define SPI1_BAUDRATE_BITS				0x5
#define SPI1_LSB_FIRST					false
#define SPI2_BAUDRATE_BITS				0x5
#define SPI2_LSB_FIRST					false
// ----------------------------------------------

// ADC
#define DMA_ADC_DUT_I_CHANNEL			DMA1_Channel1	// Канал DMA для АЦП тока
#define ADC1_CURRENT_CHANNEL			2			// АЦП1 номер канала тока
// ----------------------------------------------

// CAN
#define CAN_BAUDRATE					1000000		// Битрейт CAN
// ----------------------------------------------


#endif // __SYSCONFIG_H

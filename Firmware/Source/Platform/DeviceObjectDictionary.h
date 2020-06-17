#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning
#define ACT_RESET_DEVICE				5	// Перезапуск процессора

#define ACT_DBG_GD_I_SET				10	// Проверка установки амплитуды тока у TOMUHPGateDriver
#define ACT_DBG_GD_TRIG_THRESHOLD		11	// Проверка установки порога срабатывания компаратора
#define ACT_DBG_GD_I_RISE_RATE			12	// Проверка установки скорости нарастания тока у TOMUHPGateDriver
#define ACT_DBG_GD_I_FALL_RATE			13	// Проверка установки скорости спада тока у TOMUHPGateDriver
#define ACT_DBG_GD_SYNC					14	// Проверка выхода Sync_GD
#define ACT_DBG_PS_EN					15	// Проверка выхода PS_EN
#define ACT_DBG_FAN						16	// Проверка выхода FAN
#define ACT_DBG_IND						17	// Проверка выхода IND
#define ACT_DBG_RLC						18	// Проверка выхода RLC
#define ACT_DBG_RELAY					19	// Проверка выхода RELAY
#define ACT_DBG_SFTY_ACTIVATION			20	// Проверка выхода SFTY_EN
#define ACT_DBG_READ_EXT_REG			21	// Проверка чтения внешнего регистра
#define ACT_DBG_SNC_TOCU				22	// Проверка выхода SNC_TOCU
#define ACT_DBG_OUT_ISO					23	// Проверка изолированных выходов
#define ACT_DBG_U_REF_U10				24	// Проверка установки опорного напряжения для компаратора U10
#define ACT_DBG_U_REF_U90				25	// Проверка установки опорного напряжения для компаратора U90
#define ACT_DBG_OSC_SYNC				26	// Проверка выхода SYNC
#define ACT_DBG_TRIG_RST				27	// Проверка сброса триггера
#define ACT_DBG_M_RESET					28	// Проверка выхода M_RESET

#define ACT_MEASURE_START				100	// Запуск измерения
#define ACT_MEASURE_STOP				101	// Останов измерения

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------------------------

// Регистры
// Сохраняемые регистры
#define REG_DUT_CURRENT_SHUNT			0	// Сопротивление шунта (в мОм) (х1000)
#define REG_DUT_CURRENT_GAIN			1	// Коэффициент усиления аналогового тракта измерения тока (x100)
#define REG_DUT_CURRENT_OFFSET			2	// Офсет на входе АЦП при измерении тока (в тиках)
#define REG_P0_I_DUT					3	// Коэффициент P0 измеренного значения тока (в А)
#define REG_P1_I_DUT					4	// Коэффициент P1 измеренного значения тока (х1000)
#define REG_P2_I_DUT					5	// Коэффициент P2 измеренного значения тока (x1e6)
#define REG_TOCU_CHARGE_TIMEOUT			6
#define REG_TOCU_RES_PER_BIT			7
#define REG_MUTE_SAFETY_SYSTEM			8
// 9
#define REG_VCOMP10_500					10	// Напряжение на компараторе канала напряжения уровня 10% для 500В (в мв)
#define REG_VCOMP90_500					11	// Напряжение на компараторе канала напряжения уровня 90% для 500В (в мв)
#define REG_VCOMP10_1000				12	// Напряжение на компараторе канала напряжения уровня 10% для 1000В (в мв)
#define REG_VCOMP90_1000				13	// Напряжение на компараторе канала напряжения уровня 90% для 1000В (в мв)
#define REG_VCOMP10_1500				14	// Напряжение на компараторе канала напряжения уровня 10% для 1500В (в мв)
#define REG_VCOMP90_1500				15	// Напряжение на компараторе канала напряжения уровня 90% для 1500В (в мв)
// 16 - 19
#define REG_GD_CURRENT_SHUNT			20	// Сопротивление шунта в цепи упрвления (в мОм)
#define REG_GD_I_SET_K					21
#define REG_GD_I_SET_OFFSET				22
#define REG_GD_CURRENT_FALL_RATE		23
#define REG_GD_FALL_RATE_TO_DAC			24
#define REG_GD_RISE_RATE_TO_DAC			25
// - 127

// Несохраняемы регистры чтения-записи
#define REG_ANODE_VOLTAGE				128
#define REG_ANODE_CURRENT				129
#define REG_GATE_CURRENT				130
#define REG_GATE_CURRENT_RISE_RATE		131

#define REG_DBG_TOCU_DATA				150	// Сырое значение для отправки в TOCU

#define REG_DBG							190

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_TEST_FINISHED				197
#define REG_DEV_SUB_STATE				198
#define REG_SAFETY_STATE				199
//
#define REG_DBG_VSO_VALUE				200	// Напряжение на батарее (в В)
#define REG_DBG_I_DUT_VALUE				201	// Ток на DUT (в А)
#define REG_DBG_PRE_I_DUT_VALUE			202	// Предварительный ток на DUT (в А)
#define REG_DBG_ADC_RAW					203	// Измеренные сырые значения по каналу напряжения и каналу тока (в тиках)
//
#define REG_BHL_ERROR_CODE				210
#define REG_BHL_DEVICE					211
#define REG_BHL_FUNCTION				212
#define REG_BHL_EXT_DATA				213
//
#define REG_MEAS_CURRENT_VALUE			250	// Измеренное значение тока (А)
#define	REG_MEAS_TIME_DELAY				251	// Измеренное значение задержки включения (нс)
#define	REG_MEAS_TIME_ON				252	// Измеренное значение времени включения (нс)
//
// -----------------------------------------------


#define ACT_TOCU_ENABLE_POWER			1	// Переход в состояние ожидания
#define ACT_TOCU_DISABLE_POWER			2	// Отключение блока
#define ACT_TOCU_FAULT_CLEAR			3	// Очистка fault
#define ACT_TOCU_WARNING_CLEAR			4	// Очистка warning

#define ACT_TOCU_PULSE_CONFIG			100	// Конфигурация напряжения и настройка затворов транзисторов

// Несохраняемы регистры чтения-записи
#define REG_TOCU_VOLTAGE_SETPOINT		128	// Значение задания напряжения (в В)
#define REG_TOCU_GATE_REGISTER			129	// Значение-маска конфигурации затворов

// Регистры блоков TOCU только чтение
#define REG_TOCU_DEV_STATE				192	// Регистр состояния
#define REG_TOCU_FAULT_REASON			193	// Регистр Fault
#define REG_TOCU_DISABLE_REASON			194	// Регистр Disable
#define REG_TOCU_WARNING				195	// Регистр Warning
#define REG_TOCU_PROBLEM				196	// Регистр Problem
#define REG_TOCU_OP_RESULT				197	// Регистр результата операции


//  ENDPOINTS
//
// -----------------------------------------------


// Operation results
//
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
// 
#define DF_NONE							0
#define DF_INTERFACE					1
#define DF_TOCU_WRONG_STATE				2
#define DF_TOCU_CHARGE_TIMEOUT			3

// Problem
//
#define PROBLEM_NONE					0
#define PROBLEM_NO_CTRL_NO_PWR			1	// Отсутствие тока управления и силового тока
#define PROBLEM_NO_PWR					2	// Отсутствие силового тока
#define PROBLEM_SHORT					3	// КЗ на выходе
#define PROBLEM_NO_POT_SIGNAL			4	// Нет сигнала с потенциальной линии
#define PROBLEM_OVERFLOW90				5	// Переполнение счётчика уровня 90%
#define PROBLEM_OVERFLOW10				6	// Переполнение счётчика уровня 10%
#define PROBLEM_SAFETY_VIOLATION		7

//  Warning
// 
#define WARNING_NONE					0
#define WARNING_I_OUT_OF_RANGE			1	// Измеренное значение тока вне диапазона

//  User Errors
// 
#define ERR_NONE						0	//  Ошибок нет
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ

#endif //  __DEV_OBJ_DIC_H

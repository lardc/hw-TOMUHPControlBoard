#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning

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
#define ACT_DBG_SNC_TOCU				20	// Проверка выхода SNC_TOCU
#define ACT_DBG_OSC_SYNC				21	// Проверка выхода SYNC
#define ACT_DBG_U_REF_U10				22	// Проверка установки опорного напряжения для компаратора U10
#define ACT_DBG_U_REF_U90				23	// Проверка установки опорного напряжения для компаратора U90
#define ACT_DBG_READ_PRESSURE			24	// Проверка работы датчика давления
#define ACT_DBG_SFTY_ACTIVATION			25	// Проверка выхода SFTY_EN
#define ACT_DBG_OUT_ISO					26	// Проверка изолированных выходов
#define ACT_DBG_READ_EXT_REG			27	// Проверка чтения внешнего регистра
#define ACT_DBG_TRIG_RST				28	// Проверка сброса триггера
#define ACT_DBG_M_RESET					29	// Проверка выхода M_RESET

#define ACT_DBG_SLAVE_CALL				40	// Запуск команды на slave-узле
#define ACT_DBG_SLAVE_READ_REG			41	// Чтение регистра из slave-узла
#define ACT_DBG_SLAVE_WRITE_REG			42	// Запись регистра в slave-узел
#define ACT_DBG_RESET_INT_ERROR			43	// Сброс ошибки master-интерфейса

#define ACT_MEASURE_START				100	// Запуск измерения
#define ACT_MEASURE_STOP				101	// Остановка измерения

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------------------------

// Регистры
// Сохраняемые регистры
#define REG_DUT_CURRENT_SHUNT			0	// Сопротивление шунта (мОм) (х10)
#define REG_DUT_CURRENT_GAIN			1	// Коэффициент усиления аналогового тракта измерения тока (x100)
#define REG_DUT_CURRENT_OFFSET			2	// Офсет на входе АЦП при измерении тока (тиках)
#define REG_P0_I_DUT					3	// Коэффициент P0 измеренного значения тока (А)
#define REG_P1_I_DUT					4	// Коэффициент P1 измеренного значения тока (х1000)
#define REG_P2_I_DUT					5	// Коэффициент P2 измеренного значения тока (x1e6)
#define REG_TOCU_CHARGE_TIMEOUT			6	// Таймаут ожидания заряда блока TOCU (мс)
#define REG_TOCU_RES_PER_BIT			7	// Сопротивление одного бита блока TOCU (Ом)
#define REG_MUTE_SAFETY_MONITOR			8	// Отключение мониторинга системы безопасности
#define REG_MUTE_PRESSURE_MONITOR		9	// Отключение мониторинга давления в системе
#define REG_VCOMP10_500					10	// Напряжение на компараторе канала напряжения уровня 10% для 500В (мВ)
#define REG_VCOMP90_500					11	// Напряжение на компараторе канала напряжения уровня 90% для 500В (мВ)
#define REG_VCOMP10_1000				12	// Напряжение на компараторе канала напряжения уровня 10% для 1000В (мВ)
#define REG_VCOMP90_1000				13	// Напряжение на компараторе канала напряжения уровня 90% для 1000В (мВ)
#define REG_VCOMP10_1500				14	// Напряжение на компараторе канала напряжения уровня 10% для 1500В (мВ)
#define REG_VCOMP90_1500				15	// Напряжение на компараторе канала напряжения уровня 90% для 1500В (мВ)
#define MAX_ANODE_CURRENT				16	// Максимально допустимый анодный ток до отпирания прибора (А)
// 16 - 19
#define REG_GD_CURRENT_SHUNT			20	// Сопротивление шунта в цепи упрвления (мОм) (x10)
#define REG_GD_I_SET_K					21	// Пересчёт тока в значение ЦАП пропорциональный коэффициент (х1000)
#define REG_GD_I_SET_OFFSET				22	// Пересчёт тока в значение ЦАП смещение
#define REG_GD_CURRENT_FALL_RATE		23	// Скорость спада тока управления (А/мкс) (x10)
#define REG_GD_FALL_RATE_TO_DAC			24	// Пропорциональный коэффициент пересчёта скорости спада тока в ЦАП (х1000)
#define REG_GD_RISE_RATE_TO_DAC			25	// Пропорциональный коэффициент пересчёта скорости нарастания тока в ЦАП (х1000)
// - 127

// Несохраняемы регистры чтения-записи
#define REG_ANODE_VOLTAGE				128	// Анодное напряжение (В)
#define REG_ANODE_CURRENT				129	// Анодный ток (А)
#define REG_GATE_CURRENT				130	// Ток цепи управления (А) (х10)
#define REG_GATE_CURRENT_RISE_RATE		131	// Скорость нарастания тока цепи управления (А/мкс) (х10)

#define REG_DBG_SLAVE_NID				180	// Node ID slave-узла
#define REG_DBG_SLAVE_REG				181	// Номер регистра slave-узла
#define REG_DBG_SLAVE_DATA				182	// Данные регистра (записываемые или вычитываемые) slave-узла
#define REG_DBG_SLAVE_ACT				183	// Команда для slave-узла

#define REG_DBG							190	// Отладочный регистр

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр статуса операции
#define REG_DEV_SUB_STATE				198	// Регистр вспомогательного состояния
//
#define REG_SAFETY_STATE				200	// Состояние датчика безопасности
#define REG_PRESSURE_STATE				201	// Состояние датчика давления
//
#define REG_BHL_ERROR_CODE				210	// Ошибка интерфейса ВУ: код ошибки
#define REG_BHL_DEVICE					211	// Ошибка интерфейса ВУ: NID устройства
#define REG_BHL_FUNCTION				212	// Ошибка интерфейса ВУ: код функции
#define REG_BHL_EXT_DATA				213	// Ошибка интерфейса ВУ: расширенная информация
//
#define REG_MEAS_CURRENT_VALUE			250	// Измеренное значение тока (А)
#define	REG_MEAS_TIME_DELAY				251	// Измеренное значение задержки включения (нс)
#define	REG_MEAS_TIME_ON				252	// Измеренное значение времени включения (нс)
// -----------------------------------------------

// Секция регистров и команд блоков TOCU
#define ACT_TOCU_ENABLE_POWER			1	// Переход в состояние ожидания
#define ACT_TOCU_DISABLE_POWER			2	// Отключение блока
#define ACT_TOCU_FAULT_CLEAR			3	// Очистка fault
#define ACT_TOCU_WARNING_CLEAR			4	// Очистка warning
#define ACT_TOCU_PULSE_CONFIG			100	// Конфигурация напряжения и настройка затворов транзисторов

#define REG_TOCU_VOLTAGE_SETPOINT		128	// Значение задания напряжения (В)
#define REG_TOCU_GATE_REGISTER			129	// Значение-маска конфигурации затворов

#define REG_TOCU_DEV_STATE				192	// Регистр состояния
#define REG_TOCU_FAULT_REASON			193	// Регистр Fault
#define REG_TOCU_DISABLE_REASON			194	// Регистр Disable
#define REG_TOCU_WARNING				195	// Регистр Warning
#define REG_TOCU_PROBLEM				196	// Регистр Problem
#define REG_TOCU_OP_RESULT				197	// Регистр результата операции
// -----------------------------------------------

// Массивы
#define EP_CURRENT						1	// Массив значений тока
// -----------------------------------------------

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0
#define DF_INTERFACE					1	// Ошибка мастер-интерфейса
#define DF_PRESSURE						2	// Отсутствует давление
#define DF_TOCU_WRONG_STATE				3	// Неверное состояние блока TOCU
#define DF_TOCU_STATE_TIMEOUT			4	// Таймаут ожидания перехода TOCU в другое состояние

// Problem
#define PROBLEM_NONE					0
#define PROBLEM_NO_CTRL_NO_PWR			1	// Отсутствие тока управления и силового тока
#define PROBLEM_NO_PWR					2	// Отсутствие силового тока
#define PROBLEM_SHORT					3	// КЗ на выходе
#define PROBLEM_NO_POT_SIGNAL			4	// Нет сигнала с потенциальной линии
#define PROBLEM_OVERFLOW90				5	// Переполнение счётчика уровня 90%
#define PROBLEM_OVERFLOW10				6	// Переполнение счётчика уровня 10%
#define PROBLEM_SAFETY_VIOLATION		7	// Срабатывание контура безопасности

//  Warning
#define WARNING_NONE					0
#define WARNING_I_OUT_OF_RANGE			1	// Измеренное значение тока вне диапазона

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ

#endif //  __DEV_OBJ_DIC_H

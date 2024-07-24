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
#define ACT_DBG_TOSU_SWITCH				21	// Проверка коммутации TOSU
#define ACT_DBG_POT_SWITCH				22	// Проверка управления PotSwitch

#define ACT_DBG_U_REF_U10				24	// Проверка установки опорного напряжения для компаратора U10
#define ACT_DBG_U_REF_U90				25	// Проверка установки опорного напряжения для компаратора U90
#define ACT_DBG_READ_PRESSURE			26	// Проверка работы датчика давления
#define ACT_DBG_SFTY_ACTIVATION			27	// Проверка выхода SFTY_EN
#define ACT_DBG_OUT_ISO					28	// Проверка изолированных выходов
#define ACT_DBG_READ_EXT_REG			29	// Проверка чтения внешнего регистра
#define ACT_DBG_TRIG_RST				30	// Проверка сброса триггера
#define ACT_DBG_M_RESET					31	// Проверка выхода M_RESET


#define ACT_DBG_SLAVE_CALL				40	// Запуск команды на slave-узле
#define ACT_DBG_SLAVE_READ_REG			41	// Чтение регистра из slave-узла
#define ACT_DBG_SLAVE_WRITE_REG			42	// Запись регистра в slave-узел
#define ACT_DBG_RESET_INT_ERROR			43	// Сброс ошибки master-интерфейса

#define ACT_MEASURE_START				100	// Запуск измерения
#define ACT_MEASURE_STOP				101	// Остановка измерения
//
#define ACT_GATE_PULSE					110	// Формирование тока управления

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_READ_SYMBOL					330	// Выполнить чтение символа из памяти
#define ACT_SELECT_MEM_LABEL			331	// Переместить указатель считывания в область метки

#define ACT_FLASH_SAVE					332	// Flash save fault data
#define ACT_FLASH_ERASE					333	// Flash erase data sector

#define ACT_SELECT_COUNTER_LABEL		334	// Перемещение указателя в область счетчиков
#define ACT_READ_COUNTER_SYMBOL			335	// Считывания символа в области счетчиков
#define ACT_COUNTER_SAVE				336	// Сохранение наработки счетчиков во флеш

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------------------------

// Регистры
// Сохраняемые регистры
#define REG_DUT_CURRENT_SHUNT			0	// Сопротивление шунта (мОм) (х10)
#define REG_GATE_CURRENT_SHUNT			1	// Сопротивление шунта в цепи упрвления (мОм)
//
#define REG_TOCU_CHARGE_TIMEOUT			2	// Таймаут ожидания заряда блока TOCU (мс)
#define REG_TOCU_RES_PER_BIT			3	// Сопротивление одного бита блока TOCU (Ом)
#define REG_ID_THRESHOLD				4	// Порог определения наличия анодного тока (% от REG_ANODE_CURRENT)
#define REG_MEAS_TIME_LOW				5	// Минимальное измеряемое значение времени (нс)
#define REG_FAN_OPERATE_TIME			6	// Время работы вентилятора (с)
#define REG_FAN_OPERATE_PERIOD			7	// Период работы вентилятора
#define REG_GATE_I_FALL_RATE			8	// Скорость спада тока управления (А/мкс) (x10)
#define REG_GATE_EDGE_TIME_MIN			9	// Минимальная длительность фронта тока управления (мкс) (х10)
#define REG_GATE_TIME_CHARGE			10	// Время заряда GateDriver (мс)
//
#define REG_MUTE_SAFETY_MONITOR			11	// Отключение мониторинга системы безопасности
#define REG_MUTE_PRESSURE_MONITOR		12	// Отключение мониторинга давления в системе
#define REG_MUTE_PROBLEM				13	// Контроль ошибок (0 - вкл, 1 - выкл)
#define REG_MUTE_COMMUTATION			14	// Режим при постоянно включенной коммутации
//
#define REG_GATE_I_SET_K				15	// Пересчёт тока в значение ЦАП пропорциональный коэффициент
#define REG_GATE_I_SET_OFFSET			16	// Пересчёт тока в значение ЦАП смещение
#define REG_GATE_I_SET_P2				17	// Калибровочный коэффициент P2 задания тока управления
#define REG_GATE_I_SET_P1				18	// Калибровочный коэффициент P1 задания тока управления
#define REG_GATE_I_SET_P0				19	// Калибровочный коэффициент P0 задания тока управления
//
#define REG_GATE_I_RISE_RATE_K			20	// Пересчёт скорости нарастания тока в значение ЦАП пропорциональный коэффициент
#define REG_GATE_I_RISE_RATE_OFFSET		21	// Пересчёт скорости нарастания тока в значение ЦАП смещение
#define REG_GATE_I_RISE_RATE_P2			22	// Калибровочный коэффициент P2 скорости нарастания тока управления
#define REG_GATE_I_RISE_RATE_P1			23	// Калибровочный коэффициент P1 скорости нарастания тока управления
#define REG_GATE_I_RISE_RATE_P0			24	// Калибровочный коэффициент P0 скорости нарастания тока управления
//
#define REG_GATE_I_FALL_RATE_K			25	// Пересчёт скорости спада тока в значение ЦАП пропорциональный коэффициент
#define REG_GATE_I_FALL_RATE_OFFSET		26	// Пересчёт скорости спада тока в значение ЦАП смещение
//
#define REG_GATE_COMP_THRE_P2			27	// Калибровочный коэффициент P2 задания уровня срабатывания компаратора
#define REG_GATE_COMP_THRE_P1			28	// Калибровочный коэффициент P1 задания уровня срабатывания компаратора
#define REG_GATE_COMP_THRE_P0			29	// Калибровочный коэффициент P0 задания уровня срабатывания компаратора
//
#define REG_VCOMP10_600V				30	// Напряжение на компараторе канала напряжения уровня 10% для 500В (мВ)
#define REG_VCOMP90_600V				31	// Напряжение на компараторе канала напряжения уровня 90% для 500В (мВ)
#define REG_VCOMP10_1000V				32	// Напряжение на компараторе канала напряжения уровня 10% для 1000В (мВ)
#define REG_VCOMP90_1000V				33	// Напряжение на компараторе канала напряжения уровня 90% для 1000В (мВ)
#define REG_VCOMP10_1500V				34	// Напряжение на компараторе канала напряжения уровня 10% для 1500В (мВ)
#define REG_VCOMP90_1500V				35	// Напряжение на компараторе канала напряжения уровня 90% для 1500В (мВ)
//
#define REG_I_DUT_600V_P2				36	// Калибровочный коэффициент P2 задания тока при напряжении 600В
#define REG_I_DUT_600V_P1				37	// Калибровочный коэффициент P1 задания тока при напряжении 600В
#define REG_I_DUT_600V_P0				38	// Калибровочный коэффициент P0 задания тока при напряжении 600В
//
#define REG_I_DUT_1000V_P2				39	// Калибровочный коэффициент P2 задания тока при напряжении 1000В
#define REG_I_DUT_1000V_P1				40	// Калибровочный коэффициент P1 задания тока при напряжении 1000В
#define REG_I_DUT_1000V_P0				41	// Калибровочный коэффициент P0 задания тока при напряжении 1000В
//
#define REG_I_DUT_1500V_P2				42	// Калибровочный коэффициент P2 задания тока при напряжении 1500В
#define REG_I_DUT_1500V_P1				43	// Калибровочный коэффициент P1 задания тока при напряжении 1500В
#define REG_I_DUT_1500V_P0				44	// Калибровочный коэффициент P0 задания тока при напряжении 1500В
//
#define REG_I_DUT_GAIN					45	// Коэффициент усиления аналогового тракта измерения тока (x100)
#define REG_I_DUT_OFFSET				46	// Офсет на входе АЦП при измерении тока (тиках)
#define REG_P0_I_DUT					47	// Коэффициент P0 измеренного значения тока (А)
#define REG_P1_I_DUT					48	// Коэффициент P1 измеренного значения тока (х1000)
#define REG_P2_I_DUT					49	// Коэффициент P2 измеренного значения тока (x1e6)
//
#define REG_GATE_I_REF_K				50	// Пересчёт уровня компаратора в значение ЦАП пропорциональный коэффициент
#define REG_GATE_I_REF_OFFSET			51	// Пересчёт уровня компаратора в значение ЦАП смещение
#define REG_GATE_I_REF_COMPENSATION		52	// Компенсация сдвига уровня компаратора от скорости нарастания тока (%)
// 53 - 59
#define	REG_T_DEL_600V_P2				60	// Калибровочный коэффициент P2 измерения Tdel при напряжении 600В
#define	REG_T_DEL_600V_P1				61	// Калибровочный коэффициент P1 измерения Tdel при напряжении 600В
#define	REG_T_DEL_600V_P0				62	// Калибровочный коэффициент P0 измерения Tdel при напряжении 600В
//
#define	REG_T_DEL_1000V_P2				63	// Калибровочный коэффициент P2 измерения Tdel при напряжении 1000В
#define	REG_T_DEL_1000V_P1				64	// Калибровочный коэффициент P1 измерения Tdel при напряжении 1000В
#define	REG_T_DEL_1000V_P0				65	// Калибровочный коэффициент P0 измерения Tdel при напряжении 1000В
//
#define	REG_T_DEL_1500V_P2				66	// Калибровочный коэффициент P2 измерения Tdel при напряжении 1500В
#define	REG_T_DEL_1500V_P1				67	// Калибровочный коэффициент P1 измерения Tdel при напряжении 1500В
#define	REG_T_DEL_1500V_P0				68	// Калибровочный коэффициент P0 измерения Tdel при напряжении 1500В
// 69
#define	REG_T_ON_600V_P2				70	// Калибровочный коэффициент P2 измерения Ton при напряжении 600В
#define	REG_T_ON_600V_P1				71	// Калибровочный коэффициент P1 измерения Ton при напряжении 600В
#define	REG_T_ON_600V_P0				72	// Калибровочный коэффициент P0 измерения Ton при напряжении 600В
//
#define	REG_T_ON_1000V_P2				73	// Калибровочный коэффициент P2 измерения Ton при напряжении 1000В
#define	REG_T_ON_1000V_P1				74	// Калибровочный коэффициент P1 измерения Ton при напряжении 1000В
#define	REG_T_ON_1000V_P0				75	// Калибровочный коэффициент P0 измерения Ton при напряжении 1000В
//
#define	REG_T_ON_1500V_P2				76	// Калибровочный коэффициент P2 измерения Ton при напряжении 1500В
#define	REG_T_ON_1500V_P1				77	// Калибровочный коэффициент P1 измерения Ton при напряжении 1500В
#define	REG_T_ON_1500V_P0				78	// Калибровочный коэффициент P0 измерения Ton при напряжении 1500В
// 79 - 80
#define REG_AVERAGE_NUM					80	// Число точек усреднений
#define REG_AVERAGE_ALLOWED_SPREAD		81	// Допустимый разброс значений (нс)
#define REG_AVERAGE_PERIOD				82	// Период следования импульсов при усреднении (мс)
// 83 - 84
#define REG_AFTER_MEASURE_DELAY			85	// Пауза после измерения

// Несохраняемы регистры чтения-записи
#define REG_ANODE_VOLTAGE				128	// Анодное напряжение (В)
#define REG_ANODE_CURRENT				129	// Анодный ток (А) (х10)
#define REG_GATE_CURRENT				130	// Ток цепи управления (mА)
#define REG_GATE_CURRENT_RISE_RATE		131	// Скорость нарастания тока цепи управления (мА/мкс) (х10)

#define REG_DBG_SLAVE_NID				180	// Node ID slave-узла
#define REG_DBG_SLAVE_REG				181	// Номер регистра slave-узла
#define REG_DBG_SLAVE_DATA				182	// Данные регистра (записываемые или вычитываемые) slave-узла
#define REG_DBG_SLAVE_ACT				183	// Команда для slave-узла

#define REG_DBG							190	// Отладочный регистр 1

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
#define ACT_TOCU_VOLTAGE_CONFIG			100	// Конфигурация напряжения блока
#define ACT_TOCU_PULSE_CONFIG			101	// Конфигурация блока под требуемый ток

#define REG_TOCU_VOLTAGE_SETPOINT		128	// Значение задания напряжения (В)
#define REG_TOCU_GATE_REGISTER			129	// Значение-маска конфигурации затворов

#define REG_TOCU_DEV_STATE				192	// Регистр состояния
#define REG_TOCU_FAULT_REASON			193	// Регистр Fault
#define REG_TOCU_DISABLE_REASON			194	// Регистр Disable
#define REG_TOCU_WARNING				195	// Регистр Warning
#define REG_TOCU_PROBLEM				196	// Регистр Problem
#define REG_TOCU_OP_RESULT				197	// Регистр результата операции
// -----------------------------------------------
#define REG_FWINFO_SLAVE_NID			256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID			257	// Device CAN master node ID (if presented)
// 258 - 259
#define REG_FWINFO_STR_LEN				260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN			261	// Begining of the information string record

#define REG_MEM_SYMBOL					299	// Считанный по адресу памяти символ

// Массивы
#define EP_CURRENT						1	// Массив значений тока
#define EP_TURN_DELAY					2	// Массив значений времени задержки включения
#define EP_TURN_ON						3	// Массив значений времени включения
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
#define PROBLEM_NO_PWR					1	// Отсутствие силового тока
#define PROBLEM_SHORT					2	// КЗ на выходе
#define PROBLEM_NO_POT					3	// Нет сигнала с потенциальной линии
#define PROBLEM_OVERFLOW90				4	// Переполнение счётчика уровня 90%
#define PROBLEM_OVERFLOW10				5	// Переполнение счётчика уровня 10%
#define PROBLEM_SAFETY_VIOLATION		6	// Срабатывание контура безопасности

//  Warning
#define WARNING_NONE					0

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ

#endif //  __DEV_OBJ_DIC_H

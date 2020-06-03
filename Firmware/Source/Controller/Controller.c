//-----------------------------------------------
// Основная логика
//-----------------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "SysConfig.h"
#include "DataTable.h"
#include "SCCISlave.h"
#include "DeviceProfile.h"
#include "BCCITypes.h"
#include "Board.h"
#include "GateDriver.h"
#include "Diagnostic.h"
#include "Interrupts.h"
#include "Global.h"
#include "LowLevel.h"
#include "InitConfig.h"
#include "Measurement.h"
#include "Delay.h"


// Types
//
typedef void (*FUNC_AsyncDelegate)();
//
// Вспомогательные состояния
typedef enum __SubState
{
	SS_None				= 0,	// Неопределённое
	SS_StopProcess		= 1,	// Запрос остановки процесса
	SS_WaitVoltage		= 2,	// Ожидание выхода на заданное напряжение
	SS_VoltageReady		= 3,	// Готовность к измерению
	SS_WaitContactor	= 4		// Ожидание срабатывания контактора
} SubState;


// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile SubState SUB_State = SS_None;
static Boolean CycleActive = FALSE;
//
volatile Int64U CONTROL_TimeCounter = 0;
static Int64U CONTROL_TimeCounterDelay = 0;
static Int64U CONTROL_TimeFanLastTurnOn = 0;
static Int64U CONTROL_TimeIdleSendTOCU = 0;


// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState);
void CONTROL_BatteryVoltageMonitor();
void CONTROL_Logic();
void Delay_mS(uint32_t Delay);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_ResetToDefaults();
void CONTROL_WatchDogUpdate();
uint16_t ReadExtReg();
void OutPut_ISO(uint16_t Data);
void Delay_us(uint32_t Time);


// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT];
	Int16U EPSized[EP_COUNT];
	pInt16U EPCounters[EP_COUNT];
	pInt16U EPDatas[EP_COUNT];

	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = { (FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT };
	// Инициализация data table
	DT_Init(EPROMService, FALSE);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	//
	CONTROL_SetDeviceState(DS_None);
	CONTROL_ResetToDefaults();
	//
	// Настройка TOCU
	//CUSTINT_SendTOCU(0, FALSE, FALSE, FALSE);
}
//------------------------------------------------------------------------------

void CONTROL_ResetToDefaults()
{
	DataTable[REG_FAULT_REASON] = 0;
	DataTable[REG_DISABLE_REASON] = 0;
	DataTable[REG_WARNING] = 0;
	DataTable[REG_PROBLEM] = 0;
	DataTable[REG_TEST_FINISHED] = OPRESULT_NONE;
	//
	DataTable[REG_MEAS_CURRENT_VALUE] = 0;
	DataTable[REG_MEAS_TIME_DELAY] = 0;
	DataTable[REG_MEAS_TIME_ON] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------------------------------------------

void CONTROL_Idle()
{
	CONTROL_WatchDogUpdate();
	DEVPROFILE_ProcessRequests();
	//
	CONTROL_BatteryVoltageMonitor();
	CONTROL_Logic();
}
//-----------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;

	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if ((CONTROL_State == DS_None) || (CONTROL_State == DS_Charging) || (CONTROL_State == DS_Ready))
				{
					if (CONTROL_State == DS_None)
					{
						CONTROL_TimeFanLastTurnOn = CONTROL_TimeCounter;
						LL_ExternalFan(TRUE);
						//CUSTINT_SendTOCU(0, TRUE, FALSE, TRUE);
						//
						CONTROL_TimeCounterDelay = CONTROL_TimeCounter + T_CHARGE_DELAY;
						CONTROL_SetDeviceState(DS_Charging);
					}
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_DISABLE_POWER:
			{
				if ((CONTROL_State == DS_None) || (CONTROL_State == DS_Charging) || (CONTROL_State == DS_Ready))
				{
					LL_ExternalFan(FALSE);
					//CUSTINT_SendTOCU(0, FALSE, FALSE, FALSE);
					//
					CONTROL_SetDeviceState(DS_None);
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_MEASURE_START:
			{
				if (CONTROL_State == DS_Ready)
				{
					CONTROL_TimeFanLastTurnOn = CONTROL_TimeCounter;
					LL_ExternalLED(TRUE);
					CONTROL_ResetToDefaults();
					//
					CONTROL_TimeCounterDelay = CONTROL_TimeCounter + T_CHARGE_DELAY_SHORT;
					SUB_State = SS_WaitVoltage;
					CONTROL_SetDeviceState(DS_InProcess);
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_MEASURE_STOP:
			{
				if (CONTROL_State == DS_InProcess)
				{
					SUB_State = SS_StopProcess;
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_FAULT_CLEAR:
			{
				if (CONTROL_State == DS_Fault)
				{
					CONTROL_ResetToDefaults();
					CONTROL_SetDeviceState(DS_None);
				}
			}
			break;

		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = 0;
			break;

		default:
			return DIAG_HandleDiagnosticAction(ActionID, pUserError);
	}

	return TRUE;
}
//-----------------------------------------------

void CONTROL_Logic()
{
	float CurrentSetpoint, CurrentPreActual, CurrentActual;
	uint32_t Counter10Percent, Counter90Percent;
	uint16_t Problem = PROBLEM_NONE, Warning = WARNING_NONE;

	if (CONTROL_State == DS_InProcess)
	{
		if (SUB_State == SS_StopProcess)
		{
			//CUSTINT_SendTOCU(0, TRUE, FALSE, TRUE);
			LL_RelayControl(FALSE);
			LL_ExternalFan(FALSE);
			LL_ExternalLED(FALSE);

			SUB_State = SS_None;
			CONTROL_SetDeviceState(DS_Ready);
		}

		// Выполнение необходимой коммутации
		if (SUB_State == SS_VoltageReady)
		{
			LL_GateLatch(FALSE);
			LL_RelayControl(TRUE);									// Замыкание выходных реле
			//CUSTINT_SendTOCU(0, TRUE, TRUE, FALSE);					// Отключение PSBoard + замыкание контактора
			CONTROL_TimeCounterDelay = CONTROL_TimeCounter + DELAY_CONTACTOR;

			SUB_State = SS_WaitContactor;
		}

		// Непосредственный запуск измерения
		if ((SUB_State == SS_WaitContactor) && (CONTROL_TimeCounter > CONTROL_TimeCounterDelay))
		{
			CurrentSetpoint = DataTable[REG_CURRENT_VALUE];

			//CUSTINT_SendTOCU(CurrentSetpoint, TRUE, TRUE, FALSE);	// Отпирание нужных мосфетов
			DELAY_US(50);
			CurrentPreActual = MEASURE_DUTCurrent();				// Измерения тока до подачи сигнала управления для определения кз на выходе
			DELAY_US(10);

			LL_GateLatch(TRUE);										// Включение защёлки сигнала управления
			//LL_TimersReset(FALSE);									// Активация счётчиков
			Overflow90 = FALSE;
			Overflow10 = FALSE;

			//LL_GateControl(TRUE);									// Запуск тока управления
			//LL_ExternalSync(TRUE);
			DELAY_US(50);
			//CountersData = CUSTINT_ReceiveDataSR();					// Считывание сырых значений из системы счета времени
			CurrentActual = MEASURE_DUTCurrent();					// Измерение тока через прибор
			DELAY_US(10);

			//LL_GateControl(FALSE);									// Отключение тока управления
			//LL_TimersReset(TRUE);									// Сброс системы измерения времени
			//LL_ExternalSync(FALSE);
			LL_GateLatch(FALSE);									// Сброс защёлки
			DELAY_US(10);
			//CUSTINT_SendTOCU(0, TRUE, FALSE, FALSE);				// Закрытие силовых мосфетов + размыкание контактора
			LL_RelayControl(FALSE);									// Размыкание реле

			// Получение времён из счётчиков
			Counter10Percent = //CUSTINT_UnpackData10SR(CountersData);
			Counter90Percent = //CUSTINT_UnpackData90SR(CountersData);

			// Запись отладочных результатов по току
			DataTable[REG_DBG_I_DUT_VALUE] = CurrentActual;
			DataTable[REG_DBG_PRE_I_DUT_VALUE] = CurrentPreActual;

			// Обработка внештатных ситуаций
			/*
			if ((//CUSTINT_UnpackData90SR(CountersData) == 0) &&
				(//CUSTINT_UnpackData10SR(CountersData) == 0) && (CurrentPreActual < CURRENT_MIN_THRESHOLD))
			{
				Problem = PROBLEM_NO_CTRL_NO_PWR;
			}
			else if (Overflow90 && Overflow10)
			{
				Problem = PROBLEM_NO_PWR;
			}
			else if (CurrentPreActual > CURRENT_MIN_THRESHOLD)
			{
				Problem = PROBLEM_SHORT;
			}
			else if ((Counter90Percent + T_TIMES_DELTA) > Counter10Percent)
			{
				Problem = PROBLEM_NO_POT_SIGNAL;
			}
			else if (Overflow90)
			{
				Problem = PROBLEM_OVERFLOW90;
			}
			else if (Overflow10)
			{
				Problem = PROBLEM_OVERFLOW10;
			}
			*/

			// Запись результатов
			DataTable[REG_MEAS_CURRENT_VALUE] = CurrentActual;
			if ((CurrentActual < (1.0f - CURRENT_MAX_DISTORTION) * CurrentSetpoint) ||
				(CurrentActual > (1.0f + CURRENT_MAX_DISTORTION) * CurrentSetpoint))
			{
				Warning = WARNING_I_OUT_OF_RANGE;
			}

			DataTable[REG_MEAS_TIME_DELAY] = Counter90Percent;
			DataTable[REG_MEAS_TIME_ON] = Counter10Percent;

			if (Problem == PROBLEM_NONE)
				DataTable[REG_TEST_FINISHED] = OPRESULT_OK;
			else
			{
				DataTable[REG_TEST_FINISHED] = OPRESULT_FAIL;

				DataTable[REG_MEAS_TIME_DELAY] = 0;
				DataTable[REG_MEAS_TIME_ON] = 0;
			}

			// Запись ошибок
			DataTable[REG_WARNING] = Warning;
			DataTable[REG_PROBLEM] = Problem;

			LL_ExternalLED(FALSE);
			SUB_State = SS_None;
			CONTROL_SetDeviceState(DS_Ready);
		}
	}
}
//-----------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	//CUSTINT_SendTOCU(0, FALSE, FALSE, FALSE);
	LL_RelayControl(FALSE);
	LL_ExternalFan(FALSE);
	LL_ExternalLED(FALSE);

	SUB_State = SS_None;
	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = Reason;
}
//-----------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//-----------------------------------------------

void CONTROL_WatchDogUpdate()
{
	if (BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//-----------------------------------------------

uint16_t ReadExtReg()
{
	uint16_t Data;

	LL_HSTimers_Load(TRUE);
	LL_HSTimers_CS(TRUE);

	LL_HSTimers_CS(FALSE);
	Data = SPI_ReadByte(SPI1);
	LL_HSTimers_CS(TRUE);

	return Data;
}
//---------------------------

void OutPut_ISO(uint16_t Data)
{
	SPI_WriteByte(SPI2, Data);

	LL_OutReg_CS(TRUE);
	LL_OutReg_CS(FALSE);
}
//---------------------------

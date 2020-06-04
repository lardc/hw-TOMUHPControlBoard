// Header
#include "Controller.h"
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

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile SubState SUB_State = SS_None;
static Boolean CycleActive = FALSE;
//
volatile Int64U CONTROL_TimeCounter = 0;
static Int64U CONTROL_TimeCounterDelay = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState, SubState NewSubState);
void CONTROL_Logic();
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_WatchDogUpdate();
void CONTROL_ResetToDefaultState();
void CONTROL_ResetHardware();

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
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, FALSE);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();

	CONTROL_ResetToDefaultState();
}
//-----------------------------------------------

void CONTROL_ResetToDefaultState()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_TEST_FINISHED] = OPRESULT_NONE;

	DataTable[REG_MEAS_CURRENT_VALUE] = 0;
	DataTable[REG_MEAS_TIME_DELAY] = 0;
	DataTable[REG_MEAS_TIME_ON] = 0;
	
	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();

	CONTROL_ResetHardware();
	CONTROL_SetDeviceState(DS_None, SS_None);
}
//-----------------------------------------------

void CONTROL_ResetHardware()
{

}
//-----------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	
	CONTROL_Logic();
	CONTROL_WatchDogUpdate();
}
//-----------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if(CONTROL_State == DS_None)
				{
					LL_PsBoard_PowerInput(true);
					LL_PsBoard_PowerOutput(true);

					CONTROL_TimeCounterDelay = CONTROL_TimeCounter + T_POWER_ON_PAUSE;
					CONTROL_SetDeviceState(DS_InProcess, SS_PowerOn);
				}
				else if(CONTROL_State != DS_Ready)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_DISABLE_POWER:
			{
				if(CONTROL_State == DS_Ready)
				{
					CONTROL_ResetToDefaultState();
				}
				else if(CONTROL_State != DS_None)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_MEASURE_START:
			{
				if(CONTROL_State == DS_Ready)
				{
					// Запуск измерения
				}
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			}
			break;
			
		case ACT_MEASURE_STOP:
			{
				if(CONTROL_State == DS_InProcess)
				{
					SUB_State = SS_StopProcess;
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_FAULT_CLEAR:
			{
				if(CONTROL_State == DS_Fault)
				{
					CONTROL_ResetToDefaultState();
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

void CONTROL_HandlePowerOn()
{
	if(CONTROL_State == DS_InProcess && SUB_State == SS_StopProcess)
	{
		if(CONTROL_TimeCounter > CONTROL_TimeCounterDelay)
			CONTROL_SetDeviceState(DS_Ready, SS_None);
	}
}
//-----------------------------------------------

void CONTROL_Logic()
{
	/*
	 float CurrentSetpoint, CurrentPreActual, CurrentActual;
	 uint32_t Counter10Percent, Counter90Percent;
	 uint16_t Problem = PROBLEM_NONE, Warning = WARNING_NONE;
	 
	 if(CONTROL_State == DS_InProcess)
	 {
	 if(SUB_State == SS_StopProcess)
	 {
	 //CUSTINT_SendTOCU(0, TRUE, FALSE, TRUE);
	 LL_RelayControl(FALSE);
	 LL_ExternalFan(FALSE);
	 LL_ExternalLED(FALSE);
	 
	 SUB_State = SS_None;
	 CONTROL_SetDeviceState(DS_Ready);
	 }
	 
	 // Выполнение необходимой коммутации
	 if(SUB_State == SS_VoltageReady)
	 {
	 LL_GateLatch(FALSE);
	 LL_RelayControl(TRUE);									// Замыкание выходных реле
	 //CUSTINT_SendTOCU(0, TRUE, TRUE, FALSE);					// Отключение PSBoard + замыкание контактора
	 CONTROL_TimeCounterDelay = CONTROL_TimeCounter + DELAY_CONTACTOR;
	 
	 SUB_State = SS_WaitContactor;
	 }
	 
	 // Непосредственный запуск измерения
	 if((SUB_State == SS_WaitContactor) && (CONTROL_TimeCounter > CONTROL_TimeCounterDelay))
	 {
	 CurrentSetpoint = DataTable[REG_CURRENT_VALUE];
	 
	 //CUSTINT_SendTOCU(CurrentSetpoint, TRUE, TRUE, FALSE);	// Отпирание нужных мосфетов
	 DELAY_US(50);
	 CurrentPreActual = MEASURE_DUTCurrent();// Измерения тока до подачи сигнала управления для определения кз на выходе
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


	 // Запись результатов
	 DataTable[REG_MEAS_CURRENT_VALUE] = CurrentActual;
	 if((CurrentActual < (1.0f - CURRENT_MAX_DISTORTION) * CurrentSetpoint)
	 || (CurrentActual > (1.0f + CURRENT_MAX_DISTORTION) * CurrentSetpoint))
	 {
	 Warning = WARNING_I_OUT_OF_RANGE;
	 }
	 
	 DataTable[REG_MEAS_TIME_DELAY] = Counter90Percent;
	 DataTable[REG_MEAS_TIME_ON] = Counter10Percent;
	 
	 if(Problem == PROBLEM_NONE)
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
	 */
}
//-----------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	CONTROL_ResetHardware();
	
	CONTROL_SetDeviceState(DS_Fault, SS_None);
	DataTable[REG_FAULT_REASON] = Reason;
}
//-----------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState, SubState NewSubState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;

	SUB_State = NewSubState;
	DataTable[REG_DEV_SUB_STATE] = NewSubState;
}
//-----------------------------------------------

void CONTROL_WatchDogUpdate()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//-----------------------------------------------

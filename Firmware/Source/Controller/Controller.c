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
#include "Logic.h"
#include "Commutation.h"

// Types
//
typedef void (*FUNC_AsyncDelegate)();
typedef enum __DeviceState
{
	DS_None = 0,
	DS_Fault = 1,
	DS_Disabled = 2,
	DS_Ready = 3,
	DS_InProcess = 4
} DeviceState;
typedef enum __SubState
{
	SS_None = 0,
	SS_PowerOn = 1,
	SS_WaitCharge = 2,
	
	SS_PowerOff = 3,
	
	SS_ConfigSlaves = 4,
	SS_ConfigSlavesApply = 5,
	SS_ConfigHardware = 6
} SubState;
typedef enum __TOCUDeviceState
{
	TOCUDS_None = 0,
	TOCUDS_Fault = 1,
	TOCUDS_Disabled = 2,
	TOCUDS_Ready = 3,
	TOCUDS_InProcess = 4
} TOCUDeviceState;

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile SubState SUB_State = SS_None;
volatile Int64U CONTROL_TimeCounter = 0;
static Boolean CycleActive = FALSE;
Int64U CONTROL_TimeCounterDelay = 0;
MeasurementSettings CachedMeasurementSettings;
volatile Int16U CONTROL_Values_Current[PULSE_ARR_MAX_LENGTH] = {0};
volatile Int16U CONTROL_Values_Counter = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState, SubState NewSubState);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_WatchDogUpdate();
void CONTROL_ResetToDefaultState();
void CONTROL_ResetHardware(bool KeepPower);
void CONTROL_ResetData();
void CONTROL_SlavesStateUpdate();
void CONTROL_MonitorSafety();
void CONTROL_MonitorPressure();
void CONTROL_HandlePowerOn();
void CONTROL_HandlePowerOff();
void CONTROL_HandlePulseConfig();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_CURRENT};
	Int16U EPSized[EP_COUNT] = {PULSE_ARR_MAX_LENGTH};
	pInt16U EPCounters[EP_COUNT] = {(pInt16U)&CONTROL_Values_Counter};
	pInt16U EPDatas[EP_COUNT] = {(pInt16U)CONTROL_Values_Current};
	
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
	CONTROL_ResetData();
	BHL_ResetError();
	
	CONTROL_ResetHardware(false);
	CONTROL_SetDeviceState(DS_None, SS_None);
}
//-----------------------------------------------

void CONTROL_ResetData()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;
	
	DataTable[REG_MEAS_CURRENT_VALUE] = 0;
	DataTable[REG_MEAS_TIME_DELAY] = 0;
	DataTable[REG_MEAS_TIME_ON] = 0;
	
	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//-----------------------------------------------

void CONTROL_ResetHardware(bool KeepPower)
{
	LL_ExternalLED(false);
	LL_UnitFan(false);
	LL_PsBoard_PowerOutput(false);
	if(!KeepPower)
		LL_PsBoard_PowerInput(false);
	LL_SyncOscilloscope(false);
	LL_SyncTOCU(false);
	
	COMM_EnableSafetyInput(false);
}
//-----------------------------------------------

void CONTROL_Idle()
{
	// Обработка мастер-запросов по интерфейсу
	DEVPROFILE_ProcessRequests();
	
	// Считывание состояний блоков-рабов
	CONTROL_SlavesStateUpdate();
	
	CONTROL_MonitorSafety();
	CONTROL_MonitorPressure();
	
	// Обработка логики мастер-команд
	CONTROL_HandlePowerOn();
	CONTROL_HandlePowerOff();
	CONTROL_HandlePulseConfig();
	
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
					CONTROL_SetDeviceState(DS_InProcess, SS_PowerOn);
				else if(CONTROL_State != DS_Ready)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_DISABLE_POWER:
			{
				if(CONTROL_State == DS_Ready)
				{
					CONTROL_ResetToDefaultState();
					CONTROL_SetDeviceState(DS_None, SS_PowerOff);
				}
				else if(CONTROL_State != DS_None)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_MEASURE_START:
			{
				if(LOGIC_IsAnodeVRegCorrect())
				{
					CachedMeasurementSettings = LOGIC_CacheMeasurementSettings();
					if(CONTROL_State == DS_Ready)
					{
						CONTROL_ResetData();
						
						COMM_EnableSafetyInput(DataTable[REG_MUTE_SAFETY_MONITOR] ? false : true);
						LL_ExternalLED(true);
						
						CONTROL_SetDeviceState(DS_InProcess, SS_ConfigSlaves);
					}
					else
						*pUserError = ERR_DEVICE_NOT_READY;
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_MEASURE_STOP:
			{
				if(CONTROL_State == DS_InProcess)
				{
					// Остановка процесса
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_FAULT_CLEAR:
			{
				if(CONTROL_State == DS_Fault)
					CONTROL_ResetToDefaultState();
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

void CONTROL_MonitorSafety()
{
	bool SystemIsSafe = LOGIC_GetSafetyState();
	
	if(CONTROL_State == DS_InProcess && SUB_State == SS_ConfigSlaves)
	{
		if(!SystemIsSafe)
		{
			CONTROL_ResetHardware(true);
			CONTROL_SetDeviceState(DS_Ready, SS_None);
			
			DataTable[REG_PROBLEM] = PROBLEM_SAFETY_VIOLATION;
		}
	}
}
//-----------------------------------------------

void CONTROL_MonitorPressure()
{
	static uint64_t LastSuccessfulScan = 0;
	bool PressureOK = LOGIC_GetPressureState();
	
	if(PressureOK)
		LastSuccessfulScan = CONTROL_TimeCounter;
	
	if(CONTROL_State == DS_InProcess || CONTROL_State == DS_Ready)
	{
		if((CONTROL_TimeCounter - LastSuccessfulScan) > PRESSURE_FAULT_DELAY)
			CONTROL_SwitchToFault(DF_PRESSURE);
	}
}
//-----------------------------------------------

void CONTROL_HandlePowerOn()
{
	if(CONTROL_State == DS_InProcess)
	{
		switch (SUB_State)
		{
			case SS_PowerOn:
				{
					LL_PsBoard_PowerInput(true);
					LL_PsBoard_PowerOutput(true);
					
					if(LOGIC_CallCommandForSlaves(ACT_TOCU_ENABLE_POWER))
					{
						CONTROL_TimeCounterDelay = CONTROL_TimeCounter + DataTable[REG_TOCU_CHARGE_TIMEOUT];
						CONTROL_SetDeviceState(DS_InProcess, SS_WaitCharge);
					}
					else
						CONTROL_SwitchToFault(DF_INTERFACE);
				}
				break;
				
			case SS_WaitCharge:
				{
					if(LOGIC_AreSlavesInStateX(TOCUDS_Ready))
					{
						CONTROL_SetDeviceState(DS_Ready, SS_None);
					}
					else if(LOGIC_IsSlaveInFaultOrDisabled(TOCUDS_Fault, TOCUDS_Disabled))
					{
						CONTROL_SwitchToFault(DF_TOCU_WRONG_STATE);
					}
					else if(CONTROL_TimeCounter > CONTROL_TimeCounterDelay)
						CONTROL_SwitchToFault(DF_TOCU_STATE_TIMEOUT);
				}
				break;
				
			default:
				break;
		}
	}
}
//-----------------------------------------------

void CONTROL_HandlePowerOff()
{
	if(CONTROL_State == DS_None && SUB_State == SS_PowerOff)
	{
		CONTROL_ResetHardware(false);
		
		if(LOGIC_CallCommandForSlaves(ACT_TOCU_DISABLE_POWER))
			CONTROL_SetDeviceState(DS_None, SS_None);
		else
			CONTROL_SwitchToFault(DF_INTERFACE);
	}
}
//-----------------------------------------------

void CONTROL_HandlePulseConfig()
{
	if(CONTROL_State == DS_InProcess)
	{
		switch (SUB_State)
		{
			case SS_ConfigSlaves:
				{
					LOGIC_AssignVItoSlaves(CachedMeasurementSettings.AnodeVoltage,
							CachedMeasurementSettings.AnodeCurrent);
					if(LOGIC_WriteSlavesConfig())
						CONTROL_SetDeviceState(DS_InProcess, SS_ConfigSlavesApply);
					else
						CONTROL_SwitchToFault(DF_INTERFACE);
				}
				break;
				
			case SS_ConfigSlavesApply:
				{
					if(LOGIC_CallCommandForSlaves(ACT_TOCU_PULSE_CONFIG))
						CONTROL_SetDeviceState(DS_InProcess, SS_ConfigHardware);
					else
						CONTROL_SwitchToFault(DF_INTERFACE);
				}
				break;
				
			case SS_ConfigHardware:
				{
					// Настройка системы коммутации
					COMM_TOSU(CachedMeasurementSettings.AnodeVoltage);
					COMM_InternalCommutation(true);
					
					// Настройка компараторов напряжения
					LOGIC_ConfigVoltageComparators(CachedMeasurementSettings.AnodeVoltage);
					
					// Настройка параметров цепи управления
					GateDriver_SetCurrent(CachedMeasurementSettings.GateCurrent);
					GateDriver_SetCompThreshold(CachedMeasurementSettings.GateCurrent * GATE_CURRENT_THRESHOLD);
					GateDriver_SetFallRate(CachedMeasurementSettings.GateCurrentFallRate);
					GateDriver_SetRiseRate(CachedMeasurementSettings.GateCurrentRiseRate);
				}
				break;
				
			default:
				break;
		}
	}
}
//-----------------------------------------------

void CONTROL_SlavesStateUpdate()
{
	static uint64_t NextUpdate = 0;
	
	if(SUB_State != SS_None)
	{
		if(CONTROL_TimeCounter > NextUpdate)
		{
			NextUpdate = CONTROL_TimeCounter + SLAVE_UPDATE_PERIOD;
			if(!LOGIC_ReadSlavesState())
				CONTROL_SwitchToFault(DF_INTERFACE);
		}
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
	if(Reason == DF_INTERFACE)
	{
		BHLError Error = BHL_GetError();
		DataTable[REG_BHL_ERROR_CODE] = Error.ErrorCode;
		DataTable[REG_BHL_DEVICE] = Error.Device;
		DataTable[REG_BHL_FUNCTION] = Error.Func;
		DataTable[REG_BHL_EXT_DATA] = Error.ExtData;
	}
	
	CONTROL_ResetHardware(false);
	
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

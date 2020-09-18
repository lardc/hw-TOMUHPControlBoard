// Header
#include "Controller.h"
// Includes
#include "SysConfig.h"
#include "DataTable.h"
#include "DeviceProfile.h"
#include "GateDriver.h"
#include "Diagnostic.h"
#include "LowLevel.h"
#include "Logic.h"
#include "Commutation.h"
#include "Constraints.h"
#include "Delay.h"

// Defines
//
#define TIME_TOCU_POWER_UP		1000

// Types
//
typedef void (*FUNC_AsyncDelegate)();
typedef enum __DeviceState
{
	DS_None 				= 0,
	DS_Fault 				= 1,
	DS_Disabled 			= 2,
	DS_Ready 				= 3,
	DS_InProcess 			= 4
} DeviceState;
typedef enum __SubState
{
	SS_None 				= 0,
	SS_PowerOn 				= 1,
	SS_WaitCharge 			= 2,
	
	SS_PowerOff 			= 3,
	
	SS_ConfigSlaves 		= 4,
	SS_ConfigSlavesApply 	= 5,
	SS_WaitConfig			= 6,
	SS_ConfigCommutation	= 7,
	SS_CommPause 			= 8,
	SS_HardwareConfig		= 9,
	SS_TOCU_PulseConfig		= 10,
	SS_StartPulse 			= 11,
	SS_AfterPulseWaiting	= 12
} SubState;
typedef enum __TOCUDeviceState
{
	TOCUDS_None 			= 0,
	TOCUDS_Fault 			= 1,
	TOCUDS_Disabled 		= 2,
	TOCUDS_Ready 			= 3,
	TOCUDS_InProcess 		= 4
} TOCUDeviceState;

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile SubState SUB_State = SS_None;
volatile Int64U CONTROL_TimeCounter = 0;
volatile Int64U CONTROL_FanTimeCounter = 0;
static Boolean CycleActive = FALSE;
Int64U CONTROL_TimeCounterDelay = 0;
MeasurementSettings CachedMeasurementSettings;
volatile Int16U CONTROL_Values_Current[PULSE_ARR_MAX_LENGTH] = {0};
volatile Int16U CONTROL_Values_TurnDelay[TIME_ARR_MAX_LENGTH] = {0};
volatile Int16U CONTROL_Values_TurnOn[TIME_ARR_MAX_LENGTH] = {0};
volatile Int16U CONTROL_Values_CurrentCounter = 0;
volatile Int16U CONTROL_Values_TurnDelayCounter = 0;
volatile Int16U CONTROL_Values_TurnOnCounter = 0;
Int16U CONTROL_AverageCounter = 0;
Int64U CONTROL_AveragePeriodCounter = 0;

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
bool CONTROL_ForceSlavesStateUpdate();
void CONTROL_MonitorSafety();
void CONTROL_MonitorPressure();
void CONTROL_HandlePowerOn();
void CONTROL_HandlePowerOff();
void CONTROL_HandlePulseConfig();
void CONTROL_GateDriverCharge();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_CURRENT, EP_TURN_DELAY, EP_TURN_ON};
	Int16U EPSized[EP_COUNT] = {PULSE_ARR_MAX_LENGTH, TIME_ARR_MAX_LENGTH, TIME_ARR_MAX_LENGTH};
	pInt16U EPCounters[EP_COUNT] = {(pInt16U)&CONTROL_Values_CurrentCounter, (pInt16U)&CONTROL_Values_TurnDelayCounter,
									(pInt16U)&CONTROL_Values_TurnOnCounter};
	pInt16U EPDatas[EP_COUNT] = {(pInt16U)CONTROL_Values_Current, (pInt16U)CONTROL_Values_TurnDelay,
									(pInt16U)CONTROL_Values_TurnOn};
	
	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, FALSE);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();

	// Ожидание запуска TOCU
	uint64_t CONTROL_TOCUPowerUpTimer = CONTROL_TimeCounter + TIME_TOCU_POWER_UP;
	while(CONTROL_TimeCounter < CONTROL_TOCUPowerUpTimer){}
	CONTROL_WatchDogUpdate();

	CONTROL_ResetToDefaultState();
}
//-----------------------------------------------

void CONTROL_ResetToDefaultState()
{
	CONTROL_ResetData();
	BHL_ResetError();
	
	CONTROL_ResetHardware(false);

	if(CONTROL_ForceSlavesStateUpdate())
	{
		if(LOGIC_AreSlavesInStateX(TOCUDS_Fault))
		{
			if(!LOGIC_CallCommandForSlaves(ACT_TOCU_FAULT_CLEAR))
				CONTROL_SwitchToFault(DF_INTERFACE);
		}
		else
		{
			if(LOGIC_CallCommandForSlaves(ACT_TOCU_DISABLE_POWER))
				CONTROL_SetDeviceState(DS_None, SS_None);
			else
				CONTROL_SwitchToFault(DF_INTERFACE);
		}
	}
	else
		CONTROL_SwitchToFault(DF_INTERFACE);
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
	if(!KeepPower)
	{
		LL_PsBoard_PowerInput(false);
		LL_PsBoard_PowerOutput(false);
	}

	if(!DataTable[REG_MUTE_COMMUTATION])
	{
		COMM_TOSU(false);
		COMM_InternalCommutation(false);
	}

	LL_SyncTOCU(false);
	LL_ExternalLED(false);
	
	COMM_EnableSafetyInput(false);
	COMM_PotSwitch(false);

	LL_GateLatchReset();
	LL_HSTimers_Reset();
	LL_HSTimers_Read();

	// Установка максимального уровня компаратора для избежания ложного срабатывания от шумов TOCU HP
	GateDriver_SetForceCompThresholdMax();
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
					CONTROL_ResetHardware(false);
					CONTROL_SetDeviceState(DS_Ready, SS_None);
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;
			
		case ACT_GATE_PULSE:
			{
				CONTROL_GateDriverCharge();

				CachedMeasurementSettings = LOGIC_CacheMeasurementSettings();
				GateDriver_SetCurrent(CachedMeasurementSettings.GateCurrent);
				GateDriver_SetRiseRate(&CachedMeasurementSettings);
				GateDriver_SetFallRate(&CachedMeasurementSettings);
				GateDriver_SetCompThreshold(CachedMeasurementSettings.GateCurrent * GATE_CURRENT_THRESHOLD);

				GateDriver_Sync(true);
				DELAY_US(100);
				GateDriver_Sync(false);

				CONTROL_ResetHardware(true);
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
	
	if((CONTROL_State == DS_InProcess || CONTROL_State == DS_Ready)
			&& ((SUB_State != SS_ConfigCommutation) || (SUB_State != SS_CommPause)))
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
					CONTROL_GateDriverCharge();
					
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
	static uint64_t SlaveUpdateTimeout = 0;

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
					if(LOGIC_CallCommandForSlaves(ACT_TOCU_VOLTAGE_CONFIG))
					{
						CONTROL_TimeCounterDelay = CONTROL_TimeCounter + APPLY_SETTINGS_TIMEOUT;
						CONTROL_SetDeviceState(DS_InProcess, SS_WaitConfig);
					}
					else
						CONTROL_SwitchToFault(DF_INTERFACE);
				}
				break;

			case SS_WaitConfig:
				{
					if(CONTROL_ForceSlavesStateUpdate())
					{
						if(LOGIC_AreSlavesInStateX(TOCUDS_Ready))
							CONTROL_SetDeviceState(DS_InProcess, SS_ConfigCommutation);
						else
							if(CONTROL_TimeCounter > CONTROL_TimeCounterDelay)
								CONTROL_SwitchToFault(DF_TOCU_STATE_TIMEOUT);
					}
					else
						CONTROL_SwitchToFault(DF_INTERFACE);
				}
				break;
				
			case SS_ConfigCommutation:
				{
					// Зарядить GateDriver
					CONTROL_GateDriverCharge();

					// Настройка системы коммутации
					COMM_TOSU(CachedMeasurementSettings.AnodeVoltage);
					COMM_InternalCommutation(true);
					COMM_PotSwitch(true);

					CONTROL_TimeCounterDelay = CONTROL_TimeCounter + COMMUTATION_PAUSE;
					CONTROL_SetDeviceState(DS_InProcess, SS_CommPause);
				}
				break;
				
			case SS_CommPause:
				{
					if(CONTROL_TimeCounter > CONTROL_TimeCounterDelay)
						CONTROL_SetDeviceState(DS_InProcess, SS_TOCU_PulseConfig);
				}
				break;

			case SS_TOCU_PulseConfig:
				{
					if(CONTROL_TimeCounter > CONTROL_AveragePeriodCounter)
					{
						if(LOGIC_CallCommandForSlaves(ACT_TOCU_PULSE_CONFIG))
							CONTROL_SetDeviceState(DS_InProcess, SS_HardwareConfig);
						else
							CONTROL_SwitchToFault(DF_INTERFACE);
					}
				}
				break;
				
			case SS_HardwareConfig:
				{
					// Настройка компараторов напряжения
					LOGIC_ConfigVoltageComparators(CachedMeasurementSettings.AnodeVoltage);

					// Настройка параметров цепи управления
					GateDriver_SetCurrent(CachedMeasurementSettings.GateCurrent);
					GateDriver_SetFallRate(&CachedMeasurementSettings);
					GateDriver_SetRiseRate(&CachedMeasurementSettings);
					GateDriver_SetCompThreshold(CachedMeasurementSettings.GateCurrent * GATE_CURRENT_THRESHOLD);

					CONTROL_SetDeviceState(DS_InProcess, SS_StartPulse);

					CONTROL_AverageCounter = 0;
				}
				break;

			case SS_StartPulse:
				{
					if(CONTROL_TimeCounter > CONTROL_AveragePeriodCounter)
					{
						if(CONTROL_ForceSlavesStateUpdate())
						{
							if(LOGIC_AreSlavesInStateX(TOCUDS_Ready))
							{
								if(CONTROL_AverageCounter < DataTable[REG_AVERAGE_NUM])
								{
									DataTable[REG_PROBLEM] = LOGIC_Pulse();

									// Зарядить GateDriver перед следующим импульсом
									CONTROL_GateDriverCharge();

									CONTROL_AverageCounter++;

									CONTROL_Values_TurnDelayCounter = CONTROL_AverageCounter;
									CONTROL_Values_TurnOnCounter = CONTROL_AverageCounter;

									CONTROL_AveragePeriodCounter = CONTROL_TimeCounter + DataTable[REG_AVERAGE_PERIOD];

									if(DataTable[REG_PROBLEM])
									{
										CONTROL_SetDeviceState(DS_Fault, SS_None);
										DataTable[REG_OP_RESULT] = OPRESULT_FAIL;
									}
								}
								else
								{
									MEASURE_TurnOnAveragingProcess();

									DataTable[REG_OP_RESULT] = OPRESULT_OK;

									CONTROL_TimeCounterDelay = CONTROL_TimeCounter + DataTable[REG_AFTER_MEASURE_DELAY];
									CONTROL_SetDeviceState(DS_InProcess, SS_AfterPulseWaiting);
								}
							}
							else
							{
								if(!LOGIC_AreSlavesInStateX(TOCUDS_InProcess))
									CONTROL_SwitchToFault(DF_TOCU_WRONG_STATE);
							}
						}
						else
							CONTROL_SwitchToFault(DF_INTERFACE);
					}
				}
				break;

			case SS_AfterPulseWaiting:
				{
					if(CONTROL_TimeCounter > CONTROL_TimeCounterDelay)
					{
						if(CONTROL_TimeCounter >= SlaveUpdateTimeout)
						{
							SlaveUpdateTimeout = CONTROL_TimeCounter + SLAVE_UPDATE_PERIOD;

							if(CONTROL_ForceSlavesStateUpdate())
							{
								if(LOGIC_AreSlavesInStateX(TOCUDS_Ready))
								{
									CONTROL_SetDeviceState(DS_Ready, SS_None);
									CONTROL_ResetHardware(true);
								}
								else
									CONTROL_SwitchToFault(DF_TOCU_STATE_TIMEOUT);
							}
							else
								CONTROL_SwitchToFault(DF_INTERFACE);
						}
					}
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
			if(!CONTROL_ForceSlavesStateUpdate())
				CONTROL_SwitchToFault(DF_INTERFACE);
		}
	}
}
//-----------------------------------------------

bool CONTROL_ForceSlavesStateUpdate()
{
	return LOGIC_ReadSlavesState();
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

void CONTROL_UnitFan()
{
	static uint32_t IncrementCounter = 0;
	static uint64_t FanOnTimeout = 0;

	// Увеличение счётчика в простое
	if (CONTROL_State != DS_InProcess)
		IncrementCounter++;

	// Включение вентилятора
	if ((IncrementCounter > ((uint32_t)DataTable[REG_FAN_OPERATE_PERIOD] * 1000)) || (CONTROL_State == DS_InProcess))
	{
		IncrementCounter = 0;
		FanOnTimeout = CONTROL_TimeCounter + ((uint32_t)DataTable[REG_FAN_OPERATE_TIME] * 1000);
		LL_UnitFan(true);
	}

	// Отключение вентилятора
	if (FanOnTimeout && (CONTROL_TimeCounter > FanOnTimeout))
	{
		FanOnTimeout = 0;
		LL_UnitFan(false);
	}
}
//-----------------------------------------------

void CONTROL_GateDriverCharge()
{
	LL_PsBoard_PowerOutput(true);
	DELAY_MS(DataTable[REG_GATE_TIME_CHARGE]);
	LL_PsBoard_PowerOutput(false);
	DELAY_MS(10);
}
//-----------------------------------------------

// Header
#include "Logic.h"
// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Commutation.h"
#include "Measurement.h"
#include "LowLevel.h"
#include "Delay.h"
#include "Interrupts.h"
#include "GateDriver.h"
#include "InitConfig.h"
#include "Constraints.h"

// Definitions
//
#define TOCU1_CAN_NID			21
#define TOCU1_BIT_MASK			0x3FF
//
#define ITTERATIONS_OF_AVERAGING	3

// Types
//
typedef struct __NodeBitmask
{
	uint16_t NodeID;
	uint16_t SupportedBits;
} NodeBitmask;
typedef struct __NodeState
{
	uint16_t State;
	uint16_t OpResult;
	uint16_t Mask;
	uint16_t Voltage;
} NodeState;

// Variables
//
const NodeBitmask NodeBitmaskArray[] = {{TOCU1_CAN_NID, TOCU1_BIT_MASK}};
#define NODE_ARRAY_SIZE		(sizeof NodeBitmaskArray / sizeof NodeBitmaskArray[0])
NodeState NodeArray[NODE_ARRAY_SIZE] = {0};
Int16U LOGIC_TurnDelayResultBuffer[AVERAGE_NUM_MAX];
Int16U LOGIC_TurnOnResultBuffer[AVERAGE_NUM_MAX];


// Functions prototypes
//
void LOGIC_TurnOnMeasurement();
void LOGIC_AnodeCurrentTune(AnodeVoltageEnum AnodeVoltage, float *AnodeCurrent);
void LOGIC_AreInterruptsActive(bool State);
void LOGIC_FineTuneTdelTon(uint16_t* TurnDelay, uint16_t* TurnOn);
void LOGIC_AveragingData(Int16U *Array, Int16U *MeanValue, Int16U AllowedSpread);
//

// Functions
//
bool LOGIC_ReadSlavesState()
{
	uint16_t State, OpResult;
	bool result;
	
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
	{
		result = false;
		
		if(BHL_ReadRegister(NodeBitmaskArray[i].NodeID, REG_TOCU_DEV_STATE, &State))
			if(BHL_ReadRegister(NodeBitmaskArray[i].NodeID, REG_TOCU_OP_RESULT, &OpResult))
			{
				NodeArray[i].State = State;
				NodeArray[i].OpResult = OpResult;
				result = true;
			}

		if(!result)
			return false;
	}
	
	return true;
}
//-----------------------------------------------

bool LOGIC_WriteSlavesConfig()
{
	bool result;

	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
	{
		result = false;
		
		if(BHL_WriteRegister(NodeBitmaskArray[i].NodeID, REG_TOCU_VOLTAGE_SETPOINT, NodeArray[i].Voltage))
			if(BHL_WriteRegister(NodeBitmaskArray[i].NodeID, REG_TOCU_GATE_REGISTER, NodeArray[i].Mask))
				result = true;
		
		if(!result)
			return false;
	}
	
	return true;
}
//-----------------------------------------------

bool LOGIC_CallCommandForSlaves(uint16_t Command)
{
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
	{
		if(!BHL_Call(NodeBitmaskArray[i].NodeID, Command))
			return false;
	}
	
	return true;
}
//-----------------------------------------------

bool LOGIC_AreSlavesInStateX(uint16_t State)
{
	bool result = true;
	
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
		if(NodeArray[i].State != State)
			result = false;
	
	return result;
}
//-----------------------------------------------

bool LOGIC_IsSlaveInFaultOrDisabled(uint16_t Fault, uint16_t Disabled)
{
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
		if(NodeArray[i].State == Fault || NodeArray[i].State == Disabled)
			return true;
	
	return false;
}
//-----------------------------------------------

void LOGIC_AssignVItoSlaves(AnodeVoltageEnum AnodeVoltage, float AnodeCurrent)
{
	float CurrentPerBit;
	uint16_t ActualBitmask = 0, MaximumBitmask = 0;

	LOGIC_AnodeCurrentTune(AnodeVoltage, &AnodeCurrent);

	// ����������� ����������� ���������� ������� �����
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
		MaximumBitmask |= NodeBitmaskArray[i].SupportedBits;
	
	// ����������� �������� ���� �� ��� ��� �������� ����������
	CurrentPerBit = (float)AnodeVoltage / DataTable[REG_TOCU_RES_PER_BIT];
	
	// ����������� ������� ����� ��� ���������� �������� ����
	ActualBitmask = (uint16_t)(AnodeCurrent / CurrentPerBit);
	if(ActualBitmask > MaximumBitmask)
		ActualBitmask = MaximumBitmask;
	
	// ������������ ������� ��� ������
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
	{
		NodeArray[i].Voltage = AnodeVoltage;
		NodeArray[i].Mask = NodeBitmaskArray[i].SupportedBits & ActualBitmask;
	}
}
//-----------------------------------------------

void LOGIC_AnodeCurrentTune(AnodeVoltageEnum AnodeVoltage, float *AnodeCurrent)
{
	float P2, P1, P0, I;

	switch(AnodeVoltage)
	{
		case TOU_600V:
			P2 = ((float) ((int16_t)DataTable[REG_I_DUT_600V_P2])) / 1000000;
			P1 = ((float) DataTable[REG_I_DUT_600V_P1]) / 1000;
			P0 = (int16_t) DataTable[REG_I_DUT_600V_P0];
			break;

		case TOU_1000V:
			P2 = ((float) ((int16_t)DataTable[REG_I_DUT_1000V_P2])) / 1000000;
			P1 = ((float) DataTable[REG_I_DUT_1000V_P1]) / 1000;
			P0 = (int16_t) DataTable[REG_I_DUT_1000V_P0];
			break;

		case TOU_1500V:
			P2 = ((float) ((int16_t)DataTable[REG_I_DUT_1500V_P2])) / 1000000;
			P1 = ((float) DataTable[REG_I_DUT_1500V_P1]) / 1000;
			P0 = (int16_t) DataTable[REG_I_DUT_1500V_P0];
			break;

		default:
			return;
	}

	I = *AnodeCurrent;

	I = I * I * P2 + I * P1 + P0;

	*AnodeCurrent = I / 10;
}
//-----------------------------------------------

bool LOGIC_IsAnodeVRegCorrect()
{
	uint16_t v = DataTable[REG_ANODE_VOLTAGE];
	return (v == TOU_600V) || (v == TOU_1000V) || (v == TOU_1500V);
}
//-----------------------------------------------

bool LOGIC_GetSafetyState()
{
	bool SafetyInput = COMM_IsSafetyTrig();
	DataTable[REG_SAFETY_STATE] = SafetyInput ? 1 : 0;
	
	if(DataTable[REG_MUTE_SAFETY_MONITOR])
		return true;
	else
		return SafetyInput;
}
//-----------------------------------------------

bool LOGIC_GetPressureState()
{
	bool PressureInput = COMM_IsPressureTrig();
	DataTable[REG_PRESSURE_STATE] = PressureInput ? 1 : 0;
	
	if(DataTable[REG_MUTE_PRESSURE_MONITOR])
		return true;
	else
		return PressureInput;
}
//-----------------------------------------------

void LOGIC_ConfigVoltageComparators(AnodeVoltageEnum AnodeVoltage)
{
	switch (AnodeVoltage)
	{
		case TOU_600V:
			MEASURE_SetUref10(DataTable[REG_VCOMP10_600V]);
			MEASURE_SetUref90(DataTable[REG_VCOMP90_600V]);
			break;
			
		case TOU_1000V:
			MEASURE_SetUref10(DataTable[REG_VCOMP10_1000V]);
			MEASURE_SetUref90(DataTable[REG_VCOMP90_1000V]);
			break;
			
		case TOU_1500V:
			MEASURE_SetUref10(DataTable[REG_VCOMP10_1500V]);
			MEASURE_SetUref90(DataTable[REG_VCOMP90_1500V]);
			break;
			
		default:
			break;
	}
}
//-----------------------------------------------

uint16_t LOGIC_Pulse()
{
	uint16_t Problem = PROBLEM_NONE;

	// ������ ������������� �� TOCU HP
	LL_SyncTOCU(true);

	// ��������� ������ ������������ �����������
	GateDriver_SetCompThreshold(CachedMeasurementSettings.GateCurrent * GATE_CURRENT_THRESHOLD);
	
	// �������� ������ ���� �� ��������� �������
	if(MEASURE_CheckAnodeCurrent())
	{
		LL_SyncTOCU(false);
		Problem = PROBLEM_SHORT;
	}
	else
	{
		// ����� ������� �����
		LL_GateLatchReset();
		LL_HSTimers_Reset();
		Overflow90 = false;
		Overflow10 = false;
	
		// ������ ���������
		DMA_ChannelEnable(DMA_ADC_DUT_I_CHANNEL, true);
		TIM_Start(TIM6);

		LOGIC_AreInterruptsActive(false);

		// ������ ���� ����������
		LL_SyncOscilloscope(true);
		GateDriver_Sync(true);

		DELAY_US(30);

		// ���������� �������� ���������
		GateDriver_Sync(false);
		LL_SyncOscilloscope(false);

		DELAY_US(10);

		LL_GateLatchReset();
		LL_SyncTOCU(false);

		// ��������� ������������� ������ ����������� ��� �������� ������� ������������ �� ����� TOCU HP
		GateDriver_SetForceCompThresholdMax();

		LOGIC_AreInterruptsActive(true);

		LOGIC_TurnOnMeasurement();
	
		// ���������� ������������ �������� � endpoint
		MEASURE_ConvertRawArray(&LOGIC_OutputPulseRaw[0], &CONTROL_Values_Current[0], PULSE_ARR_MAX_LENGTH);
		CONTROL_Values_CurrentCounter = PULSE_ARR_MAX_LENGTH;
	
		// ��������� ���������� ��������
		if (DataTable[REG_MEAS_CURRENT_VALUE] < (CachedMeasurementSettings.AnodeCurrent * DataTable[REG_ID_THRESHOLD] / 100))
		{
			Problem = PROBLEM_NO_PWR;
		}
		else if(!DataTable[REG_MEAS_TIME_DELAY] || !DataTable[REG_MEAS_TIME_ON])
		{
			Problem = PROBLEM_NO_POT;
		}
		else if(Overflow90)
		{
			Problem = PROBLEM_OVERFLOW90;
		}
		else if(Overflow10)
		{
			Problem = PROBLEM_OVERFLOW10;
		}
	}

	if(DataTable[REG_MUTE_PROBLEM])
		return PROBLEM_NONE;
	else
		return Problem;
}
//-----------------------------------------------

void LOGIC_TurnOnAveragingProcess()
{
	Int16U TonAverage = 0, TonDelayAverage = 0;

	for(int i = 0; i < ITTERATIONS_OF_AVERAGING; i++)
	{
		LOGIC_AveragingData(&LOGIC_TurnDelayResultBuffer[0], &TonDelayAverage, DataTable[REG_AVERAGE_ALLOWED_SPREAD]);
		LOGIC_AveragingData(&LOGIC_TurnOnResultBuffer[0], &TonAverage, DataTable[REG_AVERAGE_ALLOWED_SPREAD]);
	}

	DataTable[REG_MEAS_TIME_DELAY] = TonDelayAverage;
	DataTable[REG_MEAS_TIME_ON] = TonAverage;
}
//-----------------------------------------------

void LOGIC_AveragingData(Int16U *Array, Int16U *MeanValue, Int16U AllowedSpread)
{
	float Result = 0;
	Int16U AverageCounter = 0;

	if(*MeanValue == 0)
	{
		for(int i = 0; i < DataTable[REG_AVERAGE_NUM]; i++)
			Result += *(Array + i);

		*MeanValue = (Int16U)(Result / DataTable[REG_AVERAGE_NUM]);
	}
	else
	{
		for(int i = 0; i < DataTable[REG_AVERAGE_NUM]; i++)
		{
			if(((*MeanValue + AllowedSpread) >= *(Array + i)) && ((*MeanValue - AllowedSpread) <= *(Array + i)))
			{
				Result += *(Array + i);
				AverageCounter++;
			}
		}

		*MeanValue = (Int16U)(Result / AverageCounter);
	}
}
//-----------------------------------------------

MeasurementSettings LOGIC_CacheMeasurementSettings()
{
	MeasurementSettings result;
	
	result.AnodeVoltage = DataTable[REG_ANODE_VOLTAGE];
	result.AnodeCurrent = (float)DataTable[REG_ANODE_CURRENT];
	result.GateCurrent = (float)DataTable[REG_GATE_CURRENT];
	result.GateCurrentRiseRate = (float)DataTable[REG_GATE_CURRENT_RISE_RATE];
	result.GateCurrentFallRate = (float)DataTable[REG_GATE_I_FALL_RATE];
	
	return result;
}
//-----------------------------------------------

void LOGIC_TurnOnMeasurement()
{
	uint32_t DataRaw;
	uint16_t TurnOn, TurnDelay;

	DataRaw = LL_HSTimers_Read();

	TurnDelay = ((DataRaw >> 12) & 0x0FF0) | ((DataRaw >> 28) & 0x000F);
	TurnDelay = TurnDelay * COUNTER_CLOCK_PERIOD_NS;

	if(TurnDelay < DataTable[REG_MEAS_TIME_LOW])
		TurnDelay = 0;

	TurnOn = (DataRaw & 0x00FF) | ((DataRaw >> 16) & 0x0F00);
	TurnOn = TurnOn * COUNTER_CLOCK_PERIOD_NS;

	if(TurnOn < DataTable[REG_MEAS_TIME_LOW])
		TurnOn = 0;

	DataTable[190] = (DataRaw & 0x00FF) | ((DataRaw >> 16) & 0x0F00);

	if(TurnDelay && TurnOn)
		LOGIC_FineTuneTdelTon(&TurnDelay, &TurnOn);

	LOGIC_TurnDelayResultBuffer[CONTROL_AverageCounter] = TurnDelay;
	LOGIC_TurnOnResultBuffer[CONTROL_AverageCounter] = TurnOn;

	// ���������� ���������� � endpoint
	CONTROL_Values_TurnDelay[CONTROL_AverageCounter] = TurnDelay;
	CONTROL_Values_TurnDelayCounter = CONTROL_AverageCounter;

	CONTROL_Values_TurnOn[CONTROL_AverageCounter] = TurnOn;
	CONTROL_Values_TurnOnCounter = CONTROL_AverageCounter;
}
//-----------------------------------------------

void LOGIC_FineTuneTdelTon(uint16_t* TurnDelay, uint16_t* TurnOn)
{
	float Tdel_P2, Tdel_P1, Ton_P2, Ton_P1;
	int16_t Tdel_P0, Ton_P0;
	uint16_t T;

	switch(DataTable[REG_ANODE_VOLTAGE])
	{
		case TOU_600V:
		{
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_600V_P2]) / 1000000;
			Tdel_P1 = (float)DataTable[REG_T_DEL_600V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_600V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_600V_P2]) / 1000000;
			Ton_P1 = (float)DataTable[REG_T_ON_600V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_600V_P0];
		}
		break;

		case TOU_1000V:
		{
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_1000V_P2]) / 1000000;
			Tdel_P1 = (float)DataTable[REG_T_DEL_1000V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_1000V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_1000V_P2]) / 1000000;
			Ton_P1 = (float)DataTable[REG_T_ON_1000V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_1000V_P0];
		}
		break;

		case TOU_1500V:
		{
			Tdel_P2 = (float)((int16_t)DataTable[REG_T_DEL_1500V_P2]) / 1000000;
			Tdel_P1 = (float)DataTable[REG_T_DEL_1500V_P1] / 1000;
			Tdel_P0 = (int16_t)DataTable[REG_T_DEL_1500V_P0];

			Ton_P2 = (float)((int16_t)DataTable[REG_T_ON_1500V_P2]) / 1000000;
			Ton_P1 = (float)DataTable[REG_T_ON_1500V_P1] / 1000;
			Ton_P0 = (int16_t)DataTable[REG_T_ON_1500V_P0];
		}
		break;

		default:
		{
			Tdel_P2 = 0;
			Tdel_P1 = 0;
			Tdel_P0 = 0;

			Ton_P2 = 0;
			Ton_P1 = 0;
			Ton_P0 = 0;
		}
			break;
	}


	T = *TurnDelay;
	*TurnDelay = T * T * Tdel_P2 + T * Tdel_P1 + Tdel_P0;

	T = *TurnOn;
	*TurnOn = T * T * Ton_P2 + T * Ton_P1 + Ton_P0;
}
//-----------------------------------------------

void LOGIC_AreInterruptsActive(bool State)
{
	NCAN_FIFOInterrupt(State);
	USART_Recieve_Interupt(USART1, 0, State);
	TIM_Interupt(TIM3, 0, State);
}
//-----------------------------------------------

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

// Definitions
//
#define TOCU1_CAN_NID		21
#define TOCU1_BIT_MASK		0x3FF

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
			MEASURE_SetUref10(DataTable[REG_VCOMP10_600]);
			MEASURE_SetUref90(DataTable[REG_VCOMP90_600]);
			break;
			
		case TOU_1000V:
			MEASURE_SetUref10(DataTable[REG_VCOMP10_1000]);
			MEASURE_SetUref90(DataTable[REG_VCOMP90_1000]);
			break;
			
		case TOU_1500V:
			MEASURE_SetUref10(DataTable[REG_VCOMP10_1500]);
			MEASURE_SetUref90(DataTable[REG_VCOMP90_1500]);
			break;
			
		default:
			break;
	}
}
//-----------------------------------------------

uint16_t LOGIC_Pulse()
{
	// ��������� ������ ����������
	LL_SyncTOCU(true);
	DELAY_US(10);
	
	// �������� ������ ���� �� ��������� �������
	if(MEASURE_ReadCurrent() > DataTable[MAX_ANODE_CURRENT])
		return PROBLEM_SHORT;
	
	// ����� ������� �����
	LL_HSTimers_Reset();
	LL_GateLatchReset();
	Overflow90 = false;
	Overflow10 = false;
	
	// ������ ���������
	DMA_ChannelReload(DMA_ADC_DUT_I_CHANNEL, PULSE_ARR_MAX_LENGTH);
	DMA_ChannelEnable(DMA_ADC_DUT_I_CHANNEL, true);
	DMAOperation = true;
	TIM_Start(TIM6);
	
	// ������ ���� ����������
	LL_SyncOscilloscope(true);
	GateDriver_Sync(true);
	DELAY_US(90);
	
	// ���������� ������ ���������
	
	// ���������� �������������
	LL_SyncOscilloscope(false);
	GateDriver_Sync(false);
	LL_SyncTOCU(false);
	
	// ����� ������� �����
	LL_HSTimers_Reset();
	LL_GateLatchReset();
	
	// ��������� ������� GateDriver
	LL_PsBoard_PowerOutput(true);

	COMM_PotSwitch(false);

	return PROBLEM_NONE;
}
//-----------------------------------------------

MeasurementSettings LOGIC_CacheMeasurementSettings()
{
	MeasurementSettings result;
	
	result.AnodeVoltage = DataTable[REG_ANODE_VOLTAGE];
	result.AnodeCurrent = (float)DataTable[REG_ANODE_CURRENT];
	result.GateCurrent = (float)DataTable[REG_GATE_CURRENT] / 10;
	result.GateCurrentRiseRate = (float)DataTable[REG_GATE_CURRENT_RISE_RATE] / 10;
	result.GateCurrentFallRate = (float)DataTable[REG_GD_CURRENT_FALL_RATE] / 10;
	
	return result;
}
//-----------------------------------------------

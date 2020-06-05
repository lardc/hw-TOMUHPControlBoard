// Header
#include "Logic.h"
// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Definitions
//
#define TOCU1_CAN_NID		1
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

// Forward functions
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

void LOGIC_AssignVoltageAndCurrentToSlaves(uint16_t Voltage, uint16_t Current)
{
	float CurrentPerBit;
	uint16_t ActualBitmask = 0, MaximumBitmask = 0;

	// ќпределение максимально допустимой битовой маски
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
		MaximumBitmask |= NodeBitmaskArray[i].SupportedBits;

	// ќпределение величины тока на бит при заданном напр€жении
	CurrentPerBit = (float)Voltage / DataTable[REG_TOCU_RES_PER_BIT];

	// ќпределение битовой маски дл€ выбранного значени€ тока
	ActualBitmask = (uint16_t)(Current / CurrentPerBit);
	if (ActualBitmask > MaximumBitmask)
		ActualBitmask = MaximumBitmask;

	// ‘ормирование уставки дл€ блоков
	for(uint16_t i = 0; i < NODE_ARRAY_SIZE; ++i)
	{
		NodeArray[i].Voltage = Voltage;
		NodeArray[i].Mask = NodeBitmaskArray[i].SupportedBits & ActualBitmask;
	}
}
//-----------------------------------------------

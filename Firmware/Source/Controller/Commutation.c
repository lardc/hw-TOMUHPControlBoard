// Header
#include "Commutation.h"
// Includes
#include "LowLevel.h"
#include "Board.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Definitions
#define COMM_TOSU_MASK_600		0x0c
#define COMM_TOSU_MASK_1000		0x0a
#define COMM_TOSU_MASK_1500		0x09


#define COMM_TOU_600			0
#define COMM_TOU_1000			1
#define COMM_TOU_1500			2
#define COMM_TOU_SW				3

#define COMM_POT_SW_MASK		0x10

// Variables
uint8_t CommutationMask = 0, PrevCommutationMask = 0;

uint8_t const CommMask[] = {COMM_TOSU_MASK_600, COMM_TOSU_MASK_1000, COMM_TOSU_MASK_1500, COMM_POT_SW_MASK};

Int32U CycleCounters[COMMUTATION_TABLE_SIZE] = {0};

// Forward functions
void COMM_OutputRegister_Write();

// Functions
//
void COMM_InternalCommutation(bool State)
{
	GPIO_SetState(GPIO_RLC, State);
}
//-----------------------------

bool COMM_IsPressureTrig()
{
	return !GPIO_GetState(GPIO_PRESSURE);
}
//-----------------------------

bool COMM_IsSafetyTrig()
{
	return GPIO_GetState(GPIO_SFTY);
}
//-----------------------------

void COMM_EnableSafetyInput(bool State)
{
	GPIO_SetState(GPIO_SFTY_EN, State);
	GPIO_SetState(GPIO_SREG_OE, State);
}
//-----------------------------

void COMM_OutputRegister_Write()
{
	for(uint8_t i = 0; i < COMMUTATION_TABLE_SIZE; i++)
	{
		if((PrevCommutationMask & CommMask[i]) == 0 && (CommutationMask & CommMask[i]) == CommMask[i])
			CycleCounters[i]++;
	}
	GPIO_SetState(GPIO_SREG_CS, false);
	SPI_WriteByte(SPI2, CommutationMask);
	GPIO_SetState(GPIO_SREG_CS, true);
	PrevCommutationMask = CommutationMask;
}
//-----------------------------

void COMM_TOSURaw(uint16_t Data)
{
	CommutationMask = Data;
	COMM_OutputRegister_Write();
}
//-----------------------------

void COMM_TOSU(AnodeVoltageEnum AnodeVoltage)
{
	switch (AnodeVoltage)
	{
		case TOU_600V:
			CommutationMask &=~ (COMM_TOSU_MASK_1000 | COMM_TOSU_MASK_1500);
			CommutationMask |= COMM_TOSU_MASK_600;
			break;

		case TOU_1000V:
			CommutationMask &=~ (COMM_TOSU_MASK_600 | COMM_TOSU_MASK_1500);
			CommutationMask |= COMM_TOSU_MASK_1000;
			break;

		case TOU_1500V:
			CommutationMask &=~ (COMM_TOSU_MASK_600 | COMM_TOSU_MASK_1000);
			CommutationMask |= COMM_TOSU_MASK_1500;
			break;
	}

	COMM_OutputRegister_Write();
}
//-----------------------------

void COMM_PotSwitch(bool State)
{
	State ? (CommutationMask |= COMM_POT_SW_MASK) : (CommutationMask &=~ COMM_POT_SW_MASK);
	COMM_OutputRegister_Write();
}
//-----------------------------

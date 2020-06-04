// Header
#include "Commutation.h"
// Includes
#include "LowLevel.h"
#include "Board.h"
#include "Global.h"

// Definitions
#define COMM_TOSU_MASK_500		0x0
#define COMM_TOSU_MASK_1000		0x0
#define COMM_TOSU_MASK_1500		0x0

// Forward functions
void COMM_OutputRegister_Write(uint16_t Data);

// Functions
//
void COMM_InternalCommutation(bool State)
{
	GPIO_SetState(GPIO_RLC, State);
}
//-----------------------------

bool COMM_IsPressureTrig()
{
	return GPIO_GetState(GPIO_PRESSURE);
}
//-----------------------------

bool COMM_IsSafetyTrig()
{
	return GPIO_GetState(GPIO_SFTY);
}
//-----------------------------

void COMM_OutputRegister_Write(uint16_t Data)
{
	GPIO_SetState(GPIO_SREG_CS, false);
	SPI_WriteByte(SPI2, Data);
	GPIO_SetState(GPIO_SREG_CS, true);
	GPIO_SetState(GPIO_SREG_OE, false);
}
//---------------------

void COMM_TOSURaw(uint16_t Data)
{
	COMM_OutputRegister_Write(Data);
}
//---------------------

void COMM_TOSU500()
{
	COMM_OutputRegister_Write(COMM_TOSU_MASK_500);
}
//-----------------------------

void COMM_TOSU1000()
{
	COMM_OutputRegister_Write(COMM_TOSU_MASK_1000);
}
//-----------------------------

void COMM_TOSU1500()
{
	COMM_OutputRegister_Write(COMM_TOSU_MASK_1500);
}
//-----------------------------

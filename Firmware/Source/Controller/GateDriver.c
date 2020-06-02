// Includes
//
#include "GateDriver.h"
#include "LowLevel.h"
//

// Defines
//
#define BIT_15		0x8000
//

// Functions prototypes
//
void GateDriver_WriteDAC1(uint16_t Data);
void GateDriver_WriteDAC2(uint16_t Data);
//

// Functions
//

void GateDriver_Set_Current(uint16_t Data)
{
	GateDriver_WriteDAC2(Data);
}
//---------------------

void GateDriver_Set_CompThreshold(uint16_t Data)
{
	Data |= BIT_15;

	GateDriver_WriteDAC2(Data);
}
//---------------------

void GateDriver_Set_FallRate(uint16_t Data)
{
	GateDriver_WriteDAC1(Data);
}
//---------------------

void GateDriver_Set_RiseRate(uint16_t Data)
{
	Data |= BIT_15;

	GateDriver_WriteDAC1(Data);
}
//---------------------

void GateDriver_WriteDAC1(uint16_t Data)
{
	LL_GD_DAC_CS_SYNC1(TRUE);
	LL_DAC_LDAC(TRUE);

	LL_GD_DAC_CS_SYNC1(FALSE);
	SPI_WriteByte(SPI2, Data);
	LL_GD_DAC_CS_SYNC1(TRUE);

	LL_DAC_LDAC(FALSE);
	LL_DAC_LDAC(TRUE);
}
//---------------------

void GateDriver_WriteDAC2(uint16_t Data)
{
	LL_GD_DAC_CS_SYNC2(TRUE);
	LL_DAC_LDAC(TRUE);

	LL_GD_DAC_CS_SYNC2(FALSE);
	SPI_WriteByte(SPI2, Data);
	LL_GD_DAC_CS_SYNC2(TRUE);

	LL_DAC_LDAC(FALSE);
	LL_DAC_LDAC(TRUE);
}
//---------------------




// Header
#include "GateDriver.h"
// Includes
#include "Board.h"
#include "Delay.h"

// Defines
#define DAC_CHANNEL_B		0x8000

// Functions prototypes
void GateDriver_WriteDACx(uint16_t Data, GPIO_PortPinSetting CS_SYNC);

// Functions
//
void GateDriver_Sync(bool State)
{
	GPIO_SetState(GPIO_SYNC_GD, State);
}
//---------------------

void GateDriver_SetCurrent(uint16_t Data)
{
	Data &= ~DAC_CHANNEL_B;
	GateDriver_WriteDACx(Data, GPIO_CS_GD2);
}
//---------------------

void GateDriver_SetCompThreshold(uint16_t Data)
{
	Data |= DAC_CHANNEL_B;
	GateDriver_WriteDACx(Data, GPIO_CS_GD2);
}
//---------------------

void GateDriver_SetFallRate(uint16_t Data)
{
	Data &= ~DAC_CHANNEL_B;
	GateDriver_WriteDACx(Data, GPIO_CS_GD1);
}
//---------------------

void GateDriver_SetRiseRate(uint16_t Data)
{
	Data |= DAC_CHANNEL_B;
	GateDriver_WriteDACx(Data, GPIO_CS_GD1);
}
//---------------------

void GateDriver_WriteDACx(uint16_t Data, GPIO_PortPinSetting CS_SYNC)
{
	GPIO_SetState(CS_SYNC, false);
	SPI_WriteByte(SPI2, Data);
	GPIO_SetState(CS_SYNC, true);
	DELAY_US(1);

	GPIO_SetState(GPIO_LDAC, false);
	DELAY_US(1);
	GPIO_SetState(GPIO_LDAC, true);
	DELAY_US(1);
}
//---------------------

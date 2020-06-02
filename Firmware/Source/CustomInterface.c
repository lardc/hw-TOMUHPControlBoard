#include "CustomInterface.h"
#include "Delay.h"

//Definitions
//
#define	ST_FAN			0x0040
#define	ST_CONTACTOR	0x0020
#define	ST_PS_ON		0x0010
#define	ST_GATE_8		0x0100
#define	ST_GATE_7		0x0200
#define	ST_GATE_6		0x0400
#define	ST_GATE_5		0x0800
#define	ST_GATE_4		0x1000
#define	ST_GATE_3		0x2000
#define	ST_GATE_2		0x4000
#define	ST_GATE_1		0x8000

// Mask array
//
static uint16_t TOCUGateArray[8] = { ST_GATE_1, ST_GATE_2, ST_GATE_3, ST_GATE_4, ST_GATE_5, ST_GATE_6, ST_GATE_7, ST_GATE_8 };

// Forward functions
//
uint16_t CUSTINT_PackData(float Current, bool Fan, bool Contactor, bool PSon);

// Functions
//
void CUSTINT_SendTOCU(float Current, bool Fan, bool Contactor, bool PSon)
{
	CUSTINT_SendRaw(CUSTINT_PackData(Current, Fan, Contactor, PSon));
}
//------------------------------------------------------------------------------

void CUSTINT_SendRaw(uint16_t Data)
{
	/*for (int cnt = 0; cnt < 16; cnt++)
	{
		LL_TOCUSerialData((Data >> cnt) & 1);
		DELAY_US(1);
		LL_TOCUSerialSRCK(TRUE);
		DELAY_US(1);
		LL_TOCUSerialSRCK(FALSE);
	}

	LL_TOCUSerialRCK(TRUE);
	DELAY_US(1);
	LL_TOCUSerialRCK(FALSE);
	LL_TOCUSerialData(FALSE);
	LL_TOCUSerialEnable(TRUE);*/
}
//------------------------------------------------------------------------------

uint16_t CUSTINT_PackData(float Current, bool Fan, bool Contactor, bool PSon)
{
	uint16_t Data = 0;
	uint8_t IntCurrent = (uint16_t)(Current / CURRENT_BIT);

	for (int cnt = 0; cnt < 8; cnt++)
	{
		if (IntCurrent & (1 << cnt))
			Data |= TOCUGateArray[cnt];
	}

	if (Fan)
		Data |= ST_FAN;
	if (Contactor)
		Data |= ST_CONTACTOR;
	if (!PSon)
		Data |= ST_PS_ON;

	return Data;
}
//------------------------------------------------------------------------------

uint32_t CUSTINT_ReceiveDataSR(void)
{
	/*uint32_t Data = 0;

	LL_ShiftRegLoad(FALSE);
	DELAY_US(1);
	LL_ShiftRegLoad(TRUE);

	LL_ShiftRegClk(TRUE);
	LL_ShiftRegCS(FALSE);

	for (int cnt = 0; cnt < 24; cnt++)
	{
		LL_ShiftRegClk(FALSE);
		if (LL_IsShiftRegData())
			Data |= 1 << cnt;
		LL_ShiftRegClk(TRUE);
	}
	LL_ShiftRegCS(TRUE);

	return Data;*/
}
//------------------------------------------------------------------------------

uint32_t CUSTINT_UnpackData10SR(uint32_t Data)
{
	uint32_t OutData;

	OutData  = (Data >> 16) & 0x00FF;
	OutData |=  Data        & 0x0F00;

	return T_OSCILLATOR * OutData;
}
//------------------------------------------------------------------------------

uint32_t CUSTINT_UnpackData90SR(uint32_t Data)
{
	uint32_t OutData;

	OutData  = (Data << 4)  & 0x0FF0;
	OutData |= (Data >> 12) & 0x000F;

	return T_OSCILLATOR * OutData;
}
//------------------------------------------------------------------------------

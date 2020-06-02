#include "Measurement.h"
#include "DataTable.h"
#include "ZwADC.h"
#include "LowLevel.h"
#include "Board.h"

// Functions prototypes
//
void MEASURE_Set_Uref(uint16_t Data);
//

// Functions
//
uint16_t MEASURE_BatteryVoltageRaw()
{
	return ADC_Measure(ADC1, 1);
}
//------------------------------------------------------------------------------

float MEASURE_BatteryVoltage()
{
	float K  = (float)DataTable[REG_VOLTAGE_GAIN] / 10;
	//
	float P0 = (Int16S)DataTable[REG_P0_U_VSO];
	float P1 = (float)DataTable[REG_P1_U_VSO] / 1000;
	float P2 = (float)((Int16S)DataTable[REG_P2_U_VSO]) / 1e6;

	// Оцифровка и удаление смещения
	float Result = (float)MEASURE_BatteryVoltageRaw() + (Int16S)DataTable[REG_VSO_MEAS_OFFSET];

	// Пересчёт результата в напряжение
	Result = (Result * ADC_REF_MV) / ADC_RESOLUTION * K / 1000;

	// Тонкая корректировка
	Result = Result * Result * P2 + Result * P1 + P0;

	return (Result > 0) ? Result : 0;
}
//------------------------------------------------------------------------------

uint16_t MEASURE_DUTCurrentRaw()
{
	return ADC_Measure(ADC1, 2);
}
//------------------------------------------------------------------------------

float MEASURE_DUTCurrent()
{
	float K = (float)DataTable[REG_CURRENT_GAIN] / 100;
	float ShuntRes = (float)DataTable[REG_CURRENT_SHUNT] / 1000;
	//
	float P0 = (Int16S)DataTable[REG_P0_I_DUT];
	float P1 = (float)DataTable[REG_P1_I_DUT] / 1000;
	float P2 = (float)((Int16S)DataTable[REG_P2_I_DUT]) / 1e6;

	// Оцифровка и удаление смещения
	float Result = (float)MEASURE_DUTCurrentRaw() + (Int16S)DataTable[REG_I_MEAS_OFFSET];

	// Пересчёт результата в напряжение
	Result = (Result * ADC_REF_MV) / (ADC_RESOLUTION * K * ShuntRes);

	// Тонкая корректировка
	Result = Result * Result * P2 + Result * P1 + P0;

	return (Result > 0) ? Result : 0;
}
//------------------------------------------------------------------------------

void MEASURE_Set_Uref10(uint16_t Data)
{
	Data |= 0x8000;

	MEASURE_Set_Uref(Data);
}
//---------------------

void MEASURE_Set_Uref90(uint16_t Data)
{
	MEASURE_Set_Uref(Data);
}
//---------------------

void MEASURE_Set_Uref(uint16_t Data)
{
	LL_DAC_CS_SYNC(TRUE);
	LL_DAC_LDAC(TRUE);

	LL_DAC_CS_SYNC(FALSE);
	SPI_WriteByte(SPI2, Data);
	LL_DAC_CS_SYNC(TRUE);

	LL_DAC_LDAC(FALSE);
	LL_DAC_LDAC(TRUE);
}
//---------------------

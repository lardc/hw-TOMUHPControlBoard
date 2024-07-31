// Header
#include "SaveToFlash.h"

// Includes
#include "ZwNFLASH.h"
#include "SysConfig.h"
#include "StorageDescription.h"
#include "Global.h"

typedef enum __ReadCountersStateMachine
{
	RCSM_DescriptionType = 0,
	RCSM_DescriptionLength,
	RCSM_Description,
	RCSM_DataType,
	RCSM_DataLength,
	RCSM_Data
} ReadCountersStateMachine;

ReadCountersStateMachine CurrentState = RCSM_DescriptionType;
Int16U LineNumber;
Int16U DataPosition;
Int32U FlashPosition;

void STF_ResetStateMachine()
{
	CurrentState = RCSM_DescriptionType;
	LineNumber = 0;
	DataPosition = 0;
	FlashPosition = FLASH_COUNTER_START_ADDR;
}

Int16U STF_ReadCounter()
{
	Int16U RetVal = 0;
	switch(CurrentState)
	{
		case RCSM_DescriptionType:
			RetVal = DT_Char;
			CurrentState = RCSM_DescriptionLength;
			DataPosition++;
			break;

		case RCSM_DescriptionLength:
			RetVal = CounterStorageDescription[LineNumber].Length;
			CurrentState = RCSM_Description;
			DataPosition = 0;
			break;

		case RCSM_Description:
			RetVal = CounterStorageDescription[LineNumber].Description[DataPosition++];
			if (RetVal == '\0')
				CurrentState = RCSM_DataType;
			break;

		case RCSM_DataType:
			RetVal = CounterStorageDescription[LineNumber].Type;
			CurrentState = RCSM_DataLength;
			break;

		case RCSM_DataLength:
			RetVal = CounterStorageDescription[LineNumber].Length;
			CurrentState = RCSM_Data;
			DataPosition = 0;
			break;

		case RCSM_Data:
			RetVal = NFLASH_ReadWord16(FlashPosition);
			FlashPosition += 2;
			DataPosition++;

			if (DataPosition >= 2)
			{
				CurrentState = RCSM_DescriptionType;
				LineNumber++;
			}
			break;
	}

	return RetVal;
}

// Forward functions
Int16U STF_StartAddressShift(Int16U Index);
Int16U STF_GetTypeLength(DataType CurrentType);
Int32U STF_ShiftStorageEnd();
Int32U STF_ShiftCounterStorageEnd();
Int16U StrLen(const char* string);

// Functions
//
void STF_AssignPointer(Int16U Index, Int32U Pointer)
{
	if(Index < StorageSize)
		TablePointers[Index] = Pointer;
}
// ----------------------------------------

void STF_AssignCounterPointer(Int16U Index, Int32U Pointer)
{
	if (Index < CounterStorageSize)
		CounterTablePointers[Index] = (CounterData){0, Pointer};
}
// ----------------------------------------

void STF_SaveDiagData()
{
	Int32U ShiftedAddress = STF_ShiftStorageEnd();
	Int16U MaxDataLength = 0;
	Int16U Description[MAX_DESCRIPTION_LEN];

	Int16U i;
	for (i = 0; i < StorageSize; ++i)
		MaxDataLength += StorageDescription[i].Length * STF_GetTypeLength(StorageDescription[i].Type) * 2 + 8 + StrLen(StorageDescription[i].Description) * 2;

	if (ShiftedAddress + MaxDataLength >= FLASH_DIAG_END_ADDR)
		return;

	NFLASH_Unlock();
	for(i = 0; i < StorageSize; i++)
	{
		Int16U DescriptionLength = StrLen(StorageDescription[i].Description);
		static Int16U DescriptionHeader[2] = {DT_Char};
		DescriptionHeader[1] = DescriptionLength;

		// Запись заголовка описания
		NFLASH_WriteArray16(ShiftedAddress, DescriptionHeader, 2);
		ShiftedAddress += 4;

		// Запись описания
		Int16U j;
		for (j = 0; j < DescriptionLength; ++j)
			Description[j] = StorageDescription[i].Description[j];
		NFLASH_WriteArray16(ShiftedAddress, Description, DescriptionLength);
		ShiftedAddress += DescriptionLength * 2;

		// Запись заголовка данных
		Int16U DataHeader[2] = {StorageDescription[i].Type, StorageDescription[i].Length};
		NFLASH_WriteArray16(ShiftedAddress, DataHeader, 2);
		ShiftedAddress += 4;

		// Запись данных при наличии указателя
		if(TablePointers[i])
		{
			Int16U DataWriteLength = StorageDescription[i].Length * STF_GetTypeLength(StorageDescription[i].Type);

			NFLASH_WriteArray16(ShiftedAddress, (pInt16U)TablePointers[i], DataWriteLength);
			ShiftedAddress += DataWriteLength * 2;
		}
	}
}
// ----------------------------------------

void STF_SaveCounterData()
{
	Int32U ShiftedAddress = STF_ShiftCounterStorageEnd();

	// Проверка на то, изменились ли данные с момента последней записи
	Int16U i;
	for (i = 0; i < CounterStorageSize; ++i)
	{
		if (CounterTablePointers[i].Value != *(pInt32U)CounterTablePointers[i].Address)
			break;
	}
	if (i == CounterStorageSize)
		return;

	// Проверка на свободное место в памяти
	if (ShiftedAddress + CounterStorageSize * 4 > FLASH_COUNTER_END_ADDR)
	{
		STF_EraseCounterDataSector();
	}

	NFLASH_Unlock();

	for (i = 0; i < CounterStorageSize; ++i)
	{
		NFLASH_WriteArray32(ShiftedAddress, (uint32_t*)CounterTablePointers[i].Address, 1);
		ShiftedAddress += 4;
	}
}
// ----------------------------------------

Int16U STF_GetTypeLength(DataType CurrentType)
{
	return (CurrentType == DT_Int32U || CurrentType == DT_Int32S || CurrentType == DT_Float) ? 2 : 1;
}
// ----------------------------------------

Int32U STF_ShiftStorageEnd()
{
	Int32U StoragePointer = FLASH_DIAG_START_ADDR;
	while (NFLASH_ReadWord16(StoragePointer) != 0xFFFF)
	{
		Int16U CurrentType = NFLASH_ReadWord16(StoragePointer);

		if (CurrentType > DT_Float)
			break;

		Int16U TypeLength = STF_GetTypeLength(CurrentType);
		StoragePointer += 2;

		Int16U Length = NFLASH_ReadWord16(StoragePointer) * (Int16U)TypeLength;
		StoragePointer += (Length + 1) * 2;
	}
	return StoragePointer;
}
// ----------------------------------------

Int32U STF_ShiftCounterStorageEnd()
{
	Int32U StoragePointer = FLASH_COUNTER_START_ADDR;
	Int16U MaxValuesCounter = 0;

	for (Int32U i = 0 ; i < FLASH_COUNTER_END_ADDR; ++i)
	{
		for (Int16U j = 0; j < CounterStorageSize; ++j)
		{
			Int32U Value = NFLASH_ReadWord32(StoragePointer);
			StoragePointer += 4;

			if (Value == 0xFFFFFFFF)
				MaxValuesCounter++;

			if (MaxValuesCounter >= CounterStorageSize)
				return StoragePointer - CounterStorageSize * 4;
		}
	}
	return StoragePointer;
}
// ----------------------------------------

void STF_EraseDataSector()
{
	NFLASH_ErasePages(FLASH_DIAG_START_ADDR, FLASH_DIAG_END_ADDR);
}
// ----------------------------------------

void STF_EraseCounterDataSector()
{
	NFLASH_ErasePages(FLASH_COUNTER_START_ADDR, FLASH_COUNTER_END_ADDR);
}
// ----------------------------------------

void STF_LoadCounters()
{
	Int32U StoragePointer = STF_ShiftCounterStorageEnd();
	StoragePointer -= CounterStorageSize * STF_GetTypeLength(DT_Int32U) * 2;
	Int32U Value;
	for (Int16U i = 0; i < CounterStorageSize; ++i)
	{
		Value = NFLASH_ReadWord32(StoragePointer);
		StoragePointer += 4;
		CounterTablePointers[i].Value = Value;
	}
}

Int16U StrLen(const char* string)
{
	Int16U n = -1;
	const char* s = string;
	do n++; while (*s++);
	return n;
}
// ----------------------------------------

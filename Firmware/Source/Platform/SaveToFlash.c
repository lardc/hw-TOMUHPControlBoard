// Header
#include "SaveToFlash.h"

// Includes
#include "ZwNFLASH.h"
#include "SysConfig.h"
#include "StorageDescription.h"
#include "Global.h"

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

	NFLASH_Unlock();

	Int16U i;
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
	Int32U StoragePointer = FLASH_CYCLE_START_ADDR;
	Int32U Value;
	for (Int32U i = 0 ; i < FLASH_CYCLE_END_ADDR; ++i)
	{
		for (Int16U j = 0; j < CounterStorageSize; ++j)
		{
			Value = 0;
			Int16U MaxValues = 0;
			for (Int16U k = 0; k < 2; ++k)
			{
				Value += NFLASH_ReadWord16(StoragePointer);
				StoragePointer += 2;
			}

			if (Value == 0xFFFFFFFF)
				MaxValues++;
			if (MaxValues >= CounterStorageSize)
				return StoragePointer - CounterStorageSize * 4;
		}
	}
}
// ----------------------------------------

void STF_EraseDataSector()
{
	NFLASH_ErasePages(FLASH_DIAG_START_ADDR, FLASH_DIAG_END_ADDR);
}
// ----------------------------------------

void STF_EraseCounterSector()
{
	NFLASH_ErasePages(FLASH_CYCLE_START_ADDR, FLASH_CYCLE_END_ADDR);
}
// ----------------------------------------

void LoadCounters()
{
	Int32U StoragePointer = STF_ShiftCounterStorageEnd();
	StoragePointer -= CounterStorageSize * STF_GetTypeLength(DT_Int32U);
	Int32U Value;
	for (Int16U i = 0; i < CounterStorageSize; ++i)
	{
		Value = 0;
		for (Int16U j = 0; j < 2; ++j)
		{
			Value += NFLASH_ReadWord16(StoragePointer);
			StoragePointer += 2;
		}
		CounterTablePointers[i] = (CounterData){Value, CounterTablePointers[i].Address};
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

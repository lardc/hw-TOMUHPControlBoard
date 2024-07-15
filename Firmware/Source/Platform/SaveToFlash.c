// Header
#include "SaveToFlash.h"

// Includes
#include "ZwNFLASH.h"
#include "SysConfig.h"
#include "StorageDescription.h"
#include "Global.h"

#define FLASH_SECTOR_MASK	SECTORH|SECTORG|SECTORF|SECTORE
#define FLASH_START_ADDR	0x08010000
#define FLASH_END_ADDR		0x0801F799

// Forward functions
Int16U STF_StartAddressShift(Int16U Index);
Int16U STF_GetTypeLength(DataType CurrentType);
void STF_Save();
Int32U STF_ShiftStorageEnd();
Int16U StrLen(const char* string);

// Functions
//
void STF_AssignPointer(Int16U Index, Int32U Pointer)
{
	if(Index < StorageSize)
		TablePointers[Index] = Pointer;
}
// ----------------------------------------

void STF_SaveFaultData()
{
	STF_Save();
}
// ----------------------------------------

void STF_Save()
{
	Int32U ShiftedAddress = STF_ShiftStorageEnd();
	Int16U MaxDataLength = 0;

	Int16U i;
	for (i = 0; i < StorageSize; ++i)
		MaxDataLength += StorageDescription[i].Length * STF_GetTypeLength(StorageDescription[i].Type) + 4 + StrLen(StorageDescription[i].Description);

	if (ShiftedAddress + MaxDataLength >= FLASH_END_ADDR)
		return;

	for(i = 0; i < StorageSize; i++)
	{
		Int16U DescriptionLength = StrLen(StorageDescription[i].Description);
		static Int16U DescriptionHeader[2] = {DT_Char};
		DescriptionHeader[1] = DescriptionLength;

		// Запись заголовка описания
		NFLASH_WriteArray16(ShiftedAddress, (pInt16U)DescriptionHeader, 2);
		ShiftedAddress += 2;

		// Запись описания
		NFLASH_WriteArray16(ShiftedAddress, (pInt16U)StorageDescription[i].Description, DescriptionLength);
		ShiftedAddress += DescriptionLength;

		// Запись заголовка данных
		Int16U DataHeader[2] = {StorageDescription[i].Type, StorageDescription[i].Length};
		NFLASH_WriteArray16(ShiftedAddress,
				(pInt16U)DataHeader, 2);
		ShiftedAddress += 2;

		// Запись данных при наличии указателя
		if(TablePointers[i])
		{
			Int16U DataWriteLength = StorageDescription[i].Length * STF_GetTypeLength(StorageDescription[i].Type);

			NFLASH_WriteArray16(ShiftedAddress,
						(pInt16U)TablePointers[i], DataWriteLength);
			ShiftedAddress += DataWriteLength;
		}
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
	Int32U StoragePointer = FLASH_START_ADDR;
	while (NFLASH_ReadWord16(StoragePointer) != 0xFFFF)
	{
		Int16U CurrentType = NFLASH_ReadWord16(StoragePointer);

		if (CurrentType > DT_Float)
			break;

		Int16U TypeLength = STF_GetTypeLength(CurrentType);
		++StoragePointer;

		Int16U Length = NFLASH_ReadWord16(StoragePointer) * (Int16U)TypeLength;
		StoragePointer += Length + 1;
	}
	return StoragePointer;
}
// ----------------------------------------

void STF_EraseDataSector()
{
	NFLASH_ErasePages(FLASH_START_ADDR, FLASH_END_ADDR);
}
// ----------------------------------------

Int16U StrLen(const char* string)
{
	Int16U n = -1;
	const char* s = string;
	do n++; while (*s++);
	return n;
}

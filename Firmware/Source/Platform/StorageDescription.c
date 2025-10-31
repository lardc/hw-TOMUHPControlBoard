// Header
#include "StorageDescription.h"
#include "Global.h"

// Variables
const RecordDescription StorageDescription[] = {};

#define SIZE_OF_POINTERS (sizeof(StorageDescription) / sizeof(StorageDescription[0]))
Int32U TablePointers[SIZE_OF_POINTERS ? SIZE_OF_POINTERS : 1] = {0};
const Int16U StorageSize = SIZE_OF_POINTERS;

const CounterDescription CounterStorageDescription[] =
{
	{"0. 600 V Contactor"},
	{"1. 1000 V Contactor"},
	{"2. 1500 V Contactor"},
	{"3. Potential switch"}
};
CounterData CounterTablePointers[sizeof(CounterStorageDescription) / sizeof(CounterStorageDescription[0])] = {0};
const Int16U CounterStorageSize = sizeof(CounterStorageDescription) / sizeof(CounterStorageDescription[0]);


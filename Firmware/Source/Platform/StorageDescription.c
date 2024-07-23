// Header
#include "StorageDescription.h"
#include "Global.h"

// Variables
RecordDescription StorageDescription[] =
{
		{"REG 1 Shunt Resistance (mOhm)",	DT_Int16U, 1},
		{"EP 1 Array of current values",	DT_Int16U, 300},
};
Int32U TablePointers[sizeof(StorageDescription) / sizeof(StorageDescription[0])] = {0};
const Int16U StorageSize = sizeof(StorageDescription) / sizeof(StorageDescription[0]);

RecordDescription CounterStorageDescription[] =
{
		{"c1", DT_Int32U, 1},
		{"c2", DT_Int32U, 1},
		{"c3", DT_Int32U, 1},
		{"c4", DT_Int32U, 1},
		{"c5", DT_Int32U, 1},
};
CounterData CounterTablePointers[sizeof(CounterStorageDescription) / sizeof(CounterStorageDescription[0])] = {0};
const Int16U CounterStorageSize = sizeof(CounterStorageDescription) / sizeof(CounterStorageDescription[0]);


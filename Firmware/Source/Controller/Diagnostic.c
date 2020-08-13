// Header
#include "Diagnostic.h"

// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "LowLevel.h"
#include "GateDriver.h"
#include "Delay.h"
#include "Measurement.h"
#include "Commutation.h"
#include "BCCIMHighLevel.h"
#include "Controller.h"

// Functions
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError)
{
	switch(ActionID)
	{
		case ACT_DBG_GD_I_SET:
			{
				CachedMeasurementSettings.GateCurrent = (float)DataTable[REG_DBG] / 10;
				GateDriver_SetCurrent(CachedMeasurementSettings.GateCurrent);
			}
			break;

		case ACT_DBG_GD_TRIG_THRESHOLD:
			{
				GateDriver_SetCompThreshold(((float)DataTable[REG_DBG] / 10) * GATE_CURRENT_THRESHOLD);
			}
			break;

		case ACT_DBG_GD_I_RISE_RATE:
			{
				CachedMeasurementSettings.GateCurrentRiseRate = (float)DataTable[REG_DBG] / 10;
				GateDriver_SetRiseRate(&CachedMeasurementSettings);
			}
			break;

		case ACT_DBG_GD_I_FALL_RATE:
			{
				CachedMeasurementSettings.GateCurrentFallRate = (float)DataTable[REG_DBG] / 10;
				GateDriver_SetFallRate(&CachedMeasurementSettings);
			}
			break;

		case ACT_DBG_GD_OSC_SYNC:
			{
				LL_SyncOscilloscope(true);
				GateDriver_Sync(true);
				DELAY_US(100);
				GateDriver_Sync(false);
				LL_SyncOscilloscope(false);
			}
			break;

		case ACT_DBG_PS_EN:
			{
				LL_PsBoard_PowerOutput(DataTable[REG_DBG] ? true : false);
			}
			break;

		case ACT_DBG_FAN:
			{
				LL_UnitFan(true);
				DELAY_US(1000000);
				LL_UnitFan(false);
			}
			break;

		case ACT_DBG_IND:
			{
				LL_ExternalLED(true);
				DELAY_US(1000000);
				LL_ExternalLED(false);
			}
			break;

		case ACT_DBG_RLC:
			{
				COMM_InternalCommutation(DataTable[REG_DBG] ? true : false);
			}
			break;

		case ACT_DBG_RELAY:
			{
				LL_PsBoard_PowerInput(DataTable[REG_DBG] ? true : false);
			}
			break;

		case ACT_DBG_SFTY_ACTIVATION:
			{
				COMM_EnableSafetyInput(DataTable[REG_DBG] ? true : false);
			}
			break;

		case ACT_DBG_READ_EXT_REG:
			{
				DataTable[REG_DBG] = LL_HSTimers_Read();
			}
			break;

		case ACT_DBG_SNC_TOCU:
			{
				LL_SyncTOCU(true);
				DELAY_US(100);
				LL_SyncTOCU(false);
			}
			break;

		case ACT_DBG_OUT_ISO:
			{
				COMM_TOSURaw(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_U_REF_U10:
			{
				MEASURE_SetUref10(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_U_REF_U90:
			{
				MEASURE_SetUref90(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_OSC_SYNC:
			{
				LL_SyncOscilloscope(true);
				DELAY_US(1000000);
				LL_SyncOscilloscope(false);
			}
			break;

		case ACT_DBG_TOSU_SWITCH:
			{
				COMM_TOSU(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_POT_SWITCH:
			{
				COMM_PotSwitch(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_READ_PRESSURE:
			{
				DataTable[REG_DBG] = COMM_IsPressureTrig();
			}
			break;

		case ACT_DBG_TRIG_RST:
			{
				LL_GateLatchReset();
			}
			break;

		case ACT_DBG_M_RESET:
			{
				LL_HSTimers_Reset();
			}
			break;

		case ACT_DBG_SLAVE_CALL:
			{
				BHL_Call(DataTable[REG_DBG_SLAVE_NID], DataTable[REG_DBG_SLAVE_ACT]);
			}
			break;

		case ACT_DBG_SLAVE_READ_REG:
			{
				uint16_t reg = 0;
				BHL_ReadRegister(DataTable[REG_DBG_SLAVE_NID], DataTable[REG_DBG_SLAVE_REG], &reg);
				DataTable[REG_DBG_SLAVE_DATA] = reg;
			}
			break;

		case ACT_DBG_SLAVE_WRITE_REG:
			{
				BHL_WriteRegister(DataTable[REG_DBG_SLAVE_NID], DataTable[REG_DBG_SLAVE_REG], DataTable[REG_DBG_SLAVE_DATA]);
			}
			break;

		case ACT_DBG_RESET_INT_ERROR:
			{
				BHL_ResetError();
			}
			break;

		default:
			return false;
	}

	return true;
}

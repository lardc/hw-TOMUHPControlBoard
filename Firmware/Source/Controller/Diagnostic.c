// Header
#include "Diagnostic.h"

// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "LowLevel.h"
#include "GateDriver.h"
#include "Delay.h"
#include "Measurement.h"

// Functions
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError)
{
	switch(ActionID)
	{
		case ACT_DBG_GD_I_SET:
			{
				GateDriver_SetCurrent(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_GD_TRIG_THRESHOLD:
			{
				GateDriver_SetCompThreshold(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_GD_I_RISE_RATE:
			{
				GateDriver_SetRiseRate(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_GD_I_FALL_RATE:
			{
				GateDriver_SetFallRate(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_GD_SYNC:
			{
				GateDriver_Sync(true);
				DELAY_US(100);
				GateDriver_Sync(false);
			}
			break;

		case ACT_DBG_PS_EN:
			{
				LL_PsBoard_PowerOutput(true);
				DELAY_US(1000000);
				LL_PsBoard_PowerOutput(false);
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
				LL_INT_Commutation(true);
				DELAY_US(1000000);
				LL_INT_Commutation(false);
			}
			break;

		case ACT_DBG_RELAY:
			{
				LL_PsBoard_PowerInput(true);
				DELAY_US(1000000);
				LL_PsBoard_PowerInput(false);
			}
			break;

		case ACT_DBG_SFTY_ACTIVATION:
			{
				LL_MonitorSafetyInput(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_READ_EXT_REG:
			{
				DataTable[REG_DBG] = ReadExtReg();
			}
			break;

		case ACT_DBG_TOCU_SYNC:
			{
				LL_TOCU_Sync(true);
				DELAY_US(1000000);
				LL_TOCU_Sync(false);
			}
			break;

		case ACT_DBG_OUT_ISO:
			{
				OutPut_ISO(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_U_REF_U10:
			{
				MEASURE_Set_Uref10(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_U_REF_U90:
			{
				MEASURE_Set_Uref90(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_OSC_SYNC:
			{
				LL_OscSync(true);
				DELAY_US(1000000);
				LL_OscSync(false);
			}
			break;

		case ACT_DBG_TRIG_RST:
			{
				LL_GateLatch(false);
				LL_GateLatch(true);
			}
			break;

		case ACT_DBG_SREG_OE:
			{
				LL_OutReg_OE(DataTable[REG_DBG]);
			}
			break;

		default:
			return false;
	}

	return true;
}

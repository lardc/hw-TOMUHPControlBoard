#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// �������
// 
#define ACT_ENABLE_POWER				1	// ������� � ��������� ��������
#define ACT_DISABLE_POWER				2	// ���������� �����
#define ACT_FAULT_CLEAR					3	// ������� fault
#define ACT_WARNING_CLEAR				4	// ������� warning
#define ACT_RESET_DEVICE				5	// ���������� ����������
//
// ������� �������
#define ACT_DBG_GD_I_SET				10	// �������� ��������� ��������� ���� � TOMUHPGateDriver
#define ACT_DBG_GD_TRIG_THRESHOLD		11	// �������� ��������� ������ ������������ �����������
#define ACT_DBG_GD_I_RISE_RATE			12	// �������� ��������� �������� ���������� ���� � TOMUHPGateDriver
#define ACT_DBG_GD_I_FALL_RATE			13	// �������� ��������� �������� ����� ���� � TOMUHPGateDriver
#define ACT_DBG_GD_SYNC					14	// �������� ������ Sync_GD
#define ACT_DBG_PS_EN					15	// �������� ������ PS_EN
#define ACT_DBG_FAN						16	// �������� ������ FAN
#define ACT_DBG_IND						17	// �������� ������ IND
#define ACT_DBG_RLC						18	// �������� ������ RLC
#define ACT_DBG_RELAY					19	// �������� ������ RELAY
#define ACT_DBG_SFTY_ACTIVATION			20	// �������� ������ SFTY_EN
#define ACT_DBG_READ_EXT_REG			21	// �������� ������ �������� ��������
#define ACT_DBG_SNC_TOCU				22	// �������� ������ SNC_TOCU
#define ACT_DBG_OUT_ISO					23	// �������� ������������� �������
#define ACT_DBG_U_REF_U10				24	// �������� ��������� �������� ���������� ��� ����������� U10
#define ACT_DBG_U_REF_U90				25	// �������� ��������� �������� ���������� ��� ����������� U90
#define ACT_DBG_OSC_SYNC				26	// �������� ������ SYNC
#define ACT_DBG_TRIG_RST				27	// �������� ������ ��������
#define ACT_DBG_SREG_OE					28	// �������� ������ SREG_OE
#define ACT_DBG_M_RESET					29	// �������� ������ M_RESET
//
#define ACT_MEASURE_START				100	// ������ ���������
#define ACT_MEASURE_STOP				101	// ������� ���������
//
#define ACT_SAVE_TO_ROM					200	// ���������� ���������������� ������ �� FLASH ����������
#define ACT_RESTORE_FROM_ROM			201	// �������������� ������ �� FLASH
#define ACT_RESET_TO_DEFAULT			202	// ����� DataTable � ��������� �� ���������
//
#define ACT_BOOT_LOADER_REQUEST			320	// ���������� ���������� � ����� ��������������������
//
// -----------------------------------------------

// ��������
// 
// ����������� ��������
// 0 - 2
#define REG_CURRENT_SHUNT				3	// ������������� ����� (� ���) (�1000)
#define REG_CURRENT_GAIN				4	// ����������� �������� ����������� ������ ��������� ���� (x100)
#define REG_P0_I_DUT					5	// ����������� P0 ����������� �������� ���� (� �)
#define REG_P1_I_DUT					6	// ����������� P1 ����������� �������� ���� (�1000)
#define REG_P2_I_DUT					7	// ����������� P2 ����������� �������� ���� (x1e6)
// 8 - 11
#define REG_VOLTAGE_GAIN				12	// ����������� ������� ����������� ������ ��������� ���������� (x10)
#define REG_P0_U_VSO					13	// ����������� P0 ����������� �������� ���������� ������ ������� (� �)
#define REG_P1_U_VSO					14	// ����������� P1 ����������� �������� ���������� ������ ������� (x1000)
#define REG_P2_U_VSO					15	// ����������� P1 ����������� �������� ���������� ������ ������� (x1�6)
// 16 - 29
#define REG_VSO_MEAS_OFFSET				30	// ����� �� ����� ��� ��� ��������� Vso [bit]
#define REG_I_MEAS_OFFSET				31	// ����� �� ����� ��� ��� ��������� I [bit]
// 32 - 127

// ������������ �������� ������-������
#define REG_CURRENT_VALUE				128	// �������� ������� ���� [�]
//
#define REG_DBG_TOCU_DATA				150	// ����� �������� ��� �������� � TOCU
//
#define REG_DBG							190

// �������� ������ ������
#define REG_DEV_STATE					192	// ������� ���������
#define REG_FAULT_REASON				193	// ������� Fault
#define REG_DISABLE_REASON				194	// ������� Disable
#define REG_WARNING						195	// ������� Warning
#define REG_PROBLEM						196	// ������� Problem
#define REG_TEST_FINISHED				197
//
#define REG_DBG_VSO_VALUE				200	// ���������� �� ������� (� �)
#define REG_DBG_I_DUT_VALUE				201	// ��� �� DUT (� �)
#define REG_DBG_PRE_I_DUT_VALUE			202	// ��������������� ��� �� DUT (� �)
#define REG_DBG_ADC_RAW					203	// ���������� ����� �������� �� ������ ���������� � ������ ���� (� �����)
//
#define REG_MEAS_CURRENT_VALUE			250	// ���������� �������� ���� [�]
#define	REG_MEAS_TIME_DELAY				251	// ���������� �������� �������� ��������� [��]
#define	REG_MEAS_TIME_ON				252	// ���������� �������� ������� ��������� [��]
//
// -----------------------------------------------


//  ENDPOINTS
//
// -----------------------------------------------


// Operation results
//
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
// 
#define DF_NONE							0
#define DF_BATTERY						1	// �������� ������ �������

// Problem
//
#define PROBLEM_NONE					0
#define PROBLEM_NO_CTRL_NO_PWR			1	// ���������� ���� ���������� � �������� ����
#define PROBLEM_NO_PWR					2	// ���������� �������� ����
#define PROBLEM_SHORT					3	// �� �� ������
#define PROBLEM_NO_POT_SIGNAL			4	// ��� ������� � ������������� �����
#define PROBLEM_OVERFLOW90				5	// ������������ �������� ������ 90%
#define PROBLEM_OVERFLOW10				6	// ������������ �������� ������ 10%

//  Warning
// 
#define WARNING_NONE					0
#define WARNING_I_OUT_OF_RANGE			1	// ���������� �������� ���� ��� ���������

//  User Errors
// 
#define ERR_NONE						0	//  ������ ���
#define ERR_CONFIGURATION_LOCKED		1	//  ���������� �������� �� ������
#define ERR_OPERATION_BLOCKED			2	//  �������� �� ����� ���� ��������� � ������� ��������� ����������
#define ERR_DEVICE_NOT_READY			3	//  ���������� �� ������ ��� ����� ���������
#define ERR_WRONG_PWD					4	//  ������������ ����

//  Password to unlock non-volatile area for write
// 
#define ENABLE_LOCKING					FALSE


typedef enum __DeviceState
{
	// �������� ���������
	DS_None							= 0,	// ���� � �������������� ���������
	DS_Fault						= 1,	// ���� � ��������� Fault
	DS_Disabled						= 2,	// ���� � ��������� Disabled
	DS_Charging						= 3,	// ���� � �������� ������ �������
	DS_Ready						= 4,	// ���� � ��������� ����������
	DS_InProcess					= 5,	// ���� � �������� ������������ ��������
} DeviceState;


#endif //  __DEV_OBJ_DIC_H

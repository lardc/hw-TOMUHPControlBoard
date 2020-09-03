#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// �������
#define ACT_ENABLE_POWER				1	// ������� � ��������� ��������
#define ACT_DISABLE_POWER				2	// ���������� �����
#define ACT_FAULT_CLEAR					3	// ������� fault
#define ACT_WARNING_CLEAR				4	// ������� warning

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
#define ACT_DBG_SNC_TOCU				20	// �������� ������ SNC_TOCU
#define ACT_DBG_OSC_SYNC				21	// �������� ������ SYNC
#define ACT_DBG_TOSU_SWITCH				22	// �������� ���������� TOSU
#define ACT_DBG_POT_SWITCH				23	// �������� ���������� PotSwitch

#define ACT_DBG_U_REF_U10				24	// �������� ��������� �������� ���������� ��� ����������� U10
#define ACT_DBG_U_REF_U90				25	// �������� ��������� �������� ���������� ��� ����������� U90
#define ACT_DBG_READ_PRESSURE			26	// �������� ������ ������� ��������
#define ACT_DBG_SFTY_ACTIVATION			27	// �������� ������ SFTY_EN
#define ACT_DBG_OUT_ISO					28	// �������� ������������� �������
#define ACT_DBG_READ_EXT_REG			29	// �������� ������ �������� ��������
#define ACT_DBG_TRIG_RST				30	// �������� ������ ��������
#define ACT_DBG_M_RESET					31	// �������� ������ M_RESET


#define ACT_DBG_SLAVE_CALL				40	// ������ ������� �� slave-����
#define ACT_DBG_SLAVE_READ_REG			41	// ������ �������� �� slave-����
#define ACT_DBG_SLAVE_WRITE_REG			42	// ������ �������� � slave-����
#define ACT_DBG_RESET_INT_ERROR			43	// ����� ������ master-����������

#define ACT_MEASURE_START				100	// ������ ���������
#define ACT_MEASURE_STOP				101	// ��������� ���������
//
#define ACT_GATE_PULSE					110	// ������������ ���� ����������

#define ACT_SAVE_TO_ROM					200	// ���������� ���������������� ������ �� FLASH ����������
#define ACT_RESTORE_FROM_ROM			201	// �������������� ������ �� FLASH
#define ACT_RESET_TO_DEFAULT			202	// ����� DataTable � ��������� �� ���������

#define ACT_BOOT_LOADER_REQUEST			320	// ���������� ���������� � ����� ��������������������
// -----------------------------------------------

// ��������
// ����������� ��������
#define REG_DUT_CURRENT_SHUNT			0	// ������������� ����� (���) (�10)
#define REG_GATE_CURRENT_SHUNT			1	// ������������� ����� � ���� ��������� (���)
//
#define REG_TOCU_CHARGE_TIMEOUT			2	// ������� �������� ������ ����� TOCU (��)
#define REG_TOCU_RES_PER_BIT			3	// ������������� ������ ���� ����� TOCU (��)
#define REG_ID_THRESHOLD				4	// ����� �������� ���� �� ��������� ������� (% �� REG_ANODE_CURRENT)
#define REG_MEAS_TIME_LOW				5	// ����������� ���������� �������� ������� (��)
#define REG_FAN_OPERATE_TIME			6	// ����� ������ ����������� (�)
#define REG_FAN_OPERATE_PERIOD			7	// ������ ������ �����������
#define REG_GATE_I_FALL_RATE			8	// �������� ����� ���� ���������� (�/���) (x10)
#define REG_GATE_EDGE_TIME_MIN			9	// ����������� ������������ ������ ���� ���������� (���) (�10)
#define REG_GATE_TIME_CHARGE			10	// ����� ������ GateDriver (��)
//
#define REG_MUTE_SAFETY_MONITOR			11	// ���������� ����������� ������� ������������
#define REG_MUTE_PRESSURE_MONITOR		12	// ���������� ����������� �������� � �������
#define REG_MUTE_PROBLEM				13	// �������� ������ (0 - ���, 1 - ����)
#define REG_MUTE_COMMUTATION			14	// ����� ��� ��������� ���������� ����������
//
#define REG_GATE_I_SET_K				15	// �������� ���� � �������� ��� ���������������� �����������
#define REG_GATE_I_SET_OFFSET			16	// �������� ���� � �������� ��� ��������
#define REG_GATE_I_SET_P2				17	// ������������� ����������� P2 ������� ���� ����������
#define REG_GATE_I_SET_P1				18	// ������������� ����������� P1 ������� ���� ����������
#define REG_GATE_I_SET_P0				19	// ������������� ����������� P0 ������� ���� ����������
//
#define REG_GATE_I_RISE_RATE_K			20	// �������� �������� ���������� ���� � �������� ��� ���������������� �����������
#define REG_GATE_I_RISE_RATE_OFFSET		21	// �������� �������� ���������� ���� � �������� ��� ��������
#define REG_GATE_I_RISE_RATE_P2			22	// ������������� ����������� P2 �������� ���������� ���� ����������
#define REG_GATE_I_RISE_RATE_P1			23	// ������������� ����������� P1 �������� ���������� ���� ����������
#define REG_GATE_I_RISE_RATE_P0			24	// ������������� ����������� P0 �������� ���������� ���� ����������
//
#define REG_GATE_I_FALL_RATE_K			25	// �������� �������� ����� ���� � �������� ��� ���������������� �����������
#define REG_GATE_I_FALL_RATE_OFFSET		26	// �������� �������� ����� ���� � �������� ��� ��������
//
#define REG_GATE_COMP_THRE_P2			27	// ������������� ����������� P2 ������� ������ ������������ �����������
#define REG_GATE_COMP_THRE_P1			28	// ������������� ����������� P1 ������� ������ ������������ �����������
#define REG_GATE_COMP_THRE_P0			29	// ������������� ����������� P0 ������� ������ ������������ �����������
//
#define REG_VCOMP10_600V				30	// ���������� �� ����������� ������ ���������� ������ 10% ��� 500� (��)
#define REG_VCOMP90_600V				31	// ���������� �� ����������� ������ ���������� ������ 90% ��� 500� (��)
#define REG_VCOMP10_1000V				32	// ���������� �� ����������� ������ ���������� ������ 10% ��� 1000� (��)
#define REG_VCOMP90_1000V				33	// ���������� �� ����������� ������ ���������� ������ 90% ��� 1000� (��)
#define REG_VCOMP10_1500V				34	// ���������� �� ����������� ������ ���������� ������ 10% ��� 1500� (��)
#define REG_VCOMP90_1500V				35	// ���������� �� ����������� ������ ���������� ������ 90% ��� 1500� (��)
//
#define REG_I_DUT_600V_P2				36	// ������������� ����������� P2 ������� ���� ��� ���������� 600�
#define REG_I_DUT_600V_P1				37	// ������������� ����������� P1 ������� ���� ��� ���������� 600�
#define REG_I_DUT_600V_P0				38	// ������������� ����������� P0 ������� ���� ��� ���������� 600�
//
#define REG_I_DUT_1000V_P2				39	// ������������� ����������� P2 ������� ���� ��� ���������� 1000�
#define REG_I_DUT_1000V_P1				40	// ������������� ����������� P1 ������� ���� ��� ���������� 1000�
#define REG_I_DUT_1000V_P0				41	// ������������� ����������� P0 ������� ���� ��� ���������� 1000�
//
#define REG_I_DUT_1500V_P2				42	// ������������� ����������� P2 ������� ���� ��� ���������� 1500�
#define REG_I_DUT_1500V_P1				43	// ������������� ����������� P1 ������� ���� ��� ���������� 1500�
#define REG_I_DUT_1500V_P0				44	// ������������� ����������� P0 ������� ���� ��� ���������� 1500�
//
#define REG_I_DUT_GAIN					45	// ����������� �������� ����������� ������ ��������� ���� (x100)
#define REG_I_DUT_OFFSET				46	// ����� �� ����� ��� ��� ��������� ���� (�����)
#define REG_P0_I_DUT					47	// ����������� P0 ����������� �������� ���� (�)
#define REG_P1_I_DUT					48	// ����������� P1 ����������� �������� ���� (�1000)
#define REG_P2_I_DUT					49	// ����������� P2 ����������� �������� ���� (x1e6)
//
#define REG_GATE_I_REF_K				50	// �������� ������ ����������� � �������� ��� ���������������� �����������
#define REG_GATE_I_REF_OFFSET			51	// �������� ������ ����������� � �������� ��� ��������
#define REG_GATE_I_REF_COMPENSATION		52	// ����������� ������ ������ ����������� �� �������� ���������� ���� (%)
// 53 - 127

// ������������ �������� ������-������
#define REG_ANODE_VOLTAGE				128	// ������� ���������� (�)
#define REG_ANODE_CURRENT				129	// ������� ��� (�) (�10)
#define REG_GATE_CURRENT				130	// ��� ���� ���������� (�) (�10)
#define REG_GATE_CURRENT_RISE_RATE		131	// �������� ���������� ���� ���� ���������� (�/���) (�10)

#define REG_DBG_SLAVE_NID				180	// Node ID slave-����
#define REG_DBG_SLAVE_REG				181	// ����� �������� slave-����
#define REG_DBG_SLAVE_DATA				182	// ������ �������� (������������ ��� ������������) slave-����
#define REG_DBG_SLAVE_ACT				183	// ������� ��� slave-����

#define REG_DBG							190	// ���������� ������� 1

// �������� ������ ������
#define REG_DEV_STATE					192	// ������� ���������
#define REG_FAULT_REASON				193	// ������� Fault
#define REG_DISABLE_REASON				194	// ������� Disable
#define REG_WARNING						195	// ������� Warning
#define REG_PROBLEM						196	// ������� Problem
#define REG_OP_RESULT					197	// ������� ������� ��������
#define REG_DEV_SUB_STATE				198	// ������� ���������������� ���������
//
#define REG_SAFETY_STATE				200	// ��������� ������� ������������
#define REG_PRESSURE_STATE				201	// ��������� ������� ��������
//
#define REG_BHL_ERROR_CODE				210	// ������ ���������� ��: ��� ������
#define REG_BHL_DEVICE					211	// ������ ���������� ��: NID ����������
#define REG_BHL_FUNCTION				212	// ������ ���������� ��: ��� �������
#define REG_BHL_EXT_DATA				213	// ������ ���������� ��: ����������� ����������
//
#define REG_MEAS_CURRENT_VALUE			250	// ���������� �������� ���� (�)
#define	REG_MEAS_TIME_DELAY				251	// ���������� �������� �������� ��������� (��)
#define	REG_MEAS_TIME_ON				252	// ���������� �������� ������� ��������� (��)
// -----------------------------------------------

// ������ ��������� � ������ ������ TOCU
#define ACT_TOCU_ENABLE_POWER			1	// ������� � ��������� ��������
#define ACT_TOCU_DISABLE_POWER			2	// ���������� �����
#define ACT_TOCU_FAULT_CLEAR			3	// ������� fault
#define ACT_TOCU_WARNING_CLEAR			4	// ������� warning
#define ACT_TOCU_VOLTAGE_CONFIG			100	// ������������ ���������� �����
#define ACT_TOCU_PULSE_CONFIG			101	// ������������ ����� ��� ��������� ���

#define REG_TOCU_VOLTAGE_SETPOINT		128	// �������� ������� ���������� (�)
#define REG_TOCU_GATE_REGISTER			129	// ��������-����� ������������ ��������

#define REG_TOCU_DEV_STATE				192	// ������� ���������
#define REG_TOCU_FAULT_REASON			193	// ������� Fault
#define REG_TOCU_DISABLE_REASON			194	// ������� Disable
#define REG_TOCU_WARNING				195	// ������� Warning
#define REG_TOCU_PROBLEM				196	// ������� Problem
#define REG_TOCU_OP_RESULT				197	// ������� ���������� ��������
// -----------------------------------------------

// �������
#define EP_CURRENT						1	// ������ �������� ����
// -----------------------------------------------

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0
#define DF_INTERFACE					1	// ������ ������-����������
#define DF_PRESSURE						2	// ����������� ��������
#define DF_TOCU_WRONG_STATE				3	// �������� ��������� ����� TOCU
#define DF_TOCU_STATE_TIMEOUT			4	// ������� �������� �������� TOCU � ������ ���������

// Problem
#define PROBLEM_NONE					0
#define PROBLEM_NO_PWR					1	// ���������� �������� ����
#define PROBLEM_SHORT					2	// �� �� ������
#define PROBLEM_NO_POT					3	// ��� ������� � ������������� �����
#define PROBLEM_OVERFLOW90				4	// ������������ �������� ������ 90%
#define PROBLEM_OVERFLOW10				5	// ������������ �������� ������ 10%
#define PROBLEM_SAFETY_VIOLATION		6	// ������������ ������� ������������

//  Warning
#define WARNING_NONE					0

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  ���������� �������� �� ������
#define ERR_OPERATION_BLOCKED			2	//  �������� �� ����� ���� ��������� � ������� ��������� ����������
#define ERR_DEVICE_NOT_READY			3	//  ���������� �� ������ ��� ����� ���������
#define ERR_WRONG_PWD					4	//  ������������ ����

#endif //  __DEV_OBJ_DIC_H

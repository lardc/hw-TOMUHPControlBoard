# TOMUHP Control Board Software Specification

## Table of Contents

1. [Writable Registers](#writable-registers)
2. [Read-Only Registers](#read-only-registers)
3. [Commands](#commands)
4. [Endpoints](#endpoints)

## Version Information

- **Branch**: `master`
- **Commit**: `e487d53`
- **Last Updated**: $(date)

## Overview

This document provides a comprehensive specification of the TOMUHP Control Board firmware, including all writable and read-only registers, commands, and endpoints. The system is designed for high-power device testing with precise current and voltage control capabilities.

### System Architecture

- **Data Type**: All registers use `Int16U` (16-bit unsigned integer)
- **Volatility**: Registers 0-127 are non-volatile (saved to flash), registers 128-191 are volatile
- **Communication**: CAN bus interface with master-slave architecture
- **Measurement**: High-precision current and voltage measurement with timing analysis

---

## Writable Registers

### Device Configuration & Calibration

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 0 | REG_DUT_CURRENT_SHUNT | Int16U | DUT current shunt resistance (mΩ ×10) | 1 - 10000 | CURRENT_SHUNT_MIN to CURRENT_SHUNT_MAX | Shunt resistance for current measurement |
| 1 | REG_GATE_CURRENT_SHUNT | Int16U | Gate current shunt resistance (mΩ) | 1 - 10000 | CURRENT_SHUNT_MIN to CURRENT_SHUNT_MAX | Shunt resistance for gate current measurement |
| 2 | REG_TOCU_CHARGE_TIMEOUT | Int16U | TOCU charge timeout (ms) | 1000 - 60000 | TOCU_CHRG_TIMEOUT_MIN to TOCU_CHRG_TIMEOUT_MAX | Timeout for TOCU block charge |
| 3 | REG_TOCU_RES_PER_BIT | Int16U | TOCU resistance per bit (Ω) | 1 - 10000 | TOCU_RES_PER_BIT_MIN to TOCU_RES_PER_BIT_MAX | Resistance per bit for TOCU block |
| 4 | REG_SHORT_THRESHOLD | Int16U | Short circuit threshold (A ×10) | 300 - 25000 | DUT_CURRENT_MIN to DUT_CURRENT_MAX | Threshold for short circuit detection |
| 5 | REG_MEAS_TIME_LOW | Int16U | Minimum measurable time (ns) | 0 - 1000 | MEAS_TIME_LOW_MIN to MEAS_TIME_LOW_MAX | Minimum time measurement value |
| 6 | REG_FAN_OPERATE_TIME | Int16U | Fan operation time (s) | 0 - 65535 | 0 to INT16U_MAX | Fan operation duration |
| 7 | REG_FAN_OPERATE_PERIOD | Int16U | Fan operation period | 0 - 65535 | 0 to INT16U_MAX | Fan operation cycle period |
| 8 | REG_GATE_I_FALL_RATE | Int16U | Gate current fall rate (A/μs ×10) | 1000 - 10000 | GATE_I_RATE_MIN to GATE_I_RATE_MAX | Gate current fall rate |
| 9 | REG_GATE_EDGE_TIME_MIN | Int16U | Minimum gate edge time (μs ×10) | 0 - 20 | GATE_FRONT_MIN to GATE_FRONT_MAX | Minimum gate edge duration |
| 10 | REG_GATE_TIME_CHARGE | Int16U | Gate driver charge time (ms) | 100 - 1000 | GATE_TIME_CHRG_MIN to GATE_TIME_CHRG_MAX | Gate driver charging time |

### Safety & Monitoring Control

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 11 | REG_MUTE_SAFETY_MONITOR | Int16U | Disable safety monitoring | 0 | NO | Safety monitoring enabled |
| | | | | 1 | YES | Safety monitoring disabled |
| 12 | REG_MUTE_PRESSURE_MONITOR | Int16U | Disable pressure monitoring | 0 | NO | Pressure monitoring enabled |
| | | | | 1 | YES | Pressure monitoring disabled |
| 13 | REG_MUTE_PROBLEM | Int16U | Error control | 0 | NO | Problem monitoring enabled |
| | | | | 1 | YES | Problem monitoring disabled |
| 14 | REG_MUTE_COMMUTATION | Int16U | Continuous commutation mode | 0 | NO | Commutation mode enabled |
| | | | | 1 | YES | Commutation mode disabled |

### Runtime Control Parameters

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 128 | REG_ANODE_VOLTAGE | Int16U | Anode voltage (V) | 600 | TOU_600V | Anode voltage 600V |
| | | | | 1000 | TOU_1000V | Anode voltage 1000V |
| | | | | 1500 | TOU_1500V | Anode voltage 1500V |
| | | | | 600 - 1500 | DUT_VOLTAGE_MIN to DUT_VOLTAGE_MAX | Anode voltage setting (continuous range) |
| 129 | REG_ANODE_CURRENT | Int16U | Anode current (A ×10) | 300 - 25000 | DUT_CURRENT_MIN to DUT_CURRENT_MAX | Anode current setting |
| 130 | REG_GATE_CURRENT | Int16U | Gate current (mA) | 1000 - 20000 | GATE_CURRENT_MIN to GATE_CURRENT_MAX | Gate current setting |
| 131 | REG_GATE_CURRENT_RISE_RATE | Int16U | Gate current rise rate (mA/μs) | 1000 - 10000 | GATE_I_RATE_MIN to GATE_I_RATE_MAX | Gate current rise rate setting |
| 132 | REG_SHORT_CALIBRATE_FLAG | Int16U | Short circuit calibration flag | 0 | 0 | Short circuit calibration disabled |
| | | | | 1 | 1 | Short circuit calibration enabled |

### Measurement Configuration

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 80 | REG_AVERAGE_NUM | Int16U | Number of averaging points | 1 - 20 | AVERAGE_NUM_MIN to AVERAGE_NUM_MAX | Number of points for averaging |
| 81 | REG_AVERAGE_ALLOWED_SPREAD | Int16U | Allowed spread for averaging (ns) | 0 - 500 | ALLOWED_SPREAD_MIN to ALLOWED_SPREAD_MAX | Allowed spread for averaging |
| 82 | REG_AVERAGE_PERIOD | Int16U | Averaging period (ms) | 300 - 2000 | AVERAGE_PERIOD_MIN to AVERAGE_PERIOD_MAX | Period between pulses during averaging |
| 84 | REG_NO_PWR_THRESHOLD | Int16U | No power threshold (%) | 0 - 80 | ID_THRESHOLD_MIN to ID_THRESHOLD_MAX | Threshold for no power detection |
| 85 | REG_AFTER_MEASURE_DELAY | Int16U | Post-measurement delay (ms) | 1000 - 10000 | AFTER_MEASURE_DELAY_MIN to AFTER_MEASURE_DELAY_MAX | Delay after measurement |

### Debug Parameters

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 180 | REG_DBG_SLAVE_NID | Int16U | Debug slave node ID | 0 - 65535 | 0 to INT16U_MAX | Debug slave node ID |
| 181 | REG_DBG_SLAVE_REG | Int16U | Debug slave register number | 0 - 65535 | 0 to INT16U_MAX | Debug slave register number |
| 182 | REG_DBG_SLAVE_DATA | Int16U | Debug slave register data | 0 - 65535 | 0 to INT16U_MAX | Debug slave register data |
| 183 | REG_DBG_SLAVE_ACT | Int16U | Debug slave action command | 0 - 65535 | 0 to INT16U_MAX | Debug slave action command |
| 190 | REG_DBG | Int16U | Debug register | 0 - 65535 | 0 to INT16U_MAX | Debug register |

### Calibration Registers (Continuous Ranges)

*Note: The following calibration registers use continuous ranges rather than discrete codes:*

- **Gate Driver Calibration** (Registers 15-29): Continuous coefficient ranges
- **Voltage Comparator Calibration** (Registers 30-35): Continuous voltage ranges (0-5000 mV)
- **Current Calibration** (Registers 36-49): Continuous coefficient ranges
- **Time Delay Calibration** (Registers 60-68): Continuous coefficient ranges
- **Turn-On Time Calibration** (Registers 70-78): Continuous coefficient ranges
- **Voltage Calibration** (Registers 86-88): Continuous coefficient ranges
- **TOCU Configuration** (Registers 90-101): Continuous CAN ID and bitmask ranges

---

## Read-Only Registers

### Device Status & State Information

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 192 | REG_DEV_STATE | Int16U | Device state register | 0 | DS_None | No state |
| | | | | 1 | DS_Fault | Fault state |
| | | | | 2 | DS_Disabled | Disabled state |
| | | | | 3 | DS_Ready | Ready state |
| | | | | 4 | DS_InProcess | In process state |
| 193 | REG_FAULT_REASON | Int16U | Fault reason register | 0 | DF_NONE | No fault |
| | | | | 1 | DF_INTERFACE | Master interface error |
| | | | | 2 | DF_PRESSURE | No pressure |
| | | | | 3 | DF_TOCU_WRONG_STATE | Wrong TOCU block state |
| | | | | 4 | DF_TOCU_STATE_TIMEOUT | TOCU state transition timeout |
| 194 | REG_DISABLE_REASON | Int16U | Disable reason register | 0 | DF_NONE | No disable reason |
| | | | | 1 | DF_INTERFACE | Master interface error |
| | | | | 2 | DF_PRESSURE | No pressure |
| | | | | 3 | DF_TOCU_WRONG_STATE | Wrong TOCU block state |
| | | | | 4 | DF_TOCU_STATE_TIMEOUT | TOCU state transition timeout |
| 195 | REG_WARNING | Int16U | Warning register | 0 | WARNING_NONE | No warnings |
| 196 | REG_PROBLEM | Int16U | Problem register | 0 | PROBLEM_NONE | No problems |
| | | | | 1 | PROBLEM_NO_PWR | No power current |
| | | | | 2 | PROBLEM_SHORT | Short circuit on output |
| | | | | 3 | PROBLEM_NO_POT | No signal from potential line |
| | | | | 4 | PROBLEM_OVERFLOW90 | 90% level counter overflow |
| | | | | 5 | PROBLEM_OVERFLOW10 | 10% level counter overflow |
| | | | | 6 | PROBLEM_SAFETY_VIOLATION | Safety circuit activation |
| | | | | 7 | PROBLEM_EMULATION | One or more slave blocks in emulation |
| | | | | 8 | PROBLEM_SLAVES_OP_FAIL | One or more slave blocks operation failed |
| 197 | REG_OP_RESULT | Int16U | Operation status register | 0 | OPRESULT_NONE | No information or not finished |
| | | | | 1 | OPRESULT_OK | Operation was successful |
| | | | | 2 | OPRESULT_FAIL | Operation failed |
| 198 | REG_DEV_SUB_STATE | Int16U | Device sub-state register | 0 | SS_None | No sub-state |
| | | | | 1 | SS_PowerOn | Power on sub-state |
| | | | | 2 | SS_WaitCharge | Wait charge sub-state |
| | | | | 3 | SS_PowerOff | Power off sub-state |
| | | | | 4 | SS_ConfigSlaves | Configure slaves sub-state |
| | | | | 5 | SS_WaitConfig | Wait configuration sub-state |
| | | | | 6 | SS_ConfigCommutation | Configure commutation sub-state |
| | | | | 7 | SS_CommPause | Commutation pause sub-state |
| | | | | 8 | SS_HardwareConfig | Hardware configuration sub-state |
| | | | | 9 | SS_AfterPulseWaiting | After pulse waiting sub-state |
| | | | | 10 | SS_TOCUPulseConfig | TOCU pulse configuration sub-state |
| | | | | 11 | SS_TOCUCheckProblem | TOCU check problem sub-state |
| | | | | 12 | SS_StartPulse | Start pulse sub-state |
| | | | | 13 | SS_NextPulseOrAverage | Next pulse or average sub-state |
| | | | | 14 | SS_AverageResult | Average result sub-state |
| 199 | REG_FAILED_SUB_STATE | Int16U | Failed sub-state register | 0 - 14 | 0 to 14 | Sub-state when error occurred |

### Safety & Monitoring Status

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 200 | REG_SAFETY_STATE | Int16U | Safety sensor state | 0 | 0 | Safety sensor inactive |
| | | | | 1 | 1 | Safety sensor active |
| 201 | REG_PRESSURE_STATE | Int16U | Pressure sensor state | 0 | 0 | Pressure sensor inactive |
| | | | | 1 | 1 | Pressure sensor active |

### Diagnostic Data

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 208 | REG_DIAG_RAW_SHIFT_REG | Int16U | Raw diagnostic data from shift register (lower bits) | 0 - 4095 | 0 to 0xFFF | Raw diagnostic data lower 12 bits |
| 209 | REG_DIAG_RAW_SHIFT_REG_2 | Int16U | Raw diagnostic data from shift register (upper bits) | 0 - 65535 | 0 to INT16U_MAX | Raw diagnostic data upper bits |

### Interface Error Information

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 210 | REG_BHL_ERROR_CODE | Int16U | Interface error code | 0 - 65535 | 0 to INT16U_MAX | Interface error code |
| 211 | REG_BHL_DEVICE | Int16U | Interface error device ID | 0 - 65535 | 0 to INT16U_MAX | Interface error device ID |
| 212 | REG_BHL_FUNCTION | Int16U | Interface error function code | 0 - 65535 | 0 to INT16U_MAX | Interface error function code |
| 213 | REG_BHL_EXT_DATA | Int16U | Interface error extended data | 0 - 65535 | 0 to INT16U_MAX | Interface error extended data |

### Measurement Results

| Num | Name | Type | Description | Value List/Range | Value Name | Value Description |
|-----|------|------|-------------|------------------|-------------|-------------------|
| 250 | REG_MEAS_CURRENT_VALUE | Int16U | Measured current value (A) | 0 - 65535 | 0 to INT16U_MAX | Measured current value |
| 251 | REG_MEAS_TIME_DELAY | Int16U | Measured turn-on delay (ns) | 0 - 65535 | 0 to INT16U_MAX | Measured turn-on delay time |
| 252 | REG_MEAS_TIME_ON | Int16U | Measured turn-on time (ns) | 0 - 65535 | 0 to INT16U_MAX | Measured turn-on time |
| 253 | REG_PULSE_COUNTER | Int16U | Generated pulse counter | 0 - 65535 | 0 to INT16U_MAX | Number of generated pulses |

---

## Commands

### Power Management Commands

| Num | Name | Description |
|-----|------|-------------|
| 1 | ACT_ENABLE_POWER | Transition to standby state |
| 2 | ACT_DISABLE_POWER | Turn off the block |

### System Control Commands

| Num | Name | Description |
|-----|------|-------------|
| 3 | ACT_FAULT_CLEAR | Clear fault state |
| 4 | ACT_WARNING_CLEAR | Clear warning state |

### Measurement Control Commands

| Num | Name | Description |
|-----|------|-------------|
| 100 | ACT_MEASURE_START | Start measurement |
| 101 | ACT_MEASURE_STOP | Stop measurement |
| 110 | ACT_GATE_PULSE | Generate control current pulse |

### Data Management Commands

| Num | Name | Description |
|-----|------|-------------|
| 200 | ACT_SAVE_TO_ROM | Save user data to processor FLASH |
| 201 | ACT_RESTORE_FROM_ROM | Restore data from FLASH |
| 202 | ACT_RESET_TO_DEFAULT | Reset DataTable to default state |

### System Maintenance Commands

| Num | Name | Description |
|-----|------|-------------|
| 320 | ACT_BOOT_LOADER_REQUEST | Restart processor for reprogramming |

### JSON Data Commands

| Num | Name | Description |
|-----|------|-------------|
| 341 | ACT_JSON_INIT_READ | Initialize JSON reading start |
| 342 | ACT_JSON_TO_EP | Execute JSON template reading to EP |

### Debug Commands - Gate Driver Testing

| Num | Name | Description |
|-----|------|-------------|
| 10 | ACT_DBG_GD_I_SET | Test TOMUHPGateDriver current amplitude setting |
| 11 | ACT_DBG_GD_TRIG_THRESHOLD | Test comparator trigger threshold setting |
| 12 | ACT_DBG_GD_I_RISE_RATE | Test TOMUHPGateDriver current rise rate setting |
| 13 | ACT_DBG_GD_I_FALL_RATE | Test TOMUHPGateDriver current fall rate setting |
| 14 | ACT_DBG_GD_SYNC | Test Sync_GD output |
| 15 | ACT_DBG_PS_EN | Test PS_EN output |
| 16 | ACT_DBG_FAN | Test FAN output |
| 17 | ACT_DBG_IND | Test IND output |
| 18 | ACT_DBG_RLC | Test RLC output |
| 19 | ACT_DBG_RELAY | Test RELAY output |
| 20 | ACT_DBG_SNC_TOCU | Test SNC_TOCU output |
| 21 | ACT_DBG_TOSU_SWITCH | Test TOSU switching |
| 22 | ACT_DBG_POT_SWITCH | Test PotSwitch control |

### Debug Commands - Reference Voltage Testing

| Num | Name | Description |
|-----|------|-------------|
| 24 | ACT_DBG_U_REF_U10 | Test U10 comparator reference voltage setting |
| 25 | ACT_DBG_U_REF_U90 | Test U90 comparator reference voltage setting |

### Debug Commands - System Testing

| Num | Name | Description |
|-----|------|-------------|
| 26 | ACT_DBG_READ_PRESSURE | Test pressure sensor operation |
| 27 | ACT_DBG_SFTY_ACTIVATION | Test SFTY_EN output |
| 28 | ACT_DBG_OUT_ISO | Test isolated outputs |
| 29 | ACT_DBG_READ_EXT_REG | Test external register reading |
| 30 | ACT_DBG_TRIG_RST | Test trigger reset |
| 31 | ACT_DBG_M_RESET | Test M_RESET output |

### Debug Commands - Slave Interface

| Num | Name | Description |
|-----|------|-------------|
| 40 | ACT_DBG_SLAVE_CALL | Execute command on slave node |
| 41 | ACT_DBG_SLAVE_READ_REG | Read register from slave node |
| 42 | ACT_DBG_SLAVE_WRITE_REG | Write register to slave node |
| 43 | ACT_DBG_RESET_INT_ERROR | Reset master interface error |

### TOCU (Test Object Control Unit) Commands

| Num | Name | Description |
|-----|------|-------------|
| 1 | ACT_TOCU_ENABLE_POWER | Transition to standby state (TOCU) |
| 2 | ACT_TOCU_DISABLE_POWER | Turn off the block (TOCU) |
| 3 | ACT_TOCU_FAULT_CLEAR | Clear fault (TOCU) |
| 4 | ACT_TOCU_WARNING_CLEAR | Clear warning (TOCU) |
| 100 | ACT_TOCU_VOLTAGE_CONFIG | Configure block voltage (TOCU) |
| 101 | ACT_TOCU_PULSE_CONFIG | Configure block for required current (TOCU) |

---

## Endpoints

| Num | Name | Description | Data Type | Size |
|-----|------|-------------|-----------|------|
| 1 | EP_CURRENT | Current values array | Int16U | 300 |
| 2 | EP_TURN_DELAY | Turn-on delay time array | Int16U | 20 |
| 3 | EP_TURN_ON | Turn-on time array | Int16U | 20 |
| 20 | EP16_ExtInfoData | Diagnostic data from flash | Int16U | 300 |

---

## Notes

### System Architecture
- **Data Type**: All registers use `Int16U` (16-bit unsigned integer)
- **Volatility**: Registers 0-127 are non-volatile (saved to flash), registers 128-191 are volatile
- **Communication**: CAN bus interface with master-slave architecture
- **Measurement**: High-precision current and voltage measurement with timing analysis

### Register Categories
1. **Device Configuration & Calibration**: Basic system setup and calibration parameters
2. **Safety & Monitoring Control**: Safety system configuration and monitoring controls
3. **Runtime Control Parameters**: Real-time operational parameters
4. **Measurement Configuration**: Measurement process configuration
5. **Debug Parameters**: Debugging and testing parameters

### Command Categories
1. **Power Management**: Basic power control commands
2. **System Control**: System state management commands
3. **Measurement Control**: Measurement process control
4. **Data Management**: Configuration data management
5. **System Maintenance**: System maintenance and updates
6. **JSON Interface**: JSON data handling
7. **Debug Commands**: Comprehensive debugging and testing commands
8. **TOCU Commands**: Slave device control commands

### Endpoint Characteristics
- All endpoints are read-only
- Current measurement data (300 points)
- Timing measurement data (20 points each for delay and turn-on)
- Diagnostic data (300 points)

This specification provides complete documentation of the TOMUHP Control Board's software interface, enabling integration with external systems and comprehensive system control and monitoring. 
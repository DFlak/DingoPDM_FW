# dingoAUX Communication Reference

## Hardware Overview

The **dingoAUX** is an auxiliary power distribution module in the DingoPDM ecosystem, built around an **STM32F446RET6** MCU.

| Feature | dingoAUX | dingoPDM_v7 |
|---|---|---|
| **PDM Type ID** | 2 | 0 |
| **Outputs** | 15 (simple MOSFETs) | 8 (Infineon Profets) |
| **Digital Inputs** | 4 (next PCB rev) | 2 |
| **Current Sensing** | 5× INA3221 (I2C, 1mΩ shunt) | Profet analog IS pins |
| **Temperature** | 4× NTC thermistors (NCP18XH103F03RB) | MCP9808 (I2C) |
| **CAN Transceiver** | PCA82C250 | PCA82C250 |
| **FRAM** | MB85RC (I2C1) | MB85RC (I2C1) |
| **Virtual Inputs** | 16 | 16 |
| **CAN Inputs** | 32 | 32 |
| **Flashers** | 4 | 4 |
| **Counters** | 4 | 4 |
| **Conditions** | 32 | 32 |

---

## CAN Bus Configuration

Both boards share the same CAN configuration framework:

- **Supported Bitrates**: 125K, 250K, 500K (default), 1000K
- **Frame Type**: Standard (11-bit SID), not extended
- **Base ID**: Configurable, default **2000** in DBC files
- **TX Messages**: 21 cyclic messages at `base_id + 0` through `base_id + 20`
- **Settings RX**: `base_id - 1` (command/config frames)
- **Settings TX**: `base_id + 30` (response frames)
- **Info Messages**: `base_id + 31` (event-triggered status messages)
- **Cyclic TX Rate**: 100ms between message bursts, 30µs inter-message gap

---

## TX CAN Messages (Cyclic)

### Message 0 — Device Status & Digital Inputs
**ID**: `base_id + 0` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

| Signal | Bits | Type | Scale | Unit | Notes |
|---|---|---|---|---|---|
| DigitalInput1 | 0:0 | bool | – | – | *dingoAUX: 4 inputs (bits 0–3)* |
| DigitalInput2 | 1:1 | bool | – | – | *dingoPDM_v7: 2 inputs (bits 0–1)* |
| DigitalInput3 | 2:2 | bool | – | – | dingoAUX only |
| DigitalInput4 | 3:3 | bool | – | – | dingoAUX only |
| DeviceState | 8:11 | enum | – | – | 0=Run, 1=Sleep, 2=Overtemp, 3=Error |
| PDMType | 12:15 | enum | – | – | 0=PDM, 1=PDM-Max, 2=AUX |
| TotalCurrent | 16:31 | uint16 | ×0.1 | A | Sum of all output currents |
| BatteryVoltage | 32:47 | uint16 | ×0.1 | V | Battery voltage via resistor divider |
| BoardTemperature | 48:63 | uint16 | ×0.1 | °C | Max of thermistors (AUX) / MCP9808 (v7) |

### Messages 1–2 — Output Currents 1–8
**ID**: `base_id + 1` / `+2` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

Each message carries 4× `OutputCurrent` (16-bit, scale ×0.1, unit A).
- Msg1: outputs 1–4
- Msg2: outputs 5–8

### Messages 16–17 — Output Currents 9–15 (dingoAUX only)
**ID**: `base_id + 16` / `+17` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

- Msg16: OutputCurrent 9–12 (4× 16-bit, ×0.1 A)
- Msg17: OutputCurrent 13–15 (3× 16-bit, ×0.1 A + padding)

### Message 3 — Output States 1–8, Wiper & Flashers
**ID**: `base_id + 3` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

| Signal | Bits | Type | Notes |
|---|---|---|---|
| OutputState1–8 | 0:31 | 4-bit enum ×8 | 0=Off, 1=On, 2=Overcurrent, 3=Fault |
| WiperSlowSpeed | 32 | bool | Wiper slow output active |
| WiperFastSpeed | 33 | bool | Wiper fast output active |
| WiperSpeed | 40:43 | enum | 0=Park…8=Intermit6 |
| WiperState | 44:47 | enum | 0=Park…7=Swipe |
| Flasher1–4 | 48:51 | bool ×4 | Flasher outputs |

### Message 18 — Output States 9–15 (dingoAUX only)
**ID**: `base_id + 18` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

7× `OutputState` (4-bit enum: Off/On/Overcurrent/Fault) for outputs 9–15.

### Message 4 — Output Reset Counts 1–8
**ID**: `base_id + 4` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

8× `OutputResetCount` (8-bit each) — overcurrent auto-reset counter per output.

### Message 19 — Output Reset Counts 9–15 (dingoAUX only)
**ID**: `base_id + 19` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

7× `OutputResetCount` (8-bit each) for outputs 9–15 + padding.

### Message 5 — CAN Inputs & Virtual Inputs
**ID**: `base_id + 5` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Conditional** (sent if any CAN/virtual input enabled)

| Signal | Bits | Notes |
|---|---|---|
| CANInput1–32 | 0:31 | 1-bit per CAN input (decoded boolean) |
| VirtualInput1–16 | 32:47 | 1-bit per virtual input result |

### Message 6 — Counters & Conditions
**ID**: `base_id + 6` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Conditional** (sent if any counter/condition enabled)

| Signal | Bits | Notes |
|---|---|---|
| CounterValue1–4 | 0:31 | 8-bit per counter (0–8) |
| ConditionResult1–32 | 32:63 | 1-bit per condition result |

### Messages 7–14 — CAN Input Raw Values
**ID**: `base_id + 7` through `+14` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Conditional**

4× `CANInputValue` (16-bit) per message, reporting raw CAN input decoded values:
- Msg7: inputs 1–4 &nbsp;|&nbsp; Msg8: 5–8 &nbsp;|&nbsp; … &nbsp;|&nbsp; Msg14: 29–32

### Message 15 — Output Duty Cycles 1–8
**ID**: `base_id + 15` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

8× `OutputDC` (8-bit, unit %) — current PWM duty cycle per output.

### Message 20 — Output Duty Cycles 9–15 (dingoAUX only)
**ID**: `base_id + 20` &nbsp;|&nbsp; **DLC**: 8 &nbsp;|&nbsp; **Always sent**

7× `OutputDC` (8-bit, unit %) for outputs 9–15 + padding.

---

## RX Command Messages

All commands are sent to **`base_id - 1`** with standard frame ID.

| Command | DLC | Byte 0 | Payload | Response |
|---|---|---|---|---|
| **Sleep** | 5 | `MsgCmd::Sleep` | `"QUIT"` | ACK at base_id+30 |
| **Burn Settings** | 4 | `MsgCmd::BurnSettings` | `1, 3, 8` | ACK + result |
| **Bootloader** | 6 | `MsgCmd::Bootloader` | `"BOOTL"` | MCU reset to bootloader |
| **Version** | 1 | `MsgCmd::Version` | – | Major, Minor, Build |
| **Config R/W** | varies | `MsgCmd::*` | per setting | Echo of configured value |

Settings commands cover: Inputs, CAN Inputs, Virtual Inputs, Outputs, Wiper, Starter, Flashers, Counters, Conditions, CAN config.

---

## Key Differences: dingoAUX vs dingoPDM_v7

| Aspect | dingoAUX | dingoPDM_v7 |
|---|---|---|
| **Current sensing** | External INA3221 via I2C (polled at 250ms in SlowThread) | Profet analog IS pins (ADC, read every cycle) |
| **Temperature** | 4 board thermistors → Steinhart-Hart equation → max of 4 reported | Single MCP9808 I2C sensor |
| **I2C buses** | 3 (I2C1=FRAM, I2C2+I2C3=INA3221) | 1 (I2C1=FRAM+MCP9808) |
| **I2C bus recovery** | Implemented (bit-bang SCL 9× + STOP) | Not needed |
| **Output type** | SimpleMOSFET (on/off + PWM via timer) | Infineon Profets (BTS7002, BTS7008) with DEN/DSEL |
| **Digital inputs** | 4 (PB4, PB5, PB11, PB12 — next PCB rev) | 2 (PA10, PC9) |
| **Msg 0 byte 0** | 4 digital input bits (0–3) | 2 digital input bits (0–1) |
| **CAN frame layout** | Identical for Msg 1–15 | Identical for Msg 1–15 |

> Both boards use the same shared firmware (`msg.cpp`, `pdm.cpp`, `digital.cpp`, etc.) with `PDM_TYPE`, `PDM_NUM_INPUTS`, and `PDM_NUM_OUTPUTS` as compile-time switches. The CAN message structure is intentionally compatible — the same DBC definitions work for shared messages.

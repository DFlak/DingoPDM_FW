## dingoAUX CAN Frame Message 0

**Message ID:** base_id + 0

| Name | Start Bit | Length | Byte Order | Signed | Conversion | Min | Max | Unit |
|---|---|---|---|---|---|---|---|---|
| DigitalInput1 | 0 | 1 | little_endian | false | - | 0 | 1 | - |
| DigitalInput2 | 1 | 1 | little_endian | false | - | 0 | 1 | - |
| DigitalInput3 | 2 | 1 | little_endian | false | - | 0 | 1 | - |
| DigitalInput4 | 3 | 1 | little_endian | false | - | 0 | 1 | - |
| DeviceState | 8 | 4 | little_endian | false | - | 0 | 3 | - |
| PDMType | 12 | 4 | little_endian | false | - | 0 | 15 | - |
| TotalCurrent | 16 | 16 | little_endian | false | 0.1 | 0 | 655.35 | A |
| BatteryVoltage | 32 | 16 | little_endian | false | 0.1 | 0 | 655.35 | V |
| BoardTemperature | 48 | 16 | little_endian | false | 0.1 | 0 | 215 | degC |

## dingoAUX CAN Frame Message 1

**Message ID:** base_id + 1

| Name | Start Bit | Length | Byte Order | Signed | Conversion | Min | Max | Unit |
|---|---|---|---|---|---|---|---|---|
| OutputCurrent1 | 0 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |
| OutputCurrent2 | 16 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |
| OutputCurrent3 | 32 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |
| OutputCurrent4 | 48 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |

## dingoAUX CAN Frame Message 2

**Message ID:** base_id + 2

| Name | Start Bit | Length | Byte Order | Signed | Conversion | Min | Max | Unit |
|---|---|---|---|---|---|---|---|---|
| OutputCurrent5 | 0 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |
| OutputCurrent6 | 16 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |
| OutputCurrent7 | 32 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |
| OutputCurrent8 | 48 | 16 | little_endian | false | 0.1 | 0 | 6553.5 | A |

## dingoAUX CAN Frame Message 3

**Message ID:** base_id + 3

| Name | Start Bit | Length | Byte Order | Signed | Conversion | Min | Max | Unit |
|---|---|---|---|---|---|---|---|---|
| OutputState1 | 0 | 4 | little_endian | false | - | 0 | 3 | - |
| OutputState2 | 4 | 4 | little_endian | false | - | 0 | 3 | - |
| OutputState3 | 8 | 4 | little_endian | false | - | 0 | 3 | - |
| OutputState4 | 12 | 4 | little_endian | false | - | 0 | 3 | - |
| OutputState5 | 16 | 4 | little_endian | false | - | 0 | 3 | - |
| OutputState6 | 20 | 4 | little_endian | false | - | 0 | 3 | - |
| OutputState7 | 24 | 4 | little_endian | false | - | 0 | 3 | - |
| OutputState8 | 28 | 4 | little_endian | false | - | 0 | 3 | - |
| WiperSlowSpeed | 32 | 1 | little_endian | false | - | 0 | 1 | - |
| WiperFastSpeed | 33 | 1 | little_endian | false | - | 0 | 1 | - |
| WiperSpeed | 40 | 4 | little_endian | false | - | 0 | 8 | - |
| WiperState | 44 | 4 | little_endian | false | - | 0 | 7 | - |
| Flasher1 | 48 | 1 | little_endian | false | - | 0 | 1 | - |
| Flasher2 | 49 | 1 | little_endian | false | - | 0 | 1 | - |
| Flasher3 | 50 | 1 | little_endian | false | - | 0 | 1 | - |
| Flasher4 | 51 | 1 | little_endian | false | - | 0 | 1 | - |

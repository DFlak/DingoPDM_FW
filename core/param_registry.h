#pragma once

#include <cstdint>

// 0x0000 - 0x0FFF: System
// 0x1000 - 0x10FF: Outputs
// 0x1100 - 0x11FF: PWM
// 0x1200 - 0x12FF: Inputs
// 0x1300 - 0x13FF: CAN Inputs
// 0x1400 - 0x14FF: Virtual Inputs
// 0x1500 - 0x15FF: Conditions
// 0x1600 - 0x16FF: Counter
// 0x1700 - 0x17FF: Flasher
// 0x1800 - 0x18FF: Starter Disable
// 0x1900 - 0x19FF: Wipers
// 0x2000 - 0x20FF: Keypads

enum class ParamType : uint8_t
{
    UInt8 = 0,
    Int8 = 1,
    UInt16 = 2,
    Int16 = 3,
    UInt32 = 4,
    Int32 = 5,
    Float = 6,
    Bool = 7,
    Enum = 8
};

struct ParamInfo
{
    uint16_t nIndex;
    uint8_t nSubIndex;
    void* pVal;
    ParamType eType;
    float fDefaultVal;
    float fMinVal;
    float fMaxVal;
};

extern const ParamInfo stParams[];
extern const uint16_t NUM_PARAMS;

const ParamInfo* FindParam(uint16_t index, uint8_t subindex);
uint32_t ReadParam(const ParamInfo* param);
bool WriteParam(const ParamInfo* param, uint32_t value);
bool IsDefaultValue(const ParamInfo* param);
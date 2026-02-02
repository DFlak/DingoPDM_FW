#pragma once

#include "port.h"
#include "param_registry.h"

struct ParamMsg
{
    MsgCmd eCmd;
    uint16_t nIndex;
    uint8_t nSubIndex;
    uint32_t nValue;
};

void ProcessParamMsg(CANRxFrame *rx);
#include "param_protocol.h"
#include "dingopdm_config.h"
#include "config.h"
#include "mailbox.h"
#include "config_handler.h"
#include <cstring>

extern PdmConfig stConfig;

void DecodeParamCmd(CANRxFrame *rx, ParamMsg *out)
{
    out->eCmd = static_cast<MsgCmd>(rx->data8[0]);
    out->nIndex = rx->data8[1] | (rx->data8[2] << 8);
    out->nSubIndex = rx->data8[3];
    out->nValue =  rx->data8[4] |
                   rx->data8[5] << 8 |
                   rx->data8[6] << 16 |
                  (rx->data8[7]) << 24;
}

void EncodeParamRsp(CANTxFrame *tx, uint8_t cmd, uint16_t index, uint8_t subindex, uint32_t value)
{
    tx->IDE = 0;
    tx->RTR = 0;
    tx->DLC = 8;

    tx->SID = stConfig.stDevConfig.nBaseId + TX_SETTINGS_ID_OFFSET;

    tx->data8[0] = cmd;
    tx->data8[1] = index & 0xFF;
    tx->data8[2] = (index >> 8) & 0xFF;
    tx->data8[3] = subindex;
    tx->data8[4] = value & 0xFF;
    tx->data8[5] = (value >> 8) & 0xFF;
    tx->data8[6] = (value >> 16) & 0xFF;
    tx->data8[7] = (value >> 24) & 0xFF;
}

void SendAllNonDefaultParams() {
    CANTxFrame tx;

    for (int i = 0; i < NUM_PARAMS; i++) {
        if (!IsDefaultValue(&stParams[i])) {
            uint32_t value = ReadParam(&stParams[i]);
            EncodeParamRsp(&tx, static_cast<uint8_t>(MsgCmd::ReadAllParams), 
                            stParams[i].nIndex, stParams[i].nSubIndex, value);
            PostTxFrame(&tx);

            // Small delay to avoid saturating CAN bus
            chThdSleepMilliseconds(1);
        }
    }
}

void SetAllDefaultParams() {
    for (int i = 0; i < NUM_PARAMS; i++) {
        uint32_t defaultVal;
        if (stParams[i].eType == ParamType::Float) {
            memcpy(&defaultVal, &stParams[i].fDefaultVal, sizeof(float));
        } else {
            defaultVal = static_cast<uint32_t>(stParams[i].fDefaultVal);
        }
        WriteParam(&stParams[i], defaultVal);
    }
}

void ProcessParamMsg(CANRxFrame *rx) {
    CANTxFrame tx;
    ParamMsg msg;

    if (rx->SID != stConfig.stDevConfig.nBaseId - 1)
        return;

    msg.eCmd = static_cast<MsgCmd>(rx->data8[0]);

    if (!(msg.eCmd == MsgCmd::ReadParam || 
        msg.eCmd == MsgCmd::WriteParam || 
        msg.eCmd == MsgCmd::ReadAllParams))
        return;

    DecodeParamCmd(rx, &msg);

    if (msg.eCmd == MsgCmd::ReadParam) {
        const ParamInfo* param = FindParam(msg.nIndex, msg.nSubIndex);
        if (param) {
            uint32_t value = ReadParam(param);
            EncodeParamRsp(&tx, static_cast<uint8_t>(msg.eCmd), msg.nIndex, msg.nSubIndex, value);
            PostTxFrame(&tx);
            return;
        }
    }

    if (msg.eCmd == MsgCmd::WriteParam) {
        const ParamInfo* param = FindParam(msg.nIndex, msg.nSubIndex);
        if (param && WriteParam(param, msg.nValue)) {
            EncodeParamRsp(&tx, static_cast<uint8_t>(msg.eCmd), msg.nIndex, msg.nSubIndex, msg.nValue);
            PostTxFrame(&tx);
            return;
        }
    }

    if (msg.eCmd == MsgCmd::ReadAllParams) {
        // Send multiple responses for non-default params
        SendAllNonDefaultParams();
        return;
    }
}
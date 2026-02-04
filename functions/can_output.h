#pragma once

#include "port.h"
#include "config.h"
#include "hal.h"

extern float *pVarMap[PDM_VAR_MAP_SIZE];

class CanOutput
{
public:
    CanOutput() {
    };

    static const uint16_t nBaseIndex = 0x2000;

    void SetConfig(Config_CanOutput* config, uint8_t index) {
        pConfig = config;
        nIndex = index;
        pInput = pVarMap[config->nInput];
    }

    static void InitAllFrames();
    void Update();

private:
    Config_CanOutput* pConfig;

    float *pInput;

    uint8_t nIndex;
    uint32_t nLastTxTime;

    void CheckTime();
    static void ClearFrames();
    static uint8_t CalcDLC(uint8_t nStartBit, uint8_t nBitLength);
};

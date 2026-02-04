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

    static const uint16_t nBaseIndex = 0x1300;

    void SetConfig(Config_CanOutput* config) { 
        pConfig = config;

        pInput = pVarMap[config->nInput];
    }

    void Update();
    void CheckTime();

private:
    Config_CanOutput* pConfig;

    float *pInput;

    uint32_t nLastTxTime;
};
#pragma once

#include <cstdint>
#include "input.h"

extern float *pVarMap[PDM_VAR_MAP_SIZE];

class KeypadButton;

class KeypadButton
{
public:
    KeypadButton() = default;

    void SetConfig(Config_KeypadButton *config)
    {
        pConfig = config;
        pLedVars[0] = pVarMap[config->nVars[0]];
        pLedVars[1] = pVarMap[config->nVars[1]];
        pLedVars[2] = pVarMap[config->nVars[2]];
        pLedVars[3] = pVarMap[config->nVars[3]];
        pFaultLedVar = pVarMap[config->nFaultVar];
    }

    Config_KeypadButton *pConfig = nullptr;

    float *pLedVars[4] = {};
    float *pFaultLedVar = nullptr;

    Input input;
    bool bVal = false;    
};

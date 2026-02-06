#pragma once

#include <cstdint>
#include "input.h"

extern float *pVarMap[PDM_VAR_MAP_SIZE];

class KeypadButton;

// Function pointer types for brand-specific LED behavior
using ButtonUpdateLedFn = void (*)(KeypadButton*);
using ButtonGetLedStateFn = uint8_t (*)(KeypadButton*);  // Returns 0=off, 1=on, 2=blink

class KeypadButton
{
public:
    KeypadButton() = default;

    void SetConfig(Config_KeypadButton *config)
    {
        pConfig = config;
        pLedVars[0] = pVarMap[config->nValVars[0]];
        pLedVars[1] = pVarMap[config->nValVars[1]];
        pLedVars[2] = pVarMap[config->nValVars[2]];
        pLedVars[3] = pVarMap[config->nValVars[3]];
        pFaultLedVar = pVarMap[config->nFaultVar];
    }

    void SetBrand(ButtonUpdateLedFn updateFn, ButtonGetLedStateFn stateFn)
    {
        fnUpdateLed = updateFn;
        fnGetLedState = stateFn;
    }

    bool Update(bool bNewVal);

    void UpdateLed()
    {
        if (fnUpdateLed)
            fnUpdateLed(this);
    }

    uint8_t GetLedState()
    {
        if (fnGetLedState)
            return fnGetLedState(this);
        return 0;
    }

    // LED state - brand-specific code sets these
    BlinkMarineButtonColor eLedOnColor = BlinkMarineButtonColor::Off;
    BlinkMarineButtonColor eLedBlinkColor = BlinkMarineButtonColor::Off;

    // Config accessible to brand-specific functions
    Config_KeypadButton *pConfig = nullptr;
    float *pLedVars[4] = {};
    float *pFaultLedVar = nullptr;

private:
    Input input;
    bool bVal = false;

    ButtonUpdateLedFn fnUpdateLed = nullptr;
    ButtonGetLedStateFn fnGetLedState = nullptr;
};

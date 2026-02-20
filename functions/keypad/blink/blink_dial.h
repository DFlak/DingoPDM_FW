#pragma once

#include <cstdint>
#include "port.h"
#include "enums.h"
#include "config.h"

class BlinkDial
{
public:
    BlinkDial() {};

    void SetConfig(Config_KeypadDial *config)
    {
        pConfig = config;
    }
    
    void CheckMsg(uint64_t data);

    void Update();

    bool GetLedState(uint8_t nIndex)
    {
        if (nIndex >= 16)
            return false;

        return bLeds[nIndex];
    }

private:
    Config_KeypadDial *pConfig;

    uint8_t nCounter;
    uint8_t nCounterMax;

    bool bLeds[16];
};
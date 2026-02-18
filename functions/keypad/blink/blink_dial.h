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

    void UpdateLeds();

private:
    Config_KeypadDial *pConfig;

    uint8_t nCounter;
    uint8_t nCounterMax;

    bool bLeds[16];
};
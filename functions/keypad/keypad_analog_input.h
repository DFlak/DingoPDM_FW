#pragma once

#include <cstdint>

class KeypadAnalogInput
{
public:
    KeypadAnalogInput() {};
    
    bool Update(uint16_t nRawVal);

    float fVal;

private:

};
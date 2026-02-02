#include "keypad_dial.h"
#include "dbc.h"

void KeypadDial::Update(uint64_t data)
{
    nTicks = data & 0x7F;
    bClockwise = (data & 0x80) >> 7 ? true : false;
    bCounterClockwise = (data & 0x80) >> 7 ? false : true;

    nEncoderTicks = (data & 0xFFFF00) >> 8;

    nMaxEncoderTicks = (data & 0xFF000000) >> 24;
}

void KeypadDial::UpdateLeds()
{
    //TODO: Implement LED update logic
}

void KeypadDial::SetDefaultConfig(Config_KeypadDial *config)
{
    config->nDialMinLed = 0;
    config->nDialMaxLed = 0;
    config->nDialLedOffset = 0;
}
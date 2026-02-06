#include "blink_dial.h"
#include "dbc.h"

void BlinkDial::Update(uint64_t data)
{
    nTicks = data & 0x7F;
    bClockwise = (data & 0x80) >> 7 ? true : false;
    bCounterClockwise = (data & 0x80) >> 7 ? false : true;

    nEncoderTicks = (data & 0xFFFF00) >> 8;

    nMaxEncoderTicks = (data & 0xFF000000) >> 24;
}

void BlinkDial::UpdateLeds()
{
    //TODO: Implement LED update logic
}
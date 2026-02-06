#include "grayhill_button.h"
#include "keypad_button.h"

// Grayhill uses simple on/off/blink (no RGB colors)
void UpdateLedGrayhill(KeypadButton* btn)
{
    if (!btn->pConfig->bEnabled)
        return;

    bool bOn = false;
    bool bBlink = false;

    for (uint8_t i = 0; i < btn->pConfig->nNumOfValColors; i++)
    {
        if (static_cast<uint8_t>(*btn->pLedVars[i]) == 1)
        {
            bOn = true;
            if (btn->pConfig->bValBlinking[i])
                bBlink = true;
        }
    }

    if (*btn->pFaultLedVar == 1)
    {
        bOn = true;
        if (btn->pConfig->bFaultBlinking)
            bBlink = true;
    }

    // Store in eLedOnColor/eLedBlinkColor for consistency
    btn->eLedOnColor = bOn ? BlinkMarineButtonColor::White : BlinkMarineButtonColor::Off;
    btn->eLedBlinkColor = bBlink ? BlinkMarineButtonColor::White : BlinkMarineButtonColor::Off;
}

uint8_t GetLedStateGrayhill(KeypadButton* btn)
{
    if (btn->eLedOnColor == BlinkMarineButtonColor::Off)
        return 0;  // Off
    if (btn->eLedBlinkColor != BlinkMarineButtonColor::Off)
        return 2;  // Blink
    return 1;  // On
}

#include "blink_button.h"
#include "keypad_button.h"

void UpdateLedBlinkMarine(KeypadButton* btn)
{
    if (!btn->pConfig->bEnabled)
        return;

    BlinkMarineButtonColor eColor = BlinkMarineButtonColor::Off;
    BlinkMarineButtonColor eBlinkColor = BlinkMarineButtonColor::Off;

    for (uint8_t i = 0; i < btn->pConfig->nNumOfValColors; i++)
    {
        // Check if the value is equal to the index of the color
        // If so, set the color to the corresponding value color
        // Boolean values only check values 0 and 1
        // Integer values check values 0 to nNumOfValColors - 1
        if (static_cast<uint8_t>(*btn->pLedVars[i]) == 1)
        {
            eColor = static_cast<BlinkMarineButtonColor>(btn->pConfig->nValColors[i]);

            if (btn->pConfig->bValBlinking[i])
            {
                if (eColor == BlinkMarineButtonColor::Off)
                    eBlinkColor = static_cast<BlinkMarineButtonColor>(btn->pConfig->nValBlinkingColors[i]);
                else
                    eBlinkColor = static_cast<BlinkMarineButtonColor>(btn->pConfig->nValBlinkingColors[i] ^ btn->pConfig->nValColors[i]);
            }
        }
    }

    // Fault color takes precedence over value colors
    if (*btn->pFaultLedVar == 1)
    {
        eColor = (BlinkMarineButtonColor)btn->pConfig->nFaultColor;

        if(btn->pConfig->bFaultBlinking)
        {
            if (eColor == BlinkMarineButtonColor::Off)
                eBlinkColor = (BlinkMarineButtonColor)btn->pConfig->nFaultBlinkingColor;
            else
                eBlinkColor = (BlinkMarineButtonColor)(btn->pConfig->nFaultBlinkingColor ^ btn->pConfig->nFaultColor);
        }
    }

    btn->eLedOnColor = eColor;
    btn->eLedBlinkColor = eBlinkColor;
}

uint8_t GetLedStateBlinkMarine(KeypadButton* btn)
{
    // BlinkMarine uses RGB colors, not simple state
    // Return simple state for compatibility
    if (btn->eLedOnColor == BlinkMarineButtonColor::Off)
        return 0;
    if (btn->eLedBlinkColor != BlinkMarineButtonColor::Off)
        return 2;
    return 1;
}

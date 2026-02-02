#include "keypad_button.h"
#include "dbc.h"

bool KeypadButton::Update(bool bNewVal)
{
    if (!pConfig->bEnabled)
        return false;

    bVal = input.Check(pConfig->eMode, false, bNewVal);

    return bVal;
}

// Set color based on var map values and fault state
void KeypadButton::UpdateLed()
{
    if (!pConfig->bEnabled)
        return;

    BlinkMarineButtonColor eColor = BlinkMarineButtonColor::Off;
    BlinkMarineButtonColor eBlinkColor = BlinkMarineButtonColor::Off;

    for (uint8_t i = 0; i < pConfig->nNumOfValColors; i++)
    {
        // Check if the value is equal to the index of the color
        // If so, set the color to the corresponding value color
        // Boolean values only check values 0 and 1
        // Integer values check values 0 to nNumOfValColors - 1
        if (static_cast<uint8_t>(*pLedVars[i]) == 1)
        {
            eColor = static_cast<BlinkMarineButtonColor>(pConfig->nValColors[i]);

            if (pConfig->bValBlinking[i])
            {
                if (eColor == BlinkMarineButtonColor::Off)
                    eBlinkColor = static_cast<BlinkMarineButtonColor>(pConfig->nValBlinkingColors[i]);
                else
                    eBlinkColor = static_cast<BlinkMarineButtonColor>(pConfig->nValBlinkingColors[i] ^ pConfig->nValColors[i]);
            }
        }
    }

    // Fault color takes precedence over value colors
    if (*pFaultLedVar == 1)
    {
        eColor = (BlinkMarineButtonColor)pConfig->nFaultColor;

        if(pConfig->bFaultBlinking)
        {
            if (eColor == BlinkMarineButtonColor::Off)
                eBlinkColor = (BlinkMarineButtonColor)pConfig->nFaultBlinkingColor;
            else
                eBlinkColor = (BlinkMarineButtonColor)(pConfig->nFaultBlinkingColor ^ pConfig->nFaultColor);
        }
    }

    eLedOnColor = eColor;
    eLedBlinkColor = eBlinkColor;
}

void KeypadButton::SetDefaultConfig(Config_KeypadButton *config)
{
    config->bEnabled = false;
    config->eMode = InputMode::Momentary;
    config->nNumOfValColors = 4;
    config->nValColors[0] = (uint8_t)BlinkMarineButtonColor::Off;
    config->nValColors[1] = (uint8_t)BlinkMarineButtonColor::Green;
    config->nValColors[2] = (uint8_t)BlinkMarineButtonColor::Violet;
    config->nValColors[3] = (uint8_t)BlinkMarineButtonColor::Blue;
    config->nFaultColor = (uint8_t)BlinkMarineButtonColor::Red;
    config->nValVars[0] = 0;
    config->nValVars[1] = 0;
    config->nValVars[2] = 0;
    config->nValVars[3] = 0;
    config->nFaultVar = 0;
    config->bValBlinking[0] = false;
    config->bValBlinking[1] = false;
    config->bValBlinking[2] = false;
    config->bValBlinking[3] = false;
    config->bFaultBlinking = false;
    config->nValBlinkingColors[0] = (uint8_t)BlinkMarineButtonColor::Blue;
    config->nValBlinkingColors[1] = (uint8_t)BlinkMarineButtonColor::Violet;
    config->nValBlinkingColors[2] = (uint8_t)BlinkMarineButtonColor::Green;
    config->nValBlinkingColors[3] = (uint8_t)BlinkMarineButtonColor::White;
    config->nFaultBlinkingColor = (uint8_t)BlinkMarineButtonColor::Orange;
}
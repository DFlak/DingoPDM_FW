#include "keypad_button.h"

bool KeypadButton::Update(bool bNewVal)
{
    if (!pConfig->bEnabled)
        return false;

    bVal = input.Check(pConfig->eMode, false, bNewVal);

    return bVal;
}

#include "blink_keypad.h"
#include "blink_button.h"
#include "keypad.h"

namespace {

bool ColorToRed(BlinkMarineButtonColor eColor)
{
    return (static_cast<uint8_t>(eColor) & 0x01);
}

bool ColorToGreen(BlinkMarineButtonColor eColor)
{
    return ((static_cast<uint8_t>(eColor) & 0x02) >> 1);
}

bool ColorToBlue(BlinkMarineButtonColor eColor)
{
    return ((static_cast<uint8_t>(eColor) & 0x04) >> 2);
}

uint64_t BuildLedMsg(Keypad* kp, bool bBlink)
{
    uint64_t nMsg = 0;
    // Blink Marine PKP-2600SI 12 button keypad
    // Has different LED mapping than other keypads
    // Stacked
    if (kp->pConfig->eModel == KeypadModel::Blink12Key)
    {
        uint8_t nIndex = 0;
        for (uint8_t i = 0; i < kp->nNumButtons; i++)
        {
            nMsg |= (ColorToRed(bBlink ? kp->button[i].eLedBlinkColor : kp->button[i].eLedOnColor) << nIndex++);
        }
        for (uint8_t i = 0; i < kp->nNumButtons; i++)
        {
            nMsg |= (ColorToGreen(bBlink ? kp->button[i].eLedBlinkColor : kp->button[i].eLedOnColor) << nIndex++);
        }
        for (uint8_t i = 0; i < kp->nNumButtons; i++)
        {
            nMsg |= (ColorToBlue(bBlink ? kp->button[i].eLedBlinkColor : kp->button[i].eLedOnColor) << nIndex++);
        }

        return nMsg;
    }

    // All other Blink keypads have padding
    // Padded
    uint8_t nBytesPerColor = (kp->nNumButtons + 7) / 8;  // Ceiling
    uint8_t nByteIndex = 0;
    uint8_t nBitIndex = 0;
    uint8_t nBitPosition = 0;

    for (uint8_t i = 0; i < kp->nNumButtons; i++) {
        nByteIndex = i / 8;
        nBitIndex = i % 8;
        nBitPosition = (nByteIndex * 8) + nBitIndex;
        nMsg |= (ColorToRed(bBlink ? kp->button[i].eLedBlinkColor : kp->button[i].eLedOnColor) << nBitPosition);
    }

    for (uint8_t i = 0; i < kp->nNumButtons; i++) {
        nByteIndex = nBytesPerColor + (i / 8);
        nBitIndex = i % 8;
        nBitPosition = (nByteIndex * 8) + nBitIndex;
        nMsg |= (ColorToGreen(bBlink ? kp->button[i].eLedBlinkColor : kp->button[i].eLedOnColor) << nBitPosition);
    }

    for (uint8_t i = 0; i < kp->nNumButtons; i++) {
        nByteIndex = 2 * nBytesPerColor + (i / 8);
        nBitIndex = i % 8;
        nBitPosition = (nByteIndex * 8) + nBitIndex;
        nMsg |= (ColorToBlue(bBlink ? kp->button[i].eLedBlinkColor : kp->button[i].eLedOnColor) << nBitPosition);
    }

    return nMsg;
}

CANTxFrame LedOnMsg(Keypad* kp)
{
    for (uint8_t i = 0; i < kp->nNumButtons; i++)
        kp->button[i].UpdateLed();

    CANTxFrame msg;
    msg.SID = kp->pConfig->nNodeId + 0x200;
    msg.IDE = CAN_IDE_STD;
    msg.DLC = 8;
    msg.data64[0] = BuildLedMsg(kp, false);

    return msg;
}

CANTxFrame LedBlinkMsg(Keypad* kp)
{
    for (uint8_t i = 0; i < kp->nNumButtons; i++)
        kp->button[i].UpdateLed();

    CANTxFrame msg;
    msg.SID = kp->pConfig->nNodeId + 0x300;
    msg.IDE = CAN_IDE_STD;
    msg.DLC = 8;
    msg.data64[0] = BuildLedMsg(kp, true);

    return msg;
}

CANTxFrame LedBrightnessMsg(Keypad* kp)
{
    CANTxFrame msg;
    msg.SID = kp->pConfig->nNodeId + 0x400;
    msg.IDE = CAN_IDE_STD;
    msg.DLC = 8;
    msg.data8[0] = *kp->pDimmingInput ? kp->pConfig->nDimButtonBrightness : kp->pConfig->nButtonBrightness;
    msg.data8[1] = 0x00;
    msg.data8[2] = 0x00;
    msg.data8[3] = 0x00;
    msg.data8[4] = 0x00;
    msg.data8[5] = 0x00;
    msg.data8[6] = 0x00;
    msg.data8[7] = 0x00;

    return msg;
}

CANTxFrame BacklightMsg(Keypad* kp)
{
    CANTxFrame msg;
    msg.SID = kp->pConfig->nNodeId + 0x500;
    msg.IDE = CAN_IDE_STD;
    msg.DLC = 8;
    msg.data8[0] = *kp->pDimmingInput ? kp->pConfig->nDimBacklightBrightness : kp->pConfig->nBacklightBrightness;
    msg.data8[1] = kp->pConfig->nBacklightColor;
    msg.data8[2] = 0x00;
    msg.data8[3] = 0x00;
    msg.data8[4] = 0x00;
    msg.data8[5] = 0x00;
    msg.data8[6] = 0x00;
    msg.data8[7] = 0x00;

    return msg;
}

}

void SetModelBlinkMarine(Keypad* kp)
{
    switch (kp->pConfig->eModel)
    {
        case KeypadModel::Blink2Key:
            kp->nNumButtons = 2;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink4Key:
            kp->nNumButtons = 4;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink5Key:
            kp->nNumButtons = 5;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink6Key:
            kp->nNumButtons = 6;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink8Key:
            kp->nNumButtons = 8;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink10Key:
            kp->nNumButtons = 10;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink12Key:
            kp->nNumButtons = 12;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink15Key:
            kp->nNumButtons = 15;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Blink13Key2Dial:
            kp->nNumButtons = 13;
            kp->nNumDials = 2;
            break;
        default:
            kp->nNumButtons = 0;
            kp->nNumDials = 0;
            break;
    }

    if (!kp->pConfig->bEnabled)
    {
        kp->nNumButtons = 0;
        kp->nNumDials = 0;
    }

    for (uint8_t i = 0; i < KEYPAD_MAX_BUTTONS; i++)
    {
        kp->button[i].SetConfig(&kp->pConfig->stButton[i]);
    }
    for (uint8_t i = 0; i < KEYPAD_MAX_DIALS; i++)
    {
        kp->dial[i].SetConfig(&kp->pConfig->stDial[i]);
    }

    // Wire button function pointers for BlinkMarine LED behavior
    for (uint8_t i = 0; i < KEYPAD_MAX_BUTTONS; i++)
        kp->button[i].SetBrand(UpdateLedBlinkMarine, GetLedStateBlinkMarine);
}

bool CheckMsgBlinkMarine(Keypad* kp, CANRxFrame frame)
{

    switch (frame.SID - kp->pConfig->nNodeId)
    {
        case (uint16_t)BlinkMarineMessageId::ButtonState:
            
            return true;
        case (uint16_t)BlinkMarineMessageId::SetLed:
            kp->nLastRxTime = SYS_TIME;
            return true;
        case (uint16_t)BlinkMarineMessageId::DialState1:
            kp->nLastRxTime = SYS_TIME;
            return true;
        case (uint16_t)BlinkMarineMessageId::SetLedBlink:
            kp->nLastRxTime = SYS_TIME;
            return true;
        case (uint16_t)BlinkMarineMessageId::DialState2:
            kp->nLastRxTime = SYS_TIME;
            return true;
        case (uint16_t)BlinkMarineMessageId::LedBrightness:
            kp->nLastRxTime = SYS_TIME;
            return true;
        case (uint16_t)BlinkMarineMessageId::AnalogInput:
            kp->nLastRxTime = SYS_TIME;
            return true;
        case (uint16_t)BlinkMarineMessageId::Backlight:
            kp->nLastRxTime = SYS_TIME;
            return true;
    }

    kp->nLastRxTime = SYS_TIME;

    return true;
}

CANTxFrame GetTxMsgBlinkMarine(Keypad* kp, uint8_t nIndex)
{
    switch (nIndex)
    {
    case 0:
        return LedOnMsg(kp);
    case 1:
        return LedBlinkMsg(kp);
    case 2:
        return LedBrightnessMsg(kp);
    case 3:
        return BacklightMsg(kp);
    default:
        return {};
    }
}

CANTxFrame GetStartMsgBlinkMarine(Keypad* kp)
{
    CANTxFrame msg;
    msg.SID = 0x00;
    msg.IDE = CAN_IDE_STD;
    msg.DLC = 8;
    msg.data8[0] = 0x01;
    msg.data8[1] = kp->pConfig->nNodeId;
    msg.data8[2] = 0x00;
    msg.data8[3] = 0x00;
    msg.data8[4] = 0x00;
    msg.data8[5] = 0x00;
    msg.data8[6] = 0x00;
    msg.data8[7] = 0x00;

    return msg;
}

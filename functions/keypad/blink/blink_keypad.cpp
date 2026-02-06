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

} // anonymous namespace

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
        case KeypadModel::BlinkRacepad:
            kp->nNumButtons = 12;
            kp->nNumDials = 4;
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

    // Wire button function pointers for BlinkMarine LED behavior
    for (uint8_t i = 0; i < KEYPAD_MAX_BUTTONS; i++)
        kp->button[i].SetBrand(UpdateLedBlinkMarine, GetLedStateBlinkMarine);
}

bool CheckMsgBlinkMarine(Keypad* kp, CANRxFrame frame)
{
    if (frame.SID != kp->pConfig->nNodeId + 0x180)
        return false;

    kp->nLastRxTime = SYS_TIME;

    kp->fVal[0] = kp->button[0].Update(frame.data8[0] & 0x01);
    kp->fVal[1] = kp->button[1].Update((frame.data8[0] & 0x02) >> 1);
    kp->fVal[2] = kp->button[2].Update((frame.data8[0] & 0x04) >> 2);
    kp->fVal[3] = kp->button[3].Update((frame.data8[0] & 0x08) >> 3);
    kp->fVal[4] = kp->button[4].Update((frame.data8[0] & 0x10) >> 4);
    kp->fVal[5] = kp->button[5].Update((frame.data8[0] & 0x20) >> 5);
    kp->fVal[6] = kp->button[6].Update((frame.data8[0] & 0x40) >> 6);
    kp->fVal[7] = kp->button[7].Update((frame.data8[0] & 0x80) >> 7);
    kp->fVal[8] = kp->button[8].Update((frame.data8[1] & 0x01));
    kp->fVal[9] = kp->button[9].Update((frame.data8[1] & 0x02) >> 1);
    kp->fVal[10] = kp->button[10].Update((frame.data8[1] & 0x04) >> 2);
    kp->fVal[11] = kp->button[11].Update((frame.data8[1] & 0x08) >> 3);
    kp->fVal[12] = kp->button[12].Update((frame.data8[1] & 0x10) >> 4);
    kp->fVal[13] = kp->button[13].Update((frame.data8[1] & 0x20) >> 5);
    kp->fVal[14] = kp->button[14].Update((frame.data8[1] & 0x40) >> 6);
    kp->fVal[15] = 0;
    kp->fVal[16] = 0;
    kp->fVal[17] = 0;
    kp->fVal[18] = 0;
    kp->fVal[19] = 0;

    // nNodeId + 0x280
    //     If not racepad:
    //     dial[0].Update(frame.data64[0]);
    //     If racepad:
    //     dial[0].Update(frame.data64[0]);
    //     dial[1].Update(frame.data64[0]);

    // nNodeId + 0x380
    //     If not racepad:
    //     dial[1].Update(frame.data64[0]);
    //     If racepad:
    //     dial[2].Update(frame.data64[0]);
    //     dial[3].Update(frame.data64[0]);

    kp->nDialVal[0] = kp->dial[0].GetTicks();
    kp->nDialVal[1] = kp->dial[1].GetTicks();
    kp->nDialVal[2] = kp->dial[2].GetTicks();
    kp->nDialVal[3] = kp->dial[3].GetTicks();

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

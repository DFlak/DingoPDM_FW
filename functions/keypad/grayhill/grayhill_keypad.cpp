#include "grayhill_keypad.h"
#include "grayhill_button.h"
#include "keypad.h"

namespace {

// Grayhill LED indicator control
// Uses CANopen RPDO format: nNodeId + 0x200 (RPDO1)
// Each byte controls one button LED (up to 8 in first message)
// Value: 0=off, 1=on, 2=blink
CANTxFrame IndicatorMsg(Keypad* kp)
{
    for (uint8_t i = 0; i < kp->nNumButtons; i++)
        kp->button[i].UpdateLed();

    CANTxFrame msg;
    msg.SID = kp->pConfig->nNodeId + 0x200;
    msg.IDE = CAN_IDE_STD;
    msg.DLC = 8;

    // Grayhill uses single byte per LED with state value
    // 0 = off, 1 = on, 2 = blink
    for (uint8_t i = 0; i < 8 && i < kp->nNumButtons; i++)
    {
        msg.data8[i] = kp->button[i].GetLedState();
    }

    return msg;
}

// Grayhill brightness control
// Uses CANopen RPDO format: nNodeId + 0x300 (RPDO2)
// Byte 0: Button brightness (0-100%)
// Byte 1: Backlight brightness (0-100%)
CANTxFrame BrightnessMsg(Keypad* kp)
{
    CANTxFrame msg;
    msg.SID = kp->pConfig->nNodeId + 0x300;
    msg.IDE = CAN_IDE_STD;
    msg.DLC = 8;
    msg.data8[0] = *kp->pDimmingInput ? kp->pConfig->nDimButtonBrightness : kp->pConfig->nButtonBrightness;
    msg.data8[1] = *kp->pDimmingInput ? kp->pConfig->nDimBacklightBrightness : kp->pConfig->nBacklightBrightness;
    msg.data8[2] = 0x00;
    msg.data8[3] = 0x00;
    msg.data8[4] = 0x00;
    msg.data8[5] = 0x00;
    msg.data8[6] = 0x00;
    msg.data8[7] = 0x00;

    return msg;
}

} // anonymous namespace

void SetModelGrayhill(Keypad* kp)
{
    switch (kp->pConfig->eModel)
    {
        case KeypadModel::Grayhill6Key:
            kp->nNumButtons = 6;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Grayhill8Key:
            kp->nNumButtons = 8;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Grayhill15Key:
            kp->nNumButtons = 15;
            kp->nNumDials = 0;
            break;
        case KeypadModel::Grayhill20Key:
            kp->nNumButtons = 20;
            kp->nNumDials = 0;
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

    // Wire button function pointers for Grayhill LED behavior
    for (uint8_t i = 0; i < KEYPAD_MAX_BUTTONS; i++)
        kp->button[i].SetBrand(UpdateLedGrayhill, GetLedStateGrayhill);
}

bool CheckMsgGrayhill(Keypad* kp, CANRxFrame frame)
{
    // Grayhill uses CANopen TPDO format
    // Button state message: nNodeId + 0x180 (TPDO1)
    // Byte 0-2: Button states (up to 20 buttons)
    if (frame.SID != kp->pConfig->nNodeId + 0x180)
        return false;

    kp->nLastRxTime = SYS_TIME;

    // Grayhill button mapping - each bit represents a button
    // Buttons 0-7 in byte 0
    kp->fVal[0] = kp->button[0].Update(frame.data8[0] & 0x01);
    kp->fVal[1] = kp->button[1].Update((frame.data8[0] & 0x02) >> 1);
    kp->fVal[2] = kp->button[2].Update((frame.data8[0] & 0x04) >> 2);
    kp->fVal[3] = kp->button[3].Update((frame.data8[0] & 0x08) >> 3);
    kp->fVal[4] = kp->button[4].Update((frame.data8[0] & 0x10) >> 4);
    kp->fVal[5] = kp->button[5].Update((frame.data8[0] & 0x20) >> 5);
    kp->fVal[6] = kp->button[6].Update((frame.data8[0] & 0x40) >> 6);
    kp->fVal[7] = kp->button[7].Update((frame.data8[0] & 0x80) >> 7);

    // Buttons 8-15 in byte 1
    kp->fVal[8] = kp->button[8].Update(frame.data8[1] & 0x01);
    kp->fVal[9] = kp->button[9].Update((frame.data8[1] & 0x02) >> 1);
    kp->fVal[10] = kp->button[10].Update((frame.data8[1] & 0x04) >> 2);
    kp->fVal[11] = kp->button[11].Update((frame.data8[1] & 0x08) >> 3);
    kp->fVal[12] = kp->button[12].Update((frame.data8[1] & 0x10) >> 4);
    kp->fVal[13] = kp->button[13].Update((frame.data8[1] & 0x20) >> 5);
    kp->fVal[14] = kp->button[14].Update((frame.data8[1] & 0x40) >> 6);
    kp->fVal[15] = kp->button[15].Update((frame.data8[1] & 0x80) >> 7);

    // Buttons 16-19 in byte 2
    kp->fVal[16] = kp->button[16].Update(frame.data8[2] & 0x01);
    kp->fVal[17] = kp->button[17].Update((frame.data8[2] & 0x02) >> 1);
    kp->fVal[18] = kp->button[18].Update((frame.data8[2] & 0x04) >> 2);
    kp->fVal[19] = kp->button[19].Update((frame.data8[2] & 0x08) >> 3);

    // Grayhill keypads don't have dials
    kp->nDialVal[0] = 0;
    kp->nDialVal[1] = 0;
    kp->nDialVal[2] = 0;
    kp->nDialVal[3] = 0;

    return true;
}

CANTxFrame GetTxMsgGrayhill(Keypad* kp, uint8_t nIndex)
{
    switch (nIndex)
    {
    case 0:
        return IndicatorMsg(kp);
    case 1:
        return BrightnessMsg(kp);
    default:
        return {};
    }
}

CANTxFrame GetStartMsgGrayhill(Keypad* kp)
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

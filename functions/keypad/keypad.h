#pragma once

#include <cstdint>
#include "config.h"
#include "mailbox.h"
#include "dingopdm_config.h"
#include "keypad_button.h"
#include "keypad_dial.h"

extern float *pVarMap[PDM_VAR_MAP_SIZE];

class Keypad;  // Forward declaration

// Function pointer types for brand-specific behavior
using KeypadCheckMsgFn = bool (*)(Keypad*, CANRxFrame);
using KeypadGetTxMsgFn = CANTxFrame (*)(Keypad*, uint8_t);
using KeypadGetStartMsgFn = CANTxFrame (*)(Keypad*);
using KeypadSetModelFn = void (*)(Keypad*);

class Keypad
{
public:
    Keypad() = default;

    void SetConfig(Config_Keypad* config);

    static const uint16_t nBaseIndex = 0x2000;

    static msg_t InitThread(Keypad *keypads);

    bool IsEnabled() const { return pConfig->bEnabled; }
    void CheckTimeout();

    bool CheckMsg(CANRxFrame frame);
    CANTxFrame GetTxMsg(uint8_t nIndex);
    CANTxFrame GetStartMsg();
    uint8_t GetNumTxMsgs() const { return nNumTxMsgs; }

    // Var map data
    float fVal[KEYPAD_MAX_BUTTONS];
    float nDialVal[KEYPAD_MAX_DIALS];
    float fAnalogVal[KEYPAD_MAX_ANALOG_INPUTS];

    Config_Keypad* pConfig = nullptr;
    uint32_t nLastRxTime = 0;
    float* pDimmingInput = nullptr;
    KeypadButton button[KEYPAD_MAX_BUTTONS];
    KeypadDial dial[KEYPAD_MAX_DIALS];
    uint8_t nNumButtons = 0;
    uint8_t nNumDials = 0;

private:
    KeypadModel eLastModel = KeypadModel::Blink2Key;
    uint8_t nNumTxMsgs = 0;

    // Function pointers - set by SetConfig based on model
    KeypadCheckMsgFn fnCheckMsg = nullptr;
    KeypadGetTxMsgFn fnGetTxMsg = nullptr;
    KeypadGetStartMsgFn fnGetStartMsg = nullptr;
    KeypadSetModelFn fnSetModel = nullptr;
};

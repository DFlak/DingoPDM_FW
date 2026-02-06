#pragma once

#include <cstdint>

class KeypadButton;

// BlinkMarine-specific LED behavior functions
void UpdateLedBlinkMarine(KeypadButton* btn);
uint8_t GetLedStateBlinkMarine(KeypadButton* btn);

#pragma once

#include <cstdint>

class KeypadButton;

// Grayhill-specific LED behavior functions
void UpdateLedGrayhill(KeypadButton* btn);
uint8_t GetLedStateGrayhill(KeypadButton* btn);

#pragma once

#include "hal.h"
#include "enums.h"

#define PDM_TYPE 2 //0 = PDM, 1 = PDM-MAX, 2 = AUX

#define PDM_NUM_OUTPUTS 15
#define PDM_NUM_INPUTS 0
#define PDM_NUM_VIRT_INPUTS 16
#define PDM_NUM_CAN_INPUTS 32
#define PDM_NUM_FLASHERS 4
#define PDM_NUM_WIPER_INTER_DELAYS 6
#define PDM_NUM_WIPER_SPEED_MAP 8
#define PDM_NUM_COUNTERS 4
#define PDM_NUM_CONDITIONS 32

#define PDM_VAR_MAP_SIZE 134

#define PDM_NUM_TX_MSGS 16

#define PDM_NUM_THERMISTORS 4
#define PDM_NUM_INA3221 5
#define PDM_NUM_INA3221_I2CD2 3  // sensors 0-2 on I2CD2
#define PDM_NUM_INA3221_I2CD3 2  // sensors 3-4 on I2CD3

#define ADC1_NUM_CHANNELS 7
#define ADC1_BUF_DEPTH 1

// NCP18XH103F03RB thermistor parameters
#define THERM_R_REF 10000.0f    // Reference resistance at 25C (10K)
#define THERM_T_REF 298.15f     // Reference temperature in Kelvin (25C)
#define THERM_BETA 3455.0f      // B25/100 value
#define THERM_R_PULLUP 10000.0f // Pullup resistor value (10K)

// INA3221 shunt resistance
#define INA3221_SHUNT_R 0.001f  // 1mOhm

#define BTS7002_1EPP_KILIS 229500
#define BTS7008_2EPA_KILIS 59500
#define BTS70012_1ESP_KILIS 350000

#define SLEEP_TIMEOUT 30000

#define SYS_TIME TIME_I2MS(chVTGetSystemTimeX())

static const uint16_t ALWAYS_FALSE = 0;
static const uint16_t ALWAYS_TRUE = 1;

enum class AnalogChannel
{
    Therm1 = 0,
    Therm2,
    Therm3,
    Therm4,
    BattVolt,
    TempSensor,
    VRefInt
};

enum class LedType
{
    Status,
    Error
};

const CANConfig &GetCanConfig(CanBitrate bitrate);

const I2CConfig i2cConfig = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
};

msg_t InitAdc();
void DeInitAdc();
uint16_t GetAdcRaw(AnalogChannel channel);
float GetBattVolt();
float GetTemperature();
float GetVDDA();

float GetThermistorTemp(AnalogChannel channel);

bool InitBoardTemp();
float BoardReadTemp();
bool IsBoardOverTemp();
bool IsBoardCritTemp();

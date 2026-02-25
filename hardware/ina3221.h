#pragma once

#include <cstdint>
#include "hal.h"

#define INA3221_TIMEOUT 100 //ms

#define INA3221_REG_CONFIG          0x00
#define INA3221_REG_SHUNT_CH1      0x01
#define INA3221_REG_BUS_CH1        0x02
#define INA3221_REG_SHUNT_CH2      0x03
#define INA3221_REG_BUS_CH2        0x04
#define INA3221_REG_SHUNT_CH3      0x05
#define INA3221_REG_BUS_CH3        0x06
#define INA3221_REG_MANUF_ID       0xFE
#define INA3221_REG_DIE_ID         0xFF

#define INA3221_MANUF_ID           0x5449
#define INA3221_DIE_ID             0x3220

// Shunt voltage LSB = 40uV
#define INA3221_SHUNT_LSB_UV       40

enum class INA3221Averaging : uint16_t {
    AVG_1    = 0,
    AVG_4    = 1,
    AVG_16   = 2,
    AVG_64   = 3,
    AVG_128  = 4,
    AVG_256  = 5,
    AVG_512  = 6,
    AVG_1024 = 7
};

enum class INA3221ConvTime : uint16_t {
    CT_140US  = 0,
    CT_204US  = 1,
    CT_332US  = 2,
    CT_588US  = 3,
    CT_1100US = 4,
    CT_2116US = 5,
    CT_4156US = 6,
    CT_8244US = 7
};

enum class INA3221Mode : uint16_t {
    POWER_DOWN           = 0,
    SHUNT_TRIGGERED      = 1,
    BUS_TRIGGERED        = 2,
    SHUNT_BUS_TRIGGERED  = 3,
    POWER_DOWN_2         = 4,
    SHUNT_CONTINUOUS     = 5,
    BUS_CONTINUOUS       = 6,
    SHUNT_BUS_CONTINUOUS = 7
};

struct INA3221Config {
    bool ch1_en = true;
    bool ch2_en = true;
    bool ch3_en = true;
    INA3221Averaging avg = INA3221Averaging::AVG_1;
    INA3221ConvTime vbus_ct = INA3221ConvTime::CT_1100US;
    INA3221ConvTime vsh_ct = INA3221ConvTime::CT_1100US;
    INA3221Mode mode = INA3221Mode::SHUNT_BUS_CONTINUOUS;
};

class INA3221
{
    public:
        INA3221(I2CDriver& i2cp, i2caddr_t addr, float shuntResistance)
            : m_driver(&i2cp)
            , m_addr(addr)
            , m_shuntR(shuntResistance)
        {
        }

        bool Init();
        bool CheckId();
        bool Configure(const INA3221Config& config);

        // Get current for a channel (1-3) in Profet format (A * 10)
        uint16_t GetCurrent(uint8_t channel);

        // Get raw shunt voltage register for a channel (1-3)
        bool GetShuntVoltage(uint8_t channel, int16_t* voltage);

        i2cflags_t GetErrors() { return lastErrors; }

    private:
        I2CDriver* m_driver;
        const i2caddr_t m_addr;
        const float m_shuntR; // Shunt resistance in ohms

        i2cflags_t lastErrors;

        bool Read16(uint8_t reg, uint16_t* val);
        bool Write16(uint8_t reg, uint16_t val);
};
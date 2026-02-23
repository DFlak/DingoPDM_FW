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
};

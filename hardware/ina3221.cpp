#include "ina3221.h"

bool INA3221::Read16(uint8_t reg, uint16_t* val)
{
    uint8_t txBuf[1] = {reg};
    uint8_t rxBuf[2] = {0};

    i2cAcquireBus(m_driver);
    msg_t ret = i2cMasterTransmitTimeout(m_driver, m_addr, txBuf, 1, rxBuf, 2, TIME_MS2I(INA3221_TIMEOUT));
    if (ret != MSG_OK)
    {
        lastErrors = i2cGetErrors(m_driver);
        i2cReleaseBus(m_driver);
        return false;
    }
    i2cReleaseBus(m_driver);

    // INA3221 is big-endian
    *val = (rxBuf[0] << 8) | rxBuf[1];
    return true;
}

bool INA3221::Write16(uint8_t reg, uint16_t val)
{
    uint8_t txBuf[3];
    txBuf[0] = reg;
    txBuf[1] = (val >> 8) & 0xFF; // INA3221 is big-endian
    txBuf[2] = val & 0xFF;

    i2cAcquireBus(m_driver);
    msg_t ret = i2cMasterTransmitTimeout(m_driver, m_addr, txBuf, 3, nullptr, 0, TIME_MS2I(INA3221_TIMEOUT));
    if (ret != MSG_OK) {
        lastErrors = i2cGetErrors(m_driver);
        i2cReleaseBus(m_driver);
        return false;
    }
    i2cReleaseBus(m_driver);
    return true;
}

bool INA3221::Configure(const INA3221Config& config)
{
    uint16_t reg = 0;
    
    if (config.ch1_en) reg |= (1 << 14);
    if (config.ch2_en) reg |= (1 << 13);
    if (config.ch3_en) reg |= (1 << 12);
    
    reg |= (static_cast<uint16_t>(config.avg) << 9);
    reg |= (static_cast<uint16_t>(config.vbus_ct) << 6);
    reg |= (static_cast<uint16_t>(config.vsh_ct) << 3);
    reg |= static_cast<uint16_t>(config.mode);

    return Write16(INA3221_REG_CONFIG, reg);
}

bool INA3221::Init()
{
    return CheckId();
}

bool INA3221::CheckId()
{
    uint16_t manufId = 0;
    if (!Read16(INA3221_REG_MANUF_ID, &manufId))
        return false;

    return (manufId == INA3221_MANUF_ID);
}

bool INA3221::GetShuntVoltage(uint8_t channel, int16_t* voltage)
{
    if (channel < 1 || channel > 3)
        return false;

    // Shunt voltage registers: CH1=0x01, CH2=0x03, CH3=0x05
    uint8_t reg = INA3221_REG_SHUNT_CH1 + (channel - 1) * 2;

    uint16_t raw = 0;
    if (!Read16(reg, &raw))
        return false;

    // Register is 13-bit signed, right-shifted by 3
    // Sign extend and shift
    *voltage = ((int16_t)raw) >> 3;
    return true;
}

uint16_t INA3221::GetCurrent(uint8_t channel)
{
    int16_t shuntRaw = 0;
    if (!GetShuntVoltage(channel, &shuntRaw))
        return 0;

    // Shunt voltage = shuntRaw * 40uV
    // Current = shuntVoltage / shuntResistance
    // Result in A * 10 (to match Profet format: 0.1A = 1, 1.0A = 10)
    float shuntV = (float)shuntRaw * INA3221_SHUNT_LSB_UV * 1e-6f;
    float currentA = shuntV / m_shuntR;

    if (currentA < 0.0f)
        currentA = 0.0f;

    return (uint16_t)(currentA * 10.0f);
}
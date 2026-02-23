#include "port.h"
#include "mcu_utils.h"
#include "dingopdm_config.h"
#include <cmath>

static const CANConfig canConfig1000 =
{
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    /*
     For 36MHz http://www.bittiming.can-wiki.info/ gives us Pre-scaler=2, Seq 1=15 and Seq 2=2. Subtract '1' for register values
    */
    CAN_BTR_SJW(0) | CAN_BTR_BRP(1)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1),
};

static const CANConfig canConfig500 =
{
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    /*
     For 36MHz http://www.bittiming.can-wiki.info/ gives us Pre-scaler=4, Seq 1=15 and Seq 2=2. Subtract '1' for register values
    */
    CAN_BTR_SJW(0) | CAN_BTR_BRP(3)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1),
};

static const CANConfig canConfig250 =
{
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    /*
     For 36MHz http://www.bittiming.can-wiki.info/ gives us Pre-scaler=8, Seq 1=15 and Seq 2=2. Subtract '1' for register values
    */
    CAN_BTR_SJW(0) | CAN_BTR_BRP(7)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1),
};

static const CANConfig canConfig125 =
{
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    /*
     For 36MHz http://www.bittiming.can-wiki.info/ gives us Pre-scaler=16, Seq 1=15 and Seq 2=2. Subtract '1' for register values
    */
    CAN_BTR_SJW(0) | CAN_BTR_BRP(15)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1),
};

const CANConfig& GetCanConfig(CanBitrate bitrate) {
    switch(bitrate) {
        case CanBitrate::Bitrate_1000K:
            return canConfig1000;
        case CanBitrate::Bitrate_500K:
            return canConfig500;
        case CanBitrate::Bitrate_250K:
            return canConfig250;
        case CanBitrate::Bitrate_125K:
            return canConfig125;
        default:
            return canConfig500;
    }
    return canConfig500;
}

adcsample_t adc1_samples[ADC1_NUM_CHANNELS] = {0};
//0 = Therm1 - ADC1_IN4 (PA4)
//1 = Therm2 - ADC1_IN5 (PA5)
//2 = Therm3 - ADC1_IN6 (PA6)
//3 = Therm4 - ADC1_IN7 (PA7)
//4 = BattVolt - ADC1_IN3 (PA3)
//5 = TempSensor (internal)
//6 = VRefInt (internal)

static const ADCConversionGroup adc1_cfg = {
    .circular = true,
    .num_channels = ADC1_NUM_CHANNELS,
    .end_cb = NULL,
    .error_cb = NULL,
    .cr1 = 0,
    .cr2 = ADC_CR2_SWSTART | ADC_CR2_CONT,
    .smpr1 = ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_15) |
             ADC_SMPR1_SMP_VREF(ADC_SAMPLE_15),
    .smpr2 = ADC_SMPR2_SMP_AN3(ADC_SAMPLE_15) |
             ADC_SMPR2_SMP_AN4(ADC_SAMPLE_15) |
             ADC_SMPR2_SMP_AN5(ADC_SAMPLE_15) |
             ADC_SMPR2_SMP_AN6(ADC_SAMPLE_15) |
             ADC_SMPR2_SMP_AN7(ADC_SAMPLE_15),
    .htr = 0,
    .ltr = 0,
    .sqr1 = 0,
    .sqr2 = ADC_SQR2_SQ7_N(ADC_CHANNEL_VREFINT),
    .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN4)  |
            ADC_SQR3_SQ2_N(ADC_CHANNEL_IN5)  |
            ADC_SQR3_SQ3_N(ADC_CHANNEL_IN6)  |
            ADC_SQR3_SQ4_N(ADC_CHANNEL_IN7)  |
            ADC_SQR3_SQ5_N(ADC_CHANNEL_IN3)  |
            ADC_SQR3_SQ6_N(ADC_CHANNEL_SENSOR)
    };

msg_t InitAdc()
{
    msg_t ret;
    ret = adcStart(&ADCD1, NULL);
    if(ret != HAL_RET_SUCCESS)
        return ret;

    adcSTM32EnableTSVREFE(); //Enable temp sensor and vref

    adcStartConversion(&ADCD1, &adc1_cfg, adc1_samples, ADC1_BUF_DEPTH);

    return HAL_RET_SUCCESS;
}

void DeInitAdc()
{
    adcStopConversion(&ADCD1);
    adcStop(&ADCD1);
}

uint16_t GetAdcRaw(AnalogChannel channel)
{
    return adc1_samples[static_cast<uint8_t>(channel)];
}

float GetBattVolt()
{
    // MCU vRef = 3.3v
    // 4095 counts full scale
    float mcuVolts = (GetVDDA() / 4095) * GetAdcRaw(AnalogChannel::BattVolt);

    const float rUpper = 47000;
    const float rLower = 4700;

    return mcuVolts * ((rUpper + rLower) / rLower);
}

float GetTemperature()
{
    return (30.0 + ((float)(GetAdcRaw(AnalogChannel::TempSensor) - STM32_TEMP_3V3_30C) / (STM32_TEMP_3V3_110C - STM32_TEMP_3V3_30C)) * (110.0 - 30.0));
}

float GetVDDA()
{
    return ((float)STM32_VREF_INT_CAL / (float)GetAdcRaw(AnalogChannel::VRefInt)) * 3.3;
}

float GetThermistorTemp(AnalogChannel channel)
{
    float vdda = GetVDDA();
    float raw = (float)GetAdcRaw(channel);

    if (raw <= 0.0f || raw >= 4095.0f)
        return -273.15f; // Invalid reading

    // Voltage at thermistor divider output
    float v = (vdda / 4095.0f) * raw;

    // Calculate thermistor resistance
    // Voltage divider: V = VDDA * R_therm / (R_therm + R_pullup)
    // R_therm = R_pullup * V / (VDDA - V)
    float denominator = vdda - v;
    if (denominator <= 0.0f)
        return 200.0f; // Very high temp (thermistor near 0 ohms)

    float rTherm = THERM_R_PULLUP * v / denominator;

    if (rTherm <= 0.0f)
        return 200.0f;

    // Steinhart-Hart B-parameter equation
    // 1/T = 1/T_ref + (1/B) * ln(R/R_ref)
    float invT = (1.0f / THERM_T_REF) + (1.0f / THERM_BETA) * logf(rTherm / THERM_R_REF);

    return (1.0f / invT) - 273.15f;
}

bool InitBoardTemp()
{
    // Thermistors are read via ADC, no init needed
    return true;
}

float BoardReadTemp()
{
    float maxTemp = -273.15f;

    const AnalogChannel thermChannels[PDM_NUM_THERMISTORS] = {
        AnalogChannel::Therm1,
        AnalogChannel::Therm2,
        AnalogChannel::Therm3,
        AnalogChannel::Therm4
    };

    for (int i = 0; i < PDM_NUM_THERMISTORS; i++)
    {
        float t = GetThermistorTemp(thermChannels[i]);
        if (t > maxTemp)
            maxTemp = t;
    }

    return maxTemp;
}

bool IsBoardOverTemp()
{
    return BoardReadTemp() > BOARD_TEMP_WARN;
}

bool IsBoardCritTemp()
{
    return BoardReadTemp() > BOARD_TEMP_CRIT;
}

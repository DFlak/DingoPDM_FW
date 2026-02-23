#include "hw_devices.h"
#include "port_pwm.h"

#if PDM_TYPE == 2

// 15 simple MOSFET outputs across 5 shared timers
// TIM3 Ch1-4: OUT1(PA0),  OUT2(PA1),  OUT3(PA2),  OUT4(PA10)
// TIM4 Ch1-4: OUT5(PA15), OUT6(PB0),  OUT7(PB1),  OUT8(PB2)
// TIM5 Ch1-4: OUT9(PB12), OUT10(PB13),OUT11(PB14),OUT12(PB15)
// TIM9 Ch1-2: OUT13(PC6), OUT14(PC7)
// TIM10 Ch1:  OUT15(PC8)
Profet pf[PDM_NUM_OUTPUTS] = {
    Profet(1,  ProfetModel::SimpleMOSFET, LINE_OUT1,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD3,  &pwm3Cfg,  PwmChannel::Ch1),
    Profet(2,  ProfetModel::SimpleMOSFET, LINE_OUT2,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD3,  &pwm3Cfg,  PwmChannel::Ch2),
    Profet(3,  ProfetModel::SimpleMOSFET, LINE_OUT3,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD3,  &pwm3Cfg,  PwmChannel::Ch3),
    Profet(4,  ProfetModel::SimpleMOSFET, LINE_OUT4,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD3,  &pwm3Cfg,  PwmChannel::Ch4),
    Profet(5,  ProfetModel::SimpleMOSFET, LINE_OUT5,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD4,  &pwm4Cfg,  PwmChannel::Ch1),
    Profet(6,  ProfetModel::SimpleMOSFET, LINE_OUT6,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD4,  &pwm4Cfg,  PwmChannel::Ch2),
    Profet(7,  ProfetModel::SimpleMOSFET, LINE_OUT7,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD4,  &pwm4Cfg,  PwmChannel::Ch3),
    Profet(8,  ProfetModel::SimpleMOSFET, LINE_OUT8,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD4,  &pwm4Cfg,  PwmChannel::Ch4),
    Profet(9,  ProfetModel::SimpleMOSFET, LINE_OUT9,  LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD5,  &pwm5Cfg,  PwmChannel::Ch1),
    Profet(10, ProfetModel::SimpleMOSFET, LINE_OUT10, LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD5,  &pwm5Cfg,  PwmChannel::Ch2),
    Profet(11, ProfetModel::SimpleMOSFET, LINE_OUT11, LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD5,  &pwm5Cfg,  PwmChannel::Ch3),
    Profet(12, ProfetModel::SimpleMOSFET, LINE_OUT12, LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD5,  &pwm5Cfg,  PwmChannel::Ch4),
    Profet(13, ProfetModel::SimpleMOSFET, LINE_OUT13, LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD9,  &pwm9Cfg,  PwmChannel::Ch1),
    Profet(14, ProfetModel::SimpleMOSFET, LINE_OUT14, LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD9,  &pwm9Cfg,  PwmChannel::Ch2),
    Profet(15, ProfetModel::SimpleMOSFET, LINE_OUT15, LINE_UNUSED, LINE_UNUSED, AnalogChannel::VRefInt, &PWMD10, &pwm10Cfg, PwmChannel::Ch1),
};

Digital in[PDM_NUM_INPUTS];

Led statusLed = Led(LedType::Status);
Led errorLed = Led(LedType::Error);

// 5x INA3221 current sensors
// I2C2: addr 0x40, 0x42, 0x43 (sensors 0-2)
// I2C3: addr 0x42, 0x43       (sensors 3-4)
INA3221 currentSensor[PDM_NUM_INA3221] = {
    INA3221(I2CD2, 0x40, INA3221_SHUNT_R),
    INA3221(I2CD2, 0x42, INA3221_SHUNT_R),
    INA3221(I2CD2, 0x43, INA3221_SHUNT_R),
    INA3221(I2CD3, 0x42, INA3221_SHUNT_R),
    INA3221(I2CD3, 0x43, INA3221_SHUNT_R),
};

#endif

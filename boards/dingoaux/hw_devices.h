#pragma once

#include "port.h"
#include "profet.h"
#include "digital.h"
#include "led.h"
#include "hardware/ina3221.h"

extern Profet pf[PDM_NUM_OUTPUTS];
extern Digital in[PDM_NUM_INPUTS];
extern Led statusLed;
extern Led errorLed;
extern INA3221 currentSensor[PDM_NUM_INA3221];

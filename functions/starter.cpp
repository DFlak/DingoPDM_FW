#include "starter.h"
#include "dbc.h"

void Starter::Update()
{
    for (uint8_t i = 0; i < PDM_NUM_OUTPUTS; i++)
    {
        if (!pConfig->bEnabled)
            fVal[i] = 1;
        else
            fVal[i] = !(pConfig->bDisableOut[i] && *pInput);
    }
}

void Starter::SetDefaultConfig(Config_Starter *config)
{
    config->bEnabled = false;
    config->nInput = 0;
    for(uint8_t i = 0; i < PDM_NUM_OUTPUTS; i++)
    {
        config->bDisableOut[i] = false;
    }
}
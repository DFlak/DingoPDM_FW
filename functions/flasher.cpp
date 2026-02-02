#include "flasher.h"
#include "dbc.h"

void Flasher::Update(uint32_t nTimeNow)
{
    if (!pConfig->bEnabled)
    {
        fVal = 0;
        return;
    }

    if (!*pInput)
    {
        fVal = 0;
        return;
    }
    
    if ((fVal == 0) && ((nTimeNow - nTimeOff) > pConfig->nFlashOffTime))
    {
        fVal = 1;
        nTimeOn = nTimeNow;
    }
    if ((fVal == 1) && ((nTimeNow - nTimeOn) > pConfig->nFlashOnTime))
    {
        fVal = 0;
        nTimeOff = nTimeNow;
    }
}

void Flasher::SetDefaultConfig(Config_Flasher *config)
{
    config->bEnabled = false;
    config->nInput = 0;
    config->nFlashOnTime = 500;
    config->nFlashOffTime = 500;
    config->bSingleCycle = false;
}
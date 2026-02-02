#include "wiper.h"
#include "dbc.h"

void Wiper::Update()
{
    if (!pConfig->bEnabled)
    {
        SetMotorSpeed(MotorSpeed::Off);
        return;
    }

    UpdateInter();

    CheckWash();
    CheckSwipe();

    switch (eState)
    {
    case WiperState::Parked:
        eLastState = eState;
        SetMotorSpeed(MotorSpeed::Off);
        break;

    case WiperState::Parking:
        eLastState = eState;
        // Keep last speed
        Parking();
        break;

    case WiperState::Slow:
        eLastState = eState;
        SetMotorSpeed(MotorSpeed::Slow);
        break;

    case WiperState::Fast:
        eLastState = eState;
        SetMotorSpeed(MotorSpeed::Fast);
        break;

    case WiperState::IntermittentOn:
        eLastState = eState;
        SetMotorSpeed(MotorSpeed::Slow);
        InterOn();
        break;

    case WiperState::IntermittentPause:
        eLastState = eState;
        SetMotorSpeed(MotorSpeed::Off);
        InterPause();
        break;

    case WiperState::Wash:
        eLastState = eState;
        // Set speed to prewash speed in Wash()
        Wash();
        break;

    case WiperState::Swipe:
        eLastState = eState;
        SetMotorSpeed(MotorSpeed::Fast);
        Swipe();
        break;
    }

    pMode->CheckInputs();

    //Set var map values
    //Fast/Slow set in SetMotorSpeed()
    nParkOut = eState == WiperState::Parked ? 1 : 0;
    nInterOut = ((eState == WiperState::IntermittentOn) || (eState == WiperState::IntermittentPause)) ? 1 : 0;
    nWashOut = (eState == WiperState::Wash) ? 1 : 0;
    nSwipeOut = (eState == WiperState::Swipe) ? 1 : 0;
}

void Wiper::Parking()
{
    // Park detected - stop motor
    if (GetParkSw())
    {
        SetMotorSpeed(MotorSpeed::Off);
        eState = WiperState::Parked;
        eSelectedSpeed = WiperSpeed::Park;
    }
}

void Wiper::InterOn()
{
    //Delay checking switch to allow motor to spin off of switch
    if (!((SYS_TIME - nMotorOnTime) > 1000))
        return;

    // Park detected
    // Stop motor
    // Save time - pause for set time
    if (GetParkSw())
    {
        nInterPauseStartTime = SYS_TIME;
        eState = WiperState::IntermittentPause;
    }
}

void Wiper::InterPause()
{
    // Pause for inter delay
    if ((SYS_TIME - nInterPauseStartTime) > nInterDelay)
    {
        eState = WiperState::IntermittentOn;
    }
}

void Wiper::Wash()
{
    if (eStatePreWash == WiperState::Fast)
        SetMotorSpeed(MotorSpeed::Fast);
    else
        SetMotorSpeed(MotorSpeed::Slow);

    if (*pWashInput)
    {
        nWashWipeCount = 0;
    }
    else
    {
        //Delay checking switch to allow motor to spin off of switch
        if (!((SYS_TIME - nMotorOnTime) > 100))
            return;
        
        if (GetParkSw() && (GetParkSw() != nLastParkSw))
            nWashWipeCount++;

        if (nWashWipeCount >= pConfig->nWashWipeCycles)
        {
            if (eStatePreWash == WiperState::Parked)
                eState = WiperState::Parking;
            else if (eStatePreWash == WiperState::IntermittentPause)
            {
                eState = WiperState::IntermittentOn;
            }
            else
            {
                eState = eStatePreWash;
            }
        }
    }
    nLastParkSw = GetParkSw();
}

void Wiper::Swipe()
{
    //Delay checking switch to allow motor to spin off of switch
    if (!((SYS_TIME - nMotorOnTime) > 100))
        return;

    // Park switch high
    // Moved past park position
    if (GetParkSw())
    {
        eState = WiperState::Parking;
    }
}

void Wiper::SetMotorSpeed(MotorSpeed speed)
{
    if((speed != eLastMotorSpeed) && (speed != MotorSpeed::Off))
        nMotorOnTime = SYS_TIME;
    
    eLastMotorSpeed = speed;
        
    switch (speed)
    {
    case MotorSpeed::Off:
        nSlowOut = 0;
        nFastOut = 0;
        break;

    case MotorSpeed::Slow:
        nSlowOut = 1;
        nFastOut = 0;
        break;

    case MotorSpeed::Fast:
        nSlowOut = 1;
        nFastOut = 1;
        break;
    }
    
}

MotorSpeed Wiper::GetMotorSpeed()
{
    if (nSlowOut && nFastOut)
        return MotorSpeed::Fast;
    else if (nSlowOut)
        return MotorSpeed::Slow;
    else
        return MotorSpeed::Off;
}

void Wiper::UpdateInter()
{
    // Map intermittent speed to delay
    if (eSelectedSpeed >= WiperSpeed::Intermittent1 && eSelectedSpeed <= WiperSpeed::Intermittent6)
    {
        nInterDelay = pConfig->nIntermitTime[static_cast<int>(eSelectedSpeed) - 3];
    }
}

void Wiper::CheckWash()
{
    if (GetWashInput() && (eState != WiperState::Wash))
    {
        eStatePreWash = eState;
        eState = WiperState::Wash;
    }
}

void Wiper::CheckSwipe()
{
    if (GetSwipeInput() && (eState == WiperState::Parked))
    {
        eState = WiperState::Swipe;
    }
}

bool Wiper::GetParkSw()
{
    return (!(*pParkSw) && !pConfig->bParkStopLevel) ||
           (*pParkSw && pConfig->bParkStopLevel);
}

void Wiper::SetDefaultConfig(Config_Wiper *config)
{
    config->bEnabled = false;
    config->eMode = WiperMode::DigIn;
    config->nSlowInput = 0;
    config->nFastInput = 0;
    config->nInterInput = 0;
    config->nOnInput = 0;
    config->nSpeedInput = 0;
    config->nParkInput = 0;
    config->bParkStopLevel = false;
    config->nSwipeInput = 0;
    config->nWashInput = 0;
    config->nWashWipeCycles = 0;
    config->eSpeedMap[0] = WiperSpeed::Intermittent1;
    config->eSpeedMap[1] = WiperSpeed::Intermittent2;
    config->eSpeedMap[2] = WiperSpeed::Intermittent3;
    config->eSpeedMap[3] = WiperSpeed::Intermittent4;  
    config->eSpeedMap[4] = WiperSpeed::Intermittent5;
    config->eSpeedMap[5] = WiperSpeed::Intermittent6;
    config->eSpeedMap[6] = WiperSpeed::Slow;
    config->eSpeedMap[7] = WiperSpeed::Fast;
    config->nIntermitTime[0] = 1000;
    config->nIntermitTime[1] = 2000;
    config->nIntermitTime[2] = 3000;
    config->nIntermitTime[3] = 4000;
    config->nIntermitTime[4] = 5000;
    config->nIntermitTime[5] = 6000;
}
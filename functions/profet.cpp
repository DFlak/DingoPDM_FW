#include "profet.h"
#include "dbc.h"

void Profet::Update(bool bOutEnabled)
{
    eLastState = eState;

    if (!pConfig->bEnabled)
    {
        pwm.Off();
        palClearLine(m_in);
        nOcCount = 0;
        eState = ProfetState::Off;
        fOutput = 0;
        return;
    }

    if ((*pInput) && bOutEnabled)
        eReqState = ProfetState::On;
    else
        eReqState = ProfetState::Off;

    // Set DSEL pin to select the appropriate IS channel
    // Only valid on 2 channel devices
    // Wait for DSEL changeover and ADC conversion to complete
    // DSEL changeover takes max ~60us
    if (m_model == ProfetModel::BTS7008_2EPA_CH1)
    {
        palClearLine(m_dsel);
        chThdSleepMicroseconds(60);
    }
    else if (m_model == ProfetModel::BTS7008_2EPA_CH2)
    {
        palSetLine(m_dsel);
        chThdSleepMicroseconds(60);
    }

    static uint32_t nCNT = 0;
    static uint32_t nCCR = 0;

    if (pwm.IsEnabled() && eState == ProfetState::On)
    {
        // Assign to local vars to prevent CNT rolling over and slipping past check
        // Example:
        // CCR = 2500
        // When checking read delay CNT = 2499
        // Before getting to the CNT < CCR check the CNT has rolled over to 0
        // This will cause an incorrect reading
        // Copying to local var freezes the CNT value
        nCCR = m_pwmDriver->tim->CCR[static_cast<uint8_t>(m_pwmChannel)];
        nCNT = m_pwmDriver->tim->CNT;

        if ((nCCR > nPwmReadDelay) &&
            (nCNT > nPwmReadDelay) &&
            (nCNT < nCCR))
        {
            palSetLine(LINE_E2);
            nIS = GetAdcRaw(m_ain);
            nLastIS = nIS;
        }
        else
        {
            nIS = nLastIS;
        }
    }
    else
        nIS = GetAdcRaw(m_ain);

    // Calculate current at ADC, multiply by kILIS ratio to get output current
    // Analog value must be ready before reading to allow for conversion after DSEL change
    // Use the measured VDDA value to calculate volts/step
    // Current = (rawVal * (VDDA / 4095)) / 1.2k) * kILIS
    fCurrent = (((float)nIS * (GetVDDA() / 4095)) / 1200) * fKILIS;

    // Ignore current less than a low value
    // Not capable of measuring that low anyways
    // Depending on the model the value changes
    switch (m_model)
    {
    case ProfetModel::BTS7002_1EPP:
        if (fCurrent <= 0.5f)
            fCurrent = 0;
        break;
    case ProfetModel::BTS7008_2EPA_CH1:
    case ProfetModel::BTS7008_2EPA_CH2:
        if (fCurrent <= 0.2f)
            fCurrent = 0;
        break;
    case ProfetModel::BTS70012_1ESP:
        if (fCurrent <= 1.0f)
            fCurrent = 0;
        break;
    }

    // Check for fault (device overcurrent/overtemp/short)
    // Raw ADC current reading will be very high
    if (nIS > 30000)
    {
        eState = ProfetState::Fault;
    }

    bInRushActive = (pConfig->nInrushTime + nInRushOnTime) > SYS_TIME;

    switch (eState)
    {
    case ProfetState::Off:
        pwm.Off();

        palClearLine(m_in);

        nOcCount = 0;

        // Check for turn on
        if (eReqState == ProfetState::On)
        {
            nInRushOnTime = SYS_TIME;
            eState = ProfetState::On;
        }
        break;

    case ProfetState::On:
        if (pwm.IsEnabled())
            pwm.On();
        else
            palSetLine(m_in);

        // Check for turn off
        if (eReqState == ProfetState::Off)
        {
            eState = ProfetState::Off;
        }

        // Overcurrent
        if (fCurrent > pConfig->fCurrentLimit && !bInRushActive)
        {
            nOcTriggerTime = SYS_TIME;
            nOcCount++;
            eState = ProfetState::Overcurrent;
        }

        // Inrush overcurrent
        if (fCurrent > pConfig->fInrushLimit && bInRushActive)
        {
            nOcTriggerTime = SYS_TIME;
            nOcCount++;
            eState = ProfetState::Overcurrent;
        }

        break;

    case ProfetState::Overcurrent:
        pwm.Off();

        palClearLine(m_in);

        // No reset, straight to fault
        if (pConfig->eResetMode == ProfetResetMode::None)
        {
            eState = ProfetState::Fault;
        }

        // Overcurrent count exceeded
        if (nOcCount >= pConfig->nResetLimit && pConfig->eResetMode == ProfetResetMode::Count)
        {
            eState = ProfetState::Fault;
        }

        // Overcurrent reset time exceeded
        // ResetEndless or ResetCount
        if ((pConfig->nResetTime + nOcTriggerTime) < SYS_TIME)
        {
            nInRushOnTime = SYS_TIME;
            eState = ProfetState::On;
        }

        // Check for turn off
        if (eReqState == ProfetState::Off)
        {
            eState = ProfetState::Off;
        }
        break;

    case ProfetState::Fault:
        pwm.Off();

        palClearLine(m_in);
        // Fault requires power cycle, no way out
        break;
    }

    pwm.Update();

    // Set var map values
    fOutput = eState == ProfetState::On ? 1 : 0;
    nOvercurrent = eState == ProfetState::Overcurrent ? 1 : 0;
    nFault = eState == ProfetState::Fault ? 1 : 0;

    palClearLine(LINE_E2);
}
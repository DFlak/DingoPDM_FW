#include "virtual_input.h"
#include "dbc.h"

void VirtualInput::Update()
{
    if (!pConfig->bEnabled)
    {
        fVal = 0;
        return;
    }
    
    if ((pVar0 == 0) || (pVar1 == 0))
        return;

    bResult0 = *pVar0;
    if (pConfig->bNot0)
        bResult0 = !bResult0;

    bResult1 = *pVar1;
    if (pConfig->bNot1)
        bResult1 = !bResult1;

    switch (pConfig->eCond0)
    {
    case BoolOperator::And:
        bResultSec0 = bResult0 && bResult1;
        break;
    case BoolOperator::Or:
        bResultSec0 = bResult0 || bResult1;
        break;
    case BoolOperator::Nor:
        bResultSec0 = !bResult0 || !bResult1;
        break;
    }

    // Only 2 conditions
    if (pConfig->nVar2 == 0)
    {
        fVal = input.Check(pConfig->eMode, false, bResultSec0);
        return;
    }

    bResult2 = *pVar2;

    if (pConfig->bNot2)
        bResult2 = !bResult2;

    switch (pConfig->eCond1)
    {
    case BoolOperator::And:
        bResultSec1 = bResultSec0 && bResult2;
        break;
    case BoolOperator::Or:
        bResultSec1 = bResultSec0 || bResult2;
        break;
    case BoolOperator::Nor:
        bResultSec1 = !bResultSec0 || !bResult2;
        break;
    }

    fVal = input.Check(pConfig->eMode, false, bResultSec1);
}

void VirtualInput::SetDefaultConfig(Config_VirtualInput *config)
{
    config->bEnabled = false;
    config->bNot0 = false;
    config->nVar0 = 0;
    config->eCond0 = BoolOperator::And;
    config->bNot1 = false;
    config->nVar1 = 0;
    config->eCond1 = BoolOperator::And;
    config->bNot2 = false;
    config->nVar2 = 0;
    config->eMode = InputMode::Momentary;
}

#include "counter.h"
#include "dbc.h"
#include "edge.h"

void Counter::Update()
{
    if (!pConfig->bEnabled)
    {
        fVal = 0;
        return;
    }

    if (Edge::Check(pConfig->eResetEdge, bLastReset, *pResetInput))
    {
        fVal = 0;
        return;
    }

    if (Edge::Check(pConfig->eIncEdge, bLastInc, *pIncInput))
    {
        fVal++;
        if (fVal > pConfig->nMaxCount)
        {
            fVal = pConfig->bWrapAround ? 0 : pConfig->nMaxCount;
        }
    }

    if (Edge::Check(pConfig->eDecEdge, bLastDec, *pDecInput))
    {
        if(fVal == 0)
        {
            fVal = pConfig->bWrapAround ? pConfig->nMaxCount : pConfig->nMinCount;
        }
        else
        {
            fVal--;
        }
    }

    bLastInc = *pIncInput;
    bLastDec = *pDecInput;
    bLastReset = *pResetInput;
}
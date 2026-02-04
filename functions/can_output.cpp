#include "can_output.h"
#include "mailbox.h"

void CanOutput::Update()
{
    if (!pConfig->bEnabled)
    {
        return;
    }

    // TODO: Handle byte order, signed, factor, offset, etc.
    // Build CAN frames from an array of CAN frames, multiple outputs may be packed into the same frame based on ID
}

void CanOutput::CheckTime()
{

    uint32_t nCurrentTime = SYS_TIME;

    if (nCurrentTime - nLastTxTime >= pConfig->nCycleTime)
    {
        CANTxFrame frame;
        PostTxFrame(&frame);

        nLastTxTime = nCurrentTime;
    }
}


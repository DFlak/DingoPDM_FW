#include "config_handler.h"
#include "msg.h"
#include "dingopdm_config.h"
#include "can.h"
#include "can_input.h"
#include "counter.h"
#include "condition.h"
#include "digital.h"
#include "flasher.h"
#include "profet.h"
#include "starter.h"
#include "virtual_input.h"
#include "wiper/wiper.h"
#include "keypad/keypad.h"

extern PdmConfig stConfig;
extern Digital in[PDM_NUM_INPUTS];
extern CanInput canIn[PDM_NUM_CAN_INPUTS];
extern VirtualInput virtIn[PDM_NUM_VIRT_INPUTS];
extern Profet pf[PDM_NUM_OUTPUTS];
extern Wiper wiper;
extern Starter starter;
extern Flasher flasher[PDM_NUM_FLASHERS];
extern Counter counter[PDM_NUM_COUNTERS];
extern Condition condition[PDM_NUM_CONDITIONS];
extern Keypad keypad[PDM_NUM_KEYPADS];

void ApplyAllConfig()
{
    ApplyConfig(Digital::nBaseIndex);
    ApplyConfig(CanInput::nBaseIndex);
    ApplyConfig(VirtualInput::nBaseIndex);
    ApplyConfig(Profet::nBaseIndex);
    ApplyConfig(Wiper::nBaseIndex);
    ApplyConfig(Starter::nBaseIndex);
    ApplyConfig(Flasher::nBaseIndex);
    ApplyConfig(Counter::nBaseIndex);
    ApplyConfig(Condition::nBaseIndex);
    ApplyConfig(Keypad::nBaseIndex);
}

void ApplyConfig(uint16_t nIndex)
{
    uint16_t nBaseIndex = nIndex & 0xFF00;

    // Device config (0x0000)
    if (nBaseIndex == 0x0000)
    {
        // TODO: Change CAN speed and filters without requiring reset
        
        SetCanFilterEnabled(stConfig.stDevConfig.bCanFilterEnabled);
    }

    if (nBaseIndex == Digital::nBaseIndex)
    {
        for (uint8_t i = 0; i < PDM_NUM_INPUTS; i++)
            in[i].SetConfig(&stConfig.stInput[i]);
    }

    if (nBaseIndex == CanInput::nBaseIndex)
    {
        ClearCanFilters(); // Clear all filters before setting new ones

        // Set filter for CAN settings request message, (Base ID - 1)
        // Use filter 0, it is always enabled to allow all messages by hal so it must be used
        SetCanFilterId(0, stConfig.stDevConfig.nBaseId - 1, false);

        for (uint8_t i = 0; i < PDM_NUM_CAN_INPUTS; i++)
        {
            canIn[i].SetConfig(&stConfig.stCanInput[i]);
            if(!stConfig.stCanInput[i].bEnabled)
                continue; // Skip if not enabled
            
            // Set filter for this input
            uint32_t nId = 0;
            if(stConfig.stCanInput[i].nIDE == 1)
                nId = stConfig.stCanInput[i].nEID;
            else
                nId = stConfig.stCanInput[i].nSID;
            SetCanFilterId(i + 1, nId, stConfig.stCanInput[i].nIDE == 1);
        }

        //TODO: Set can filter without requiring reset, need a new message to indicate all IDs set before stopping CAN
    }

    if (nBaseIndex == VirtualInput::nBaseIndex)
    {
        for (uint8_t i = 0; i < PDM_NUM_VIRT_INPUTS; i++)
            virtIn[i].SetConfig(&stConfig.stVirtualInput[i]);
    }

    if (nBaseIndex == Profet::nBaseIndex)
    {
        for (uint8_t i = 0; i < PDM_NUM_OUTPUTS; i++)
            pf[i].SetConfig(&stConfig.stOutput[i]);
    }

    if (nBaseIndex == Wiper::nBaseIndex)
    {
        wiper.SetConfig(&stConfig.stWiper);
    }

    if (nBaseIndex == Starter::nBaseIndex)
    {
        starter.SetConfig(&stConfig.stStarter);
    }

    if (nBaseIndex == Flasher::nBaseIndex)
    {
        for (uint8_t i = 0; i < PDM_NUM_FLASHERS; i++)
            flasher[i].SetConfig(&stConfig.stFlasher[i]);
    }

    if (nBaseIndex == Counter::nBaseIndex)
    {
        for (uint8_t i = 0; i < PDM_NUM_COUNTERS; i++)
            counter[i].SetConfig(&stConfig.stCounter[i]);
    }

    if (nBaseIndex == Condition::nBaseIndex)
    {
        for (uint8_t i = 0; i < PDM_NUM_CONDITIONS; i++)
            condition[i].SetConfig(&stConfig.stCondition[i]);
    }

    if (nBaseIndex == Keypad::nBaseIndex)
    {
        for (uint8_t i = 0; i < PDM_NUM_KEYPADS; i++)
            keypad[i].SetConfig(&stConfig.stKeypad[i]);
    }
}
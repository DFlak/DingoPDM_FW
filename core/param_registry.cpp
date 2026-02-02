#include "param_registry.h"

const ParamInfo* FindParam(uint16_t index, uint8_t subindex) {
    for (int i = 0; i < NUM_PARAMS; i++) {
        if (stParams[i].nIndex == index &&
            stParams[i].nSubIndex == subindex) {
            return &stParams[i];
        }
    }
    return nullptr;
}

uint32_t ReadParam(const ParamInfo* param)
{
    if(param == nullptr)
        return 0;

    return *(static_cast<uint32_t*>(param->pVal));
}

bool WriteParam(const ParamInfo* param, uint32_t value)
{
    if(param == nullptr)
        return false;

    if (value < param->nMinVal || value > param->nMaxVal) 
        return false; // Out of range

    if (param->eType == ParamType::Bool)
    {
        value = (value != 0) ? 1 : 0;
    }

    *(static_cast<uint32_t*>(param->pVal)) = value;
    return true;
}

bool IsDefaultValue(const ParamInfo* param)
{
    if(param == nullptr)
        return false;

    return *(static_cast<uint32_t*>(param->pVal)) == param->nDefaultVal;
}
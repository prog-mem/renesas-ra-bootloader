#include "progmem_load.h"

progmem_rt ProgMem_LoadBuffer(void *flash_ctrl, const uint8_t *buffer, uint32_t address, uint32_t size)
{

    return ProgMem_ProcessBuffer(flash_ctrl,buffer, address,size);
}
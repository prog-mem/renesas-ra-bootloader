#include "progmem_boot.h"

_BOOL g_b_flash_bgo_active = false;
_Bool g_b_flash_event_bgo_triggred = false;

flash_event_t g_b_flash_event;

uint32_t p_flash_block_size = 0;

void ProgMem_FlashCallback(void *parg)
{
    flash_callback_args_t *p_args = (flash_callback_args_t *)parg;

    g_b_flash_event = p_args->event;
    g_b_flash_event_bgo_triggred = true;
}

progmem_rt ProgMem_FlashOpen(void *flash_ctrl, void *flash_cfg)
{
    if (R_FLASH_HP_Open((flash_hp_instance_ctrl_t *)flash_ctrl, (flash_cfg_t *)flash_cfg); != FSP_SUCCESS)
    {
        return ((progmem_rt)-1);
    }

    g_b_flash_bgo_active = ((flash_cfg_t *)flash_cfg)->data_flash_bgo;

    return ((progmem_rt)0);
}

progmem_rt ProgMem_FlashClose(void *flash_ctrl, void *flash_cfg)
{
    if (R_FLASH_HP_Close((flash_hp_instance_ctrl_t *)flash_ctrl); != FSP_SUCCESS)
    {
        return ((progmem_rt)-1);
    }

    return ((progmem_rt)0);
}

progmem_rt ProgMem_FlashBlankCheck(void *flash_ctrl, uint32_t address, uint32_t size)
{
    flash_result_t p_blank_check_result = FLASH_RESULT_BLANK;

    g_b_flash_event_bgo_triggred = false;

    if (R_FLASH_HP_BlankCheck((flash_hp_instance_ctrl_t *)flash_ctrl, address, size, &p_blank_check_result) != FSP_SUCCESS)
    {
        return ((progmem_rt)-1);
    }

    if (p_blank_check_result == FLASH_RESULT_BGO_ACTIVE)
    {

        while (!g_b_flash_event_bgo_triggred)
            ;
        return ((progmem_rt)g_b_flash_event);
    }
    else
        return ((progmem_rt)p_blank_check_result);

    return ((progmem_rt)-1);
}

progmem_rt ProgMem_FlashErase(void *flash_ctrl, uint32_t address, uint32_t size, uint32_t flash_block_size)
{

    g_b_flash_event_bgo_triggred = false;

    __disable_irq();

    uint32_t num_blocks = (size / flash_block_size);

    if (R_FLASH_HP_Erase((flash_hp_instance_ctrl_t *)flash_ctrl, address, num_blocks); != FSP_SUCCESS)
    {
        return ((progmem_rt)-1);
    }
    __enable_irq();

    if (true == g_b_flash_bgo_active)
    {
        while (!g_b_flash_event_bgo_triggred)
            ;

        if (g_b_flash_event != FLASH_EVENT_ERASE_COMPLETE)
            return ((progmem_rt)-1);
    }

    return ((progmem_rt)0);
}

progmem_rt ProgMem_FlashWrite(void *flash_ctrl, const uint8_t *buffer, uint32_t addr, uint32_t size)
{
    g_b_flash_event_bgo_triggred = false;

    __disable_irq();
    /* Write code flash data*/
    /* Error Handle */
    if (FSP_SUCCESS != R_FLASH_HP_Write((flash_hp_instance_ctrl_t *)flash_ctrl, (uint32_t)buffer, addr, size))
    {
        return ((progmem_rt)-1);
    }

    __enable_irq();
    /* Wait for the write complete event flag, if BGO is SET  */
    if (true == g_b_flash_bgo_active)
    {
        while (!g_b_flash_event_bgo_triggred)
            ;

        if (g_b_flash_event != FLASH_EVENT_WRITE_COMPLETE)
            return ((progmem_rt)-1);
    }

    return ((progmem_rt)0);
}

progmem_rt ProgMem_FlashWriteAndVerfiy(void *flash_ctrl, const uint8_t *buffer, uint32_t addr, uint32_t size)
{
    g_b_flash_event_bgo_triggred = false;

    __disable_irq();
    /* Write code flash data*/
    /* Error Handle */
    if (FSP_SUCCESS != R_FLASH_HP_Write((flash_hp_instance_ctrl_t *)flash_ctrl, (uint32_t)buffer, addr, size))
    {
        return ((progmem_rt)-1);
    }

    __enable_irq();
    /* Wait for the write complete event flag, if BGO is SET  */
    if (true == g_b_flash_bgo_active)
    {
        while (!g_b_flash_event_bgo_triggred)
            ;

        if (g_b_flash_event != FLASH_EVENT_WRITE_COMPLETE)
            return ((progmem_rt)-1);
    }

    if (memcmp(buffer, (void *)addr, size))
        return ((progmem_rt)-1);

    return ((progmem_rt)0);
}

progmem_rt ProgMem_FlashRead(void *flash_ctrl, uint8_t *buffer, uint32_t addr, uint32_t size)
{
    memcpy(buffer, (void *)addr, size);

    return ((progmem_rt)0);
}


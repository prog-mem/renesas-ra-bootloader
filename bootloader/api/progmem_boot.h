/*
 * progmem_boot.h
 *
 *  Created on: 05-Aug-2022
 *      Author: progmem
 */


#ifndef PROGMEM_BOOT_H_
#define PROGMEM_BOOT_H_

#include "progmem_jump.h"

#define PROGMEM_FT iHEX

typedef uint32_t progmem_rt ;

extern _Bool g_b_flash_event_bgo_triggred = false;

extern flash_event_t g_b_flash_event;

void ProgMem_FlashCallback(void *parg);

progmem_rt ProgMem_FlashOpen(void *flash_ctrl,void *flash_cfg);
progmem_rt ProgMem_FlashClose(void *flash_ctrl,void *flash_cfg);
progmem_rt ProgMem_FlashBlankCheck(void *flash_ctrl, uint32_t address, uint32_t size);
progmem_rt ProgMem_FlashErase(void *flash_ctrl, uint32_t address, uint32_t size, uint32_t flash_block_size);


progmem_rt ProgMem_FlashWrite(void *flash_ctrl,const uint8_t *buffer, uint32_t addr, uint32_t size);

progmem_rt ProgMem_FlashRead(void *flash_ctrl,uint8_t *buffer, uint32_t addr, uint32_t size);


progmem_rt ProgMem_FlashRead(void *flash_ctrl,uint8_t *buffer, uint32_t addr, uint32_t size);

progmem_rt ProgMem_FlashWriteAndVerfiy(void *flash_ctrl,uint8_t *buffer, uint32_t addr, uint32_t size);


#endif
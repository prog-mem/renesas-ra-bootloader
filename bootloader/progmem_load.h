/*
 * progmem_load.h
 *
 *  Created on: 05-Aug-2022
 *      Author: progmem
 */

#ifndef PROGMEM_LOAD_H_
#define PROGMEM_LOAD_H_

#define FLASH_IMAGE IHEX
//#define FLASH_IMAGE SREC
//#define FLASH_IMAGE BIN

#if defined(FLASH_IMAGE == IHEX)
#include "flash_image/progmem_ihex.h"
#elif defined(FLASH_IMAGE == SREC)
#include "flash_image/progmem_srec.h"
#elif defined(FLASH_IMAGE == BIN)
#include "flash_image/progmem_bin.h"
#else
#error "Please Select Flash Image Type";
#endif

progmem_rt ProgMem_LoadBuffer(void *flash_ctrl, const uint8_t *buffer, uint32_t address, uint32_t size);

#endif
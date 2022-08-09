/*
 * progmem_bin.h
 *
 *  Created on: 05-Aug-2022
 *      Author: progmem
 */


#ifndef PROGMEM_BIN_H_
#define PROGMEM_BIN_H_

#include "api/progmem_boot.h"

progmem_rt ProgMem_ProcessBuffer(void *flash_ctrl, const uint8_t *buffer, uint32_t address, uint32_t size);
#endif
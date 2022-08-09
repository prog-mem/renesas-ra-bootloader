/*
 * progmem_ihex.h
 *
 *  Created on: 05-Aug-2022
 *      Author: progmem
 */

#ifndef PROGMEM_IHEX_H_
#define PROGMEM_IHEX_H_

#include "api/progmem_boot.h"

#define HEX_RECORED_TYPE_DATA 0    //Data Records
#define HEX_RECORED_TYPE_EOF 1     //End-of-File (EOF) Records
#define HEX_RECORED_TYPE_ESA 2     //Extended Segment Address Records (HEX86)
#define HEX_RECORED_TYPE_SSA 3     //Starting Execution Address
#define HEX_RECORED_TYPE_ELA 4     //Extended Linear Address Records (HEX386)
#define HEX_RECORED_TYPE_SLA 5     //Start Linear Address Records

#define HEX_RECORED_START  ':'
#define HEX_RECORED_ER     '\r'
#define HEX_RECORED_END    '\n'


progmem_rt ProgMem_ProcessBuffer(void *flash_ctrl, const uint8_t *buffer, uint32_t address, uint32_t size);

#endif
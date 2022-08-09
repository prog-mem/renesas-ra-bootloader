/*
 * progmem_r7fa4e1.h
 *
 *  Created on: 05-Aug-2022
 *      Author: progmem
 */

#ifndef PROGMEM_R7FA4E1_H_
#define PROGMEM_R7FA4E1_H_

#include "hal_data.h"

#define PROGMEM_FLASHCF_REGION0_BLOCK_SIZE        (0x2000U)
#define PROGMEM_FLASH_HP_CF_REGION0_SIZE           (0x10000U)

#define PROGMEM_FLASH_CF_SECTOR_SIZE               (0x400)

#define PROGMEM_FLASH_CF_REGION1_BLOCK_SIZE        (0x8000U)
#define PROGMEM_FLASH_CF_WRITE_SIZE                (128U)
#define PROGMEM_FLASH_CF_DUAL_BANK_START           (0) 

#define PROGMEM_FLASH_DATA_FLASH_START             (0x08000000U)
#define PROGMEM_FLASH_DF_BLOCK_SIZE                (64U)
#define PROGMEM_FLASH_DF_WRITE_SIZE                (4U)
#define PROGMEM_FLASH_HAS_FMEPROT                  (1)
#define PROGMEM_FLASH_SUPPORTS_DUAL_BANK           (0)

#define PROGMEM_SECTOR_SIZE (1024 * 2)

#endif
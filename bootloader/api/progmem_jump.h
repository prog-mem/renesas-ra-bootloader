/*
 * progmem_jump.h
 *
 *  Created on: 05-Aug-2022
 *      Author: progmem
 */

#ifndef PROGMEM_JUMP_H_
#define PROGMEM_JUMP_H_


#include "device/progmem_dev.h"

#define MSP_SPMPU (0)
#define PSP_SPMPU (1)

#define EnablePrivilegedMode() __asm("SVC #0")

void ProgMem_Jump(uint32_t jumpLocation);
void ProgMem_JumpToApp(void);

#endif /* APP_LOAD_H_ */

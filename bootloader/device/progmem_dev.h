/*
 * progmem_dev.h
 *
 *  Created on: 05-Aug-2022
 *      Author: progmem
 */

#ifndef PROGMEM_DEV_H_
#define PROGMEM_DEV_H_

//define your device name here
#define DEVICE_NAME R7FA4E1

#define PROGMEM_DEVICE (DEVICE_NAME)

#if defined(PROGMEM_DEVICE == R7FA4E1)
#include "progmem_r7fa4e1.h"
#elif defined(PROGMEM_DEVICE == R7FA6M2)
#include "progmem_r7fa6m2.h"
#else
#error "please define your device first !!! add your device file in this directory and add here ^^^^!."
#endif

#endif

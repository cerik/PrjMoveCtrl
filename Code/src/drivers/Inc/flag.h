/*
 * flag.h
 *
 *  Created on: 
 *      Author: B37531
 */

#ifndef FLAG_H_
#define FLAG_H_
#include "common.h"
/**************************************/
// macro define 
#define FLASH_FLAG_START_ADDRESS		0xC00
#define FLASH_FLAG_SIZE					1024
#define FLASH_FLAG_END_ADDRESS			FLASH_FLAG_START_ADDRESS + FLASH_FLAG_SIZE

#define WORK_MODE_BOOT_LOADER				0xFF
#define WORK_MODE_USER_APP					0x5a
#define WORK_MODE_INVALID_FLAG				0x00

/**************************************/
// function declare

uint32_t Check_WorkMode( uint32_t uiStartAddress, uint32_t uiEndAddress,
 uint32_t * pCurrentAddress );

unsigned char Write_WorkMode(uint8_t WorkMode);

#endif


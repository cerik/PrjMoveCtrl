/*
 * flag.c
 *
 *  Created on: 
 *      Author: B37531
 */

#include "flag.h"
#include "common.h"
#include "flash_FTFL.h"
/*********************************** Macros ************************************/

/*********************************** Defines ***********************************/

/********************************** Constant ***********************************/

/*********************************** Variables *********************************/

/*********************************** Prototype *********************************/

/*********************************** Function **********************************/


/*******************************************************************************
 * Function:        Check_WorkMode
 *
 * Description:     
 *
 * Returns:         work mode
 *
 * Notes:
 *
 *******************************************************************************/

 uint32_t Check_WorkMode( uint32_t uiStartAddress, uint32_t uiEndAddress,uint32_t * 
 pCurrentAddress )
 {
	uint32_t i;
   	for(i=uiStartAddress;i<uiEndAddress;i++)
	{
		switch(*((uint8_t *)i))
		{
		case WORK_MODE_BOOT_LOADER:
			*pCurrentAddress = i;
			return WORK_MODE_BOOT_LOADER;
		
		case WORK_MODE_USER_APP:
			*pCurrentAddress = i;
			return WORK_MODE_USER_APP;

		case WORK_MODE_INVALID_FLAG:
			break;
		default:
      return WORK_MODE_INVALID_FLAG;
			//break;

		}
	}
	*pCurrentAddress = uiStartAddress;
	// if all flag is not matching, return to boot loader mode
	return WORK_MODE_INVALID_FLAG;
 }

 /*******************************************************************************
 * Function:        Write_WorkMode
 *
 * Description:    modify flag to expected status 
 *
 * Returns:         result  1 - success
 *                                0 - fail
 *
 * Notes:
 *
 *******************************************************************************/
 unsigned char Write_WorkMode(uint8_t WorkMode)
 {
	uint32_t uiCurrentAddress;
 	uint32_t uiCurrentAddressContent;
 	uint32_t ui32CurrentAddress;
 	uint8_t  *pPointer;
 	uint8_t uiCurrentWorkMode;

 	pPointer = (uint8_t *)&uiCurrentAddressContent;
 	uiCurrentWorkMode = Check_WorkMode(FLASH_FLAG_START_ADDRESS,FLASH_FLAG_END_ADDRESS,&uiCurrentAddress);

 	if( WORK_MODE_INVALID_FLAG == uiCurrentWorkMode )
 	{
		// erase flash
		if(Flash_SectorErase(FLASH_FLAG_START_ADDRESS)!= Flash_OK)
		{
			return 0;
		}
		uiCurrentWorkMode = WORK_MODE_BOOT_LOADER;
 	}
	if( WorkMode == WORK_MODE_BOOT_LOADER )
	{
		if( uiCurrentWorkMode == WORK_MODE_USER_APP )
		{
			ui32CurrentAddress = (uiCurrentAddress/4)*4;
			uiCurrentAddressContent = *((uint32_t *)ui32CurrentAddress);
			pPointer[uiCurrentAddress%4] = WORK_MODE_INVALID_FLAG;
			if( Flash_ByteProgram( ui32CurrentAddress,&uiCurrentAddressContent,4) != Flash_OK )
			{
				return 0;
			}
		}
		if( (uiCurrentAddress+1) >= FLASH_FLAG_END_ADDRESS )
		{
			// this is the last record
			if(Flash_SectorErase(FLASH_FLAG_START_ADDRESS)!= Flash_OK)
			{
				return 0;
			}
		}
		// after erase, default is boot loader mode
		//
	}
	else if( WorkMode == WORK_MODE_USER_APP )
	{
		if( uiCurrentWorkMode == WORK_MODE_BOOT_LOADER )
		{
			ui32CurrentAddress = (uiCurrentAddress/4)*4;
			uiCurrentAddressContent = *((uint32_t *)ui32CurrentAddress);
			pPointer[uiCurrentAddress%4] = WORK_MODE_USER_APP;
			if( Flash_ByteProgram( ui32CurrentAddress,&uiCurrentAddressContent,4) != Flash_OK )
			{
				return 0;
			}
		}
	}
	else
	{
		//
	}
    return 1;
 }



/*
 * Flash_iap.c
 *
 *  Created on: 30 mai 2020
 *      Author: kabri
 */

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include "board.h"

#include "Flash_iap.h"
#include "iap.h"


void Flash_Write_Block(uint32_t strSector, uint32_t endSector, uint32_t *srcAdd, uint32_t byteswrt)
{
	uint8_t ret_code;
	uint32_t part_id;

	/* Read Part Identification Number*/
	part_id = Chip_IAP_ReadPID();
	DEBUGOUT("Part ID is: %x\r\n", part_id);

	/* Disable interrupt mode so it doesn't fire during FLASH updates */
	__disable_irq();

	/* IAP Flash programming */
	/* Prepare to write/erase the last sector */
	ret_code = Chip_IAP_PreSectorForReadWrite(strSector, endSector);

	/* Error checking */
	if (ret_code != IAP_CMD_SUCCESS) {
		DEBUGOUT("Chip_IAP_PreSectorForReadWrite() failed to execute, return code is: %x\r\n", ret_code);
	}

	/* Erase the last sector */
	ret_code = Chip_IAP_EraseSector(strSector, endSector);

	/* Error checking */
	if (ret_code != IAP_CMD_SUCCESS) {
		DEBUGOUT("Chip_IAP_EraseSector() failed to execute, return code is: %x\r\n", ret_code);
	}

	/* Prepare to write/erase the last sector */
	ret_code = Chip_IAP_PreSectorForReadWrite(strSector, endSector);

	/* Error checking */
	if (ret_code != IAP_CMD_SUCCESS) {
		DEBUGOUT("Chip_IAP_PreSectorForReadWrite() failed to execute, return code is: %x\r\n", ret_code);
	}

	/* Write to the last sector */
	ret_code = Chip_IAP_CopyRamToFlash(strSector, srcAdd, byteswrt);

	/* Error checking */
	if (ret_code != IAP_CMD_SUCCESS) {
		DEBUGOUT("Chip_IAP_CopyRamToFlash() failed to execute, return code is: %x\r\n", ret_code);
	}

	/* Re-enable interrupt mode */
	__enable_irq();
}


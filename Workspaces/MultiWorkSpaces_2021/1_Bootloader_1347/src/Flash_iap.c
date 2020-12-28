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

#include "Configuration.h"

unsigned int last_erased_sector = 0;


void Flash_Write_Block(uint32_t destadrr, uint32_t *srcAdd, uint32_t byteswrt)
{
	uint8_t ret_code;

	unsigned int sector_start = (destadrr) / 0x1000;

	/* Disable interrupt mode so it doesn't fire during FLASH updates */
	__disable_irq();

	/* IAP Flash programming */
	/* Prepare to write/erase the last sector */
	ret_code = Chip_IAP_PreSectorForReadWrite(sector_start, sector_start);

	if(sector_start != last_erased_sector)
	{
		/* Erase the last sector */
		ret_code = Chip_IAP_EraseSector(sector_start, sector_start);

		last_erased_sector = sector_start;
	}

	/* Prepare to write/erase the last sector */
	ret_code = Chip_IAP_PreSectorForReadWrite(sector_start, sector_start);

	/* Write to the last sector */
	ret_code = Chip_IAP_CopyRamToFlash(destadrr, srcAdd, byteswrt);

	/* Re-enable interrupt mode */
	__enable_irq();
}


/*
 * Bootloader.c
 *
 *  Created on: 30 mai 2020
 *      Author: kabri
 */

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include "board.h"
#include "Bootloader.h"
#include "FreeRTOS.h"
#include "task.h"

unsigned sector_start_map[MAX_FLASH_SECTOR] = {SECTOR_0_START,             \
		SECTOR_1_START,SECTOR_2_START,SECTOR_3_START,SECTOR_4_START,SECTOR_5_START,      \
		SECTOR_6_START,SECTOR_7_START,SECTOR_8_START,SECTOR_9_START,SECTOR_10_START,     \
		SECTOR_11_START,SECTOR_12_START,SECTOR_13_START,SECTOR_14_START,SECTOR_15_START, \
		SECTOR_16_START,SECTOR_17_START,SECTOR_18_START,SECTOR_19_START,SECTOR_20_START, \
		SECTOR_21_START,SECTOR_22_START,SECTOR_23_START,SECTOR_24_START,SECTOR_25_START, \
		SECTOR_26_START,SECTOR_27_START,SECTOR_28_START,SECTOR_29_START};

unsigned sector_end_map[MAX_FLASH_SECTOR] = {SECTOR_0_END,SECTOR_1_END,    \
		SECTOR_2_END,SECTOR_3_END,SECTOR_4_END,SECTOR_5_END,SECTOR_6_END,SECTOR_7_END,   \
		SECTOR_8_END,SECTOR_9_END,SECTOR_10_END,SECTOR_11_END,SECTOR_12_END,             \
		SECTOR_13_END,SECTOR_14_END,SECTOR_15_END,SECTOR_16_END,SECTOR_17_END,           \
		SECTOR_18_END,SECTOR_19_END,SECTOR_20_END,SECTOR_21_END,SECTOR_22_END,           \
		SECTOR_23_END,SECTOR_24_END,SECTOR_25_END,SECTOR_26_END,                         \
		SECTOR_27_END,SECTOR_28_END,SECTOR_29_END                                         };


unsigned param_table[5];
unsigned result_table[5];


void execute_user_code(void)
{
	void (*user_code_entry)(void);

		unsigned *p;	// used for loading address of reset handler from user flash

		/* Change the Vector Table to the USER_FLASH_START
		in case the user application uses interrupts */
		SCB->VTOR = (USER_FLASH_START & 0x1FFFFF80);

		// Load contents of second word of user flash - the reset handler address
		// in the applications vector table
		p = (unsigned *)(USER_FLASH_START +4);

		// Set user_code_entry to be the address contained in that second word
		// of user flash
		user_code_entry = (void *) *p;

		// Jump to user application
		user_code_entry();
}



int user_code_present(void)
{
	param_table[0] = BLANK_CHECK_SECTOR;
	param_table[1] = USER_START_SECTOR;
	param_table[2] = USER_START_SECTOR;

	iap_entry(param_table,result_table);

	if( result_table[0] == CMD_SUCCESS )
	{
		return (false);
	}
	return (true);
}


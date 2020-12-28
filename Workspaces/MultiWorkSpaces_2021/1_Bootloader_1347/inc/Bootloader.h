/*
 * Bootloader.h
 *
 *  Created on: 30 mai 2020
 *      Author: kabri
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_


#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include "board.h"

/***********************************************************************
 * Code Red Technologies - Minor modifications to original code for use
 * in RDB1768 secondary USB bootloader based on LPCUSB USB stack.
 * *********************************************************************/

//*** <<< Use Configuration Wizard in Context Menu >>> ***

/*
// <h> Flash Configuration
//   <o0> User Start Sector <0-29>
//   <o1> Device Type
//            <7=>  LPC17x1 - 8 KB
//            <15=> LPC17x2 - 64 KB
//            <17=> LPC17x4 - 128 KB
//            <21=> LPC17x5/6 - 256 KB
//            <29=> LPC17x8 - 512 KB
//   <o2> Code Read Protection
//        <0x11223344=> NO CRP <0x12345678=> CRP1 <0x87654321=> CRP2 <0x43218765=> CRP3
// </h>
*/


/*

 * CodeRed - changed start sector from bank 2 to bank 16 - ie 64k into flash.

 */

#define USER_START_SECTOR 4

#define MAX_USER_SECTOR 16



/*
 * CodeRed - change FLASH_BUF_SIZE from 256 to 512 to match buffer
 * size used by SCSI layer of LPCUSB
 */

#define FLASH_BUF_SIZE 256
#define USER_FLASH_START (sector_start_map[USER_START_SECTOR])
#define USER_FLASH_END   (sector_end_map[MAX_USER_SECTOR])
#define USER_FLASH_SIZE  ((USER_FLASH_END - USER_FLASH_START) + 1)
#define MAX_FLASH_SECTOR 16


/* Define start address of each Flash sector */

#define SECTOR_0_START      0x00000000

#define SECTOR_1_START      0x00001000

#define SECTOR_2_START      0x00002000

#define SECTOR_3_START      0x00003000

#define SECTOR_4_START      0x00004000

#define SECTOR_5_START      0x00005000

#define SECTOR_6_START      0x00006000

#define SECTOR_7_START      0x00007000

#define SECTOR_8_START      0x00008000

#define SECTOR_9_START      0x00009000

#define SECTOR_10_START     0x0000A000

#define SECTOR_11_START     0x0000B000

#define SECTOR_12_START     0x0000C000

#define SECTOR_13_START     0x0000D000

#define SECTOR_14_START     0x0000E000

#define SECTOR_15_START     0x0000F000



/* Define end address of each Flash sector */

#define SECTOR_0_END        0x00000FFF

#define SECTOR_1_END        0x00001FFF

#define SECTOR_2_END        0x00002FFF

#define SECTOR_3_END        0x00003FFF

#define SECTOR_4_END        0x00004FFF

#define SECTOR_5_END        0x00005FFF

#define SECTOR_6_END        0x00006FFF

#define SECTOR_7_END        0x00007FFF

#define SECTOR_8_END        0x00008FFF

#define SECTOR_9_END        0x00009FFF

#define SECTOR_10_END       0x0000AFFF

#define SECTOR_11_END       0x0000BFFF

#define SECTOR_12_END       0x0000CFFF

#define SECTOR_13_END       0x0000DFFF

#define SECTOR_14_END       0x0000EFFF

#define SECTOR_15_END       0x0000FFFF



typedef enum
{
	PREPARE_SECTOR_FOR_WRITE    =50,
	COPY_RAM_TO_FLASH           =51,
	ERASE_SECTOR                =52,
	BLANK_CHECK_SECTOR          =53,
	READ_PART_ID                =54,
	READ_BOOT_VER               =55,
	COMPARE                     =56,
	REINVOKE_ISP                =57
}IAP_Command_Code;


#define CMD_SUCCESS 0
#define IAP_ADDRESS 0x1FFF1FF1


int user_code_present(void);
void execute_user_code(void);
void bootjump(uint32_t adress);


#endif /* BOOTLOADER_H_ */

/*
 * Bootloader_Data.h
 *
 *  Created on: 12 nov. 2020
 *      Author: kabri
 */

#ifndef BOOTLOADER_DATA_H_
#define BOOTLOADER_DATA_H_

#include "Bootloader.h"

#include "Communication_Typedef.h"

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include "board.h"


/*
 * Bootloader
 */
enum Type_Bootloader
{
	bootloader_data,
	bootloader_FindeFichier,
	bootloader_Adresse_Etendue,
	bootloader_StartSegment_Address,
	bootloader_ExtendedLinear_Address,
	bootloader_StartLinear_Address,
};

struct st_LineBootloader
{
	//Instruction 253
	uint8_t longueur;
	unsigned short adresse;
	enum Type_Bootloader Type;
	uint8_t data[16];
	uint8_t CRC;
};

void Init_bootloader(void);

void Reception_Trame_Bootloader(struct Communication_Trame *_trame);
void Traitement_Data(struct st_LineBootloader *line);


#endif /* BOOTLOADER_DATA_H_ */

/*
 * Bootloader_Data.c
 *
 *  Created on: 12 nov. 2020
 *      Author: kabri
 */


#include "Bootloader_Data.h"
#include "string.h"
#include "Flash_iap.h"

#include "FreeRTOS.h"
#include "task.h"
#include "Configuration.h"

long Offset_Adresse_Ecriture = 0;
long Adresse_Ecriture = 0;
long Adresse_Depart = 0;

char flash_buf[FLASH_BUF_SIZE];

unsigned int block_start_address = 0;

void Init_bootloader(void)
{
	//Clear buffer
	memset(&flash_buf, 0xFF, sizeof(flash_buf));
}

bool LedState = false;
void Reception_Trame_Bootloader(struct Communication_Trame *_trame)
{
	struct st_LineBootloader _line;

	//_trame.Date[0] = ':';
	_line.longueur = _trame->Data[1];
	_line.adresse = _trame->Data[2];
	_line.adresse = _line.adresse << 8;
	_line.adresse += _trame->Data[3];
	_line.Type = _trame->Data[4];

	int i;
	for(i = 0; i < _line.longueur ; i++)
	{
		_line.data[i] = _trame->Data[5 + i];
	}

	_line.CRC = _trame->Data[5 + i];


	Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, LedState);
	LedState = !LedState;

	Traitement_Data(&_line);
}


void Traitement_Data(struct st_LineBootloader *line)
{
	unsigned int line_address, i;


	//Fct du type de Data
	switch(line->Type)
	{
	case bootloader_data:
		line_address = line->adresse;

		if(block_start_address == 0)
		{
			block_start_address = Offset_Adresse_Ecriture + line_address;
		}

		for(i = 0; i < line->longueur; i++ )
		{
			flash_buf[(line_address + i) % FLASH_BUF_SIZE] = line->data[i];

			if( (line_address + i + 1) % FLASH_BUF_SIZE == 0)
			{
				//Buffer is full
				//One sector to write
				Flash_Write_Block(block_start_address, (uint32_t*)&flash_buf, FLASH_BUF_SIZE);

				//Clear buffer
				memset(&flash_buf, 0xFF, sizeof(flash_buf));

				//next sector
				block_start_address = 0;
			}
		}
		break;

	case bootloader_FindeFichier:
		//Force l'écriture du dernier bloc de flash
		//Buffer is full
		//One sector to write
		Flash_Write_Block(block_start_address, (uint32_t*)&flash_buf, FLASH_BUF_SIZE);
		break;

	case bootloader_Adresse_Etendue:
		//Ajoute les données << 4 à l'offset d'adresse
		Offset_Adresse_Ecriture = line->data[0];
		Offset_Adresse_Ecriture <<= 8;
		Offset_Adresse_Ecriture += line->data[1];

		Offset_Adresse_Ecriture <<= 4;
		break;

	case bootloader_ExtendedLinear_Address:
		//Ajoute les données << 16 à l'offset d'adresse
		Offset_Adresse_Ecriture = line->data[0];
		Offset_Adresse_Ecriture <<= 8;
		Offset_Adresse_Ecriture += line->data[1];
		Offset_Adresse_Ecriture <<= 16;
		break;

	case bootloader_StartLinear_Address:
		break;

	default:
		break;
	}
}




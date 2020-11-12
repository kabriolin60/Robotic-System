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

long Offset_Adresse_Ecriture = 0;
long Adresse_Ecriture = 0;
long Nb_Bloc = 0;
long Adresse_Depart = 0;
uint8_t checksum_calculated = 0;

char flash_buf[FLASH_BUF_SIZE];

unsigned int byte_ctr = 0;
unsigned int sector_start = 0;

extern TaskHandle_t Run_Application_Handler;
uint8_t is_RunApplication_running = true;
unsigned int block_start_address = 0;

void Reception_Trame_Bootloader(struct Communication_Trame *_trame)
{
	if(is_RunApplication_running && Run_Application_Handler != NULL)
	{
		vTaskSuspend( Run_Application_Handler );
		is_RunApplication_running = false;
	}

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

		/*
		//Toogle_Pin(LED_1_PORT, LED_1_BIT);

		Adresse_Ecriture = Offset_Adresse_Ecriture + Nb_Bloc * FLASH_BUF_SIZE;

		//Memorise l'adresse du début de l'application
		if(Adresse_Depart == 0)
		{
			Adresse_Depart = Offset_Adresse_Ecriture;
		}

		//Ecriture des données reçues en Flash
		write_flash(Adresse_Ecriture, &line->data, line->longueur);

		//Si l'adresse est la fin du premier bloc
		/*
		if(line->adresse == 0x1F0 && checksum_calculated == 0)
		{
			//Calcul le checksum de la table de vecteur, à écrire à offset + 0x1C
			long Checksum = 0;

			Checksum = *(long *)Adresse_Depart;
			Checksum += *(long *)(Adresse_Depart + 4);
			Checksum += *(long *)(Adresse_Depart + 8);
			Checksum += *(long *)(Adresse_Depart + 12);
			Checksum += *(long *)(Adresse_Depart + 16);
			Checksum += *(long *)(Adresse_Depart + 20);
			Checksum += *(long *)(Adresse_Depart + 24);

			Checksum = 0xFFFFFFFF - Checksum;
			Checksum += 1;

			//Lecture du premier bloc
			long Premier_Bloc[128];
			int i;

			for(i = 0; i < 128; i++)
			{
				Premier_Bloc[i] = *(long*)(Adresse_Depart + (i * 4));
			}

			//Integration du checksum dans le premier bloc @ 0x1C
			Premier_Bloc[7] = (unsigned int)Checksum;

			checksum_calculated = 1;

			//Reecriture du premier bloc dans la Flash
			write_flash(Offset_Adresse_Ecriture, &Premier_Bloc, sizeof(Premier_Bloc));
			Nb_Bloc--;
		}
		/* */
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
		Nb_Bloc = 0;
		break;

	case bootloader_ExtendedLinear_Address:
		//Ajoute les données << 16 à l'offset d'adresse
		Offset_Adresse_Ecriture = line->data[0];
		Offset_Adresse_Ecriture <<= 8;
		Offset_Adresse_Ecriture += line->data[1];
		Offset_Adresse_Ecriture <<= 16;
		Nb_Bloc = 0;
		break;

	case bootloader_StartLinear_Address:
		break;

	default:
		break;
	}
}




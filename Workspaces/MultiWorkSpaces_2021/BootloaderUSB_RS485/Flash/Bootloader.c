/*
 * Bootloader.c
 *
 *  Created on: 17 déc. 2012
 *      Author: Robotic System
 */

#include "Bootloader.h"
#include "sbl_config.h"
#include "sbl_iap.h"

long Offset_Adresse_Ecriture = 0;
long Adresse_Ecriture = 0;
long Nb_Bloc = 0;
long Adresse_Depart = 0;
byte checksum_calculated = 0;

TaskHandle_t StartUserCode;

#define MAX_TIME_BEFORE_JUMP_USER_CODE 5000

unsigned char Flag_Start_User_Code = 0;

extern void task_Start_UserCode(void *pv_parameter);


void Traitement_Data(struct st_LineBootloader *line)
{
	if(StartUserCode != NULL)
		vTaskDelete(StartUserCode);
	StartUserCode = NULL;

	//Fct du type de Data
	switch(line->Type)
	{
	case bootloader_data:
		Toogle_Pin(LED_1_PORT, LED_1_BIT);

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
		Write_Bloc(Adresse_Ecriture);

		Task_Delay(1000);

		//xTaskCreate( task_Start_UserCode, "user code", 150, NULL, tskIDLE_PRIORITY, &StartUserCode );

		//Jump du user code
		//Si user code présent:
		Flag_Start_User_Code = 1;

		//Si user code présent:
		if(user_code_present())
		{
			//execute user code
			execute_user_code();
		}

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

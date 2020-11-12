/*
 * 2_Echange_Datas.c
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#include "2_Echange_Datas.h"
#include "1_Trame_Communication.h"
#include "stdio.h"

static TO_AHBS_RAM3 struct Communication_Trame trame_echange;// TO_AHBS_RAM0;

/*****************************************************************************
 ** Function name:		_2_Communication_Init
 **
 ** Descriptions:		Init de la comm niveau 2
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_Init()
{
	memset(&trame_echange, 0, sizeof(struct Communication_Trame));
}




/*****************************************************************************
 ** Function name:		_2_Comm_Send_PING
 **
 ** Descriptions:		Fonction d'envoie d'un ping à une carte
 **
 ** parameters:			Adresse de la carte cible
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_PING(uint8_t adresse_cible, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = PING;
	trame_echange.Slave_Adresse = adresse_cible;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Send_PONG
 **
 ** Descriptions:		Fonction d'envoie d'une réponse à un ping
 **
 ** parameters:			None, le PONG est envoyé à toutes les cartes
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_PONG(enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = PONG;
	trame_echange.Slave_Adresse = ALL_CARDS;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);

	//Send the revision of this board firmware
	static char str[70];
	sprintf(str, "Bootloader release= %s.%s; %s; %s\n",
			MAJOR_RELEASE,
			MINOR_RELEASE,
			__DATE__,
			__TIME__);
	_2_Comm_Send_Log_Message(str, Color_Black, RS485_port);
}




/*****************************************************************************
 ** Function name:		_2_Comm_Send_Log_Message
 **
 ** Descriptions:		Fonction de formatage d'une chaine de caractère en un message vers le PC
 **
 ** parameters:			Pointeur vers la chaine de caracteres
 ** 					Couleur
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
static struct Logger_Debug_Data log_message;// TO_AHBS_RAM0;

void _2_Comm_Send_Log_Message(char* str, enum Logger_Debug_Color color, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = LOGGER_DEBUG;
	trame_echange.Slave_Adresse = PC;

	log_message.Color = color;

	uint16_t stringlength;
	//Calcul la longueur de la chaine
	stringlength = strlen(str);

	if(stringlength < sizeof(log_message.Text))
	{
		log_message.Nombre_Carateres = COPYSTRING(str, log_message.Text);

		trame_echange.Length = COPYDATA(log_message, trame_echange.Data);
		trame_echange.XBEE_DEST_ADDR = XBee_PC;

		_1_Communication_Create_Trame(&trame_echange, canal);

	}else
	{
		//Plus d'une trame à envoyer pour tout le texte
		while(stringlength)
		{
			log_message.Nombre_Carateres = COPYSTRING(str, log_message.Text);

			trame_echange.Length = COPYDATA(log_message, trame_echange.Data);
			trame_echange.XBEE_DEST_ADDR = 0;

			_1_Communication_Create_Trame(&trame_echange, canal);

			//Suite du message
			str += log_message.Nombre_Carateres;
			stringlength = strlen(str);
		}
	}
}


/*****************************************************************************
 ** Function name:		_2_Communication_Boards_Status
 **
 ** Descriptions:		Request for sub boards status
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_Boards_Status(void* pvParameters)
{
	Task_Delay(100);
	_2_Comm_Send_PONG(RS485_port);
	for(int i = 0; i < 4; i++)
	{
		Task_Delay(2);
		_2_Comm_Send_PING(i+1, RS485_port);
	}

	Task_Delay(10);
	Task_Delete_Current;
}

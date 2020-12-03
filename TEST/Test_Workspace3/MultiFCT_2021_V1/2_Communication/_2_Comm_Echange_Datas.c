/*
 * _2_Comm_Echange_Datas.c
 *
 *  Created on: 3 déc. 2020
 *      Author: PC-INOVA
 */

#include "Lib_includes.h"
#include "_2_Comm_Echange_Datas.h"
#include "Parametres.h"

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
 ** Function name:		_2_Comm_Send_Demande_Info
 **
 ** Descriptions:		Fonction d'envoie d'une la demande d'infos
 **
 ** parameters:			Adresse de la carte cible
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Demande_Info(uint8_t adresse_cible, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DEMANDE_INFO;
	trame_echange.Slave_Adresse = adresse_cible;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo);
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
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = PING;
	trame_echange.Slave_Adresse = adresse_cible;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo);
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
uint8_t relese_sent = 0;
extern long Nb_Messages_recus;
extern long Nb_Erreurs_com;
static char str[70];

void _2_Comm_Send_PONG(enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = PONG;
	trame_echange.Slave_Adresse = ALL_CARDS;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo);

	//Send the revision of this board firmware

	if(!relese_sent)
	{
		if(static_BOARD_TYPE == TYPE_IA_BOARD)
		{
			sprintf(str, "IA release= %s.%s; %s; %s\n",
					MAJOR_RELEASE,
					MINOR_RELEASE,
					__DATE__,
					__TIME__);
		}else if(static_BOARD_TYPE == TYPE_MULTIFCT)
		{
			sprintf(str, "MultiFct @%c, release= %s.%s; %s; %s\n",
					ADRESSE_CARTE_CHAR,
					MAJOR_RELEASE,
					MINOR_RELEASE,
					__DATE__,
					__TIME__);
		}
		_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Divers, RS485_port);
		relese_sent = 1;
	}
	_2_Comm_Send_Communication_Status(canal);
}


void _2_Comm_Send_Communication_Status(enum enum_canal_communication canal)
{
	if(static_BOARD_TYPE == TYPE_IA_BOARD)
	{
		sprintf(str, "IA mess= %ld // erreurs= %ld\n",
				Nb_Messages_recus,
				Nb_Erreurs_com);
	}else if(static_BOARD_TYPE == TYPE_MULTIFCT)
	{
		sprintf(str, "Multi_Fct @%c mess= %ld // erreurs= %ld\n",
				ADRESSE_CARTE_CHAR,
				Nb_Messages_recus,
				Nb_Erreurs_com);
	}

	if(Nb_Erreurs_com < 5)
	{
		_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Divers, RS485_port);
	}else
	{
		_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Divers, RS485_port);
	}
}


/*****************************************************************************
 ** Function name:		_2_Comm_Send_Log_Message
 **
 ** Descriptions:		Fonction de formatage d'une chaine de caractère en un message vers le PC
 **
 ** parameters:			Pointeur vers la chaine de caracteres
 ** 					Couleur
 ** 					Cannal
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
static struct Logger_Debug_Data log_message;// TO_AHBS_RAM0;

void _2_Comm_Send_Log_Message(char* str, enum Logger_Debug_Color color, uint8_t Channel, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = LOGGER_DEBUG;
	trame_echange.Slave_Adresse = PC;

	log_message.Color = color;

	log_message.Channel = Channel;

	uint16_t stringlength;
	//Calcul la longueur de la chaine
	stringlength = strlen(str);

	if(stringlength < sizeof(log_message.Text))
	{
		log_message.Nombre_Carateres = COPYSTRING(str, log_message.Text);

		trame_echange.Length = COPYDATA(log_message, trame_echange.Data);
		trame_echange.XBEE_DEST_ADDR = XBee_PC;

		_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo);

#ifdef TYPE_CARTE_IA
#ifdef ENREGISTREMENT_FLASH
		//Renvoie le message pour etre enregistre dans la Flash
		xQueueSend(QueueEnregistrementMessages, &Com_Position_Robot, 0);
#endif
#endif

	}else
	{
		//Plus d'une trame à envoyer pour tout le texte
		while(stringlength)
		{
			log_message.Nombre_Carateres = COPYSTRING(str, log_message.Text);

			trame_echange.Length = COPYDATA(log_message, trame_echange.Data);
			trame_echange.XBEE_DEST_ADDR = 0;

			_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo);

#ifdef TYPE_CARTE_IA
#ifdef ENREGISTREMENT_FLASH
			//Renvoie le message pour etre enregistre dans la Flash
			xQueueSend(QueueEnregistrementMessages, &Com_Position_Robot, 0);
#endif
#endif

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
	Init_Timing_Tache;

	Task_Delay_Until(100);
	_2_Comm_Send_PONG(RS485_port); //premier pong initial avec la version de la carte IA
	Task_Delay_Until(1);

	//Demande un ping sur les autres cartes
	for(int i = 0; i < 4; i++)
	{
		_2_Comm_Send_PING(i+1, RS485_port);
		Task_Delay_Until(5.0F);
	}


	Task_Delay_Until(100);
	int boucle = 0;
	while(true)
	{
		for(int i = 0; i < 4; i++)
		{
			_2_Comm_Send_Demande_Info(i+1, RS485_port);

			Task_Delay_Until(5.0F);
		}

		//Envoi l'état de la carte IA
		_2_Comm_Send_Info_Carte_IA(RS485_port);
		Task_Delay_Until(5.0F);

		boucle++;

		if(boucle == 1000)
		{
			_2_Comm_Send_PONG(RS485_port); //pongs suivants sans la version de la carte IA

			for(int i = 0; i < 4; i++)
			{
				_2_Comm_Send_PING(i+1, RS485_port);
				Task_Delay_Until(5.0F);
			}
			boucle = 0;
		}
	}
}


/*****************************************************************************
 ** Function name:		_2_Comm_Send_Info_Carte_IA
 **
 ** Descriptions:		Fonction d'envoi de l'état de la carte IA vers le PC
 **
 ** parameters:			Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Info_Carte_IA(enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if (_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo) == pdFAIL)
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	struct Com_Reponse_Info_IA Infos;
	Infos.Numero_Robot = _Strategie_Get_Robot_ID();

	Infos.Strategie = _Strategie_Get_Chosen_Strategie();

	Infos.Temps_Match = (unsigned short)(_Strategie_Get_Temps_Match() / 10);                             //Temps /10		//2 octets

	//0= Jack; 1 = Color; 2 = Switchs; 3 = LED Red; 4 = LED Yellow; 5 = LED Green
	Infos.Etat_Inputs = _Strategie_Get_Jack_Status();
	Infos.Etat_Inputs |= _Strategie_Get_Color_Status() << 1;
	Infos.Etat_Inputs |= _Strategie_Get_Switch_Status() << 2;
	Infos.Etat_Inputs |= _Strategie_Get_External_LED_RED_Status() << 3;
	Infos.Etat_Inputs |= _Strategie_Get_External_LED_YELLOW_Status() << 4;
	Infos.Etat_Inputs |= _Strategie_Get_External_LED_GREEN_Status() << 5;


	trame_echange.Instruction = REPONSE_INFO_IA;
	trame_echange.Slave_Adresse = PC;

	trame_echange.Length = COPYDATA(Infos, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = XBee_PC;

	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo);
}


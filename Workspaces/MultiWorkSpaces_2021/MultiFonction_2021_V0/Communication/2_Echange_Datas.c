/*
 * 2_Echange_Datas.c
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#include "2_Echange_Datas.h"
#include "1_Trame_Communication.h"

static TO_AHBS_RAM3 struct Communication_Trame trame_echange;

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
extern long Nb_Messages_recus;
extern long Nb_Erreurs_com;
byte relese_sent = 0;
static char str[70];
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

	struct Communication_PONG Pong;
	Pong.Adresse = ADRESSE_CARTE;
	Pong.Nombre_Messages_Recus = Nb_Messages_recus;
	Pong.Nombre_Erreurs_Communication = Nb_Erreurs_com;

	trame_echange.Length = COPYDATA(Pong, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);

	//Send the revision of this board firmware
	if(!relese_sent)
	{
		sprintf(str, "MultiFct @%c, release= %s.%s; %s; %s\n",
				ADRESSE_CARTE_CHAR,
				MAJOR_RELEASE,
				MINOR_RELEASE,
				__DATE__,
				__TIME__);
		_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Divers, RS485_port);
		relese_sent = 1;
	}

	_2_Comm_Send_Communication_Status(canal);
}




void _2_Comm_Send_Communication_Status(enum enum_canal_communication canal)
{
	sprintf(str, "Multi_Fct @%c mess= %ld // erreurs= %ld\n",
			ADRESSE_CARTE_CHAR,
			Nb_Messages_recus,
			Nb_Erreurs_com);

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

void _2_Comm_Send_Log_Message(char* str, enum Logger_Debug_Color color, byte Channel, enum enum_canal_communication canal)
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

	log_message.Channel = Channel;

	uint16_t stringlength;
	//Calcul la longueur de la chaine
	stringlength = strlen(str);

	if(stringlength < sizeof(log_message.Text))
	{
		log_message.Nombre_Carateres = COPYSTRING(str, log_message.Text);

		trame_echange.Length = COPYDATA(log_message, trame_echange.Data);
		trame_echange.XBEE_DEST_ADDR = XBee_PC;

		_1_Communication_Create_Trame(&trame_echange, canal);

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

			_1_Communication_Create_Trame(&trame_echange, canal);

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
 ** Function name:		_2_Comm_Send_Infos
 **
 ** Descriptions:		Fonction d'envoi des infos de la cartes vers la carte IA/PC
 **
 ** parameters:			Pointeur vers les infos à envoyer
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Infos(struct Com_Reponse_Info *Infos, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = REPONSE_INFO_Multi_FCT;
	trame_echange.Slave_Adresse = IA_BOARD;

	trame_echange.Length = COPYDATA(*Infos, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = XBee_PC;

	_1_Communication_Create_Trame(&trame_echange, canal);
}



/*****************************************************************************
 ** Function name:		_2_Comm_Send_Graph
 **
 ** Descriptions:		Fonction d'envoi de datas pour Graphiques
 **
 ** parameters:			Pointeur vers les datas à envoyer
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Graph(struct st_Graph_Datas* Datas, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if (_1_Communication_Wait_To_Send(ms_to_tick(5)) == pdFAIL)
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = GRAPHIQUES_ADD_DATA;
	trame_echange.Slave_Adresse = PC;

	trame_echange.Length = COPYDATA(*Datas, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = XBee_PC;

	_1_Communication_Create_Trame(&trame_echange, canal);
}



/*****************************************************************************
 ** Function name:		_2_Comm_Send_ACKNOWLEDGE
 **
 ** Descriptions:		Fonction de renvoi d'un ACK suite à reception d'un ordre
 **
 ** parameters:			Type d'ACK à envoyer
 ** 					canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_ACKNOWLEDGE(enum enum_ACK_Types ACK_TYPE, enum enum_canal_communication canal)
{
	struct Communication_ACK ACK;

	ACK.Adresse = ADRESSE_CARTE;		//Adresse de la carte envoyant l'ACK
	ACK.ACK_TYPE = ACK_TYPE;	//Quel a été le type de message reçu auquel répondre

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if (_1_Communication_Wait_To_Send(ms_to_tick(5)) == pdFAIL)
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = ACKNOWLEDGE;
	trame_echange.Slave_Adresse = IA_BOARD;

	trame_echange.Length = COPYDATA(ACK, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = XBee_PC;

	_1_Communication_Create_Trame(&trame_echange, canal);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Send_ASTAR_Vectors
 **
 ** Descriptions:		Fonction d'envoie du status des noeuds du ASTAR
 **
 ** parameters:			Pointeur vers la carte des vecteurs
 **						Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_ASTAR_Vectors(struct Astar_smoothing_vector_multiFCT* vectors, enum enum_canal_communication canal)
{
	struct st_ASTAR_Vecteur loc_vector;
	struct st_ASTAR_VECTEURS Vectors_to_Send;

	//Commence par demander un effacement des vecteurs déjà présents
	Vectors_to_Send.Effacement = 1;

	//Pour chacun des vecteurs dans la map
	byte index_vecteur_to_send = 0;
	for (int index_vecteur = 0; index_vecteur < vectors->Nb_Vectors; index_vecteur++)
	{
		loc_vector.Color = vectors->Vectors[index_vecteur].Color;
		loc_vector.Start_X = vectors->Vectors[index_vecteur].Start_Point.x;
		loc_vector.Start_Y = vectors->Vectors[index_vecteur].Start_Point.y;
		loc_vector.End_X = vectors->Vectors[index_vecteur].End_Point.x;
		loc_vector.End_Y = vectors->Vectors[index_vecteur].End_Point.y;

		Vectors_to_Send.Vecteurs[index_vecteur_to_send++] = loc_vector;

		if (index_vecteur_to_send == NB_ASTAR_Vecteur_Par_Message)
		{
			//Il est temps d'envoyer ce paquet
			Vectors_to_Send.Nb_vecteurs = index_vecteur_to_send;
			if (_1_Communication_Wait_To_Send(ms_to_tick(5)) == pdFAIL)
			{
				//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
				//Abandon
				return;
			}

			trame_echange.Instruction = ASTAR_VECTEURS;
			trame_echange.Slave_Adresse = PC;
			trame_echange.XBEE_DEST_ADDR = XBee_PC;

			trame_echange.Length = COPYDATA(Vectors_to_Send, trame_echange.Data);

			//Envoi sans attente d'ACK
			_1_Communication_Create_Trame(&trame_echange, canal);

			Task_Delay(2);

			//Pour les prochains messages, il n'est plus utile de demander un effacement
			Vectors_to_Send.Effacement = 0;
			index_vecteur_to_send = 0; //On recommence au début du tableau des infos à envoyer
		}
	}

	if(index_vecteur_to_send != 0)
	{
		//Dernier paquet
		Vectors_to_Send.Nb_vecteurs = index_vecteur_to_send;
		if (_1_Communication_Wait_To_Send(ms_to_tick(5)) == pdFAIL)
		{
			//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
			//Abandon
			return;
		}

		trame_echange.Instruction = ASTAR_VECTEURS;
		trame_echange.Slave_Adresse = PC;
		trame_echange.XBEE_DEST_ADDR = XBee_PC;

		trame_echange.Length = COPYDATA(Vectors_to_Send, trame_echange.Data);

		//Envoi sans attente d'ACK
		_1_Communication_Create_Trame(&trame_echange, canal);
	}
}

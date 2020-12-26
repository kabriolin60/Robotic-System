/*
 * 2_Echange_Datas.c
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#include "2_Echange_Datas.h"
#include "1_Trame_Communication.h"
#include "stdio.h"
#include "semphr.h"

#include "0_ADC.h"

#include "0_Infos.h"
#include "0_Event_Group.h"

#include "Strategie.h"
#include "0_Deplacements.h"

extern EventGroupHandle_t _0_Deplacement_EventGroup;


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
 ** Function name:		_2_Comm_Send_Destination_Robot
 **
 ** Descriptions:		Fonction d'envoie d'une la demande d'infos
 **
 ** parameters:			Destination,
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Destination_Robot(struct st_DESTINATION_ROBOT* destination, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DESTINATION_ROBOT;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(*destination, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo,
			pdTRUE, //Wait for ACK
			ACK_DEPLACEMENT, //Type d'ACK attendu
			eGROUP_STATUS_CARTE_MultiFCT_1); //Cartes devant renvoyer un ACK

	static char str[80];
	static char* dir;


	switch(destination->coord.Type_Deplacement)
	{
	case TYPE_MOVE_xy_tour_av_avant:
		dir = "AV";
		sprintf(str, "Dest= %s From X= %d, Y= %d to X= %d, Y= %d\n",
				dir,
				(short)_0_Get_Robot_Position().Position_X,
				(short)_0_Get_Robot_Position().Position_Y,
				(short)destination->coord.X,
				(short)destination->coord.Y);
		break;

	case TYPE_MOVE_xy_tour_av_arriere:
		dir = "AR";
		sprintf(str, "Dest= %s From X= %d, Y= %d to X= %d, Y= %d\n",
				dir,
				(short)_0_Get_Robot_Position().Position_X,
				(short)_0_Get_Robot_Position().Position_Y,
				(short)destination->coord.X,
				(short)destination->coord.Y);
		break;

	case TYPE_MOVE_consigne_vitesse_independantes:
		dir = "VITESSE";
		sprintf(str, "Dest= %s Gauche= %d, Droite= %d\n",
				dir,
				(short)destination->coord.Vitesse_Roue_Gauche,
				(short)destination->coord.Vitesse_Roue_Droite);
		break;

	case TYPE_MOVE_aucun_mouvement:
		dir = "AUCUN MOUVEMENT";
		sprintf(str, "Dest= %s\n",
				dir);
		break;

	case TYPE_MOVE_tourne_vers_cap_rad:
		dir = "Tourne cap";
		sprintf(str, "Dest= %s Theta %d\n",
				dir,
				(short)destination->coord.Angle);
		break;

	default:
		break;
	}

	_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Deplacement, LOG_Debug_Port);
}

/*****************************************************************************
 ** Function name:		_2_Comm_Send_Destination_Robot
 **
 ** Descriptions:		Fonction d'envoie d'une la demande d'infos
 **
 ** parameters:			Destination,
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Destination_Spline_CubiqueRobot(struct CubicSpline* destination, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DESTINATION_ROBOT_SPLINE_CUBIQUE;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(*destination, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo,
			pdTRUE, //Wait for ACK
			ACK_DEPLACEMENT, //Type d'ACK attendu
			eGROUP_STATUS_CARTE_MultiFCT_1); //Cartes devant renvoyer un ACK

	static char str[80];

	sprintf(str, "Dest SPLINE: From X= %d, Y= %d to X= %d, Y= %d\n",
			(short)_0_Get_Robot_Position().Position_X,
			(short)_0_Get_Robot_Position().Position_Y,
			(short)destination->P1.X,
			(short)destination->P1.Y);

	_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Deplacement, LOG_Debug_Port);
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
	trame_echange.Instruction = DEMANDE_INFO;
	trame_echange.Slave_Adresse = adresse_cible;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi sans attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);
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

	/*
	 * Avant d'envoyer un PING
	 * Efface le bit de sa presence sur le bus à l'interieur de l'Event_Group
	 * Il sera remis à 1 lors de la reception du PONG
	 */
	xEventGroupClearBits(_0_Status_EventGroup,    /* The event group being updated. */
			(1 << adresse_cible));		 /* The bits being set. */

	//Envoi sans attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);
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
byte release_sent = 0;
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

	//Envoi sans attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);

	//Send the revision of this board firmware
	if(!release_sent)
	{
		sprintf(str, "IA release= %s.%s; %s; %s\n",
				MAJOR_RELEASE,
				MINOR_RELEASE,
				__DATE__,
				__TIME__);
		_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Divers, LOG_Debug_Port);
		release_sent = 1;
	}
	_2_Comm_Send_Communication_Status(canal);
}


void _2_Comm_Send_Communication_Status(enum enum_canal_communication canal)
{
	sprintf(str, "IA mess= %ld // erreurs= %ld\n",
			Nb_Messages_recus,
			Nb_Erreurs_com);

	if(Nb_Erreurs_com < 5)
	{
		_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Divers, LOG_Debug_Port);
	}else
	{
		_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Divers, LOG_Debug_Port);
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

void _2_Comm_Send_Log_Message(char* str, enum Logger_Debug_Color color, byte Channel, enum enum_canal_communication canal)
{
	struct Logger_Debug_Data log_message;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = LOGGER_DEBUG;
	trame_echange.Slave_Adresse = PC;
	trame_echange.XBEE_DEST_ADDR = XBee_PC;


	log_message.Color = color;

	log_message.Channel = Channel;

	uint16_t stringlength;
	//Calcul la longueur de la chaine
	stringlength = strlen(str);

	if(stringlength < sizeof(log_message.Text))
	{
		log_message.Nombre_Carateres = COPYSTRING(str, log_message.Text);

		trame_echange.Length = COPYDATA(log_message, trame_echange.Data);

		//Envoi sans attente d'ACK
		_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);

#ifdef ENREGISTREMENT_FLASH
		//Renvoie le message pour etre enregistre dans la Flash
		xQueueSend(QueueEnregistrementMessages, &Com_Position_Robot, 0);
#endif

	}else
	{
		//Plus d'une trame à envoyer pour tout le texte
		while(stringlength)
		{
			log_message.Nombre_Carateres = COPYSTRING(str, log_message.Text);

			trame_echange.Length = COPYDATA(log_message, trame_echange.Data);

			//Envoi sans attente d'ACK
			_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);

#ifdef ENREGISTREMENT_FLASH
			//Renvoie le message pour etre enregistre dans la Flash
			xQueueSend(QueueEnregistrementMessages, &Com_Position_Robot, 0);
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
const float delai_demande_info = 5.0f;
void _2_Communication_Boards_Status(void* pvParameters)
{
	Init_Timing_Tache;

	Task_Delay_Until(100);

	/*
	 * Par défault, la carte IA est présente
	 */
	xEventGroupSetBits(_0_Status_EventGroup,    /* The event group being updated. */
			eGROUP_STATUS_CARTE_IA);		 /* The bits being set. */

	/*
	 * Ping chaque carte pour vérifier et mettre à jour sa présence sur le bus
	 */
	_2_Comm_Check_Presence_Cartes(RS485_port);

	Task_Delay_Until(100);


	int boucle = 0;
	while(true)
	{
		/*
		 * Demande les infos de chaque carte attendue sur le bus
		 */
		for(int i = 0; i < 4; i++)
		{
			_2_Comm_Send_Demande_Info(i+1, RS485_port);
			Task_Delay_Until(delai_demande_info);
		}

		if(boucle % 50 == 0)
		{
			/*
			 * Envoi l'état de la carte IA
			 */
			_2_Comm_Send_Info_Carte_IA(LOG_Debug_Port);
		}

		if(boucle % 10 == 0)
		{
			/*
			 * Envoi de la position de ce Robot à l'autre Robot
			 */
			_2_Comm_Envoi_Position_Autre_Robot(_0_Get_Robot_Position_Communication(), Xbee_port, _Strategie_Get_Robot_ID() == 0 ? Xbee_address_Petit_Robot : Xbee_address_Gros_Robot);
		}

		//Demande les infos du PC s'il a des trucs à envoyer
		_2_Comm_Send_Demande_Info(PC, LOG_Debug_Port);
		Task_Delay_Until(delai_demande_info);


		if(boucle % 20 == 0 && LOG_Debug_Port == Xbee_port)
		{
			//Si la communication de Debug se fait par Xbee
			_2_Comm_Send_Infos(_0_Get_Ptr_Card(1), LOG_Debug_Port);

			Task_Delay_Until(delai_demande_info);
		}

		boucle++;
		if(boucle == 400)
		{
			/*
			 * Ping chaque carte pour vérifier et mettre à jour sa présence sur le bus
			 */
			_2_Comm_Check_Presence_Cartes(RS485_port);
			boucle = 0;
		}
	}
}


/*****************************************************************************
 ** Function name:		_2_Comm_Check_Presence_Cartes
 **
 ** Descriptions:		Fonction qui PING chaque carte puis compare le résultat à l'attendu
 **
 ** parameters:			Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Check_Presence_Cartes(enum enum_canal_communication canal)
{
	_2_Comm_Send_PONG(canal); //pongs suivants sans la version de la carte IA
	Task_Delay(1.0f); //Pas besoin d'attendre longtemps entre 2 pings

	for(int i = 0; i < 4; i++)
	{
		/*
		 * Chaque bit de présence des cartes sera effacé avant l'envoi du PING, et remonté à Réception du PONG
		 */
		_2_Comm_Send_PING(i+1, canal);
		Task_Delay(4.0f); //Pas besoin d'attendre longtemps entre 2 pings
	}



	long board_status = xEventGroupGetBits(_0_Status_EventGroup);    /* The event group being updated. */

	if(board_status != (eGROUP_STATUS_CARTES_ATTENDUES | eGROUP_STATUS_CARTE_IA ))
	{
		_Strategie_Set_External_LED_RED_Status(TRUE);
	}else
	{
		_Strategie_Set_External_LED_RED_Status(FALSE);
	}

}



/*****************************************************************************
 ** Function name:		_2_Comm_Send_Servos_Destinations
 **
 ** Descriptions:		Fonction d'envoie d'une demande de deplacement d'un ou plusieurs servos
 **
 ** parameters:			Destination,
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Servos_Destinations(struct st_Destination_Servos* destination, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DESTINATION_SERVOS_AND_AX12;
	trame_echange.Slave_Adresse = ALL_CARDS;

	trame_echange.Length = COPYDATA(*destination, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdTRUE, ACK_MOVE_SERVOS, eGROUP_STATUS_CARTES_SERVO_ATTENDUES);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Robot_ID
 **
 ** Descriptions:		Fonction d'envoie d'une réponse à un ping
 **
 ** parameters:			ID du robot
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Robot_ID(byte ID, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DEFINITION_ID_ROBOT;
	trame_echange.Slave_Adresse = ALL_CARDS;

	trame_echange.Length = 1;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	trame_echange.Data[0] = ID;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdTRUE, ACK_DEFINITION_ID_ROBOT, eGROUP_STATUS_CARTES_ATTENDUES);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Demande_Simulation
 **
 ** Descriptions:		Fonction d'envoie d'un passage en simulation ou non
 **
 ** parameters:			simulation?
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Demande_Simulation(bool sim, enum enum_canal_communication canal)
{
	struct Simulation_Deplacement simulation;
	simulation.simulation = sim;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	if(sim)
	{
		xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
				eGROUP_DEPLA_SIMULATION);/* The bits being set. */
	}else
	{
		xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
				eGROUP_DEPLA_SIMULATION);/* The bits being set. */
	}

	trame_echange.Instruction = DEMANDE_SIMULATION_MOTEURS;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(simulation, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdTRUE, ACK_DEMANDE_SIMULATION_MOTEURS, eGROUP_STATUS_CARTE_MultiFCT_1);
}

/*****************************************************************************/


/*****************************************************************************
 ** Function name:		_2_Comm_Demande_Motor_Power
 **
 ** Descriptions:		Fonction d'envoie d'activation de la puissance des moteurs
 **
 ** parameters:			power?
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Demande_Motor_Power(bool power, enum enum_canal_communication canal)
{
	struct Motor_Power pow;
	pow.power_Gauche = power;
	pow.power_Droite = power;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DEMANDE_MOTEURS_POWER;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(pow, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdTRUE, ACK_POWER_MOTOR, eGROUP_STATUS_CARTE_MultiFCT_1);
}

/*****************************************************************************/


/*****************************************************************************
 ** Function name:		_2_Comm_Set_Robot_Position
 **
 ** Descriptions:		Fonction d'envoie d'activation de la puissance des moteurs
 **
 ** parameters:			X (mm)
 ** 					Y (mm)
 ** 					Orientation (degres)
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Set_Robot_Position(float X, float Y, float Angle, enum enum_canal_communication canal)
{
	/*
	 * Arrete le robot pour éviter de partir au mauvais endroit dans ses deplacements
	 * j'ausqu'au prochain deplacement
	 */
	ARRET_DEPLACEMENT();


	struct st_POSITION_ROBOT pos;
	pos.Position_X = X;
	pos.Position_Y = Y;
	pos.Angle_rad = Convert_Deg_Rad(Angle);

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = SET_ROBOT_POSITION;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(pos, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdTRUE, ACK_POSITION_ROBOT, eGROUP_STATUS_CARTE_MultiFCT_1);

	static char str[70];
	sprintf(str, "Set Robot position= X=%.1fmm Y=%.1fmm A=%.2f°\n",
			X,
			Y,
			Angle);
	_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Deplacement, LOG_Debug_Port);

	//delay pour s'assurer que l'info est bien partie à la carte et prise en compte
	Task_Delay(25);
}

/*****************************************************************************/



/*****************************************************************************
 ** Function name:		_2_Comm_Send_ASTAR_Contenu
 **
 ** Descriptions:		Fonction d'envoie du status des noeuds du ASTAR
 **
 ** parameters:			Pointeur vers la map
 **						Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/

#include "Astar.h"
void _2_Comm_Send_ASTAR_Contenu(struct Astar_Map* map, enum enum_canal_communication canal)
{
#if Astar_Diplay_Contenu == 0
	return;
#endif

	struct st_ASTAR_Data data_to_send;

	for(int x = 0; x < Astar_Node_Nb_X; x+=2)
	{
		if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
		{
			//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
			//Abandon
			return;
		}

		trame_echange.Instruction = ASTAR_CONTENU;
		trame_echange.Slave_Adresse = PC;
		trame_echange.XBEE_DEST_ADDR = XBee_PC;

		data_to_send.line_id = x;
		//Pour chaque ligne
		//1ère ligne
		for(int y = 0; y < Astar_Node_Nb_Y; y+=2)
		{
			//Pour chaque noeud de la ligne
			data_to_send.Node_Data[y/2] = (map->Nodes[x][y].Astar_Node_Access) | (map->Nodes[x][y+1].Astar_Node_Access << 4);
		}

		//2ème ligne
		for(int y = 0; y < Astar_Node_Nb_Y; y+=2)
		{
			//Pour chaque noeud de la ligne
			data_to_send.Node_Data[y/2 + Astar_Node_Nb_Y/2] = (map->Nodes[x+1][y].Astar_Node_Access) | (map->Nodes[x+1][y+1].Astar_Node_Access << 4);
		}

		trame_echange.Length = COPYDATA(data_to_send, trame_echange.Data);

		//Envoi sans attente d'ACK
		_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);

		Task_Delay(2);
	}
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
void _2_Comm_Send_ASTAR_Vectors(struct Astar_smoothing_vector* vectors, enum enum_canal_communication canal)
{
#if Astar_Display_Vectors == 0
	return;
#endif

	struct st_ASTAR_Vecteur loc_vector;
	struct st_ASTAR_VECTEURS Vectors_to_Send;

	//Commence par demander un effacement des vecteurs déjà présents
	Vectors_to_Send.Effacement = 1 | (_Strategie_Get_Robot_ID() << 1);

	//Vectors_to_Send.Robot_ID = _Strategie_Get_Robot_ID();

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
			if (_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo) == pdFAIL)
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
			_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);

			Task_Delay(2);

			//Pour les prochains messages, il n'est plus utile de demander un effacement
			Vectors_to_Send.Effacement = 0 | (_Strategie_Get_Robot_ID() << 1);
			index_vecteur_to_send = 0; //On recommence au début du tableau des infos à envoyer
		}
	}

	if(index_vecteur_to_send != 0)
	{
		//Dernier paquet
		Vectors_to_Send.Nb_vecteurs = index_vecteur_to_send;
		if (_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo) == pdFAIL)
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
		_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);	}
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
void _2_Comm_Send_Robot_Speed(float Vitesse_avance, float Vitesse_Rotation, float Acceleration_Avance, float Decceleration_Avance, float Acceleration_Rotation, float Decceleration_Rotation, enum enum_canal_communication canal)
{
	struct reglage_speed speed;

	speed.Vitesse_Avance = Vitesse_avance*1000;
	speed.Accel_Avance = Acceleration_Avance*1000;
	speed.Deccel_Avance = Decceleration_Avance*1000;

	speed.Vitesse_Rotation = Vitesse_Rotation*1000;
	speed.Accel_Rotation = Acceleration_Rotation*1000;
	speed.Deccel_Rotation = Decceleration_Rotation*1000;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = VITESSE_ROBOT;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(speed, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdTRUE, ACK_VITESSE_ROBOT, eGROUP_STATUS_CARTE_MultiFCT_1);

	static char str[70];
	sprintf(str, "Speed= %.1fm/s %.1fm/s² %.1fm/s², %.1frad/s %.1frad/s² %.1frad/s²\n",
			Vitesse_avance,
			Acceleration_Avance,
			Decceleration_Avance,
			Vitesse_Rotation,
			Acceleration_Rotation,
			Decceleration_Rotation);
	_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Deplacement, LOG_Debug_Port);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Send_Infos
 **
 ** Descriptions:		Fonction d'envoi des infos d'une carte MultiFCT vers la carte PC
 **
 ** parameters:			Pointeur vers les infos à envoyer
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Infos(struct Com_Reponse_Info *Infos, enum enum_canal_communication canal)
{
	if(Infos->Numero_Carte == 0)
		return;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = REPONSE_INFO_Multi_FCT;
	trame_echange.Slave_Adresse = PC;

	trame_echange.Length = COPYDATA(*Infos, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = XBee_PC;

	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);
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

	Infos.Strategie = _Strategie_Get_Choosen_Strategie();

	Infos.Temps_Match = (unsigned short)(_Strategie_Get_Temps_Match() / 10);                             //Temps /10		//2 octets

	//0= Jack; 1 = Color; 2 = Switchs; 3 = LED Red; 4 = LED Yellow; 5 = LED Green; 6 = User_BP
	Infos.Etat_Inputs = _Strategie_Get_Jack_Status();
	Infos.Etat_Inputs |= _Strategie_Get_Color_Status() << 1;
	Infos.Etat_Inputs |= _Strategie_Get_Switch_Status() << 2;
	Infos.Etat_Inputs |= _Strategie_Get_External_LED_RED_Status() << 3;
	Infos.Etat_Inputs |= _Strategie_Get_External_LED_YELLOW_Status() << 4;
	Infos.Etat_Inputs |= _Strategie_Get_External_LED_GREEN_Status() << 5;
	Infos.Etat_Inputs |= _Strategie_Get_User_BP_Status() << 6;


	float tension_Batterie;
	tension_Batterie = _0_ADC_Read_Channel(5); //Lecture de la valeur analogique lue sur le selecteur de stratégie

	tension_Batterie *= 0.00609F;
	Infos.Tension_Batterie = tension_Batterie * 100;

	//Envoi l'etat de présence des cartes sur le bus issues du dernier PING
	Infos.Boards_Comm_Status = xEventGroupGetBits( _0_Status_EventGroup );


	trame_echange.Instruction = REPONSE_INFO_IA;
	trame_echange.Slave_Adresse = PC;

	trame_echange.Length = COPYDATA(Infos, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = XBee_PC;

	//Envoi sans attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);
}



/*****************************************************************************
 ** Function name:		_2_Comm_Send_Robot_PID
 **
 ** Descriptions:		Fonction d'envoi de Parametres PID
 **
 ** parameters:			Set de parametres
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Robot_PID(enum PID_Id ID, float P, float I, float D, byte Min_Max, short Cumul, byte Sommation, byte Enable, enum enum_canal_communication canal)
{
	struct reglage_pid pid;
	pid.P = P;
	pid.I = I;
	pid.D = D;
	pid.Min_Max = Min_Max;
	pid.Cumul = Cumul;
	pid.Sommation = Sommation;
	pid.Enable = Enable;
	pid.id = ID;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = PARAMETRES_PID;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(pid, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdTRUE, ACK_PARAMETRES_PID, eGROUP_STATUS_CARTE_MultiFCT_1);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Envoi_Position_Autre_Robot
 **
 ** Descriptions:		Fonction d'envoi de la position au second Robot
 **
 ** parameters:			ID du Robot emetteur
 ** 					ptr sur la position à envoyer
 ** 					Canal de communication
 ** 					@ de l'autre Robot
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Envoi_Position_Autre_Robot(struct Com_Position_Robot PositionRobot, enum enum_canal_communication canal, byte adresse_Recepteur)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = POSITION_AUTRE_ROBOT;
	trame_echange.Slave_Adresse = 0; //La carte IA de l'autre Robot est destinataire

	trame_echange.Length = COPYDATA(PositionRobot, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = adresse_Recepteur;

	//Envoi sans attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_TxTrameDispo, pdFALSE, 0, 0);
}

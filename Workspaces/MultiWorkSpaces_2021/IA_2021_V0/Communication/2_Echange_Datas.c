/*
 * 2_Echange_Datas.c
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#include "2_Echange_Datas.h"
#include "1_Trame_Communication.h"
#include "stdio.h"

#include "0_Infos.h"

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
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DESTINATION_ROBOT;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(*destination, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);

	static char str[70];
	static char* dir;

	if(destination->coord.Type_Deplacement == xy_tour_av_avant)
	{
		dir = "AV";
	}else if(destination->coord.Type_Deplacement == xy_tour_av_arriere)
	{
		dir = "AR";
	}
	sprintf(str, "Dest= %s From X= %d, Y= %d to X= %d, Y= %d\n",
			dir,
			(short)_0_Get_Robot_Position().Position_X,
			(short)_0_Get_Robot_Position().Position_Y,
			(short)destination->coord.X,
			(short)destination->coord.Y);
	_2_Comm_Send_Log_Message(str, Color_Black, Channel_Debug_Deplacement, RS485_port);
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
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DEMANDE_INFO;
	trame_echange.Slave_Adresse = adresse_cible;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);
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
byte relese_sent = 0;
extern long Nb_Messages_recus;
extern long Nb_Erreurs_com;
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

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);

	//Send the revision of this board firmware

	if(!relese_sent)
	{
		sprintf(str, "IA release= %s.%s; %s; %s\n",
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
	sprintf(str, "IA mess= %ld // erreurs= %ld\n",
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
 ** Function name:		_2_Comm_Send_Robot_Position
 **
 ** Descriptions:		Fonction d'envoie de la position d'un Robot
 **
 ** parameters:			Struct st_POSITION_ROBOT
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
static struct Com_Position_Robot Com_Position_Robot;

void _2_Comm_Send_Robot_Position(struct st_POSITION_ROBOT rob_pos, enum enum_canal_communication canal)
{
	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = REPONSE_ROBOT_POSITION;
	trame_echange.Slave_Adresse = ALL_CARDS;

	//Position du robot
	Com_Position_Robot.Position_X = (short)(rob_pos.Position_X * 10);
	Com_Position_Robot.Position_Y = (short)(rob_pos.Position_Y * 10);
	Com_Position_Robot.Angle = (short)(rob_pos.Angle_Deg * 100);

	trame_echange.Length = COPYDATA(Com_Position_Robot, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);

#ifdef TYPE_CARTE_IA
#ifdef ENREGISTREMENT_FLASH
	//Renvoie le message pour etre enregistre dans la Flash
	xQueueSend(QueueEnregistrementMessages, &Com_Position_Robot, 0);
#endif
#endif
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
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DESTINATION_SERVOS_AND_AX12;
	trame_echange.Slave_Adresse = ALL_CARDS;

	trame_echange.Length = COPYDATA(*destination, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);
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
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
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

	_1_Communication_Create_Trame(&trame_echange, canal);
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
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DEMANDE_SIMULATION_MOTEURS;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(simulation, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);
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
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = DEMANDE_MOTEURS_POWER;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(pow, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);
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
	struct st_POSITION_ROBOT pos;
	pos.Position_X = X;
	pos.Position_Y = Y;
	pos.Angle_rad = Convert_Deg_Rad(Angle);

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = SET_ROBOT_POSITION;
	trame_echange.Slave_Adresse = 1;

	trame_echange.Length = COPYDATA(pos, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal);

	static char str[70];
	sprintf(str, "Set Robot position= X%.1fmm Y%.1fmm A%.2f°\n",
			X,
			Y,
			Angle);
	_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Deplacement, RS485_port);
}

/*****************************************************************************/


/*****************************************************************************
 ** Function name:		_2_Comm_Envoi_Fin_Communication
 **
 ** Descriptions:		Fonction d'envoie de la fin d'une communication
 **
 ** parameters:			none
 ** Returned value:		None
 **
 *****************************************************************************/
struct Communication_Message* _2_Comm_Envoi_Fin_Communication(void)
{
	trame_echange.Instruction = END_COMMUNICATION;
	trame_echange.Slave_Adresse = ALL_CARDS;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	return _1_Communication_Create_Message(&trame_echange);
}



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
	struct st_ASTAR_Data data_to_send;

	for(int x = 0; x < Astar_Node_Nb_X; x+=2)
	{
		if(_1_Communication_Wait_To_Send(ms_to_tick(5))== pdFAIL )
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
		_1_Communication_Create_Trame(&trame_echange, canal);

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
			//Il est temps d'envoyer ce paquer

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
			_1_Communication_Create_Trame(&trame_echange, canal);

			Task_Delay(2);


			//Pour les prochains messages, il n'est plus utile de demander un effacement
			Vectors_to_Send.Effacement = 0;
			index_vecteur_to_send = 0; //On recommence au début du tableau des infos à envoyer

		}
	}

}
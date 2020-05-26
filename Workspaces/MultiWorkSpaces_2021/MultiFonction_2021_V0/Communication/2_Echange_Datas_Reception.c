/*
 * 2_Echange_Datas_Reception.c
 *
 *  Created on: 25 avr. 2020
 *      Author: kabri
 */

#include <Init.h>
#include "2_Echange_Datas_Reception.h"
#include "0_Communication.h"
#include "1_Trame_Communication.h"
#include "1_Odometrie.h"
#include "Type_Declaration.h"

#include "2_Echange_Datas.h"
#include "1_Update_Values.h"

#include "1_Asservissement.h"
#include "2_Asservissement.h"
#include "1_PID.h"

extern QueueHandle_t _1_xQueue_Message_Receive; 				//Queue Recevant les messages des canaux de communication
static long Nb_Messages_Interpretes = 0;

/*****************************************************************************
 ** Function name:		_2_Communication_RX_Init
 **
 ** Descriptions:		Init de la comm de reception niveau 2
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_RX_Init()
{
	//Tache de décodage des donnees messages reçus
	xTaskCreate(_2_Communication_RX_Lectures_Messages, (char *) "1_Com_Lecture_RX", 100, _1_xQueue_Message_Receive, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
}


/*****************************************************************************
 ** Function name:		_2_Communication_RX_Lectures_Messages
 **
 ** Descriptions:		Lecture des messages en reception
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
static TO_AHBS_RAM3 struct Communication_Trame received_trame;

struct Com_Position_Robot Com_Position_Robot;
__attribute__((optimize("O3"))) void _2_Communication_RX_Lectures_Messages(void *pvParameters)
{
	if(pvParameters == NULL)
		Task_Delete_Current;

	for(;;)
	{
		//Reception d'un message depuis une Queue
		if(xQueueReceive(pvParameters, &received_trame, portMAX_DELAY))
		{
			Debug_Trace_Texte("Debut_Interpretation");

			Nb_Messages_Interpretes++;

			switch(received_trame.Instruction)
			{
			case DESTINATION_ROBOT:
				_2_Communication_RX_Destination_Robot(&received_trame);
				break;


			case DEMANDE_ROBOT_POSITION:
#ifdef TYPE_CARTE_MULTIFCT
				_2_Comm_Send_Robot_Position(_1_Odometrie_GetRobot_Position(), RS485_port);
#endif
				break;
					
				case DESTINATION_SERVOS_AND_AX12:
#ifdef TYPE_CARTE_MULTIFCT
				_2_Comm_RX_Destination_Servos(&received_trame);
#endif
				break;

			case DEMANDE_INFO:
				//Mise en pile des infos à envoyer
				_2_Comm_Send_Infos(Get_ptr_Reponse_info(), RS485_port);
				//Cette commande déclanche en même temps l'autorisation d'envoyer les messages contenus dans la pile
				_0_Communication_Give_Sending_Clearance();
				break;

			case PARAMETRES_PID:
				_2_Communication_RX_Parametres_PID(&received_trame);
				break;

			case PING:
				//A la demande d'une carte, on répond par un PONG
				_2_Comm_Send_PONG(RS485_port);
				break;	
					
			default:
				break;
			}

			Debug_Trace_Texte("Fin_Interpretation");
		}
	}
}


/*****************************************************************************
 ** Function name:		_2_Communication_RX_Destination_Robot
 **
 ** Descriptions:		Receive a Robot destination
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_RX_Destination_Robot(struct Communication_Trame* datas)
{
	struct st_DESTINATION_ROBOT dest;

	COPYDATA2(datas->Data, dest);

	//If requested to flush the current destination Buffer (in case of obstacle avoidance, or emergency breaking, or destination changement)
	if(dest.Replace)
		_2_Asservissement_DestinationBuffer_Clear();

	//Add this new destination in the buffer
	_2_Deplacement_Ajout_Point(&dest.coord);

	if(dest.Replace)
		_2_Asservissement_Read_Next_Desti_Point_extern();
}



/*****************************************************************************
 ** Function name:		_2_Communication_RX_Parametres_PID
 **
 ** Descriptions:		Receive a PID set of parameters
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_RX_Parametres_PID(struct Communication_Trame* datas)
{
	struct reglage_pid pid;

	COPYDATA2(datas->Data, pid);

	struct st_pid_filter* ptr_PID;

	switch(pid.id)
	{
	case vitesse_roues_independantes:
		ptr_PID = _1_Get_prt_PID_Vit_Droite();
		ptr_PID->Enable = pid.Enable;
		ptr_PID->gain_P = pid.P;
		ptr_PID->gain_I = pid.I;
		ptr_PID->gain_D = pid.D;
		ptr_PID->Max_Erreur_Cumul = pid.Cumul;
		ptr_PID->commande_max = pid.Min_Max;
		ptr_PID->commande_min = -pid.Min_Max;
		ptr_PID->Sommation_Sortie = pid.Sommation;

		ptr_PID = _1_Get_prt_PID_Vit_Gauche();
		ptr_PID->Enable = pid.Enable;
		ptr_PID->gain_P = pid.P;
		ptr_PID->gain_I = pid.I;
		ptr_PID->gain_D = pid.D;
		ptr_PID->Max_Erreur_Cumul = pid.Cumul;
		ptr_PID->commande_max = pid.Min_Max;
		ptr_PID->commande_min = -pid.Min_Max;
		ptr_PID->Sommation_Sortie = pid.Sommation;
		break;

	case vitesse_distance:
		ptr_PID = _1_Asserv_GetPtr_PID_Vit_Pos();
		ptr_PID->Enable = pid.Enable;
		ptr_PID->gain_P = pid.P;
		ptr_PID->gain_I = pid.I;
		ptr_PID->gain_D = pid.D;
		ptr_PID->Max_Erreur_Cumul = pid.Cumul;
		ptr_PID->commande_max = pid.Min_Max;
		ptr_PID->commande_min = -pid.Min_Max;
		ptr_PID->Sommation_Sortie = pid.Sommation;
		break;

	case vitesse_orientation:
		ptr_PID = _1_Asserv_GetPtr_PID_Vit_Rot();
		ptr_PID->Enable = pid.Enable;
		ptr_PID->gain_P = pid.P;
		ptr_PID->gain_I = pid.I;
		ptr_PID->gain_D = pid.D;
		ptr_PID->Max_Erreur_Cumul = pid.Cumul;
		ptr_PID->commande_max = pid.Min_Max;
		ptr_PID->commande_min = -pid.Min_Max;
		ptr_PID->Sommation_Sortie = pid.Sommation;
		break;

	case distance:
		ptr_PID = _2_Asserv_GetPtr_PID_Pos();
		ptr_PID->Enable = pid.Enable;
		ptr_PID->gain_P = pid.P;
		ptr_PID->gain_I = pid.I;
		ptr_PID->gain_D = pid.D;
		ptr_PID->Max_Erreur_Cumul = pid.Cumul;
		ptr_PID->commande_max = pid.Min_Max;
		ptr_PID->commande_min = -pid.Min_Max;
		ptr_PID->Sommation_Sortie = pid.Sommation;
		break;

	case orientation:
		ptr_PID = _2_Asserv_GetPtr_PID_Rot();
		ptr_PID->Enable = pid.Enable;
		ptr_PID->gain_P = pid.P;
		ptr_PID->gain_I = pid.I;
		ptr_PID->gain_D = pid.D;
		ptr_PID->Max_Erreur_Cumul = pid.Cumul;
		ptr_PID->commande_max = pid.Min_Max;
		ptr_PID->commande_min = -pid.Min_Max;
		ptr_PID->Sommation_Sortie = pid.Sommation;
		break;

	default:
		break;
	}
}


/*****************************************************************************
 ** Function name:		_2_Comm_RX_Destination_Servos
 **
 ** Descriptions:		Receive a set a servos destination
 **						No matters, which board or servo ID						
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_Destination_Servos(struct Communication_Trame* datas)
{
	struct st_Destination_Servos destinations;	
	COPYDATA2(datas->Data, destinations);
	
	//For each servos asked
	for(int i = 0; i < destinations.Nombre_servos_to_move; i++)
	{
		//IDs x0 to x5 = servos
		//IDs x6 to x9 = AX12
		//ID = @carte*10 + Local_servo_id
		//Local_ID = global_ID % 10
		
		//Check if this servo is associated to this board
		if(destinations.servo[i].ID / 10 == ADRESSE_CARTE)
		{
			//Servo or AX12
			if(destinations.servo[i].ID % 10 <= 5)
			{
				//Servo
				_1_Servos_Set_Destination(destinations.servo[i].ID % 10, destinations.servo[i].Destination, destinations.Time_to_move);
			}else
			{
				//AX12
				_1_AX12_MOVE_WITH_TIME(destinations.servo[i].ID % 10, destinations.servo[i].Destination, destinations.servo[i].Torque, destinations.Time_to_move)
			}
		}
	}
}

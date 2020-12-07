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

#include "1_Servos.h"
#include "1_AX12.h"

#include "0_Motors.h"

static long Nb_Messages_Interpretes = 0;

static byte ID_Robot;	//Id du Robot sur lequel est monté cette carte

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

}


void _2_Communication_Interprete_message(struct Communication_Trame* trame)
{
	Debug_Trace_Texte("Debut_Interpretation");

	Nb_Messages_Interpretes++;

	switch(trame->Instruction)
	{
	case DESTINATION_ROBOT:
		_2_Communication_RX_Destination_Robot(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_DEPLACEMENT, RS485_port);
		break;


	case SET_ROBOT_POSITION:
		_2_Comm_RX_Set_Position_Robot(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_POSITION_ROBOT, RS485_port);
		break;


	case DESTINATION_SERVOS_AND_AX12:
		_2_Comm_RX_Destination_Servos(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_MOVE_SERVOS, RS485_port);
		break;


	case DEFINITION_ID_ROBOT:		//Permet à la carte ID de donner l'ID du robot sur les autres cartes
		_2_Comm_RX_Id_Robot(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_DEFINITION_ID_ROBOT, RS485_port);
		break;


	case DEMANDE_INFO:
		//Mise en pile des infos à envoyer
		_2_Comm_Send_Infos(Get_ptr_Reponse_info(), RS485_port);
		//Cette commande déclanche en même temps l'autorisation d'envoyer les messages contenus dans la pile
		_0_Communication_Give_Sending_Clearance();
		break;


	case DEMANDE_MOTEURS_POWER:
		_2_Comm_RX_Motor_Power(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_POWER_MOTOR, RS485_port);
		break;


	case DEMANDE_SIMULATION_MOTEURS:
		_2_Comm_RX_Simulation_Deplacement(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_DEMANDE_SIMULATION_MOTEURS, RS485_port);
		break;


	case PARAMETRES_ODOMETRIE:
		_2_Comm_RX_Odometrie(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_PARAMETRES_ODOMETRIE, RS485_port);
		break;

	case PARAMETRES_PID:
		_2_Communication_RX_Parametres_PID(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_PARAMETRES_PID, RS485_port);
		break;


	case VITESSE_ROBOT:
		_2_Communication_RX_Vitesse(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_VITESSE_ROBOT, RS485_port);
		break;


	case PING:
		//A la demande d'une carte, on répond par un PONG
		_2_Comm_Send_PONG(RS485_port);
		_0_Communication_Give_Sending_Clearance();
		break;


	case END_COMMUNICATION:
		_0_Communication_Give_Sending_Clearance();
		break;


	default:
		break;
	}
	Debug_Trace_Texte("Fin_Interpretation");
}


/*****************************************************************************
 ** Function name:		_2_Communication_RX_Destination_Robot
 **
 ** Descriptions:		Receive a Robot destination
 **
 ** parameters:			Received message
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


	_2_Deplacement_Ajout_Point(&dest.coord);

	if(dest.coord.Type_Deplacement == consigne_vitesse_independantes)
	{
		PID_update_Consign(_1_Get_prt_PID_Vit_Gauche(), dest.coord.Vitesse_Roue_Gauche);
		PID_update_Consign(_1_Get_prt_PID_Vit_Droite(), dest.coord.Vitesse_Roue_Droite);
	}


	//Add this new destination in the buffer
	if(dest.coord.Type_Deplacement != consigne_vitesse_independantes)
	{
		//Deplacement en coordonnées ou distance / angle
		//_2_Deplacement_Ajout_Point(&dest.coord);

		//Dans ce cas, force un deplacement en type vitesse
		/*struct st_ROBOT_PARAMETRES* newparameters;
		newparameters = _1_Odometrie_Get_Parameters();
		newparameters->_1_Odometrie_Type_Asserv = Polaire_Tourne_Avance_point_unique;
		_1_Odometrie_Set_Parameters(newparameters);*/

	}else
	{
		//Deplacement en vitesse uniquement
		//PID_update_Consign(_1_Get_prt_PID_Vit_Gauche(), dest.coord.Vitesse_Roue_Gauche);
		//PID_update_Consign(_1_Get_prt_PID_Vit_Droite(), dest.coord.Vitesse_Roue_Droite);

		//Dans ce cas, force un deplacement en type vitesse
		/*struct st_ROBOT_PARAMETRES* newparameters;
		newparameters = _1_Odometrie_Get_Parameters();
		newparameters->_1_Odometrie_Type_Asserv = Vitesse_Droite_Vitesse_Gauche_Indep;
		_1_Odometrie_Set_Parameters(newparameters);*/
	}

	if(dest.Replace)
		_2_Asservissement_Read_Next_Desti_Point_extern();
}



/*****************************************************************************
 ** Function name:		_2_Comm_RX_Odometrie
 **
 ** Descriptions:		Receive a set of parameter for odometrie
 **
 ** parameters:			Received message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_Odometrie(struct Communication_Trame* datas)
{
	struct st_ROBOT_PARAMETRES newparameters;

	COPYDATA2(datas->Data, newparameters);

	_1_Odometrie_Set_Parameters(&newparameters);
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
 ** Function name:		_2_Communication_RX_Vitesse
 **
 ** Descriptions:		Receive a speed set of parameters
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_RX_Vitesse(struct Communication_Trame* datas)
{
	struct reglage_speed speed;

	COPYDATA2(datas->Data, speed);

	//Get Robot asserv Param
	struct st_ROBOT_PARAMETRES* param = _1_Odometrie_Get_Parameters();

	//Convert to Robot_System
	//m/s*100 => pas/it
	/*float consigne_speed = ((float)speed.Vitesse_Avance)*10; //mm/s = /100*1000
	consigne_speed *= param->COEF_D; //pas/s
	consigne_speed /= 1000;	//pas/ms
	consigne_speed *= PERIODE_PID_VITESSE; //pas/IT*/
	float consigne_speed = ((float)speed.Vitesse_Avance)/100; //m/s = mm/ms
	consigne_speed *= PERIODE_PID_VITESSE; //mm/it
	consigne_speed *= param->COEF_D; //pas/it


	/*float consigne_accel = ((float)speed.Accel_Avance)*10; //mm/s² = /100*1000
	consigne_accel *= param->COEF_D; //pas/s²
	consigne_accel /= 1000;	//pas/ms²
	consigne_accel *= PERIODE_PID_VITESSE; //pas/IT²
	consigne_accel /= 1000;	//pas/ms²
	consigne_accel *= PERIODE_PID_VITESSE; //pas/IT²*/
	float consigne_accel = ((float)speed.Accel_Avance)/100; //mm/s²
	consigne_accel /= 1000; //mm/ms²
	consigne_accel *= PERIODE_PID_VITESSE;
	consigne_accel *= PERIODE_PID_VITESSE; //mm/it
	consigne_accel *= param->COEF_D; //pas/it

	/*float consigne_Deccel = ((float)speed.Deccel_Avance)*10; //mm/s² = /100*1000
	consigne_Deccel *= param->COEF_D; //pas/s²
	consigne_Deccel /= 1000;	//pas/ms²
	consigne_Deccel *= PERIODE_PID_VITESSE; //pas/IT²
	consigne_Deccel /= 1000;	//pas/ms²
	consigne_Deccel *= PERIODE_PID_VITESSE; //pas/IT²*/
	float consigne_Deccel = ((float)speed.Deccel_Avance)/100; //mm/s²
	consigne_Deccel /= 1000; //mm/ms²
	consigne_Deccel *= PERIODE_PID_VITESSE;
	consigne_Deccel *= PERIODE_PID_VITESSE; //mm/it
	consigne_Deccel *= param->COEF_D; //pas/it

	_2_Asservissement_Set_Distance_Speed_Accel(consigne_speed,consigne_accel, consigne_Deccel);




	/*float consigne_rotation = ((float)speed.Vitesse_Rotation)/100; //rad/s
	consigne_rotation *= param->COEF_ROT; //pas/s
	consigne_rotation /= 1000;	//pas/ms
	consigne_rotation *= PERIODE_PID_VITESSE; //pas/IT*/
	float consigne_rotation = ((float)speed.Vitesse_Rotation)/100; //rad/s
	consigne_rotation *= PERIODE_PID_VITESSE; //mrad/it
	consigne_rotation *= param->COEF_D; //pas/it

	/*float consigne_accel_rotation = ((float)speed.Accel_Rotation)/100; //rad/s² = /100*1000
	consigne_accel_rotation *= param->COEF_ROT; //pas/s²
	consigne_accel_rotation /= 1000;	//pas/ms²
	consigne_accel_rotation *= PERIODE_PID_VITESSE; //pas/IT²
	consigne_accel_rotation /= 1000;	//pas/ms²
	consigne_accel_rotation *= PERIODE_PID_VITESSE; //pas/IT²*/
	float consigne_accel_rotation = ((float)speed.Accel_Rotation)/100; //rad/s²
	consigne_accel_rotation /= 1000; //mrad/ms²
	consigne_accel_rotation *= PERIODE_PID_VITESSE;
	consigne_accel_rotation *= PERIODE_PID_VITESSE; //mrad/it
	consigne_accel_rotation *= param->COEF_ROT; //pas/it

	/*float consigne_Deccel_rotation = ((float)speed.Deccel_Rotation)/100; //ras/s² = /100*1000
	consigne_Deccel_rotation *= param->COEF_ROT; //pas/s²
	consigne_Deccel_rotation /= 1000;	//pas/ms²
	consigne_Deccel_rotation *= PERIODE_PID_VITESSE; //pas/IT²
	consigne_Deccel_rotation /= 1000;	//pas/ms²
	consigne_Deccel_rotation *= PERIODE_PID_VITESSE; //pas/IT²*/
	float consigne_Deccel_rotation = ((float)speed.Deccel_Rotation)/100; //rad/s²
	consigne_Deccel_rotation /= 1000; //mrad/ms²
	consigne_Deccel_rotation *= PERIODE_PID_VITESSE;
	consigne_Deccel_rotation *= PERIODE_PID_VITESSE; //mrad/it
	consigne_Deccel_rotation *= param->COEF_ROT; //pas/it

	_2_Asservissement_Set_Rotation_Speed_Accel(consigne_rotation, consigne_accel_rotation, consigne_Deccel_rotation);
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
				_1_AX12_MOVE_WITH_TIME(destinations.servo[i].ID % 10, destinations.servo[i].Destination, destinations.servo[i].Torque, destinations.Time_to_move);
			}
		}
	}
}


/*****************************************************************************
 ** Function name:		_2_Comm_RX_Id_Robot
 **
 ** Descriptions:		Receive Robot id
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_Id_Robot(struct Communication_Trame* datas)
{
	struct Com_Def_Id_Robot id;
	COPYDATA2(datas->Data, id);

	ID_Robot = id.Numero_Robot;
}


byte _2_Comm_Get_Robot_ID()
{
	return ID_Robot;
}


/*****************************************************************************
 ** Function name:		_2_Comm_RX_Simulation_Deplacement
 **
 ** Descriptions:		Receive Robot id
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_Simulation_Deplacement(struct Communication_Trame* datas)
{
	struct Simulation_Deplacement sim;

	COPYDATA2(datas->Data, sim);
	_1_Omodetrie_Set_Simulation(sim.simulation);
}


/*****************************************************************************
 ** Function name:		_2_Comm_RX_Motor_Power
 **
 ** Descriptions:		Activation de la puissance des moteurs
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_Motor_Power(struct Communication_Trame* datas)
{
	struct Motor_Power sim;

	COPYDATA2(datas->Data, sim);
	_0_Set_Motor_Power(0, sim.power_Gauche);
	_0_Set_Motor_Power(1, sim.power_Droite);
}


/*****************************************************************************
 ** Function name:		_2_Comm_RX_Set_Position_Robot
 **
 ** Descriptions:		Set la position du Robot (recallage ou depart)
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_Set_Position_Robot(struct Communication_Trame* datas)
{
	//Position initiale du Robot
	struct st_POSITION_ROBOT newposition;
	COPYDATA2(datas->Data, newposition);

	newposition.Position_X = newposition.Position_X;
	newposition.Position_Y = newposition.Position_Y;
	newposition.Angle_rad = newposition.Angle_rad;
	newposition.orient_init = newposition.orient_init;
	newposition.Angle_Deg = newposition.Angle_Deg;
	_1_Odometrie_SetRobot_Position(&newposition);
}

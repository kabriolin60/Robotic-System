/*
 * 0_Deplacements.c
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#include "FreeRTOS.h"
#include "event_groups.h"
#include "Init.h"
#include "task.h"

#include "0_Deplacements.h"
#include "0_Infos.h"

#include "Astar.h"

static TO_AHBS_RAM3 struct st_DESTINATION_ROBOT Current_destination; //= last sended destination

/* Event_Group de deplacement */
EventGroupHandle_t _0_Deplacement_EventGroup;

TaskHandle_t Astar_Task_Handler;


/*****************************************************************************
 ** Function name:		_0_Deplacement_Init
 **
 ** Descriptions:		Fonction d'initialisation de l'Event Group de deplacement
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Deplacement_Init(void)
{
	_0_Deplacement_EventGroup = xEventGroupCreate();
	vTraceSetEventGroupName( _0_Deplacement_EventGroup, "_0_Depla_EventGroup");
}




/*****************************************************************************
 ** Function name:		_0_Deplacement_Get_ptr_Current_Destination
 **
 ** Descriptions:		Get a pointer to the current destination (last sended)
 **
 ** parameters:			None
 ** Returned value:		pointer to the last sent destination
 **
 *****************************************************************************/
struct st_DESTINATION_ROBOT* _0_Deplacement_Get_ptr_Current_Destination(void)
{
	return &Current_destination;
}


/*****************************************************************************
 ** Function name:		Strategie_Get_Simulation
 **
 ** Descriptions:		Get a pointer to the current destination (last sended)
 **
 ** parameters:			None
 ** Returned value:		pointer to the last sent destination
 **
 *****************************************************************************/
byte _0_Deplacement_Get_Simulation(void)
{
	EventBits_t uxBits;

	uxBits = xEventGroupGetBits(_0_Deplacement_EventGroup);    /* The event group being updated. */
	uxBits = uxBits & eGROUP_DEPLA_SIMULATION;

	if(uxBits)
		return pdTRUE;

	return pdFALSE;
}



/*****************************************************************************
 ** Function name:		_0_Deplacement_Wait_For_Arrival
 **
 ** Descriptions:		Check if the Robot reach it's destination
 **
 ** parameters:			ptr to coord cible
 ** Returned value:		true: deplacement done successfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Wait_For_Arrival(struct st_COORDONNEES* coord)
{
	EventBits_t uxBits;

	while(Distance_To_Destination(_0_Get_Robot_Position(), coord) * 100 > coord->ptrParameters.Distance_Detection_Fin_Trajectoire * 3)
	{
		//Check if the "no path found" flag hab been rised
		//If no Pathfinding is used, Flag will keep low
		uxBits = xEventGroupWaitBits(_0_Deplacement_EventGroup,   /* The event group being tested. */
				eGROUP_DEPLA_path_NOT_FOUND, /* The bits within the event group to wait for. */
				pdFALSE,        /* Clear bits before returning. */
				pdTRUE,        /* Wait for ALL bits to be set */
				0 );/* Wait a maximum of xTicksToWait for either bit to be set. */

		if( ( uxBits & (eGROUP_DEPLA_path_NOT_FOUND ) ) == ( eGROUP_DEPLA_path_NOT_FOUND ) )
		{
			//No path found using the PathFinding

			//Set the Flag (no found | Not Arrived to Destination) before sending or waiting
			xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_NOT_ARRIVED );/* The bits being set. */

			//Stop deplacement
			//Return Error
			return pdFALSE;
		}

		//Vérifie si un bloquage est detecté
		if(_0_Get_Robot_Position().Bloquage_Deplacement)
		{
			//Set the Flag (eGROUP_DEPLA_BLOQUAGE) before sending or waiting
			xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_BLOQUAGE );/* The bits being set. */

			//Stop deplacement
			//Return Error
			return pdFALSE;
		}
		Task_Delay(20);
	}

	//We arrived at destination
	//Set the Flag (no found | Not Arrived to Destination) before sending or waiting
	xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
			eGROUP_DEPLA_ARRIVED );/* The bits being set. */

	//return success
	return pdTRUE;
}


float Distance_To_Destination(struct st_POSITION_ROBOT position, struct st_COORDONNEES* dest)
{
	short delta_x, delta_y;

	delta_x = dest->X - position.Position_X;
	delta_y = dest->Y - position.Position_Y;

	return sqrtf((delta_x*delta_x)+(delta_y*delta_y));
}



float Distance_Two_Points(short xa, short ya, short xb, short yb)
{
	short delta_x, delta_y;

	delta_x = xb - xa;
	delta_y = yb - ya;

	return sqrtf((delta_x*delta_x)+(delta_y*delta_y));
}


/*****************************************************************************
 ** Function name:		_0_Deplacement_Clear_Flags
 **
 ** Descriptions:		Efface les flags de deplacement avant d'en envoyer un nouveau
 **
 ** parameters:			None
 **
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Deplacement_Clear_Flags(void)
{
	//Clear the Flags (path found | not found) before sending or waiting
	xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
			eGROUP_DEPLA_path_NOT_FOUND | eGROUP_DEPLA_pathFOUND | eGROUP_DEPLA_NOT_ARRIVED | eGROUP_DEPLA_ARRIVED | eGROUP_DEPLA_BLOQUAGE);/* The bits being set. */
}


/*****************************************************************************
 ** Function name:		_0_Deplacement_Check_Bloquage
 **
 ** Descriptions:		EDetecte le flag de bloquage lors d'un deplacement
 **
 ** parameters:			None
 **
 ** Returned value:		true: bloquage detecte
 ** 					false: pas de bloquage detecte
 **
 *****************************************************************************/
bool _0_Deplacement_Check_Bloquage(void)
{
	char str[30];

	EventBits_t uxBits;
	//If no Pathfinding is used, Flag will keep low
	uxBits = xEventGroupWaitBits(_0_Deplacement_EventGroup,   /* The event group being tested. */
			eGROUP_DEPLA_BLOQUAGE, /* The bits within the event group to wait for. */
			pdFALSE,        /* Clear bits before returning. */
			pdTRUE,        /* Wait for ALL bits to be set */
			0 );/* Wait a maximum of xTicksToWait for either bit to be set. */
	//Si un bloquage a ete detecté
	if( ( uxBits & (eGROUP_DEPLA_BLOQUAGE ) ) == ( eGROUP_DEPLA_BLOQUAGE ) )
	{
		sprintf(str, "DEPLA: Bloquage detecte\n");
		_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Deplacement, LOG_Debug_Port);
		return pdTRUE;
	}

	return pdFALSE;
}


/*****************************************************************************
 ** Function name:		_0_Deplacement_Tourne_Avance
 **
 ** Descriptions:		Deplacement to X;Y with Rotation first, direction Forward
 **
 ** parameters:			X coord cible
 ** 					Y coord cible
 ** 					Remplacement (if needed to erase previous dest)
 ** 					Attente = Wait or not for the end of Deplacement
 ** 					Direction: 0 = forward
 ** 								1 = backward
 **
 ** Returned value:		true: deplacement done successfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Tourne_Avance(short X, short Y, bool remplacement, bool Attente, bool direction)
{
	struct st_DESTINATION_ROBOT dest = { 0 };
	struct st_COORDONNEES coord = { 0 };

	//Check if the new dest is != to the previous sent
	if(_0_Deplacement_Get_ptr_Current_Destination()->coord.X != X || _0_Deplacement_Get_ptr_Current_Destination()->coord.Y != Y)
	{
		coord.X = X;
		coord.Y = Y;
		if(direction == 0)
		{
			coord.Type_Deplacement = TYPE_MOVE_xy_tour_av_avant;
		}else
		{
			coord.Type_Deplacement = TYPE_MOVE_xy_tour_av_arriere;
		}

		//dest.Bloquage_Possible = Bloquage;

		//Parametres classiques pour deplacement classiques
		coord.ptrParameters.Angle_Avant_Debut_Avance = 0.08F * 100;
		coord.ptrParameters.Angle_Detection_Fin_Trajectoire = 0.05F * 100;
		coord.ptrParameters.Distance_Detection_Fin_Trajectoire = 2 * 100;

		coord.Type_Arret = depla_AVEC_freinage;

		dest.coord = coord;
		dest.Replace = remplacement;

		_0_Deplacement_Clear_Flags();


		_0_Deplacement_Get_ptr_Current_Destination()->coord = coord;
		_2_Comm_Send_Destination_Robot(&dest, RS485_port);
	}

	if(!Attente)
		return true;

	bool result = _0_Deplacement_Wait_For_Arrival(&coord);

	//Check bloquage
	_0_Deplacement_Check_Bloquage();


	return result;
}



/*****************************************************************************
 ** Function name:		_0_Deplacement_Tourne_Avance_ASTAR
 **
 ** Descriptions:		Deplacement to X;Y with Rotation first, direction Forward
 ** 					Using Pathfinding algo
 **
 ** parameters:			X coord cible
 ** 					Y coord cible
 ** 					Attente = Wait or not for the end of Deplacement
 ** 					Direction: 0 = forward
 ** 								1= backward
 **
 ** 					pointer to the pathfinding obstacle creation function
 **
 ** Returned value:		true: deplacement done successfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Tourne_Avance_ASTAR(short X, short Y, bool Attente, bool direction, void* obstacle_creation_fct)
{
	struct st_DESTINATION_ROBOT dest = { 0 };
	struct st_COORDONNEES coord = { 0 };
	char str[70];

	coord.X = X;
	coord.Y = Y;
	if(direction == 0)
	{
		coord.Type_Deplacement = TYPE_MOVE_xy_tour_av_avant;
	}else
	{
		coord.Type_Deplacement = TYPE_MOVE_xy_tour_av_arriere;
	}
	//dest.Bloquage_Possible = Bloquage;

	//Parametres classiques pour deplacement classiques
	coord.ptrParameters.Angle_Avant_Debut_Avance = 0.08F * 100;
	coord.ptrParameters.Angle_Detection_Fin_Trajectoire = 0.05F * 100;
	coord.ptrParameters.Distance_Detection_Fin_Trajectoire = 2 * 100;

	coord.Type_Arret = depla_AVEC_freinage;

	dest.coord = coord;

	//Create struct with parameters for Astar task
	struct Astar_deplacement parameters;
	parameters.destination = dest;
	parameters.obstacle_creation_fct = obstacle_creation_fct;

	//Clear the Flags (path found | not found) before sending or waiting
	_0_Deplacement_Clear_Flags();

	//Create the Astar deplacement task
	xTaskCreate(_0_Deplacement_ASTAR, (char *) "0_Depl_Astar", 320, &parameters, (tskIDLE_PRIORITY + 1UL), &Astar_Task_Handler);

	if(!Attente)
		return true;

	bool result;
	result = _0_Deplacement_Wait_For_Arrival(&coord);

	//Check bloquage
	_0_Deplacement_Check_Bloquage();


	eTaskState state = eTaskGetState( Astar_Task_Handler );
	if(state != eDeleted)
	{
		sprintf(str, "ASTAR: Delete task to: X:%dmm Y:%dmm\n",
				X,
				Y);
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Deplacement, LOG_Debug_Port);

		//Delete the Astar task
		vTaskDelete(Astar_Task_Handler);
	}

	//true: we arrived at destination
	//false: destination is not reachable
	return result;
}



/*****************************************************************************
 ** Function name:		_0_Deplacement_Recalage_Bordure
 **
 ** Descriptions:		Deplacement en vitesse avec detection de bloquage pour un recalage bordure
 **
 ** parameters:			Direction: 0 = forward
 ** 								1= backward
 ** 					Vitesse
 ** 					TimeOUT (ms)
 **
 ** Returned value:		true: deplacement done succesfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Recalage_Bordure(bool direction, short speed, short TIMEOUT)
{
	char str[70];

	//En mode simulation, pas de recallage possible, on est tout de suite arrivé
	if((xEventGroupGetBits( _0_Deplacement_EventGroup) & eGROUP_DEPLA_SIMULATION) == eGROUP_DEPLA_SIMULATION)
	{
		return pdTRUE;
	}

	sprintf(str, "DEPLA: Recalage Start\n");
	_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Deplacement, LOG_Debug_Port);


	struct st_DESTINATION_ROBOT dest = { 0 };
	struct st_COORDONNEES coord = { 0 };

	coord.Type_Deplacement = TYPE_MOVE_consigne_vitesse_independantes;

	if(direction == 0)
	{
		//Recallage en marche arriere
		coord.Vitesse_Roue_Gauche = -speed;
		coord.Vitesse_Roue_Droite = -speed;
	}else
	{
		//Recallage en marche avant
		coord.Vitesse_Roue_Gauche = speed;
		coord.Vitesse_Roue_Droite = speed;
	}

	//Parametres classiques pour deplacement classiques
	coord.ptrParameters.Angle_Avant_Debut_Avance = 0.08F * 100;
	coord.ptrParameters.Angle_Detection_Fin_Trajectoire = 0.05F * 100;
	coord.ptrParameters.Distance_Detection_Fin_Trajectoire = 2 * 100;

	coord.Type_Arret = depla_AVEC_freinage;

	dest.coord = coord;
	dest.Replace = pdTRUE;

	//Clear the Flags (path found | not found) before sending or waiting
	_0_Deplacement_Clear_Flags();


	_0_Deplacement_Get_ptr_Current_Destination()->coord = coord;
	_2_Comm_Send_Destination_Robot(&dest, RS485_port);


	short _timeout = TIMEOUT;
	Task_Delay(20);

	//Attente du Flag de bloquage ou du TimeOUT
	while(!_0_Get_Robot_Position().Bloquage_Deplacement && _timeout > 0)
	{
		Task_Delay(20);
		_timeout -= 20;
	}

	//Si un bloquage a ete detecté
	if( _0_Get_Robot_Position().Bloquage_Deplacement )
	{
		/*
		 *Arret du robot
		 */
		_0_Deplacement_STOP();

		sprintf(str, "DEPLA: Recalage arrive\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Deplacement, LOG_Debug_Port);
		return pdTRUE;
	}

	/*
	 *Arret du robot
	 */
	_0_Deplacement_STOP();

	sprintf(str, "DEPLA: Recalage TIMEOUT\n");
	_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Deplacement, LOG_Debug_Port);
	return pdFALSE;
}


/*****************************************************************************
 ** Function name:		_0_Deplacement_Recalage_Bordure
 **
 ** Descriptions:		Deplacement en vitesse avec detection de bloquage pour un recalage bordure
 **
 ** parameters:			Direction: 0 = forward
 ** 								1= backward
 ** 					Attente
 ** 					Astar?
 ** 					Coordonnées des points de contrôle
 ** 					pointer to the pathfinding obstacle creation function
 **
 ** Returned value:		true: deplacement done successfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Spline_Cubique(bool direction, bool Attente, bool use_Astar, short P0_X, short P0_Y, short M0_X, short M0_Y, short M1_X, short M1_Y, short P1_X, short P1_Y, void* obstacle_creation_fct)
{
	struct CubicSpline spline;

	spline.P0.X = P0_X;
	spline.P0.Y = P0_Y;
	spline.M0.X = M0_X;
	spline.M0.Y = M0_Y;

	spline.M1.X = M1_X;
	spline.M1.Y = M1_Y;
	spline.P1.X = P1_X;
	spline.P1.Y = P1_Y;

	spline.Direction = direction;
	//0= Marche avant
	//1= Marche arriere

	spline.Nombre_Points = 20;
	spline.Taille_Terrain.X = Map_size_X;
	spline.Taille_Terrain.Y = Map_size_Y;

	spline.Replace = pdTRUE;

	spline.Use_Current_speed = 0;
	//0= Utilisation des valeurs de M0 fournies par l’IA
	//1= Utilise la vitesse courante comme vecteur M0

	struct st_COORDONNEES coord = { 0 };

	//Parametres classiques pour deplacement classiques
	coord.ptrParameters.Distance_Detection_Fin_Trajectoire = 10 * 100;
	coord.ptrParameters.Angle_Avant_Debut_Avance = 30;

	spline.ptrParameters = coord.ptrParameters;

	//Utilisés pour la detection de l'arrivee
	coord.X = P1_X;
	coord.Y = P1_Y;

	//Clear the Flags (path found | not found) before sending or waiting
	_0_Deplacement_Clear_Flags();

	//Envoi la demande de deplacement
	_2_Comm_Send_Destination_Spline_CubiqueRobot(&spline, RS485_port);

	//Create the Astar deplacement task
	if(use_Astar)
	{
		//Create struct with parameters for Astar task
		struct Astar_SPLINE_deplacement parameters;
		parameters.destination = spline;
		parameters.obstacle_creation_fct = obstacle_creation_fct;

		xTaskCreate(_0_Deplacement_ASTAR_SPLINE, (char *) "0_Dep_Ast_Spli", 320, &parameters, (tskIDLE_PRIORITY + 1UL), &Astar_Task_Handler);
	}

	if(!Attente)
		return pdPASS;


	//Attente d'être arrives
	bool result;
	result = _0_Deplacement_Wait_For_Arrival(&coord);

	//Check bloquage
	_0_Deplacement_Check_Bloquage();

	if(use_Astar)
	{
		eTaskState state = eTaskGetState( Astar_Task_Handler );
		if(state != eDeleted)
		{
			char str[70];
			sprintf(str, "ASTAR SPLINE: Delete task to: X:%dmm Y:%dmm\n",
					P1_X,
					P1_Y);
			_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Deplacement, LOG_Debug_Port);

			//Delete the Astar task
			vTaskDelete(Astar_Task_Handler);
		}
	}

	//true: we arrived at destination
	//false: destination is not reachable
	return result;
}


/*****************************************************************************
 ** Function name:		_0_Deplacement_STOP
 **
 ** Descriptions:		Arret des deplacements du robot, jusqu'au prochain mouvement
 **
 ** parameters:			None
 **
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Deplacement_STOP(void)
{
	struct st_DESTINATION_ROBOT dest = { 0 };
	struct st_COORDONNEES coord = { 0 };

	coord.Type_Deplacement = TYPE_MOVE_aucun_mouvement;
	dest.coord = coord;
	dest.Replace = pdTRUE;

	//Clear the Flags (path found | not found) before sending or waiting
	xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
			eGROUP_DEPLA_path_NOT_FOUND | eGROUP_DEPLA_pathFOUND | eGROUP_DEPLA_NOT_ARRIVED | eGROUP_DEPLA_ARRIVED | eGROUP_DEPLA_BLOQUAGE);/* The bits being set. */

	_0_Deplacement_Get_ptr_Current_Destination()->coord = coord;
	_2_Comm_Send_Destination_Robot(&dest, RS485_port);
}



/*****************************************************************************
 ** Function name:		_0_Deplacement_ASTAR
 **
 ** Descriptions:		Proceed to Astar computation until we reach the destination
 ** 					Rise a flag when arrived or Blocked
 **
 ** parameters:			A structure with:
 ** 						pointer to the pathfinding obstacle creation function
 ** 						struct st_DESTINATION_ROBOT
 **
 ** Returned value:		None
 **
 *****************************************************************************/
/*
 * Private task!!!!
 * Use _0_Deplacement_Tourne_Avance_ASTAR for external calls
 */
void _0_Deplacement_ASTAR(void* pvParameter)
{
	char str_ASTAR[70];
	Init_Timing_Tache;

	struct Point found_destination;
	struct Astar_deplacement parameters = *(struct Astar_deplacement*)pvParameter;


	//Keep safe the real destination
	struct st_COORDONNEES Final_Destination = parameters.destination.coord;

	sprintf(str_ASTAR, "ASTAR: Creation task to: X:%dmm Y:%dmm\n",
			Final_Destination.X,
			Final_Destination.Y);
	_2_Comm_Send_Log_Message(str_ASTAR, Color_Blue, Channel_Debug_ASTAR, LOG_Debug_Port);

	//All new destination will be with replacement active
	parameters.destination.Replace = true;

	//Start with a fully absurd previous_destination
	_0_Deplacement_Get_ptr_Current_Destination()->coord.X = -1;
	_0_Deplacement_Get_ptr_Current_Destination()->coord.Y = -1;

	//While we didn't reach the destination point
	while(Distance_To_Destination(_0_Get_Robot_Position(), &Final_Destination) * 100 > Final_Destination.ptrParameters.Distance_Detection_Fin_Trajectoire)
	{
		//Set_Debug_Pin_0_High();

		/*
		 * Step 1: Init the Map
		 */
		Astar_Map_Init(Astar_Get_Map(), Astar_Get_Vector_Map(), _0_Get_Robot_Position().Position_X, _0_Get_Robot_Position().Position_Y, Final_Destination.X, Final_Destination.Y);

		/*
		 * Step 2: Create obstacles according to the function passed
		 */
		if(parameters.obstacle_creation_fct != NULL)
			parameters.obstacle_creation_fct();

		/*
		 * Step 3: Execute Pathfinding (take around 30ms)
		 */
		if(Astar_Find_Path(Astar_Get_Map(), Astar_Get_Vector_Map()) > 0)
		{
			//Rise the Flag of Path Found
			xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_pathFOUND );/* The bits being set. */

			//Clear the Flag of Path Not Found
			xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_path_NOT_FOUND );/* The bits being set. */

			//A path as been found
			//Load the next destination
			found_destination = Astar_Get_Map()->First_Destination;

			//Send the new destination if != from the previous sent, with replacement
			if(_0_Deplacement_Get_ptr_Current_Destination()->coord.X != found_destination.x || _0_Deplacement_Get_ptr_Current_Destination()->coord.Y != found_destination.y)
			{
				/*
				 *Check if the previous sent destination is still an available path
				 */
				struct Astar_Vector tested_vector;
				tested_vector.Start_Point.x = _0_Get_Robot_Position().Position_X;
				tested_vector.Start_Point.y = _0_Get_Robot_Position().Position_Y;

				tested_vector.End_Point.x = _0_Deplacement_Get_ptr_Current_Destination()->coord.X;
				tested_vector.End_Point.y = _0_Deplacement_Get_ptr_Current_Destination()->coord.Y;

				if(Dijkstra_Intersect_Any_Segment(&tested_vector, Astar_Get_Vector_Map()) ||
						_0_Deplacement_Get_ptr_Current_Destination()->coord.X == -1 ||
						Distance_Two_Points(tested_vector.Start_Point.x, tested_vector.Start_Point.y, tested_vector.End_Point.x, tested_vector.End_Point.y) < Map_Resolution / 10)
				{
					//Intersection or first point, or distance too short, don't keep the last sent destination as-it

					/*
					 * For the firsts deplacement, use a larger parameter
					 */
					/*if(found_destination.x != Final_Destination.X && found_destination.y != Final_Destination.Y)
					{
						//This is not the Final Destination
						parameters.destination.coord.ptrParameters.Distance_Detection_Fin_Trajectoire = 150*100;
						parameters.destination.coord.ptrParameters.Angle_Avant_Debut_Avance = (30*PI/180)*100;
					}else
					{
						//Last point, use initials parameters
						parameters.destination.coord.ptrParameters = Final_Destination.ptrParameters;
					}*/

					/*
					 * Step 4: Send new destination to the Robot = new founded Pathfinding Result
					 */
					parameters.destination.coord.X = found_destination.x;
					parameters.destination.coord.Y = found_destination.y;
					_2_Comm_Send_Destination_Robot(&parameters.destination, RS485_port);

					//Set the new Current destination
					_0_Deplacement_Get_ptr_Current_Destination()->coord = parameters.destination.coord;

					//Astar_Debug_Display_Map(Astar_Get_Map());
					/*
					 * Step 5: Debug, display Pathfinding Map
					 */
					//_2_Comm_Send_ASTAR_Contenu(Astar_Get_Map(), Xbee_port);
					_2_Comm_Send_ASTAR_Vectors(Astar_Get_Vector_Map(), LOG_Debug_Port);
				}else
				{
					//No Intersection, the previous destination is still a good choose
				}
			}
		}else
		{
			//No path has been found
			//Rise the corresponding Flag
			xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_path_NOT_FOUND );/* The bits being set. */

			//Clear the Flag of Path Found
			xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_pathFOUND );/* The bits being set. */

			//Send a no mouvement order to the Robot, with replacement to flush all existing deplacement, and stop the Robot
			parameters.destination.Replace = true;
			parameters.destination.coord.Type_Deplacement = TYPE_MOVE_aucun_mouvement;
			_2_Comm_Send_Destination_Robot(&parameters.destination, RS485_port);

			sprintf(str_ASTAR, "No path found, X= %d, Y= %d; to X= %d, Y= %d \n",
					(short)_0_Get_Robot_Position().Position_X,
					(short)_0_Get_Robot_Position().Position_Y,
					(short)Final_Destination.X,
					(short)Final_Destination.Y);
			_2_Comm_Send_Log_Message(str_ASTAR, Color_Red, Channel_Debug_ASTAR, LOG_Debug_Port);

			/*
			 * Step 5: Debug, display Pathfinding Map
			 */
			_2_Comm_Send_ASTAR_Contenu(Astar_Get_Map(), Xbee_port);
		}


		//Astar_Debug_Display_Map(Astar_Get_Map());
		//Set_Debug_Pin_0_Low();
		/*
		 * Step 6: Wait for the next Astar Loop
		 */
		Task_Delay_Until(Astar_Recherche_Periode);
	}

	/*
	 * Step 7: When we reach the final destination, delete this task
	 */
	sprintf(str_ASTAR, "ASTAR: Self Delete task to: X:%dmm Y:%dmm\n",
			Final_Destination.X,
			Final_Destination.Y);
	_2_Comm_Send_Log_Message(str_ASTAR, Color_Blue, Channel_Debug_ASTAR, LOG_Debug_Port);
	Task_Delete_Current;
}


/*****************************************************************************
 ** Function name:		_0_Deplacement_ASTAR_SPLINE
 **
 ** Descriptions:		Check if Spline move is possible based on vectors and Spline controls points
 ** 					Rise a flag when arrived or Blocked
 **
 ** parameters:			A structure with:
 ** 						pointer to the pathfinding obstacle creation function
 ** 						CubicSpline
 **
 ** Returned value:		None
 **
 *****************************************************************************/
/*
 * Private task!!!!
 * Use _0_Deplacement_Tourne_Avance_ASTAR for external calls
 */
void _0_Deplacement_ASTAR_SPLINE(void* pvParameter)
{
	char str_ASTAR[70];
	Init_Timing_Tache;

	struct Astar_SPLINE_deplacement parameters = *(struct Astar_SPLINE_deplacement*)pvParameter;

	sprintf(str_ASTAR, "ASTAR SPLINE: Creation task to: X:%dmm Y:%dmm\n",
			parameters.destination.P1.X,
			parameters.destination.P1.Y);
	_2_Comm_Send_Log_Message(str_ASTAR, Color_Blue, Channel_Debug_ASTAR, LOG_Debug_Port);



	struct CubicSpline_Point Control_Points_Modified_M0bis; //A 1/3 entre P0 et M0
	struct CubicSpline_Point Control_Points_Modified_M1bis; //Au milieu entre M0 et M1
	struct CubicSpline_Point Control_Points_Modified_M2bis; //A 1/3 entre M1 et P1

	//Premier point de controle
	Control_Points_Modified_M0bis.X = (parameters.destination.M0.X - parameters.destination.P0.X) / 3 + parameters.destination.P0.X;
	Control_Points_Modified_M0bis.Y = (parameters.destination.M0.Y - parameters.destination.P0.Y) / 3 + parameters.destination.P0.Y;

	//Second point de contrôle
	Control_Points_Modified_M1bis.X = (parameters.destination.M0.X + parameters.destination.M1.X) / 2;
	Control_Points_Modified_M1bis.Y = (parameters.destination.M0.Y + parameters.destination.M1.Y) / 2;

	//Troisième point de contrôle
	Control_Points_Modified_M2bis.X = (parameters.destination.M1.X - parameters.destination.P1.X) / 3 + parameters.destination.P1.X;
	Control_Points_Modified_M2bis.Y = (parameters.destination.M1.Y - parameters.destination.P1.Y) / 3 + parameters.destination.P1.Y;


	struct CubicSpline_Point Control_Points_Modified_ter[6];
	/*Au milieu entre P0 et M0bis
	Au milieu entre M0bis et M1bis
	Au milieu entre M1bis et M2bis
	Au milieu entre M2bis et P1*/

	//Points de contrôle de second stage
	Control_Points_Modified_ter[0].X = parameters.destination.P0.X;
	Control_Points_Modified_ter[0].Y = parameters.destination.P0.Y;

	Control_Points_Modified_ter[1].X = (parameters.destination.P0.X + Control_Points_Modified_M0bis.X) / 2;
	Control_Points_Modified_ter[1].Y = (parameters.destination.P0.Y + Control_Points_Modified_M0bis.Y) / 2;

	Control_Points_Modified_ter[2].X = (Control_Points_Modified_M0bis.X + Control_Points_Modified_M1bis.X) / 2;
	Control_Points_Modified_ter[2].Y = (Control_Points_Modified_M0bis.Y + Control_Points_Modified_M1bis.Y) / 2;

	Control_Points_Modified_ter[3].X = (Control_Points_Modified_M1bis.X + Control_Points_Modified_M2bis.X) / 2;
	Control_Points_Modified_ter[3].Y = (Control_Points_Modified_M1bis.Y + Control_Points_Modified_M2bis.Y) / 2;

	Control_Points_Modified_ter[4].X = (Control_Points_Modified_M2bis.X + parameters.destination.P1.X) / 2;
	Control_Points_Modified_ter[4].Y = (Control_Points_Modified_M2bis.Y + parameters.destination.P1.Y) / 2;

	Control_Points_Modified_ter[5].X = parameters.destination.P1.X;
	Control_Points_Modified_ter[5].Y = parameters.destination.P1.Y;

	//Calcul la distance cumulée entre chaque segments depuis le point de depart
	unsigned int Cumulated_Segments_Length[5];
	for(int i = 0; i < 5; i++)
	{
		Cumulated_Segments_Length[i] = (unsigned int)Distance_Two_Points(Control_Points_Modified_ter[i].X, Control_Points_Modified_ter[i].Y, Control_Points_Modified_ter[i+1].X, Control_Points_Modified_ter[i+1].Y);
		if(i>0)
		{
			Cumulated_Segments_Length[i] += Cumulated_Segments_Length[i-1];
		}
	}



	struct st_COORDONNEES Final_Destination;
	Final_Destination.X = parameters.destination.P1.X;
	Final_Destination.Y = parameters.destination.P1.Y;

	//While we didn't reach the destination point
	while(Distance_To_Destination(_0_Get_Robot_Position(), &Final_Destination) * 100 > parameters.destination.ptrParameters.Distance_Detection_Fin_Trajectoire)
	{
		/*
		 * Step 1: Init the Map
		 */
		Astar_Map_Init(Astar_Get_Map(), Astar_Get_Vector_Map(), _0_Get_Robot_Position().Position_X, _0_Get_Robot_Position().Position_Y, Final_Destination.X, Final_Destination.Y);


		/*
		 * Step 2: Create obstacles according to the function passed
		 */
		if(parameters.obstacle_creation_fct != NULL)
			parameters.obstacle_creation_fct();

		_2_Comm_Send_ASTAR_Vectors(Astar_Get_Vector_Map(), LOG_Debug_Port);

		/*
		 * Step 3: Check if Spline Path is clear
		 */
		struct Astar_Vector tested_vector;

		int intermediate = 0;
		/*
		 * Cherche à partir de quel noeud intermediaire commencer la vérification d'intersection
		 */
		//Calcule la distance entre le robot et le point de départ
		unsigned int Distance_Robot_P0 = (unsigned int)Distance_Two_Points(_0_Get_Robot_Position().Position_X, _0_Get_Robot_Position().Position_Y, Control_Points_Modified_ter[0].X, Control_Points_Modified_ter[0].Y);
		for(; intermediate < 5; intermediate++)
		{
			if(Cumulated_Segments_Length[intermediate] > Distance_Robot_P0)
				break;
		}

		for(; intermediate < 5; intermediate++)
		{
			//From P0 to Control_Points_Modified_M0ter
			//			tested_vector.Start_Point.x = Control_Points_Modified_ter[intermediate].X;
			//			tested_vector.Start_Point.y = Control_Points_Modified_ter[intermediate].Y;

			tested_vector.Start_Point.x = _0_Get_Robot_Position().Position_X;
			tested_vector.Start_Point.y = _0_Get_Robot_Position().Position_Y;

			tested_vector.End_Point.x = Control_Points_Modified_ter[intermediate+1].X;
			tested_vector.End_Point.y = Control_Points_Modified_ter[intermediate+1].Y;

			if(Dijkstra_Intersect_Any_Segment(&tested_vector, Astar_Get_Vector_Map()))
			{
				//There is an intersection between this points
				//Stop the Robot and clear path

				//Send a no mouvement order to the Robot, with replacement to flush all existing deplacement, and stop the Robot
				struct st_DESTINATION_ROBOT coord;
				coord.Replace = true;
				coord.coord.Type_Deplacement = TYPE_MOVE_aucun_mouvement;
				_2_Comm_Send_Destination_Robot(&coord, RS485_port);

				sprintf(str_ASTAR, "SPLINE no path found, X= %d, Y= %d; to X= %d, Y= %d",
						(short)_0_Get_Robot_Position().Position_X,
						(short)_0_Get_Robot_Position().Position_Y,
						(short)Final_Destination.X,
						(short)Final_Destination.Y);
				_2_Comm_Send_Log_Message(str_ASTAR, Color_Red, Channel_Debug_ASTAR, LOG_Debug_Port);

				Task_Delay(5);
				sprintf(str_ASTAR, "SPLINE Intersect, X= %d, Y= %d; to X= %d, Y= %d",
						(short)tested_vector.Start_Point.x,
						(short)tested_vector.Start_Point.y,
						(short)tested_vector.End_Point.x,
						(short)tested_vector.End_Point.y);
				_2_Comm_Send_Log_Message(str_ASTAR, Color_Red, Channel_Debug_ASTAR, LOG_Debug_Port);

				/*
				 * Step 4: No path possible, delete this task after stopping to Robot and informed the IA
				 */
				sprintf(str_ASTAR, "ASTAR Spline: Self Delete task to: X:%dmm Y:%dmm\n",
						Final_Destination.X,
						Final_Destination.Y);
				_2_Comm_Send_Log_Message(str_ASTAR, Color_Blue, Channel_Debug_ASTAR, LOG_Debug_Port);

				//No path has been found
				//Rise the corresponding Flag
				xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
						eGROUP_DEPLA_path_NOT_FOUND );/* The bits being set. */

				//Clear the Flag of Path Found
				xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
						eGROUP_DEPLA_pathFOUND );/* The bits being set. */

				Task_Delete_Current;
			}
		}

		/*
		 * Step 5: Wait for the next Astar Loop
		 */
		Task_Delay_Until(Astar_Recherche_Periode);
	}
	/*
	 * Step 5: We reached the destination
	 */
	sprintf(str_ASTAR, "ASTAR Spline: Self Delete task to: X:%dmm Y:%dmm\n",
			Final_Destination.X,
			Final_Destination.Y);
	_2_Comm_Send_Log_Message(str_ASTAR, Color_Blue, Channel_Debug_ASTAR, LOG_Debug_Port);
	Task_Delete_Current;
}







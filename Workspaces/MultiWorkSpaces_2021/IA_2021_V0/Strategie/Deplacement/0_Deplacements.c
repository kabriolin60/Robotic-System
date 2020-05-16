/*
 * 0_Deplacements.c
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#include "FreeRTOS.h"
#include "event_groups.h"

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
 ** Returned value:		pointer to the last sended destination
 **
 *****************************************************************************/
struct st_DESTINATION_ROBOT* _0_Deplacement_Get_ptr_Current_Destination(void)
{
	return &Current_destination;
}



/*****************************************************************************
 ** Function name:		_0_Deplacement_Wait_For_Arrival
 **
 ** Descriptions:		Check if the Robot reach it's destination
 **
 ** parameters:			ptr to coord cible
 ** Returned value:		true: deplacement done succesfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Wait_For_Arrival(struct st_COORDONNEES* coord)
{
	EventBits_t uxBits;

	while(Distance_To_Destination(_0_Get_Ptr_Robot_Position(), coord) * 100 > coord->ptrParameters.Distance_Detection_Fin_Trajectoire)
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
		Task_Delay(20);
	}

	//We arrived at destination
	//Set the Flag (no found | Not Arrived to Destination) before sending or waiting
	xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
			eGROUP_DEPLA_ARRIVED );/* The bits being set. */

	//return succes
	return true;
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
 ** Returned value:		true: deplacement done succesfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Tourne_Avance(short X, short Y, bool remplacement, bool Attente, bool direction)
{
	struct st_DESTINATION_ROBOT dest = { 0 };
	struct st_COORDONNEES coord = { 0 };


	//Check if the new dest is != to the previous sended
	if(_0_Deplacement_Get_ptr_Current_Destination()->coord.X != X || _0_Deplacement_Get_ptr_Current_Destination()->coord.Y != Y)
	{
		coord.X = X;
		coord.Y = Y;
		if(direction == 0)
		{
			coord.Type_Deplacement = xy_tour_av_avant;
		}else
		{
			coord.Type_Deplacement = xy_tour_av_arriere;
		}

		//dest.Bloquage_Possible = Bloquage;

		//Parametres classiques pour deplacement classiques
		coord.ptrParameters.Angle_Avant_Debut_Avance = 0.08F * 100;
		coord.ptrParameters.Angle_Detection_Fin_Trajectoire = 0.05F * 100;
		coord.ptrParameters.Distance_Detection_Fin_Trajectoire = 4 * 100;

		coord.Type_Arret = depla_AVEC_freinage;

		dest.coord = coord;
		dest.Replace = remplacement;

		//Clear the Flags (path found | not found) before sending or waiting
		xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
				eGROUP_DEPLA_path_NOT_FOUND | eGROUP_DEPLA_pathFOUND | eGROUP_DEPLA_NOT_ARRIVED | eGROUP_DEPLA_ARRIVED );/* The bits being set. */


		_0_Deplacement_Get_ptr_Current_Destination()->coord = coord;
		_2_Comm_Send_Destination_Robot(&dest, RS485_port);
	}

	if(!Attente)
		return true;

	return _0_Deplacement_Wait_For_Arrival(&coord);
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
 ** Returned value:		true: deplacement done succesfully
 ** 					false: error
 **
 *****************************************************************************/
bool _0_Deplacement_Tourne_Avance_ASTAR(short X, short Y, bool Attente, bool direction, void* obstacle_creation_fct)
{
	struct st_DESTINATION_ROBOT dest = { 0 };
	struct st_COORDONNEES coord = { 0 };

	coord.X = X;
	coord.Y = Y;
	if(direction == 0)
	{
		coord.Type_Deplacement = xy_tour_av_avant;
	}else
	{
		coord.Type_Deplacement = xy_tour_av_arriere;
	}
	//dest.Bloquage_Possible = Bloquage;

	//Parametres classiques pour deplacement classiques
	coord.ptrParameters.Angle_Avant_Debut_Avance = 0.08F * 100;
	coord.ptrParameters.Angle_Detection_Fin_Trajectoire = 0.05F * 100;
	coord.ptrParameters.Distance_Detection_Fin_Trajectoire = 4 * 100;

	coord.Type_Arret = depla_AVEC_freinage;

	dest.coord = coord;

	//Create struct with parameters for Astar task
	struct Astar_deplacement parameters;
	parameters.destination = dest;
	parameters.obstacle_creation_fct = obstacle_creation_fct;

	//Clear the Flags (path found | not found) before sending or waiting
	xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
			eGROUP_DEPLA_path_NOT_FOUND | eGROUP_DEPLA_pathFOUND | eGROUP_DEPLA_NOT_ARRIVED | eGROUP_DEPLA_ARRIVED );/* The bits being set. */

	//Create the Astar deplacement task
	xTaskCreate(_0_Deplacement_ASTAR, (char *) "0_Depl_Astar", 220, &parameters, (tskIDLE_PRIORITY + 1UL), &Astar_Task_Handler);

	if(!Attente)
		return true;

	bool result;

	result = _0_Deplacement_Wait_For_Arrival(&coord);
	if(result == false)
	{
		//No deplacement possible
		//Delete the Astar task
		//vTaskDelete(Astar_Task_Handler);
	}

	return result;
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
	Init_Timing_Tache;

	struct Point founded_destination;
	struct Astar_deplacement parameters = *(struct Astar_deplacement*)pvParameter;

	//Keep safe the real destination
	struct st_COORDONNEES Final_Destination = parameters.destination.coord;

	//All new destination will be with replacement active
	parameters.destination.Replace = true;

	//Start with a fully absurd previous_destination
	_0_Deplacement_Get_ptr_Current_Destination()->coord.X = -1;
	_0_Deplacement_Get_ptr_Current_Destination()->coord.Y = -1;


	//While we didn't reach the destination point
	while(Distance_To_Destination(_0_Get_Ptr_Robot_Position(), &Final_Destination) * 100 > Final_Destination.ptrParameters.Distance_Detection_Fin_Trajectoire)
	{
		Set_Debug_Pin_0_High();


		/*
		 * Step 1: Init the Map
		 */
		Astar_Map_Init(Astar_Get_Map(), Astar_Get_Vector_Map(), _0_Get_Ptr_Robot_Position().Position_X, _0_Get_Ptr_Robot_Position().Position_Y, Final_Destination.X, Final_Destination.Y);

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
			founded_destination = Astar_Get_Map()->First_Destination;

			//Send the new destination if != from the previous sent, with replacement
			if(_0_Deplacement_Get_ptr_Current_Destination()->coord.X != founded_destination.x || _0_Deplacement_Get_ptr_Current_Destination()->coord.Y != founded_destination.y)
			{
				/*
				 *Check if the previous sent destination is still an available path
				 */
				struct Astar_Vector tested_vector;
				tested_vector.Start_Point.x = _0_Get_Ptr_Robot_Position().Position_X;
				tested_vector.Start_Point.y = _0_Get_Ptr_Robot_Position().Position_Y;

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
					/*if(founded_destination.x != Final_Destination.X && founded_destination.y != Final_Destination.Y)
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
					parameters.destination.coord.X = founded_destination.x;
					parameters.destination.coord.Y = founded_destination.y;
					_2_Comm_Send_Destination_Robot(&parameters.destination, RS485_port);

					//Set the new Current destination
					_0_Deplacement_Get_ptr_Current_Destination()->coord = parameters.destination.coord;

					Astar_Debug_Display_Map(Astar_Get_Map());
				}else
				{
					//No Intersection, the previous destination is still a good choose
				}
			}
		}else
		{
			//No path has been found
			//Rise the correspond Flag
			xEventGroupSetBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_path_NOT_FOUND );/* The bits being set. */

			//Clear the Flag of Path Found
			xEventGroupClearBits(_0_Deplacement_EventGroup,    /* The event group being updated. */
					eGROUP_DEPLA_pathFOUND );/* The bits being set. */

			//Send a no mouvement order to the Robot, with replacement to flush all existing deplacement, and stop the Robot
			parameters.destination.coord.Type_Deplacement = aucun_mouvement;
			_2_Comm_Send_Destination_Robot(&parameters.destination, RS485_port);

			static char str[70];
			sprintf(str, "No path found, X= %d, Y= %d; to X= %d, Y= %d \n",
					(short)_0_Get_Ptr_Robot_Position().Position_X,
					(short)_0_Get_Ptr_Robot_Position().Position_Y,
					(short)Final_Destination.X,
					(short)Final_Destination.Y);
			_2_Comm_Send_Log_Message(str, Color_Red, RS485_port);
		}

		/*
		 * Step 5: Debug, display Pathfinding Map
		 */
		//Astar_Debug_Display_Map(Astar_Get_Map());



		Set_Debug_Pin_0_Low();
		/*
		 * Step 6: Wait for the next Astar Loop
		 */
		Task_Delay_Until(Astar_Recherche_Periode);
	}

	/*
	 * Step 7: When we reach the final destination, delete this task
	 */
	Task_Delete_Current;
}







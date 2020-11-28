/*
 * Strategie.c
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#include "Strategie_Actions.h"
#include "Strategie.h"

struct Actions_List Actions_2021;


/*****************************************************************************
 ** Function name:		_Strategie_Init_Strategie_2021
 **
 ** Descriptions:		Init de la strategie
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _Strategie_Init_Strategie_2021(void* pvparameters)
{
	//Ajouter l'attente de l'initialisation
	Task_Delay(50);


	//Creation des actions
	struct Action_Datas temp_action;

	temp_action.ID = Actions_2021.Nombre_Actions;
	temp_action.Name = "First Action";
	temp_action.State = Action_En_Attente;
	temp_action.Step = 0;
	temp_action.Priority = 1;
	temp_action.Points = 12;
	temp_action.Qui_Fait = Action_Personne;
	temp_action.Qui_Peut = Action_Gros_Robot;
	temp_action.Temps_mini = 5000;
	temp_action.Temps_maxi = 109000;
	temp_action.Fct = NULL;
	temp_action.Param = NULL;
	temp_action.StartPoint_X = 1000;
	temp_action.StartPoint_Y = 1500;

	Actions_2021.Actions[Actions_2021.Nombre_Actions] = temp_action;

	//Envoie au PC cette action
	_2_Comm_Strategie_Send_Action_State_Update(&Actions_2021.Actions[Actions_2021.Nombre_Actions++], RS485_port);



	/* Demande de presence des cartes à la fin de l'init de la stratégie*/
	xTaskCreate(_2_Communication_Boards_Status, "Board Status", 200, NULL, 1, NULL);



	//Do strategic stuff


	//Never reach
	Task_Delete_Current;
}



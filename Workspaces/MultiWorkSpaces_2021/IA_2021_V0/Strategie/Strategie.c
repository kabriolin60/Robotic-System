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
	_2_Comm_Strategie_Send_Action_Creation(&Actions_2021.Actions[Actions_2021.Nombre_Actions++], RS485_port);



	temp_action.ID = Actions_2021.Nombre_Actions;
	temp_action.Name = "Second Action";
	temp_action.State = Action_En_Attente;
	temp_action.Step = 0;
	temp_action.Priority = 12;
	temp_action.Points = 45;
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
	_2_Comm_Strategie_Send_Action_Creation(&Actions_2021.Actions[Actions_2021.Nombre_Actions++], RS485_port);



	temp_action.ID = Actions_2021.Nombre_Actions;
	temp_action.Name = "Third Action Bleu Gobelets Centre";
	temp_action.State = Action_En_Attente;
	temp_action.Step = 0;
	temp_action.Priority = 3;
	temp_action.Points = 4;
	temp_action.Qui_Fait = Action_Personne;
	temp_action.Qui_Peut = Action_Gros_Robot;
	temp_action.Temps_mini = 12000;
	temp_action.Temps_maxi = 90000;
	temp_action.Fct = NULL;
	temp_action.Param = NULL;
	temp_action.StartPoint_X = 1000;
	temp_action.StartPoint_Y = 1500;

	Actions_2021.Actions[Actions_2021.Nombre_Actions] = temp_action;

	//Envoie au PC cette action
	_2_Comm_Strategie_Send_Action_Creation(&Actions_2021.Actions[Actions_2021.Nombre_Actions++], RS485_port);



	/* Demande de presence des cartes à la fin de l'init de la stratégie*/
	xTaskCreate(_2_Communication_Boards_Status, "Board Status", 200, NULL, 1, NULL);



	//Do strategic stuff
	Task_Delay(2000);
	_Strategie_Change_Action_State(&Actions_2021.Actions[0], Action_En_cours, "Start");

	_Strategie_Change_Action_Step(&Actions_2021.Actions[0], 1, "Step 1");
	Task_Delay(2000);

	_Strategie_Change_Action_Step(&Actions_2021.Actions[0], 2, "Step 2");
	Task_Delay(2000);

	_Strategie_Change_Action_State(&Actions_2021.Actions[0], Action_Terminee, "End");
	_Strategie_Change_Action_State(&Actions_2021.Actions[1], Action_En_cours, "Start");
	Task_Delay(2000);

	_Strategie_Change_Action_Step(&Actions_2021.Actions[1], 1, "Step 1");
	Task_Delay(2000);

	_Strategie_Change_Action_Step(&Actions_2021.Actions[1], 2, "Step 2");
	Task_Delay(2000);

	_Strategie_Change_Action_Step(&Actions_2021.Actions[1], 1, "Step 1");
	Task_Delay(2000);

	_Strategie_Change_Action_Step(&Actions_2021.Actions[1], 3, AT_FILE_LINE);
	Task_Delay(2000);

	_Strategie_Change_Action_State(&Actions_2021.Actions[1], Action_Terminee, "End");
	_Strategie_Change_Action_State(&Actions_2021.Actions[2], Action_Annulee, "Cancel");



	//Never reach
	Task_Delete_Current;
}



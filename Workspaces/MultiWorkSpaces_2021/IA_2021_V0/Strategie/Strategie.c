/*
 * Strategie.c
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#include "Strategie_Actions.h"
#include "Strategie.h"
#include "0_ADC.h"

#include "2_Echange_Datas.h"
#include "Init.h"

struct Actions_List Actions_2021;

byte Strategie_Choisie = 0;
unsigned long Temps_Match = 0;


/*****************************************************************************
 ** Function name:		_Strategie_Get_Robot_ID
 **
 ** Descriptions:		Lecture de l'ID du robot
 **
 ** parameters:			None
 ** Returned value:		Robot ID
 **
 *****************************************************************************/
byte _Strategie_Get_Robot_ID(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, GROS_PETIT_PIN_PORT, GROS_PETIT_PIN_BIT);
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_Input_Strategie_Selector
 **
 ** Descriptions:		Lecture de la strategie demandée par l'utilisateur
 **
 ** parameters:			None
 ** Returned value:		Strategie_Choisie
 **
 *****************************************************************************/
byte _Strategie_Get_Input_Strategie_Selector(void)
{
	short input_value;
	input_value = _0_ADC_Read_Channel(7); //Lecture de la valeur analogique lue sur le selecteur de stratégie

	Strategie_Choisie = input_value / (4096 / 10);

	return Strategie_Choisie;
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_Chosen_Strategie
 **
 ** Descriptions:		Lecture de la strategie Choisie
 **
 ** parameters:			None
 ** Returned value:		Strategie_Choisie
 **
 *****************************************************************************/
byte _Strategie_Get_Chosen_Strategie(void)
{
	return Strategie_Choisie;
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_Temps_Match
 **
 ** Descriptions:		Lecture du temps de match actuel
 **
 ** parameters:			None
 ** Returned value:		temps de match
 **
 *****************************************************************************/
unsigned long _Strategie_Get_Temps_Match(void)
{
	return Temps_Match;
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_Jack_Status
 **
 ** Descriptions:		Lecture de la valeur de l'entrée Jack
 **
 ** parameters:			None
 ** Returned value:		Entrée Jack
 **
 *****************************************************************************/
byte _Strategie_Get_Jack_Status(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, JACK_PORT, JACK_BIT);
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_Color_Status
 **
 ** Descriptions:		Lecture de la valeur de l'entrée Color
 **
 ** parameters:			None
 ** Returned value:		Entrée Color
 **
 *****************************************************************************/
byte _Strategie_Get_Color_Status(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, COLOR_PORT, COLOR_BIT);
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_Switch_Status
 **
 ** Descriptions:		Lecture de la valeur de l'entrée Switch
 **
 ** parameters:			None
 ** Returned value:		Entrée Switch
 **
 *****************************************************************************/
byte _Strategie_Get_Switch_Status(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, SWITCH_PORT, SWITCH_BIT);
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_External_LED_RED_Status
 **
 ** Descriptions:		Lecture de la valeur de la led externe ROUGE
 **
 ** parameters:			None
 ** Returned value:		sortie led rouge
 **
 *****************************************************************************/
byte _Strategie_Get_External_LED_RED_Status(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, LED_EXTERNE_RED_PORT, LED_EXTERNE_RED_BIT);
}

void _Strategie_Set_External_LED_RED_Status(byte value)
{
	Chip_GPIO_SetPinState(LPC_GPIO, LED_EXTERNE_RED_PORT, LED_EXTERNE_RED_BIT, value);
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_External_LED_YELLOW_Status
 **
 ** Descriptions:		Lecture de la valeur de la led externe JAUNE
 **
 ** parameters:			None
 ** Returned value:		sortie led jaune
 **
 *****************************************************************************/
byte _Strategie_Get_External_LED_YELLOW_Status(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, LED_EXTERNE_YELLOW_PORT, LED_EXTERNE_YELLOW_BIT);
}

void _Strategie_Set_External_LED_YELLOW_Status(byte value)
{
	Chip_GPIO_SetPinState(LPC_GPIO, LED_EXTERNE_YELLOW_PORT, LED_EXTERNE_YELLOW_BIT, value);
}


/*****************************************************************************
 ** Function name:		_Strategie_Get_External_LED_GREEN_Status
 **
 ** Descriptions:		Lecture de la valeur de la led externe Vertye
 **
 ** parameters:			None
 ** Returned value:		sortie led verte
 **
 *****************************************************************************/
byte _Strategie_Get_External_LED_GREEN_Status(void)
{
	return Chip_GPIO_GetPinState(LPC_GPIO, LED_EXTERNE_GREEN_PORT, LED_EXTERNE_GREEN_BIT);
}

void _Strategie_Set_External_LED_GREEN_Status(byte value)
{
	Chip_GPIO_SetPinState(LPC_GPIO, LED_EXTERNE_GREEN_PORT, LED_EXTERNE_GREEN_BIT, value);
}


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
	Task_Delay(500);

	//vTraceEnable(TRC_START);

	/* Envoie l'ID du Robot vers les cartes sur le réseau */
	Init_Send_Robot_ID();


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



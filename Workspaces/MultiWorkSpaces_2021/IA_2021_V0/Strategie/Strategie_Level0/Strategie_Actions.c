/*
 * Strategie_Actions.c
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#include "Strategie.h"

#include "1_Trame_Communication.h"
#include "stdio.h"

#include "0_Infos.h"
#include "0_Event_Group.h"

#include "2_Echange_Data_Strategie.h"



/*****************************************************************************
 ** Function name:		_Strategie_Change_Action_State
 **
 ** Descriptions:		Changement d'un etat de la stratégie
 **
 ** parameters:			pointeur sur l'action à modifier
 ** 					new state
 ** 					commentaire
 ** Returned value:		None
 **
 *****************************************************************************/
void _Strategie_Change_Action_State(struct Action_Datas* action_to_change, enum Action_State new_state, char* commentaire)
{
	action_to_change->State = new_state;

	if(new_state == Action_En_cours)
	{
		if(Chip_GPIO_GetPinState(LPC_GPIO, GROS_PETIT_PIN_PORT, GROS_PETIT_PIN_BIT) == GROS_ROBOT)
		{
			action_to_change->Qui_Fait = Action_Gros_Robot;
		}else
		{
			action_to_change->Qui_Fait = Action_Petit_Robot;
		}
	}else
	{
		action_to_change->Qui_Fait = Action_Personne;
	}

	//Envoie au PC cette action
	_2_Comm_Strategie_Send_Action_State_Update(action_to_change, commentaire, RS485_port);
}


/*****************************************************************************
 ** Function name:		_Strategie_Change_Action_Step
 **
 ** Descriptions:		Init de la strategie
 **
 ** parameters:			pointeur sur l'action à modifier
 ** 					new step
 ** 					commentaire
 ** Returned value:		None
 **
 *****************************************************************************/
void _Strategie_Change_Action_Step(struct Action_Datas* action_to_change, byte new_step, char* commentaire)
{
	action_to_change->Step = new_step;

	//Envoie au PC cette action
	_2_Comm_Strategie_Send_Action_State_Update(action_to_change, commentaire, RS485_port);
}






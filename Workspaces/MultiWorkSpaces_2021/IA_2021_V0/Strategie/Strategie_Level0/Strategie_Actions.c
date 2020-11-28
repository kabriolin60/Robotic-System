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
 ** Function name:		_Strategie_Init_Strategie_2021
 **
 ** Descriptions:		Init de la strategie
 **
 ** parameters:			pointeur sur l'action à modifier
 ** 					new state
 ** Returned value:		None
 **
 *****************************************************************************/
void _Strategie_Change_Action_State(struct Action_Datas* action_to_change, enum Action_State new_state)
{
	action_to_change->State = new_state;

	//Envoie au PC cette action
	_2_Comm_Strategie_Send_Action_State_Update(action_to_change, RS485_port);
}



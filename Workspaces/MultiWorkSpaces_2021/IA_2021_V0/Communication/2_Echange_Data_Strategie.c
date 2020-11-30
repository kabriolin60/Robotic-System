/*
 * 2_Echange_Data_Strategie.c
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#include "2_Echange_Data_Strategie.h"

static TO_AHBS_RAM3 struct Communication_Trame trame_echange;// TO_AHBS_RAM0;



/*****************************************************************************
 ** Function name:		_2_Comm_Strategie_Init
 **
 ** Descriptions:		Fonction d'init de la communication partie Strategie
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Strategie_Init()
{
	//Mise à dispo du bit de liberation de la trame d'envoi et de reception
	xEventGroupSetBits(_0_Comm_EventGroup, eGROUP_SYNCH_STRATEGIE_TxTrameDispo);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Strategie_Send_Action_Creation
 **
 ** Descriptions:		Fonction d'envoie de la creation d'une action et de son état initial
 **
 ** parameters:			pointer vers l'action,
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Strategie_Send_Action_Creation(struct Action_Datas* data, enum enum_canal_communication canal)
{
	struct Communication_Action_Datas data_to_send;

	data_to_send.ID = data->ID;

	data_to_send.Name_Length = COPYSTRING(data->Name, data_to_send.Name);
	data_to_send.Name[data_to_send.Name_Length] = 0;

	data_to_send.Points = data->Points;
	data_to_send.Priority = data->Priority;
	data_to_send.Qui_Fait = data->Qui_Fait;
	data_to_send.Qui_Peut = data->Qui_Peut;
	data_to_send.State = data->State;
	data_to_send.Step = data->Step;
	data_to_send.Temps_maxi = data->Temps_maxi / 10;
	data_to_send.Temps_mini = data->Temps_mini / 10;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_STRATEGIE_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = STRATEGIE_CHANGEMENT_ETAT;
	trame_echange.Slave_Adresse = PC;

	trame_echange.Length = COPYDATA(data_to_send, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_STRATEGIE_TxTrameDispo);
}


/*****************************************************************************
 ** Function name:		_2_Comm_Send_Action_State_Update
 **
 ** Descriptions:		Fonction d'envoie de la mise à jour du status d'une action
 **
 ** parameters:			pointer vers l'action
 ** 					Commentaire
 ** 					Canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Strategie_Send_Action_State_Update(struct Action_Datas* data, char* commentaire, enum enum_canal_communication canal)
{
	struct Communication_Action_Datas data_to_send;

	data_to_send.ID = data->ID;

	data_to_send.Name_Length = COPYSTRING(commentaire, data_to_send.Name);
	data_to_send.Name[data_to_send.Name_Length] = 0;

	data_to_send.Points = data->Points;
	data_to_send.Priority = data->Priority;
	data_to_send.Qui_Fait = data->Qui_Fait;
	data_to_send.Qui_Peut = data->Qui_Peut;
	data_to_send.State = data->State;
	data_to_send.Step = data->Step;
	data_to_send.Temps_maxi = data->Temps_maxi / 10;
	data_to_send.Temps_mini = data->Temps_mini / 10;

	//Attente du Bit de synchro donnant l'autorisation d'envoyer un nouveau message vers la Queue
	if(_1_Communication_Wait_To_Send(ms_to_tick(5), eGROUP_SYNCH_STRATEGIE_TxTrameDispo)== pdFAIL )
	{
		//Le bit n'est pas dispo, délai dépassé, le message n'est pas envoyé
		//Abandon
		return;
	}

	trame_echange.Instruction = STRATEGIE_CHANGEMENT_ETAT;
	trame_echange.Slave_Adresse = PC;

	trame_echange.Length = COPYDATA(data_to_send, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange, canal, eGROUP_SYNCH_STRATEGIE_TxTrameDispo);
}

/*
 * _2_Comm_Receive_Data.c
 *
 *  Created on: 3 déc. 2020
 *      Author: PC-INOVA
 */
#include "Lib_includes.h"
#include "_2_Comm_Echange_Datas.h"

static long Nb_Messages_Interpretes = 0;

void _2_Communication_Interprete_message(struct Communication_Trame* trame)
{
	Nb_Messages_Interpretes++;

	switch(trame->Instruction)
	{
	case REPONSE_INFO:
		_2_Communication_RX_Reponse_Infos(trame);
		break;

	case PING:
		//A la demande d'une carte, on répond par un PONG
		_2_Comm_Send_PONG(RS485_port);
		break;

	case END_COMMUNICATION:
		_0_Communication_Give_Sending_Clearance();
		break;

	default:
		break;
	}
}


/*****************************************************************************
 ** Function name:		_2_Communication_RX_Reponse_Infos
 **
 ** Descriptions:		Receive Cartes Informations
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_RX_Reponse_Infos(struct Communication_Trame* datas)
{
	struct Com_Reponse_Info infos;

	COPYDATA2(datas->Data, infos);

	//_0_Update_Card_Datas(&infos);
}

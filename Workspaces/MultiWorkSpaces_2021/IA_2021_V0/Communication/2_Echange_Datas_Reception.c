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
#include "Type_Declaration.h"

#include "2_Echange_Datas.h"
#include "0_Infos.h"
#include "0_Event_Group.h"

static long Nb_Messages_Interpretes = 0;

long Nb_PONG_recus = 0;

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


/*****************************************************************************
 ** Function name:		_2_Communication_RX_Lectures_Messages
 **
 ** Descriptions:		Lecture des messages en reception
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/

void _2_Communication_Interprete_message(struct Communication_Trame* trame)
{
	Debug_Trace_Texte("Debut_Interpretation");

	Nb_Messages_Interpretes++;

	switch(trame->Instruction)
	{
	case ACKNOWLEDGE:
		_2_Communication_RX_ACK(trame);
		break;

	case REPONSE_INFO:
		_2_Communication_RX_Reponse_Infos(trame);
		Nb_PONG_recus++;
		break;

	case PING:
		//A la demande d'une carte, on répond par un PONG
		_2_Comm_Send_PONG(RS485_port);

	case PONG:
		//Reception d'un Pong
		Nb_PONG_recus++;
		_2_Comm_RX_PONG(trame);
		break;

	default:
		break;
	}

	Debug_Trace_Texte("Fin_Interpretation");
}



/*****************************************************************************
 ** Function name:		_2_Communication_RX_ACK
 **
 ** Descriptions:		Receive Cartes Informations
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_RX_ACK(struct Communication_Trame* data)
{
	struct Communication_ACK ACK;
	COPYDATA2(data->Data, ACK);

	//Set le type d'ACK reçu
	xEventGroupSetBits(_0_ACK_Type_EventGroup,    /* The event group being updated. */
				(1 << ACK.ACK_TYPE));		 /* The bits being set. */

	//Set l'adresse de la carte ayant reçu l'ACK
	xEventGroupSetBits(_0_ACK_Adresses,    /* The event group being updated. */
					(1 << ACK.Adresse));		 /* The bits being set. */
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

	_0_Update_Card_Datas(&infos);
}


/*****************************************************************************
 ** Function name:		_2_Comm_RX_PONG
 **
 ** Descriptions:		Reception d'un pong en provenance d'une autre carte après son PING
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_PONG(struct Communication_Trame* datas)
{
	/* A reception d'un PONG
	 * Set le bit correspondant à 1 pour indiquer la présence de la carte sur le Bus
	 */
	struct Communication_PONG PONG;
	COPYDATA2(datas->Data, PONG);

	xEventGroupSetBits(_0_Status_EventGroup,    /* The event group being updated. */
			(1 << PONG.Adresse));		 /* The bits being set. */
}

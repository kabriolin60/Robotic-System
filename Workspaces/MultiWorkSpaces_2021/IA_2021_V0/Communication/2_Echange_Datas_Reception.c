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
	case DEMANDE_ROBOT_POSITION:
#ifdef TYPE_CARTE_MULTIFCT
		_2_Comm_Send_Robot_Position(_1_Odometrie_GetRobot_Position(), RS485_port);
#endif
		break;

	case REPONSE_INFO:
		Set_Debug_Pin_0_Low();
		_2_Communication_RX_Reponse_Infos(trame);
		//Nb_PONG_recus++;
		//Set_Debug_Pin_0_Low();
		break;

	case PING:
		//A la demande d'une carte, on répond par un PONG
		_2_Comm_Send_PONG(RS485_port);

	case PONG:
		//Reception d'un Pong
		Nb_PONG_recus++;
		//Set_Debug_Pin_0_Low();
		break;

	default:
		break;
	}

	Debug_Trace_Texte("Fin_Interpretation");
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

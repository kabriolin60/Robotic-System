/*
 * 2_Echange_Datas_Reception.c
 *
 *  Created on: 28 déc. 2020
 *      Author: kabri
 */

#include <Init.h>
#include "2_Echange_Datas_Reception.h"
#include "0_Communication.h"
#include "1_Trame_Communication.h"
#include "Type_Declaration.h"

#include "2_Echange_Datas.h"
//#include "1_Update_Values.h"


static long Nb_Messages_Interpretes = 0;

static byte ID_Robot;	//Id du Robot sur lequel est monté cette carte

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


void _2_Communication_Interprete_message(struct Communication_Trame* trame)
{
	Nb_Messages_Interpretes++;

	switch(trame->Instruction)
	{
	case DEFINITION_ID_ROBOT:		//Permet à la carte ID de donner l'ID du robot sur les autres cartes
		_2_Comm_RX_Id_Robot(trame);
		/*
		 * Envoi de l'ACK concerné par le message
		 */
		_2_Comm_Send_ACKNOWLEDGE(ACK_DEFINITION_ID_ROBOT, RS485_port);
		break;


	case DEMANDE_INFO:
		//Mise en pile des infos à envoyer
		//_2_Comm_Send_Infos(Get_ptr_Reponse_info(), RS485_port);
		//Cette commande déclanche en même temps l'autorisation d'envoyer les messages contenus dans la pile
		_0_Communication_Give_Sending_Clearance();
		break;

	case PING:
		//A la demande d'une carte, on répond par un PONG
		_2_Comm_Send_PONG(RS485_port);
		_0_Communication_Give_Sending_Clearance();
		break;


	case END_COMMUNICATION:
		_0_Communication_Give_Sending_Clearance();
		break;


	default:
		break;
	}
}


/*****************************************************************************
 ** Function name:		_2_Comm_RX_Id_Robot
 **
 ** Descriptions:		Receive Robot id
 **
 ** parameters:			Recieved message
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_RX_Id_Robot(struct Communication_Trame* datas)
{
	struct Com_Def_Id_Robot id;
	COPYDATA2(datas->Data, id);

	ID_Robot = id.Numero_Robot;
}


byte _2_Comm_Get_Robot_ID()
{
	return ID_Robot;
}

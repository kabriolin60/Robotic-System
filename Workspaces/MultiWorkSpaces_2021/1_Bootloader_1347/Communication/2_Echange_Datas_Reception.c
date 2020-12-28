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
#include "Bootloader_Data.h"

static long Nb_Messages_Interpretes = 0;

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

struct Communication_Trame received_trame;
void _2_Communication_Interprete_message(struct Communication_Trame* trame)
{
	Nb_Messages_Interpretes++;

	switch(trame->Instruction)
	{
	case BOOTLOADER:
		Reception_Trame_Bootloader(&received_trame);
		//Envoi de l'ACK
		_2_Comm_Send_Bootloader_ACK(RS485_port);
		break;


	default:
		break;
	}
}

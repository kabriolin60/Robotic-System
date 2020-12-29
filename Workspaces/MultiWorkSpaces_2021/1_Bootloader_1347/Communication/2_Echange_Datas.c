/*
 * 2_Echange_Datas.c
 *
 *  Created on: 27 dec. 2020
 *      Author: kabri
 */

#include "2_Echange_Datas.h"
#include "2_Echange_Datas_Reception.h"
#include "1_Trame_Communication.h"
#include "0_Communication.h"


struct Communication_Trame trame_echange;

/*****************************************************************************
 ** Function name:		_2_Communication_Init
 **
 ** Descriptions:		Init de la comm niveau 2
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Communication_Init()
{

}


/*****************************************************************************
 ** Function name:		_2_Comm_Send_Bootloader_ACK
 **
 ** Descriptions:		Fonction d'envoie d'un ACK de la part du bootloader
 **
 ** parameters:			canal de communication
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Comm_Send_Bootloader_ACK(enum enum_canal_communication canal)
{
	trame_echange.Instruction = BOOTLOADER_ACK;
	trame_echange.Slave_Adresse = PC;

	trame_echange.Length = 0;
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	//Envoi avec attente d'ACK
	_1_Communication_Create_Trame(&trame_echange, canal);
}

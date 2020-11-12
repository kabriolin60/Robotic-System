/*
 * 2_Echange_Datas_Reception.c
 *
 *  Created on: 25 avr. 2020
 *      Author: kabri
 */

#include "2_Echange_Datas_Reception.h"
#include "0_Communication.h"
#include "1_Trame_Communication.h"
#include "Type_Declaration.h"
#include "2_Echange_Datas.h"


//#include "2_Echange_Datas.h"
//#include "0_Infos.h"

extern QueueHandle_t _1_xQueue_Message_Receive; 				//Queue Recevant les messages des canaux de communication
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
	//Tache de décodage des donnees messages reçus
	xTaskCreate(_2_Communication_RX_Lectures_Messages, (char *) "1_Com_Lecture_RX", 100, _1_xQueue_Message_Receive, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
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
static TO_AHBS_RAM3 struct Communication_Trame received_trame;

struct Com_Position_Robot Com_Position_Robot;
__attribute__((optimize("O3"))) void _2_Communication_RX_Lectures_Messages(void *pvParameters)
{
	if(pvParameters == NULL)
		Task_Delete_Current;

	for(;;)
	{
		//Reception d'un message depuis une Queue
		if(xQueueReceive(pvParameters, &received_trame, portMAX_DELAY))
		{
			Nb_Messages_Interpretes++;

			switch(received_trame.Instruction)
			{
			case BOOTLOADER:
				Reception_Trame_Bootloader(&received_trame);
				break;

			case PING:
				//A la demande d'une carte, on répond par un PONG
				_2_Comm_Send_PONG(RS485_port);

			case PONG:
				//Reception d'un Pong
				Nb_PONG_recus++;
				break;

			default:
				break;
			}
		}
	}
}

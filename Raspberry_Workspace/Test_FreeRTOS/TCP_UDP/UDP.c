/*
 * UDP.c
 *
 *  Created on: 11 nov. 2020
 *      Author: pi
 */

#include "UDP.h"

#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"

struct st_POSITION_ROBOT rob_pos;


void error(const char *msg)
{
	//perror(msg);
	//exit(0);
	vTaskDelay(pdMS_TO_TICKS(250));
}


int sock;
socklen_t fromlen;
struct sockaddr_in from;

void Init_UDP(int port)
{
	int length;
	struct sockaddr_in server;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		error("Opening socket");
	length = sizeof(server);
	bzero(&server, length);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *)&server, length) < 0)
		error("binding");
	fromlen = sizeof(struct sockaddr_in);

	printf("Serveur UDP en écoute sur le port %d !\n", port);



	rob_pos.Position_X = 1000;
	rob_pos.Position_Y = 1500;
}



struct Communication_Trame trame_echange;




/*****************************************************************************
 ** Function name:		_2_Comm_Send_Robot_Position
 **
 ** Descriptions:		Fonction d'envoie de la position d'un Robot
 **
 ** parameters:			Struct st_POSITION_ROBOT
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
static struct Com_Position_Robot Com_Position_Robot;

void _2_Comm_Send_Robot_Position(struct st_POSITION_ROBOT rob_pos)
{
	printf("Send Robot Position\n");
	trame_echange.Instruction = REPONSE_ROBOT_POSITION;
	trame_echange.Slave_Adresse = ALL_CARDS;

	//Position du robot
	Com_Position_Robot.Position_X = (short)(rob_pos.Position_X * 10);
	Com_Position_Robot.Position_Y = (short)(rob_pos.Position_Y * 10);
	Com_Position_Robot.Angle = (short)(rob_pos.Angle_Deg * 100);
	Com_Position_Robot.Numero_Robot = 1;

	trame_echange.Length = COPYDATA(Com_Position_Robot, trame_echange.Data);
	trame_echange.XBEE_DEST_ADDR = ALL_XBEE;

	_1_Communication_Create_Trame(&trame_echange);
}


/*****************************************************************************
 ** Function name:		_1_Communication_Create_Trame
 **
 ** Descriptions:		Fonction de formalisation d'une trame de communication en un message à envoyer à une Queue
 **
 ** parameters:			Pointeur vers la Communication_Trame
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		pdTRUE : message mis en Queue
 ** 					pdFAIL : message NON envoyé
 **
 *****************************************************************************/
static struct Communication_Message Message_To_Send;
void _1_Communication_Create_Trame(struct Communication_Trame *pMessage_to_send)
{
	//Le bit de synchro issu de l'EventGroup, est pris par la couche 2

	Message_To_Send.Data[0] = 0x7E;                         //Xbee API start byte

	int length = pMessage_to_send->Length + 7;

	Message_To_Send.length = pMessage_to_send->Length + 11;

	Message_To_Send.Data[1] = (uint8_t)(length >> 8);                  //length high
	Message_To_Send.Data[2] = (uint8_t)(length & 0xFF);                //length low

	Message_To_Send.Data[3] = 0x01;                         //Frame type: Tx, 16 bits addr
	Message_To_Send.Data[4] = 0x01;                         //Frame ID

	Message_To_Send.Data[5] = (uint8_t)((int)pMessage_to_send->XBEE_DEST_ADDR >> 8);        //add high
	Message_To_Send.Data[6] = (uint8_t)((int)pMessage_to_send->XBEE_DEST_ADDR & 0xFF);      //add low

	Message_To_Send.Data[7] = 0x00;                         //Option

	uint8_t index = 0;
	//Datas
	{
		Message_To_Send.Data[8] = (uint8_t)(pMessage_to_send->Instruction);

		Message_To_Send.Data[9] = (uint8_t)(pMessage_to_send->Slave_Adresse);

		for (index = 0; index < pMessage_to_send->Length; index++)
		{
			Message_To_Send.Data[10 + index] = pMessage_to_send->Data[index];
		}
	}

	short API_CRC = 0;
	for (index = 3; index < 10 + pMessage_to_send->Length; index++)
	{
		API_CRC += Message_To_Send.Data[index];
	}

	API_CRC &= 0xFF;
	API_CRC = (uint8_t)(0xFF - API_CRC);

	Message_To_Send.Data[index] = (uint8_t)(API_CRC);

	//Envoi de la trame
	sendto(sock, &Message_To_Send, Message_To_Send.length + 1 , 0, (struct sockaddr *)&from, fromlen);
}



void Task_UDP(void* pvparameter)
{
	int n;
	char buf[1024];

	while (1)
	{
		while ((n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen)) == -1 )
		{
			vTaskDelay(pdMS_TO_TICKS(5));
		}
		//n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
		//if (n < 0) error("recvfrom");
		//write(1, buf, n);
		//n = sendto(sock, "Etat PiRobot", 12, 0, (struct sockaddr *)&from, fromlen);
		//if (n < 0) error("sendto");

		_2_Comm_Send_Robot_Position(rob_pos);
	}
}



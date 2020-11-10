/*
 * Init.c
 *
 *  Created on: 1 févr. 2014
 *      Author: Fixe
 */

#include "Init.h"

struct Communication_Trame
{
	signed char Slave_Adresse;							//Adresse de la carte ciblée //ALL_CARDS (-1) pour toutes les cartes
	unsigned char Length;								//Nombre d'octets dans le tableau de data
	unsigned char Data[64];
};

struct Communication_Trame test;

xQueueHandle QueueEchangeRXTrame_RS485;
xQueueHandle QueueEchangeRXTrame_RS485_2;
xQueueHandle QueueEchangeRXTrame_RS485_3;
xQueueHandle QueueEchangeRXTrame_RS485_4;


void TaskTest(void *pv)
{
	QueueEchangeRXTrame_RS485 = xQueueCreate(11, sizeof(struct Communication_Trame));
	QueueEchangeRXTrame_RS485_2 = xQueueCreate(11, sizeof(struct Communication_Trame));
	QueueEchangeRXTrame_RS485_3 = xQueueCreate(11, sizeof(struct Communication_Trame));
	QueueEchangeRXTrame_RS485_4 = xQueueCreate(11, sizeof(struct Communication_Trame));

	xQueueSend(QueueEchangeRXTrame_RS485, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_2, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_3, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_4, &test, ( portTickType )  50);

	xQueueSend(QueueEchangeRXTrame_RS485, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_2, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_3, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_4, &test, ( portTickType )  50);

	xQueueSend(QueueEchangeRXTrame_RS485, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_2, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_3, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_4, &test, ( portTickType )  50);

	xQueueSend(QueueEchangeRXTrame_RS485, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_2, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_3, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_4, &test, ( portTickType )  50);

	xQueueSend(QueueEchangeRXTrame_RS485, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_2, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_3, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_4, &test, ( portTickType )  50);

	xQueueSend(QueueEchangeRXTrame_RS485, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_2, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_3, &test, ( portTickType )  50);
	xQueueSend(QueueEchangeRXTrame_RS485_4, &test, ( portTickType )  50);


	int i;
	for(i = 0; i< 3; i++)
	{
		xQueueReceive(QueueEchangeRXTrame_RS485, &test, ( portTickType ) 50 );
		xQueueReceive(QueueEchangeRXTrame_RS485_2, &test, ( portTickType ) 50 );
		xQueueReceive(QueueEchangeRXTrame_RS485_3, &test, ( portTickType ) 50 );
		xQueueReceive(QueueEchangeRXTrame_RS485_4, &test, ( portTickType ) 50 );
	}

	Task_Delete_Current;
}

void Init_Robot(void)
{
	GPIOSetDir(0, 22, 1); //Led 0
	GPIOSetDir(LED_1_PORT, LED_1_BIT, 1); //Led 1
	GPIOSetDir(LED_2_PORT, LED_2_BIT, 1); //Led 2

	xTaskCreate( TaskTest, "test", 550, NULL, tskIDLE_PRIORITY, NULL );
}

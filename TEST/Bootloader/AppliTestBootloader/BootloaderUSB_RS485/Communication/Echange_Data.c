/*
 * Echange_Data.c
 *
 *  Created on: 1 févr. 2014
 *      Author: adrien
 */

#include "Echange_Data.h"
#include "cdc_vcom.h"
#include "cdc_main.h"

#include "sbl_iap.h"

xQueueHandle QueueEchangeTX_RS485;
xQueueHandle QueueEchangeRXTrame_RS485;

xQueueHandle QueueEchangeTX_USB;
xQueueHandle QueueEchangeRXTrame_USB;

/*
 * Fonction d'Init des moyens de communication
 */
void Init_Echange_Data(void)
{
	QueueEchangeTX_RS485 = xQueueCreate(5, sizeof(struct Communication_Trame));

	xTaskCreate( uTask_ReceptionDatas, "RX Data", 350, NULL, tskIDLE_PRIORITY + 3, NULL );

	QueueEchangeTX_USB = xQueueCreate(5, sizeof(struct Communication_Trame));


	/*
	 * Init UART1 as RS485
	 */

	FIFO_FLUSH(&Buffer_UART1);

	RS485_Init_IO();
	RS485_Slave_Mode();
	Init_UART_1(BAUDRATE_RS485, PORT_RS485_IA);
	UART_1_Enable_Interrupts();

	//Flush USB buffer
	FIFO_FLUSH(&Buffer_UART_USB);

	//Init le module USB
	USBINIT();

	/*
	 * USB Receive
	 */
	xTaskCreate( Task_USBReceived, "USB RX", 200, NULL, tskIDLE_PRIORITY, NULL );

	/*
	 * USB TX
	 */
	xTaskCreate(uTask_SendUSB, "TX USB", 130, NULL, tskIDLE_PRIORITY + 4, NULL );	//300
}
/*-----------------------------------------------------------*/


void uTask_SendUSB(void *pvParameter)
{
	struct Communication_Trame temp;

	for(;;)
	{
		//S'il y a des messages à envoyer en USB
		if(uxQueueMessagesWaiting(QueueEchangeTX_USB))
		{
			while(uxQueueMessagesWaiting(QueueEchangeTX_USB))
			{
				(void)memset(&temp, 0, sizeof(struct Communication_Trame));

				if(xQueueReceive(QueueEchangeTX_USB, &temp, ( TickType_t ) 0 ))
				{
					Communication_Send_Trame(temp, &vcom_write);
				}
			}
		}
		Task_Delay(2);
	}
}



/*
 * Fonction de décodage des messages recus par RS485
 */
void Communication_Decodage_Trame(struct Communication_Trame *received)
{
	//La trame concerne-t-elle cette carte
	if(received->Slave_Adresse == ALL_CARDS || received->Slave_Adresse == IA_BOARD)
	{
		GPIOSetValue(LED_0_PORT, LED_0_BIT, 1);

		switch(received->Instruction)
		{
		case RESET_CMD:
			NVIC_SystemReset();
			break;

		case BOOTLOADER:
			Reception_Bootloader(received);
			break;

		default:
			break;
		}
	}
}
/*-----------------------------------------------------------*/



/*
 * Fct de reception d'une instruction de type bootloader
 */
void Reception_Bootloader(struct Communication_Trame *trame)
{
	Traitement_Data(trame->Data);
	Bootloader_Send_ACK();
}


/*
 * Fonction d'envoie un message de Debug vers le PC
 */
void DebugLogger(char* str, byte color, xQueueHandle QueueToAdd)
{
	struct Logger_Debug log;

	log.Color = color;

	UInt16 stringlength;
	//Calcul la longueur de la chaine
	stringlength = strlen(str);

	struct Communication_Trame trame;
	trame.Instruction = LOGGER_DEBUG;
	trame.Slave_Adresse = PC;

	if(stringlength < sizeof(log.Text))
	{
		log.Nombre_Carateres = COPYSTRING(str, log.Text);

		trame.Length = COPYDATA(log, trame.Data);

		xQueueSend(QueueToAdd, &trame, ( portTickType ) 50);
	}else
	{
		//Plus d'une trame à envoyer pour tout le texte
		while(stringlength)
		{
			log.Nombre_Carateres = COPYSTRING(str, log.Text);

			trame.Length = COPYDATA(log, trame.Data);

			xQueueSend(QueueToAdd, &trame, ( portTickType ) 50);

			//Suite du message
			str += log.Nombre_Carateres;
			stringlength = strlen(str);
		}
	}
}
//*-----------------------------------------------------------*/



void Bootloader_Send_ACK(void)
{
	struct Communication_Trame trame;
	trame.Instruction = BOOTLOADER_ACK;
	trame.Slave_Adresse = PC;
	trame.XBEE_DEST_ADDR = 0xFFFF;
	trame.Length = 0;

	Communication_Send_Trame(trame, &vcom_write);

	//xQueueSend(QueueEchangeTX_RS485, &trame, ( portTickType ) 50);
	//xQueueSend(QueueEchangeTX_USB, &trame, ( portTickType ) 50);
}


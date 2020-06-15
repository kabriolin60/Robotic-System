/*
 * Communication_NG.c
 *
 *  Created on: 1 févr. 2014
 *      Author: Fixe
 */

#include "Communication_NG.h"
#include "Communication_struct.h"
#include "Includes.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "FIFO.h"

__attribute__ ((__section__(".bss.$RAM2"))) volatile struct st_FIFO Buffer_UART1 = { 0 };
__attribute__ ((__section__(".bss.$RAM2"))) volatile struct st_FIFO Buffer_UART_USB = { 0 };

UInt32 Erreurs = 0;
UInt32 Messages_recus = 0;
UInt32 Messages_envoyes = 0;
UInt32 MissedData = 0;

#define USE_RS485_RECEPTION_FLAG 0

#if USE_RS485_RECEPTION_FLAG == 1
unsigned char RS485_RECEPTION = 0;
#endif


void Add_Trame_ToTXQueue(xQueueHandle QueueToAdd, struct Communication_Trame *send, TickType_t xTicksToWait)
{
	xQueueSend(QueueToAdd, send, ( portTickType )  xTicksToWait);
}


void Communication_Send_Trame(struct Communication_Trame send, byte *SendFct(byte))
{
	//Envoie par Xbee
	byte data[COMMUNICATION_TRAME_MAX_DATA + 11];
	data[0] = 0x7E;							//Xbee API start byte

	short length = send.Length + 7;

	data[1] = length >> 8;					//length high
	data[2] = length & 0xFF;				//length low

	data[3] = 0x01;							//Frame type: Tx, 16 bits addr
	data[4] = 0x01;							//Frame ID

	data[5] = send.XBEE_DEST_ADDR >> 8;		//add high
	data[6] = send.XBEE_DEST_ADDR & 0xFF;	//add low

	data[7] = 0x00;							//Option

	byte index = 0;
	//Datas
	{
		data[8] = send.Instruction;

		data[9] = send.Slave_Adresse;

		for(index = 0; index < send.Length; index++)
		{
			data[10 + index] = send.Data[index];
		}
	}

	short API_CRC = 0;
	for(index = 3; index < 10 + send.Length; index++)
	{
		API_CRC += data[index];
	}

	API_CRC &= 0xFF;
	API_CRC = 0XFF - API_CRC;

	data[index] = API_CRC;

	if(SendFct != vcom_write)
	{
		//Envoie de la trame
		for(index = 0; index < 10 + send.Length + 1; index++)
		{
			SendFct(data[index]);
		}
	}else
	{
		vcom_write(&data, 10 + send.Length + 1);
	}

	Messages_envoyes++;
}


void uTask_ReceptionDatas(void *pvParameter)
{
	short temps_sans_com = 50;

	for(;;)
	{
		if(FIFO_NB_DATA(&Buffer_UART1) >= 7)
		{
			Communication_Reception_Data(&Buffer_UART1);
			temps_sans_com = 0;
		}else if(FIFO_NB_DATA(&Buffer_UART_USB) >= 7)
		{
			Communication_Reception_Data(&Buffer_UART_USB);
			temps_sans_com = 0;
		}else
		{
			Task_Delay(1);
			temps_sans_com++;
		}

		if(temps_sans_com < 50)
		{
			GPIOSetValue(LED_0_PORT, LED_0_BIT, 1);
		}else
		{
			temps_sans_com = 100;
			GPIOSetValue(LED_0_PORT, LED_0_BIT, 0);
		}
	}
}
/*-----------------------------------------------------------*/

void Communication_Reception_Data(void *pvParameters)
{
	struct Communication_Trame received;
	unsigned char API_start = 0, API_LENGTH_HIGH = 0, API_LENGTH_LOW = 0, API_DUMMY, API_FRAME_TYPE = 0;
	unsigned short API_LENGTH = 0;
	unsigned short crc = 0;
	unsigned char index = 0, rx_crc = 0;

	struct st_FIFO *buffer = pvParameters;

	if(FIFO_NB_DATA(buffer) < 7)
		return;

	//Reception En-tête API
	API_start = 0;

	while(API_start != 0x7E)
	{
		//Attente reception en-tête de trame API
		if(FIFO_NB_DATA(buffer) < 7)
		{
			return;
		}
		FIFO_READ_ELEMENT(buffer, &API_start);
	}

	FIFO_READ_ELEMENT(buffer, &API_LENGTH_HIGH);
	FIFO_READ_ELEMENT(buffer, &API_LENGTH_LOW);
	API_LENGTH = API_LENGTH_HIGH;
	API_LENGTH <<= 8;
	API_LENGTH += API_LENGTH_LOW;

	if(API_LENGTH > COMMUNICATION_TRAME_MAX_DATA + 11)
		return;

	//Packet Type
	FIFO_READ_ELEMENT(buffer, &API_FRAME_TYPE);
	crc += API_FRAME_TYPE;

	if(API_FRAME_TYPE == 0x89)
	{
		//Tx status
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;

		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;

		if(API_DUMMY == 00)
		{
			//Mess tx succes
			//RxXbeeAck = OK;
		}

		//API CRC
		FIFO_READ_ELEMENT(buffer, &rx_crc);
		return;
	}else if(API_FRAME_TYPE == 0x81)
	{
		//Reception d'une data Xbee
		while(FIFO_NB_DATA(buffer) < API_LENGTH - 1)
		{
			Task_Delay(1);
		}

		//Il s'agit d'une reception Xbee
		received.XBEE_DEST_ADDR = 0;

		//Tx Address
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		received.XBEE_DEST_ADDR = API_DUMMY;
		received.XBEE_DEST_ADDR <<= 8;
		crc += API_DUMMY;

		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		received.XBEE_DEST_ADDR += API_DUMMY;
		crc += API_DUMMY;

		//RSSI
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;

		//Option
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;

	}else if(API_FRAME_TYPE == 0x01)
	{
		//Reception d'une data RS485 ou USB
		while(FIFO_NB_DATA(buffer) < API_LENGTH - 1)
		{
			Task_Delay(1);
		}

		//Il s'agit d'une reception NON Xbee
		received.XBEE_DEST_ADDR = 0;

		//Frame ID
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;

		//Address TX High
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;

		//Address TX Low
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;

		//Option
		FIFO_READ_ELEMENT(buffer, &API_DUMMY);
		crc += API_DUMMY;
	}


	//Reception datas
	{
		FIFO_READ_ELEMENT(buffer, &received.Instruction);
		crc += received.Instruction;

		FIFO_READ_ELEMENT(buffer, &received.Slave_Adresse);
		crc += received.Slave_Adresse;

		received.Length = API_LENGTH - 7;

		if(received.Length <= COMMUNICATION_TRAME_MAX_DATA)
		{
			while(FIFO_NB_DATA(buffer) < received.Length + 1)
			{
				Task_Delay(1);
			}

			//Reception des data
			for(index = 0; index < received.Length; index++)
			{
				FIFO_READ_ELEMENT(buffer, &received.Data[index]);
				crc += received.Data[index];
			}

			//API CRC
			FIFO_READ_ELEMENT(buffer, &rx_crc);

			//Contrôle CRC
			crc &= 0xFF;
			crc = 0xFF - crc;

			if(crc == rx_crc)
			{
				if(received.Slave_Adresse == ALL_CARDS || received.Slave_Adresse == ADRESSE_CARTE || received.Slave_Adresse == PC)
				{
					Communication_Decodage_Trame(&received);
					Messages_recus++;
				}
			}else
			{
				Erreurs++;
				Communication_Envoie_Nombre_Messages();
			}
		}else
		{
			Erreurs++;
			Communication_Envoie_Nombre_Messages();
		}
	}
}

void Communication_Lecture_Trames(void *pvParameters)
{
	struct Communication_Trame rxtrame;

	for(;;)
	{
		if(xQueueReceive(QueueEchangeRXTrame_RS485, &rxtrame, ( portTickType ) 50 ))
		{
			Communication_Decodage_Trame(&rxtrame);
		}else
		{
			GPIOSetValue(LED_0_PORT, LED_0_BIT, 0);
		}
	}

	for(;;)
	{

	}
}

void Communication_Reception_Data_USB(void *pvParameters)
{
	struct Communication_Trame received;
	unsigned char start1 = 0, start2 = 0;
	unsigned short crc = 0, rx_crc = 0;
	unsigned char index = 0;

	unsigned int boucle_secu = 0;

	struct st_FIFO *buffer = pvParameters;

	FIFO_FLUSH(buffer);

	for(;;)
	{
		boucle_secu = 0;
		start1 = 0;
		start2 = 0;

		while(start2 != 0xFF)
		{
			boucle_secu = 0;
			start1 = 0;
			start2 = 0;

			//Attente reception en-tête de trame
			while(FIFO_NB_DATA(buffer) < 6)
			{
				Task_Delay(1);
			}

			FIFO_READ_ELEMENT(buffer, &start1);

			while(start1 != 0xFF)
			{
				FIFO_READ_ELEMENT(buffer, &start1);

				boucle_secu++;
				if(boucle_secu > 60)
				{
					Task_Delay(1);
					break;
				}
			}
			FIFO_READ_ELEMENT(buffer, &start2);
		}
		//En-tete recue correctement

		crc = start1 + start2;

		//Attente reception en-tête de trame
		while(FIFO_NB_DATA(buffer) < 4)
		{
			Task_Delay(1);
		}

		FIFO_READ_ELEMENT(buffer, &received.Instruction);
		crc += received.Instruction;

		FIFO_READ_ELEMENT(buffer, &received.Slave_Adresse);
		crc += received.Slave_Adresse;

		FIFO_READ_ELEMENT(buffer, &received.Length);
		crc += received.Length;

		if(received.Length <= COMMUNICATION_TRAME_MAX_DATA)
		{
			boucle_secu = 0;
			//Attente reception data
			while(FIFO_NB_DATA(buffer) < received.Length + 1)
			{
				boucle_secu++;
				Task_Delay(1);
				if(boucle_secu > 60)
				{
					Task_Delay(1);
					break;
				}
			}

			if(boucle_secu < 60)
			{
				//Reception des data
				for(index = 0; index < received.Length; index++)
				{
					boucle_secu = 0;
					while(!FIFO_NB_DATA(buffer))
					{
						boucle_secu++;
						Task_Delay(1);
						if(boucle_secu > 60)
						{
							Task_Delay(1);
							break;
						}
					}

					FIFO_READ_ELEMENT(buffer, &received.Data[index]);
					crc += received.Data[index];
				}

				boucle_secu = 0;

				while(FIFO_NB_DATA(buffer) < 1)
				{
					boucle_secu++;
					Task_Delay(1);
					if(boucle_secu > 60)
					{
						Task_Delay(1);
						break;
					}
				}

				//Reception du CRC
				FIFO_READ_ELEMENT(buffer, &rx_crc);

				//Contrôle CRC
				crc &= 0xFF;
				crc = (unsigned char)crc;

				if(crc == rx_crc)
				{
					if(received.Slave_Adresse == ALL_CARDS || received.Slave_Adresse == ADRESSE_CARTE)
					{
						xQueueSend(QueueEchangeRXTrame_USB, &received, ( portTickType )  50);
						Messages_recus++;
					}
				}else
				{
					Erreurs++;
					Communication_Envoie_Nombre_Messages();
				}
			}
		}
	}

	for(;;)
	{

	}
}

void Communication_Lecture_Trames_USB(void *pvParameters)
{
	struct Communication_Trame rxtrame;

	for(;;)
	{
		if(xQueueReceive(QueueEchangeRXTrame_USB, &rxtrame, ( portTickType ) 50 ))
		{
			Communication_Decodage_Trame(&rxtrame);
		}else
		{
			GPIOSetValue(LED_0_PORT, LED_0_BIT, 0);
		}
	}

	for(;;)
	{

	}
}


__attribute__((optimize("O0"))) byte SendRS485Byte(byte data)
{
	//Passe en TX
	RS485_Master_Mode();

	UART_1_Send_Byte(data);

	//byte i;
	//for(i = 0; i < 50; i++);

	//Passe en RX
	RS485_Slave_Mode();

	return data;
}


__attribute__((optimize("O0"))) void UART1_IRQHandler (void)
{
	if(FIFO_ADD_ELEMENT(&Buffer_UART1, UART_1_Read_Byte()))
	{

	}else
	{
		RS485_Slave_Mode(); //pour le bluff d'avoir un code ici
		MissedData++;
	}
	NVIC_ClearPendingIRQ(UART1_IRQn);
}


void Communication_Envoie_Nombre_Messages(void)
{
	char str[62];
	sprintf(&str, "IA: Tx: %d, Rx: %d, Err: %d, Missed: %d\r", Messages_envoyes, Messages_recus, Erreurs, MissedData);
	DebugLogger(&str, 0, QueueEchangeTX_DEBUG);
}

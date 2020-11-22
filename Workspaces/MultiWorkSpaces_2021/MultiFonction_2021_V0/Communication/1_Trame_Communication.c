/*
 * _1_Trame_Communication.c
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#include "0_Communication.h"
#include "1_Trame_Communication.h"
#include "semphr.h"
#include "event_groups.h"
#include "0_Event_Group.h"

#include <cr_section_macros.h>
#include <Init.h>

QueueHandle_t _1_xQueue_Message_Receive; 				//Queue Recevant les messages des canaux de communication
extern QueueHandle_t _1_xQueue_Message_TO_Send;					//Queue Recevant les messages à envoyer pour TOUS les cannaux

long Nb_Messages_recus = 0;
long Nb_Erreurs_com = 0;

//Canal de tracalyser
traceString MyChannel_Recompo;
extern traceString MyChannel_RX_RS485;

/*****************************************************************************
 ** Function name:		_1_Communication_Init
 **
 ** Descriptions:		Fonction d'initialisation des actions de communication de niveau 1
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Communication_Init(void)
{
	_1_Communication_Create_Queues_Semaphores();

	//Tache de décodage des donnees recues par differentes FIFO
	xTaskCreate(_1_Communication_Recomposition_Rx, (char *) "1_Com_Recompo_Rx", 320, _1_xQueue_Message_Receive, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);

#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	MyChannel_Recompo = xTraceRegisterString("Recompo_Mess");
#endif
}


/*****************************************************************************
 ** Function name:		_1_Communication_Create_Queues_Semaphores
 **
 ** Descriptions:		Fonction de création des Queues et sémaphores de communication de niveau 1
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Communication_Create_Queues_Semaphores(void)
{
	//Création de la Queue de reception pour les datas
	_1_xQueue_Message_Receive = xQueueCreate( 5, sizeof( struct Communication_Trame )); //Queue contenant les messages reçus
	vQueueAddToRegistry( _1_xQueue_Message_Receive, "_1_xQue_Mess_Receive");

	//Mise à dispo du bit de liberation de la trame d'envoi et de reception
	xEventGroupSetBits(_0_Comm_EventGroup, eGROUP_SYNCH_TxTrameDispo | eGROUP_SYNCH_RxTrameDispo);
}


/*****************************************************************************
 ** Function name:		_1_Communication_Wait_To_Send
 **
 ** Descriptions:		Fonction d'attente que la trame soit disponible pour envoyer un nouveau message
 ** 					Cette fonction se trouve en niveau 1, mais est appellée par le niveau 2
 ** 					Le bit de mise à disposition est activé par la mise en Queue de la trame finale, permettant de créer un nouveau message à envoyer
 **
 ** parameters:			Temps d'attente maximum
 ** Returned value:		None
 **
 *****************************************************************************/
BaseType_t _1_Communication_Wait_To_Send(TickType_t xTicksToWait)
{
	EventBits_t uxBits;
	uxBits = xEventGroupWaitBits(_0_Comm_EventGroup,   /* The event group being tested. */
			eGROUP_SYNCH_TxTrameDispo, /* The bits within the event group to wait for. */
			pdTRUE,        /* Clear bits before returning. */
			pdTRUE,        /* Wait for ALL bits to be set */
			xTicksToWait );/* Wait a maximum of xTicksToWait for either bit to be set. */

	if( ( uxBits & (eGROUP_SYNCH_TxTrameDispo ) ) == ( eGROUP_SYNCH_TxTrameDispo ) )
	{
		return pdTRUE;
	}
	//delay depassé
	return pdFALSE;
}


/*****************************************************************************
 ** Function name:		_1_Communication_Free_Send_Bit
 **
 ** Descriptions:		Fonction de libération du Bit, indiquant qu'un message peut être envoyé
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Communication_Free_Send_Bit(void)
{
	xEventGroupSetBits(_0_Comm_EventGroup,    /* The event group being updated. */
			eGROUP_SYNCH_TxTrameDispo );/* The bits being set. */
}


/*****************************************************************************
 ** Function name:		_1_Communication_Wait_To_Receive
 **
 ** Descriptions:		Fonction d'attente que la trame soit disponible pour recevoir un nouveau message
 ** 					Le bit de mise à disposition est activé par la mise en Queue de la trame finale, permettant de créer un nouveau message
 **
 ** parameters:			Temps d'attente maximum
 ** Returned value:		None
 **
 *****************************************************************************/
BaseType_t _1_Communication_Wait_To_Receive(TickType_t xTicksToWait)
{
	EventBits_t uxBits;
	uxBits = xEventGroupWaitBits(_0_Comm_EventGroup,   /* The event group being tested. */
			eGROUP_SYNCH_RxTrameDispo, /* The bits within the event group to wait for. */
			pdTRUE,        /* Clear bits before returning. */
			pdTRUE,        /* Wait for ALL bits to be set */
			xTicksToWait );/* Wait a maximum of xTicksToWait for either bit to be set. */

	if( ( uxBits & (eGROUP_SYNCH_RxTrameDispo ) ) == ( eGROUP_SYNCH_RxTrameDispo ) )
	{
		return pdTRUE;
	}
	//delay depassé
	return pdFALSE;
}


/*****************************************************************************
 ** Function name:		_1_Communication_Free_Receive_Bit
 **
 ** Descriptions:		Fonction de libération du Bit, indiquant qu'un message peut être envoyé
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Communication_Free_Receive_Bit(void)
{
	xEventGroupSetBits(_0_Comm_EventGroup,    /* The event group being updated. */
			eGROUP_SYNCH_RxTrameDispo );/* The bits being set. */
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
static TO_AHBS_RAM3 struct Communication_Message Message_To_Send;
struct Communication_Message* _1_Communication_Create_Message(struct Communication_Trame *pMessage_to_send)
{
	//Le bit de synchro issu de l'EventGroup, est pris par la couche 2

	Message_To_Send.Data[0] = 0x7E;                         //Xbee API start byte

	int length = pMessage_to_send->Length + 7;

	Message_To_Send.length = pMessage_to_send->Length + 11;

	Message_To_Send.Data[1] = (byte)(length >> 8);                  //length high
	Message_To_Send.Data[2] = (byte)(length & 0xFF);                //length low

	Message_To_Send.Data[3] = 0x01;                         //Frame type: Tx, 16 bits addr
	Message_To_Send.Data[4] = 0x01;                         //Frame ID

	Message_To_Send.Data[5] = (byte)((int)pMessage_to_send->XBEE_DEST_ADDR >> 8);        //add high
	Message_To_Send.Data[6] = (byte)((int)pMessage_to_send->XBEE_DEST_ADDR & 0xFF);      //add low

	Message_To_Send.Data[7] = 0x00;                         //Option

	byte index = 0;
	//Datas
	{
		Message_To_Send.Data[8] = (byte)(pMessage_to_send->Instruction);

		Message_To_Send.Data[9] = (byte)(pMessage_to_send->Slave_Adresse);

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
	API_CRC = (byte)(0xFF - API_CRC);

	Message_To_Send.Data[index] = (byte)(API_CRC);

	return &Message_To_Send;
}


BaseType_t _1_Communication_Create_Trame(struct Communication_Trame *pMessage_to_send, enum enum_canal_communication canal)
{
	//Mise en forme des datas
	(void)_1_Communication_Create_Message(pMessage_to_send);

	//Ajoute au message le cannal de communication à utilisre
	Message_To_Send.canal_communication = canal;

	//Envoi de la trame
	if(xQueueSend(_1_xQueue_Message_TO_Send, &Message_To_Send, ms_to_tick(10)))
	{
		//Libère le bit de synchro pour pouvoir envoyer un autre message
		_1_Communication_Free_Send_Bit();
		return pdPASS;
	}
	//Libère le bit de synchro pour pouvoir envoyer un autre message
	_1_Communication_Free_Send_Bit();
	return pdFAIL;
}


static TO_AHBS_RAM3 struct Communication_Trame received_trame;
/*****************************************************************************
 ** Function name:		_1_Communication_Create_Trame_From_Buffer
 **
 ** Descriptions:		Fonction de lecture d'un Buffer afin d'en refaire une trame
 **
 ** parameters:			Pointeur vers le Buffer
 ** 					Queue à la quelle ajouter le message
 ** Returned value:		None
 **
 *****************************************************************************/
static TO_AHBS_RAM3 byte Data_rx[COMMUNICATION_TRAME_MAX_DATA + 11];
__attribute__((optimize("O0"))) BaseType_t _1_Communication_Create_Trame_From_Buffer(RINGBUFF_T *RingBuff)
{
	if(_1_xQueue_Message_Receive == NULL)
		return pdFAIL;

	if(RingBuff == NULL)
		return pdFAIL;

	//_1_Communication_Wait_To_Receive(ms_to_tick(50));

	byte API_start = 0;
	static short API_LENGTH = 0;
	short crc = 0;
	byte index = 0, rx_crc = 0;

	//Reception En-tête API
	static byte boucle = 0;
	RingBuffer_Pop(RingBuff, &API_start);

	boucle = 0;
	while (API_start != 0x7E)
	{
		//Attente reception en-tête de trame API
		if(RingBuffer_Pop(RingBuff, &API_start))
		{
			boucle++;
			if(boucle > 5)
			{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
				vTracePrint(MyChannel_Recompo, "Start not Rx");
#endif
				//_1_Communication_Free_Receive_Bit();
				Nb_Erreurs_com++;
				return pdFAIL;
			}
		}else
		{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
			vTracePrint(MyChannel_Recompo, "Start not Rx2");
#endif
			//_1_Communication_Free_Receive_Bit();
			Nb_Erreurs_com++;
			return pdFAIL;
		}
	}

	//Le start byte est recu, on demarre la lecture de la trame
	RingBuffer_PopMult(RingBuff, &Data_rx, 3);

	API_LENGTH = Data_rx[0];
	API_LENGTH <<= 8;
	API_LENGTH += Data_rx[1];

	if (API_LENGTH > COMMUNICATION_TRAME_MAX_DATA + 11)
	{
		//_1_Communication_Free_Receive_Bit();
		Nb_Erreurs_com++;
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
		vTracePrint(MyChannel_Recompo, "API LENGTH ERROR");
#endif
		return pdFAIL;
	}

	//Packet Type
	crc += Data_rx[2];
	if (Data_rx[2] == 0x89)
	{
		RingBuffer_PopMult(RingBuff, &Data_rx, 3);
		/*
		//Tx status
		FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
		crc += API_DUMMY;

		FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
		crc += API_DUMMY;

		if (API_DUMMY == 00)
		{
			//Mess tx succes
			//RxXbeeAck = OK;
		}

		//API CRC
		FIFO_READ_ELEMENT(Fifo, &rx_crc);*/
		//Dummy Read 3 bytes

		//_1_Communication_Free_Receive_Bit();
		Nb_Erreurs_com++;
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
		vTracePrint(MyChannel_Recompo, "Packet 0x89");
#endif
		return pdFAIL;
	}

	boucle = 0;
	while (RingBuffer_Count(RingBuff) < API_LENGTH - 1)
	{
		boucle++;

		if (boucle > 5)
		{
			//_1_Communication_Free_Receive_Bit();
			Nb_Erreurs_com++;
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
			vTracePrint(MyChannel_Recompo, "Data missing");
#endif
			return pdFAIL;
		}
		Task_Delay(0.1F);
	}

	RingBuffer_PopMult(RingBuff, &Data_rx, 6);
	for(int i = 0; i < 4; i++)
	{
		crc += Data_rx[i];
	}

	/*//Tx Address
	FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
	crc += API_DUMMY;

	FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
	crc += API_DUMMY;

	//RSSI
	FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
	crc += API_DUMMY;

	//Option
	FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
	crc += API_DUMMY;*/

	//Reception datas
	received_trame.Instruction = Data_rx[4];
	crc += (byte)received_trame.Instruction;

	received_trame.Slave_Adresse = Data_rx[5];
	crc += (byte)received_trame.Slave_Adresse;

	received_trame.Length = (byte)(API_LENGTH - 7);

	if (received_trame.Length <= COMMUNICATION_TRAME_MAX_DATA)
	{
		boucle = 0;
		while (RingBuffer_Count(RingBuff) < received_trame.Length + 1)
		{
			boucle++;

			if (boucle > 5)
			{
				//_1_Communication_Free_Receive_Bit();
				Nb_Erreurs_com++;
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
				vTracePrint(MyChannel_Recompo, "Data missing2");
#endif
				return pdFAIL;
			}
			Task_Delay(0.1F);
		}

		RingBuffer_PopMult(RingBuff, &received_trame.Data, received_trame.Length);

		//Reception des data
		for (index = 0; index < received_trame.Length; index++)
		{
			crc += (byte)(received_trame.Data[index]);
		}

		//API CRC
		RingBuffer_Pop(RingBuff, &rx_crc);

		//Contrôle CRC
		crc &= 0xFF;
		crc = (byte)(0xFF - crc);

		//Vérifie le CRC
		if (crc == rx_crc)
		{
			Nb_Messages_recus++;
			//Vérifie l'adressage du message
			//_1_Communication_Free_Receive_Bit();
			_1_Communication_Check_Rx_Adresse(&received_trame);
		}else
		{
			//_1_Communication_Free_Receive_Bit();
			Nb_Erreurs_com++;
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
			vTracePrint(MyChannel_Recompo, "CRC Error");
#endif
			_2_Comm_Send_Communication_Status(RS485_port);
			return pdFAIL;
		}
	}else
	{
		//_1_Communication_Free_Receive_Bit();
		Nb_Erreurs_com++;
	}
	return pdFAIL;
}


/*****************************************************************************
 ** Function name:		_1_Communication_Check_Rx_Adresse
 **
 ** Descriptions:		Verifie si le message en question est adressé à cette carte (ou à toutes les cartes)
 **
 ** parameters:			Pointeur vers le message à contrôler
 ** 					Pointeur vers la Queue qui doit recevoir le message termine
 ** Returned value:		True: message ajouté à la Queue
 ** 					False: message non ajouté (non adressé à la carte ou Queeu pleine)
 **
 *****************************************************************************/
BaseType_t _1_Communication_Check_Rx_Adresse(struct Communication_Trame *received_trame)
{
	if(received_trame->Slave_Adresse == ALL_CARDS || received_trame->Slave_Adresse == ADRESSE_CARTE)
	{
		xQueueSend(_1_xQueue_Message_Receive, received_trame, portMAX_DELAY);
		return true;
	}

	return pdFALSE;
}


/*****************************************************************************
 ** Function name:		_1_Communication_Check_Priority_Messages
 **
 ** Descriptions:		Verifie si le message en question est consideré comme prioritaire et doit passer en haut de la pile
 **
 ** parameters:			Pointeur vers le message à contrôler
 ** Returned value:		True: message prioritaire
 ** 					False: message non prioritaire
 **
 *****************************************************************************/
BaseType_t _1_Communication_Check_Priority_Messages(struct Communication_Trame *received_trame)
{
	return pdFALSE;
}


/*****************************************************************************
 ** Function name:		_1_Communication_Recomposition_Rx
 **
 ** Descriptions:		Tache de recomposition des messages reçus
 **
 ** parameters:			Pointeur vers la Queue qui doit recevoir le message termine
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Communication_Recomposition_Rx(void *pvParameters)
{
	RINGBUFF_T *Falged_ringBuffer = NULL;
	Task_Delay(5);

	//Active les ISR du RS485 après l'initialisation des taches
	NVIC_EnableIRQ(RS485_IRQ_SELECTION);

	EventBits_t uxBits;

	while (true)
	{
		//Attente de l'info qu'une data est dispo dans un buffer
		uxBits = xEventGroupWaitBits(_0_Comm_EventGroup,   /* The event group being tested. */
				eGROUP_SYNCH_RS485_Rx_Data_Avail | eGROUP_SYNCH_USB_Rx_Data_Avail, /* The bits within the event group to wait for. */
				pdTRUE,        /* Clear bits before returning. */
				pdFALSE,        /* Wait for ALL bits to be set */
				portMAX_DELAY );/* Wait a maximum of xTicksToWait for either bit to be set. */

		if(uxBits & (eGROUP_SYNCH_RS485_Rx_Data_Avail ))
		{
			//Un message est dispo dans le buffer RS485
			Falged_ringBuffer = &rxring_RS485;
			vTracePrintF(MyChannel_RX_RS485, "RS485_Before Read = %d", RingBuffer_Count(&rxring_RS485));
		}else if(uxBits & (eGROUP_SYNCH_USB_Rx_Data_Avail ))
		{
			//Un message est dispo dans le buffer USB
#ifdef USE_USB
			Falged_ringBuffer = &rxring_USB;
#else
			Falged_ringBuffer = NULL;
#endif
		}

		//Temps qu'il y a des datas à lire dans le buffer en question
		//Creation de la trame à partir des data reçues
		while(RingBuffer_Count(Falged_ringBuffer))
		{
			_1_Communication_Create_Trame_From_Buffer(Falged_ringBuffer);
		}
	}
}

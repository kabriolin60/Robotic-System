/*
 * 0_Communication.c
 *
 *  Created on: 19 avr. 2020
 *      Author: kabri
 */

#include "FreeRTOS.h"
#include "board.h"
#include "chip.h"

#include "Configuration.h"
#include "Type_Declaration.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"

#include "event_groups.h"
#include "ring_buffer.h"
#include "cdc_usb_main.h"
#include "cdc_vcom.h"


#include "0_RS485.h"
#include "0_Communication.h"
#include "0_Event_Group.h"



__attribute__((optimize("O0"))) void _0_Communication_Send_RS485(LPC_USART_T *pUART, RINGBUFF_T *data, int length);



/* Transmit and receive ring buffer sizes */
#define TX_RB_SIZE 				128		/* Send */
#define RS485_RX_RB_SIZE 		256		/* Receive RS485*/
#define USB_RX_RB_SIZE 			128		/* Receive USB */


/* Receive ring buffer for RS485*/
RINGBUFF_T rxring_RS485;
/* Receive buffer for RS485*/
static TO_AHBS_RAM3 uint8_t rxbuff_RS485[RS485_RX_RB_SIZE];


/* Receive ring buffer for USB*/
#ifdef USE_USB
RINGBUFF_T rxring_USB;

/* Receive buffer for USB*/
static TO_AHBS_RAM3 uint8_t rxbuff_USB[USB_RX_RB_SIZE];

#endif


/* Transmit buffer */
RINGBUFF_T txring;
static TO_AHBS_RAM3 uint8_t txbuff[TX_RB_SIZE];

//Déclaration des Queues contenant les messages à envoyer et l'indication de messages reçus
QueueHandle_t _1_xQueue_Message_TO_Send; //Queue Recevant les messages des canaux de communication




/*****************************************************************************
 ** Function name:		_0_Communication_Init
 **
 ** Descriptions:		Fonction d'initialisation des actions de communication de niveau 0
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Communication_Init(void)
{
	/* Before using the ring buffers, initialize them using the ring buffer init function */
	/* Tx ring Buffer Init */
	RingBuffer_Init(&txring, txbuff, 1, TX_RB_SIZE);

	_0_Communication_Init_USB();

	_0_Communication_Init_RS485();

	//Création de la Queue contenant les messages qui doivent être envoyés
	_1_xQueue_Message_TO_Send = xQueueCreate( 5, sizeof( struct Communication_Message ));
	vQueueAddToRegistry( _1_xQueue_Message_TO_Send, "_1_xQue_Mess_Send");

	//Tache d'envoi des messages pour tous les cannaux
	xTaskCreate(_0_Communication_Send_Data, (char *) "_0_Com_Send_Data", 130, _1_xQueue_Message_TO_Send, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
}


/*****************************************************************************
 ** Function name:		_0_Communication_Init_USB
 **
 ** Descriptions:		Fonction d'initialisation des actions de communication de l'USB
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Communication_Init_USB(void)
{
#ifdef USE_USB

#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	//Identifie l'interruption USB pour FreeRTOS+Trace
	Trace_Timer_USB_Handle = xTraceSetISRProperties("ID_ISR_RX_USB", configUSB_INTERRUPT_PRIORITY);
#endif

	//Create USB rx Ring Buffer
	RingBuffer_Init(&rxring_USB, rxbuff_USB, 1, USB_RX_RB_SIZE);
#endif
}


/*****************************************************************************
 ** Function name:		_0_Communication_Init_RS485
 **
 ** Descriptions:		Fonction d'initialisation des actions de communication pour le RS485
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Communication_Init_RS485(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, 0, 15, IOCON_MODE_INACT, IOCON_FUNC1);	// P0.15 TXD1
	Chip_IOCON_PinMux(LPC_IOCON, 0, 16, IOCON_MODE_INACT, IOCON_FUNC1); // P0.16 RXD1

	//Init de la pin de direction du RS485
	_0_RS485_Init(RS485_DIR_PORT, RS485_DIR_BIT);

	NVIC_DisableIRQ(RS485_IRQ_SELECTION);

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(RS484_UART);
	Chip_UART_SetBaud(RS484_UART, BAUDRATE_RS485);
	Chip_UART_ConfigData(RS484_UART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_TXEnable(RS484_UART);

	/* Before using the ring buffers, initialize them using the ring buffer init function */
	/* RS485 RX ring buffer init */
	RingBuffer_Init(&rxring_RS485, rxbuff_RS485, 1, RS485_RX_RB_SIZE);


	/* Reset and enable FIFOs, FIFO trigger level 2 (8 chars) */
	Chip_UART_SetupFIFOS(RS484_UART, (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS | UART_FCR_TRG_LEV2));

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(RS484_UART, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* Disable transmit status interrupt */
	Chip_UART_IntDisable(RS484_UART, UART_IER_THREINT);

	/* preemption = 1, sub-priority = 1 */
	NVIC_ClearPendingIRQ(RS485_IRQ_SELECTION);
	NVIC_SetPriority(RS485_IRQ_SELECTION, 6);
}


/*****************************************************************************
 ** Function name:		RS485_HANDLER_NAME
 **
 ** Descriptions:		Handler de reception RS485
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void RS485_HANDLER_NAME(void)
{
	static bool already_flaged = pdFALSE;
	BaseType_t pxHigherPriorityTaskWoken = false;

	if(RingBuffer_Count(&rxring_RS485) <= 10)
	{
		already_flaged = pdFALSE;
	}

	//Manage ISR, and read datas
	Chip_UART_IRQRBHandler(RS484_UART, &rxring_RS485, &txring);

	//If received data count > 10, notify the reception task
	if(RingBuffer_Count(&rxring_RS485) > 10 && !already_flaged)
	{
		xEventGroupSetBitsFromISR(_0_Comm_EventGroup,    /* The event group being updated. */
				eGROUP_SYNCH_RS485_Rx_Data_Avail,		 /* The bits being set. */
				&pxHigherPriorityTaskWoken);

		already_flaged = pdTRUE;
	}else if(RingBuffer_Count(&rxring_RS485) >= RS485_RX_RB_SIZE/2)
	{
		//Assure le coup en forçant un reset du Flag si la moitié du Buffer est atteinte
		xEventGroupSetBitsFromISR(_0_Comm_EventGroup,    /* The event group being updated. */
				eGROUP_SYNCH_RS485_Rx_Data_Avail,		 /* The bits being set. */
				&pxHigherPriorityTaskWoken);
	}

	//Force un changement de tache
	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

	//Clear ISR flag
	NVIC_ClearPendingIRQ(RS485_IRQ_SELECTION);
}


/*****************************************************************************
 ** Function name:		USB_IRQHandler
 **
 ** Descriptions:		Handler de reception USB
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
extern USBD_HANDLE_T g_hUsb;

void USB_IRQHandler(void)
{
#ifdef USE_USB
	static bool already_flaged = pdFALSE;
	BaseType_t pxHigherPriorityTaskWoken = false;

	USBD_API->hw->ISR(g_hUsb);

	uint32_t rdCnt = 0;
	static uint8_t g_rxBuff[64];

	if(RingBuffer_Count(&rxring_USB) <= 10)
	{
		already_flaged = pdFALSE;
	}

	//Lecture des données USB
	rdCnt = vcom_bread(&g_rxBuff[0], 64);
	if(rdCnt)
	{
		RingBuffer_InsertMult(&rxring_USB, &g_rxBuff[0], rdCnt);

		//If received data count > 10, notify the reception task
		if(RingBuffer_Count(&rxring_USB) > 10 && !already_flaged)
		{
			xEventGroupSetBitsFromISR(_0_Comm_EventGroup,    /* The event group being updated. */
					eGROUP_SYNCH_USB_Rx_Data_Avail,		 /* The bits being set. */
					&pxHigherPriorityTaskWoken);

			already_flaged = pdTRUE;
		}else if(RingBuffer_Count(&rxring_USB) >= USB_RX_RB_SIZE/2)
		{
			//Assure le coup en forçant un reset du Flag si la moitié du Buffer est atteinte
			xEventGroupSetBitsFromISR(_0_Comm_EventGroup,    /* The event group being updated. */
					eGROUP_SYNCH_USB_Rx_Data_Avail,		 /* The bits being set. */
					&pxHigherPriorityTaskWoken);
		}

		//Force un changement de tache
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
#endif
}


/*****************************************************************************
 ** Function name:		_0_Communication_Send_Data
 **
 ** Descriptions:		Tache d'envoi des messages par tous les cannaux de communication
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/

void _0_Communication_Send_Data(void *pvParameters)
{

	static struct Communication_Message Message;
	static uint8_t g_txBuff[TX_RB_SIZE];
	byte length_remaining_to_send = 0;

	Task_Delay(5);

	while (1)
	{
		if( xQueueReceive( pvParameters, &Message, portMAX_DELAY ) == pdPASS )
		{
			//Un message est pret à être envoyé
			//L'ajouter au Txring Buffer
			RingBuffer_InsertMult(&txring, &Message.Data[0], (int)Message.length);

			//Envoi de la Datas sur le Canal concerné
			switch(Message.canal_communication)
			{
#ifdef USE_USB
			case USB_port:
				//Au depart le nombre de datas à envoyer = nombre de data initial
				length_remaining_to_send = Message.length;
				while(length_remaining_to_send)
				{
					//Temps qu'il reste des datas à envoyer
					//Prend un paquet de datas dans le buffer de TX, si >= 64, envoi 64 octets, sinon envoi le nombre de data restantes
					RingBuffer_PopMult(&txring, &g_txBuff[0], length_remaining_to_send >= 64 ? 64 : length_remaining_to_send);

					//Le message doit être envoyé par USB
					_0_Communication_Send_USB(&g_txBuff[0], length_remaining_to_send >= 64 ? 64 : length_remaining_to_send);

					//Mise à jour du nombre de datas à envoyer
					length_remaining_to_send = RingBuffer_Count(&txring);
					Task_Delay(0.5F);
				}
				break;
#endif

			case RS485_port:
				_0_Communication_Send_RS485(RS484_UART, &txring, (int)Message.length);
				break;

			default:
				//Dans le doute, vide le buffer de TX de tout ce qu'il contient
				RingBuffer_PopMult(&txring, &g_txBuff[0], RingBuffer_Count(&txring));
				break;
			}
		}
	}
}


/*****************************************************************************
 ** Function name:		_0_Communication_Send_RS485
 **
 ** Descriptions:		Fonction d'envoi d'un message par RS485
 **
 ** parameters:			Pointeur vers le ring buffer contenant les datas à envoyer
 ** 					Nombre d'octets à envoyer
 ** Returned value:		None
 **
 *****************************************************************************/
__attribute__((optimize("O0"))) void _0_Communication_Send_RS485(LPC_USART_T *pUART, RINGBUFF_T *data, int length)
{
	uint8_t ch;

	//Passe en TX
	_0_RS485_Master_Mode(RS485_DIR_PORT, RS485_DIR_BIT);

	while (RingBuffer_Pop(data, &ch))
	{
		Chip_UART_SendByte(pUART, ch);
		__asm volatile( "nop" );
		while((Chip_UART_ReadLineStatus(pUART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
		{
			__asm volatile( "nop" );
		}
	}

	for(int i =0; i < 100; i++);

	//Passe en RX
	_0_RS485_Slave_Mode(RS485_DIR_PORT, RS485_DIR_BIT);
}


/*****************************************************************************
 ** Function name:		_0_Communication_Send_USB
 **
 ** Descriptions:		Fonction d'envoi d'un message par USB
 **
 ** parameters:			Pointeur vers le buffer contenant des datas "uint8_t data[]"
 ** 					Nombre d'octets à envoyer
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Communication_Send_USB(uint8_t *data, uint8_t length)
{
	if (vcom_connected())
	{
		vcom_write(data, length);
		Task_Delay(1);
	}
}

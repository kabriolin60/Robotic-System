/*
 * 0_AX12.c
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */
#include "board.h"
#include "chip.h"

#include "0_AX12.h"
#include "ring_buffer.h"
#include "Configuration.h"
#include "Type_Declaration.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"

#include "0_Event_Group.h"

/* Transmit and receive ring buffer sizes */
#define TX_RB_AX12_SIZE 				128		/* Send */
#define RX_RB_AX12_SIZE 				128		/* Send */

/* Receive ring buffer for AX12*/
RINGBUFF_T rxring_AX12;
/* Receive buffer for AX12*/
static TO_AHBS_RAM3 uint8_t rxbuff_AX12[RX_RB_AX12_SIZE];

/* Transmit ring buffer for AX12*/
RINGBUFF_T txring_AX12;
/* Transmit buffer for AX12*/
static TO_AHBS_RAM3 uint8_t txbuff_AX12[TX_RB_AX12_SIZE];


//Canal de tracalyser
traceString MyChannel_RX_AX12;

/* FreeRTOS TRACE */
#define ID_ISR_RX_AX12 3       // lowest valid ID is 1
traceHandle Trace_Timer_AX12_Handle;



/**************************************************
Fonction d'init des AX12 de niveau 0
Input: None
Output: None
 **************************************************/
void _0_Init_AX12(void)
{
	//Power output des AX-12
	Chip_IOCON_PinMux(LPC_IOCON, AX_12_ON_OFF_PORT, AX_12_ON_OFF_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, AX_12_ON_OFF_PORT, AX_12_ON_OFF_BIT, true);
	_0_AX_12_POWER(false);


	//Init de la pin de direction de la COM AX12
	Chip_IOCON_PinMux(LPC_IOCON, DIR_AX_12_PORT, DIR_AX_12_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DIR_AX_12_PORT, DIR_AX_12_BIT, true);

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(AX12_UART);
	Chip_UART_SetBaud(AX12_UART, BAUDRATE_AX_12);
	Chip_UART_ConfigData(AX12_UART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_TXEnable(AX12_UART);


	/* Before using the ring buffers, initialize them using the ring buffer init function */
	/* RS485 RX ring buffer init */
	RingBuffer_Init(&rxring_AX12, rxbuff_AX12, 1, RX_RB_AX12_SIZE);

	/* RS485 TX ring buffer init */
	RingBuffer_Init(&txring_AX12, txbuff_AX12, 1, TX_RB_AX12_SIZE);


	/* Reset and enable FIFOs, FIFO trigger level 2 (8 chars) */
	Chip_UART_SetupFIFOS(AX12_UART, (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS | UART_FCR_TRG_LEV2));

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(AX12_UART, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* Disable transmit status interrupt */
	Chip_UART_IntDisable(AX12_UART, UART_IER_THREINT);

#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	//Identifie l'interruption de RS485 pour FreeRTOS+Trace
	Trace_Timer_AX12_Handle = xTraceSetISRProperties("ID_ISR_RX_AX12", AX12_IRQ_SELECTION);
#endif

	//Passe en reception
	SET_RX();

	/* preemption = 1, sub-priority = 1 */
	NVIC_ClearPendingIRQ(AX12_IRQ_SELECTION);
	NVIC_SetPriority(AX12_IRQ_SELECTION, 6);
}




/**************************************************
Fonction de mise sous et hors tension les AX12
Input: Power (on/OFF)
Output: None
 **************************************************/
void _0_AX_12_POWER(byte state)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, AX_12_ON_OFF_PORT, AX_12_ON_OFF_BIT, state);//Dir AX_12 = 1
}


/**************************************************
Fonction de reception d'une trame
Input: pointeur vers la Queue de Messages AX-12 reçus
Output: None
 **************************************************/
static struct st_AX_12_Trame rx_trame;
__attribute__((optimize("O0"))) void Rx_Ax_12_Trame(xQueueHandle pQueue_To_Send)
{
	unsigned int Checksum = 0;
	unsigned char index = 0;
	byte dummy = 0;

	//Reception des bits de start
	char boucle = 0;
	while(dummy != 0xFF)
	{
		RingBuffer_Pop(&rxring_AX12, &dummy);
		boucle++;
		if(boucle > 3)
			return;
	}

	boucle = 0;
	dummy = 0;
	while(dummy != 0xFF)
	{
		RingBuffer_Pop(&rxring_AX12, &dummy);
		boucle++;
		if(boucle > 3)
			return;
	}

	//Reception de l'ID
	RingBuffer_Pop(&rxring_AX12, &rx_trame.ID);
	Checksum += rx_trame.ID;

	//Reception de la longueur de la trame
	RingBuffer_Pop(&rxring_AX12, &rx_trame.Length);
	Checksum += rx_trame.Length;

	//Reception de l'instruction
	RingBuffer_Pop(&rxring_AX12, &rx_trame.Instruction);
	Checksum += rx_trame.Instruction;

	//Reception des parametres
	for(index = 0; index < rx_trame.Length - 2; index++)
	{
		if(index > AX_12_MAX_DATA_LENGTH)
			return;

		RingBuffer_Pop(&rxring_AX12, &rx_trame.Data[index]);
		Checksum += rx_trame.Data[index];
	}

	//Reception du Checksum
	RingBuffer_Pop(&rxring_AX12, &dummy);
	Checksum = ~Checksum;

	xQueueSend(pQueue_To_Send, &rx_trame, portMAX_DELAY);
}


/**************************************************
Fonction d'emission d'une trame vers les AX12
Input: pointeur vers la trame à envoyer
Output: None
 **************************************************/
static byte ax_12_tx[AX_12_MAX_DATA_LENGTH + 10];

__attribute__((optimize("O0"))) void Tx_Ax_12_Trame(struct st_AX_12_Trame *trame)
{
	unsigned short Checksum = 0;
	unsigned char index = 0;

	//Envoie des bits de start
	ax_12_tx[index++] = 0XFF;
	ax_12_tx[index++] = 0xFF;

	//Envoie de l'ID
	ax_12_tx[index++] = trame->ID;
	Checksum += trame->ID;

	//Envoie de la longueur de la trame
	ax_12_tx[index++] = trame->Length;
	Checksum += trame->Length;

	//Envoie de l'instruction
	ax_12_tx[index++] = trame->Instruction;
	Checksum += trame->Instruction;

	//Envoie des parametres
	for(int index2 = 0; index2 < trame->Length - 2; index2++)
	{
		if(index2 > AX_12_MAX_DATA_LENGTH)
			break;
		ax_12_tx[index++] = trame->Data[index2];
		Checksum += trame->Data[index2];
	}

	Checksum = ~Checksum;

	trame->Data_Recieved = 0;

	//Envoie du Checksum
	ax_12_tx[index++] = (unsigned char)Checksum;

	//Mise en ring Buffer
	RingBuffer_InsertMult(&txring_AX12, &ax_12_tx[0], (int)index);

	_0_Communication_Send_AX12(AX12_UART, &txring_AX12, (int)index);
}



/*****************************************************************************
 ** Function name:		_0_Communication_Send_AX12
 **
 ** Descriptions:		Fonction d'envoi d'un message pour les AX12
 **
 ** parameters:			Pointeur vers le ring buffer contenant les datas à envoyer
 ** 					Nombre d'octets à envoyer
 ** Returned value:		None
 **
 *****************************************************************************/
__attribute__((optimize("O0"))) void _0_Communication_Send_AX12(LPC_USART_T *pUART, RINGBUFF_T *data, int length)
{
	uint8_t ch;

	//Passe en TX
	SET_TX();

	//Desactiver les it de reception
	DISABLE_RX_IT();

	while (RingBuffer_Pop(data, &ch))
	{
		Chip_UART_SendByte(pUART, ch);
		__asm volatile( "nop" );
		while((Chip_UART_ReadLineStatus(pUART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
		{
			__asm volatile( "nop" );
		}
	}

	int i=0;
	for(i=0; i<200;i++);

	//Passe en RX
	SET_RX();

	//Activer les it de reception
	ENABLE_RX_IT();
}



/*****************************************************************************
 ** Function name:		AX12_HANDLER_NAME
 **
 ** Descriptions:		Handler de reception AX-12
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void AX12_HANDLER_NAME(void)
{
	BaseType_t pxHigherPriorityTaskWoken = false;

	//Trace tracking of ISR entry
	vTraceStoreISRBegin(Trace_Timer_AX12_Handle);

	//Manage ISR, and read datas
	Chip_UART_IRQRBHandler(AX12_UART, &rxring_AX12, &txring_AX12);

	//If received data count > 10, notify the reception task
	if(RingBuffer_Count(&rxring_AX12) >= 6)
	{
		xEventGroupSetBitsFromISR(_0_Comm_EventGroup,    /* The event group being updated. */
				eGROUP_SYNCH_AX12_Rx_Data_Avail,		 /* The bits being set. */
				&pxHigherPriorityTaskWoken);
	}

	//Force un changement de tache
	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

	//Trace un user Event pour debug
	vTracePrintF(MyChannel_RX_AX12, "AX12_Buff_usage = %d", RingBuffer_Count(&rxring_AX12));

	//Clear ISR flag
	NVIC_ClearPendingIRQ(AX12_IRQ_SELECTION);

	//Trace tracking of ISR exit
	vTraceStoreISREnd(0);
}


__attribute__((optimize("O0"))) void SET_TX(void)
{
	//Passe en TX
	Chip_GPIO_WritePortBit(LPC_GPIO, DIR_AX_12_PORT, DIR_AX_12_BIT, true);//Dir AX_12 = 1

	//Desactive la pin de RX, et passe en input avec pull-up
	//Disable RX
	Chip_IOCON_PinMux(LPC_IOCON, 0, 11, IOCON_MODE_INACT, IOCON_FUNC0); // P0.11 as GPIO
	Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 11, false); //as input

	//Enable TX
	Chip_IOCON_PinMux(LPC_IOCON, 0, 10, IOCON_MODE_INACT, IOCON_FUNC1); // P0.10 TXD2
}

__attribute__((optimize("O0"))) void SET_RX(void)
{
	//Passe en RX
	Chip_GPIO_WritePortBit(LPC_GPIO, DIR_AX_12_PORT, DIR_AX_12_BIT, false);//Dir AX_12 = 0

	//Desactive la pin de TX, et passe en input avec pull-up
	//Desable TX
	Chip_IOCON_PinMux(LPC_IOCON, 0, 10, IOCON_MODE_INACT, IOCON_FUNC0); //TX0 as GPIO (P0.10)
	Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 10, false); //as input

	//Enable RX
	Chip_IOCON_PinMux(LPC_IOCON, 0, 11, IOCON_MODE_INACT, IOCON_FUNC1); // P0.11 RXD2
}


__attribute__((optimize("O0"))) void ENABLE_RX_IT(void)
{
	NVIC_EnableIRQ(AX12_IRQ_SELECTION);
}

__attribute__((optimize("O0"))) void DISABLE_RX_IT(void)
{
	NVIC_DisableIRQ(AX12_IRQ_SELECTION);
}

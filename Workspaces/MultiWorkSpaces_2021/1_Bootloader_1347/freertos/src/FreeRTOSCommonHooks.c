/*
 * @brief Common FreeRTOS functions shared among platforms
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSCommonHooks.h"

#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Delay for the specified number of milliSeconds */
void FreeRTOSDelay(uint32_t ms)
{
	portTickType xDelayTime;

	xDelayTime = xTaskGetTickCount();
	vTaskDelayUntil(&xDelayTime, ms);
}

/* FreeRTOS malloc fail hook */
void vApplicationMallocFailedHook(void)
{
	//DEBUGSTR("DIE:ERROR:FreeRTOS: Malloc Failure!\r\n");
	taskDISABLE_INTERRUPTS();
	for (;; ) {}
}

/* FreeRTOS application idle hook */
void vApplicationIdleHook(void)
{
	/* Best to sleep here until next systick */
	__WFI();
}

//#include "Communication_Typedef.h"
/*#include "0_RS485.h"
#include "1_Trame_Communication.h"
#include "2_Echange_Datas.h"


static TO_AHBS_RAM3 struct Logger_Debug_Data log_message_overflow;
static TO_AHBS_RAM3 struct Communication_Trame trame_echange_overflow;
static TO_AHBS_RAM3 struct Communication_Message Message_To_Send_overflow;
static TO_AHBS_RAM3 uint8_t txbuff[128];
static TO_AHBS_RAM3 char str[70];*/

/* FreeRTOS stack overflow hook */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
	(void) pxTask;
	(void) pcTaskName;


	/*sprintf(str, "DIE:ERROR: Stack overflow in task \"%s\"\r\n", pcTaskName);
	trame_echange_overflow.Instruction = LOGGER_DEBUG;
	trame_echange_overflow.Slave_Adresse = PC;

	log_message_overflow.Color = Color_Red;

	log_message_overflow.Channel = Channel_Debug_Error;

	log_message_overflow.Nombre_Carateres = COPYSTRING(str, log_message_overflow.Text);

	trame_echange_overflow.Length = COPYDATA(log_message_overflow, trame_echange_overflow.Data);
	trame_echange_overflow.XBEE_DEST_ADDR = XBee_PC;

	//Creation du message
	Message_To_Send_overflow = (struct Communication_Message)* _1_Communication_Create_Message(&trame_echange_overflow);

	//Transforme le message en [byte]
	int length = COPYDATA(Message_To_Send_overflow, txbuff);

	//Passe en RX
	_0_RS485_Slave_Mode(RS485_DIR_PORT, RS485_DIR_BIT);	for(int i = 0; i < 3000; i++)__asm volatile( "nop" );

	//Passe en TX
	_0_RS485_Master_Mode(RS485_DIR_PORT, RS485_DIR_BIT);

	for(int j = 0; j < length; j++)
	{
		Chip_UART_SendByte(RS484_UART, txbuff[j]);
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );

		while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
		{
			for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
		}
	}

	for(int i = 0; i < 100; i++)__asm volatile( "nop" );

	//Passe en RX
	_0_RS485_Slave_Mode(RS485_DIR_PORT, RS485_DIR_BIT);

	//DEBUGOUT("DIE:ERROR:FreeRTOS: Stack overflow in task %s\r\n", pcTaskName);
	/* Run time stack overflow checking is performed if
	   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	   function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for (;; ) {}
}

/* FreeRTOS application tick hook */
void vApplicationTickHook(void)
{}

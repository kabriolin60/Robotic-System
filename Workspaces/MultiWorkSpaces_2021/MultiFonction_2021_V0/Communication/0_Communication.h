/*
 * 0_Communication.h
 *
 *  Created on: 19 avr. 2020
 *      Author: kabri
 */

#ifndef _0_COMMUNICATION_H_
#define _0_COMMUNICATION_H_

#include "Communication_Typedef.h"
#include "Configuration.h"

/* Receive ring buffer for RS485*/
extern RINGBUFF_T rxring_RS485;

void _0_Communication_Init(void);

void _0_Communication_Init_RS485(void);

void _0_Communication_Init_Debug_UART(void);

void RS485_HANDLER_NAME(void);

void _0_Communication_Send_Data(void *pvParameters);

void _0_Communication_Send_RS485(LPC_USART_T *pUART, RINGBUFF_T *data, int length);

void _0_Communication_Send_Debug_UART(LPC_USART_T *pUART, RINGBUFF_T *data, int length);

void _0_Communication_Give_Sending_Clearance(void);

void _0_Communication_Wait_Sending_Clearance(void);

#endif /* 0_COMMUNICATION_0_COMMUNICATION_H_ */

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
#include "ring_buffer.h"
#include "uart_11xx.h"

/* Receive ring buffer for RS485*/
extern RINGBUFF_T rxring_RS485;


void _0_Communication_Init(void);

void _0_Communication_Init_RS485(void);

void RS485_HANDLER_NAME(void);

void _0_Communication_Send_Data(void *pvParameters);

void _0_Communication_Send_RS485(LPC_USART_T *pUART, RINGBUFF_T *data, int length);

#endif /* 0_COMMUNICATION_0_COMMUNICATION_H_ */

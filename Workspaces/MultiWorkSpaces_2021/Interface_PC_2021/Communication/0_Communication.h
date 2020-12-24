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

/* Receive ring buffer for XBEE*/
extern RINGBUFF_T rxring_XBEE;

/* Receive ring buffer for RS485_2*/
extern RINGBUFF_T rxring_RS485_2;

/* Receive ring buffer for USB*/
#ifdef USE_USB
extern RINGBUFF_T rxring_USB;
#endif

void _0_Communication_Init(void);

void _0_Communication_Init_USB(void);

void _0_Communication_Init_RS485(void);

void _0_Communication_Init_RS485_2(void);

void _0_Communication_Init_XBEE(void);

void RS485_HANDLER_NAME(void);

void XBEE_HANDLER_NAME(void);

void USB_IRQHandler(void);

void _0_Communication_Send_Data(void *pvParameters);

void _0_Communication_Send_RS485(LPC_USART_T *pUART, RINGBUFF_T *data, int length, uint8_t RS485_dir_port, uint8_t RS485_dir_bit);

__attribute__((optimize("O0"))) void _0_Communication_Send_XBEE(LPC_USART_T *pUART, RINGBUFF_T *data, int length);

void _0_Communication_Send_USB(uint8_t *data, uint8_t length);

void _0_Communication_Wait_Sending_Clearance(void);

void _0_Communication_Give_Sending_Clearance(void);

#endif /* 0_COMMUNICATION_0_COMMUNICATION_H_ */

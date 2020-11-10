/*
 * Uart1.h
 *
 *  Created on: 1 févr. 2014
 *      Author: Fixe
 */

#ifndef UART1_H_
#define UART1_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "LPC17xx.h"
#include "system_LPC17xx.h"

#include "Includes.h"

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80


void Init_UART_1(unsigned long Baudrate, char port);

unsigned char UART_1_Read_Byte(void);

void UART_1_Send_Byte(unsigned char Data);

void UART_1_Enable_Interrupts(void);

void UART_1_Disable_Interrupts(void);

void UART1_IRQHandler (void);

void Fct_NULL(void);

void RS485_Init_IO(void);

void RS485_Master_Mode(void);

void RS485_Slave_Mode(void);

#endif /* UART1_H_ */

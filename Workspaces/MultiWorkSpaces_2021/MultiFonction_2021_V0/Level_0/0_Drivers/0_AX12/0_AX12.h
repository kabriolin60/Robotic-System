/*
 * 0_AX12.h
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */

#ifndef _0_AX12_H_
#define _0_AX12_H_

#include "Communication_Typedef.h"
#include "Configuration.h"

#include "FreeRTOS.h"
#include "board.h"
#include "chip.h"
#include "queue.h"



#define Nb_AX_12 4

#define AX_12_MAX_DATA_LENGTH 17 * Nb_AX_12

enum _AX_12_Instructions
{
	AX_12_PING = 0x01,
	AX_12_READ_DATA,
	AX_12_WRITE_DATA,
	AX_12_REG_WRITE,
	AX_12_ACTION,
	AX_12_RESET,
	AX_12_SYNCH_WRITE,
};

enum _AX_12_Registers
{
	AX_12_ID = 0x03,
	AX_12_Baud_Rate,
	AX_12_Return_Delay_Time,
	AX_12_CW_Angle_Limit_L,
	AX_12_CW_Angle_Limit_H,
	AX_12_CCW_Angle_Limit_L,
	AX_12_CCW_Angle_Limit_H,
	AX_12_High_Temp_Limit = 0x0B,
	AX_12_Low_Volt_Limit,
	AX_12_High_Volt_Limit,
	AX_12_Max_Torque_L,
	AX_12_Max_Torque_H,
	AX_12_Status_Return_Level,
	AX_12_Alarm_LED,
	AX_12_Alarm_ShutDown,
	AX_12_Torque_Enable = 0x18,
	AX_12_LED,
	AX_12_Goal_Position_L = 0x1E,
	AX_12_Goal_Position_H,
	AX_12_Moving_Speed_L,
	AX_12_Moving_Speed_H,
	AX_12_Torque_Limite_L,
	AX_12_Torque_Limite_H,
	AX_12_Present_Position_L,
	AX_12_Present_Position_H,
	AX_12_Present_Speed_L,
	AX_12_Present_Speed_H,
	AX_12_Present_Load_L,
	AX_12_Present_Load_H,
	AX_12_Present_Voltage,
	AX_12_Present_Temperature,
};

struct st_AX_12_Trame
{
	unsigned char ID;
	unsigned char Length;
	enum _AX_12_Instructions Instruction;
	unsigned char Data[AX_12_MAX_DATA_LENGTH];

	unsigned char Data_Recieved;
};


/**************************************************
Declaration des Reglages liés à la Communication RS485
**************************************************/
#define BAUDRATE_AX_12				1000000
#define AX12_UART					LPC_UART2

#define AX12_IRQ_SELECTION 			UART2_IRQn
#define AX12_HANDLER_NAME 			UART2_IRQHandler

#define DIR_AX_12_PORT				2
#define DIR_AX_12_BIT				13



void _0_Init_AX12(void);

void _0_AX_12_POWER(byte state);

__attribute__((optimize("O0"))) void Rx_Ax_12_Trame(xQueueHandle pQueue_To_Send);

__attribute__((optimize("O0"))) void Tx_Ax_12_Trame(struct st_AX_12_Trame *trame);

__attribute__((optimize("O0"))) void _0_Communication_Send_AX12(LPC_USART_T *pUART, RINGBUFF_T *data, int length);

void AX12_HANDLER_NAME(void);

__attribute__((optimize("O0"))) void SET_TX(void);

__attribute__((optimize("O0"))) void SET_RX(void);

__attribute__((optimize("O0"))) void ENABLE_RX_IT(void);

__attribute__((optimize("O0"))) void DISABLE_RX_IT(void);


#endif /* 0_DRIVERS_0_AX12_0_AX12_H_ */

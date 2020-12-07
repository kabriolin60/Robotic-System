/*
 * 1_Trame_Communication.h
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#ifndef _1_TRAME_COMMUNICATION_H_
#define _1_TRAME_COMMUNICATION_H_

#include "board.h"
#include "Configuration.h"
#include "Type_Declaration.h"
#include "queue.h"
#include "semphr.h"

extern QueueHandle_t xQueue_Message_Receive;
extern QueueHandle_t _1_xQueue_RX_DATA;


void _1_Communication_Init(void);

void _1_Communication_Create_Queues_Semaphores(void);



BaseType_t _1_Communication_Wait_To_Send(TickType_t xTicksToWait, byte bit_to_check);

void _1_Communication_Free_Send_Bit(byte bit_to_check);



BaseType_t _1_Communication_Wait_To_Receive(TickType_t xTicksToWait);

void _1_Communication_Free_Receive_Bit(void);



struct Communication_Message* _1_Communication_Create_Message(struct Communication_Trame *pMessage_to_send);

BaseType_t _1_Communication_Create_Trame(struct Communication_Trame *pMessage_to_send, enum enum_canal_communication canal, byte bit_to_check, byte WAIT_FOR_ACK, enum enum_ACK_Types ACK_TYPE, long Cartes_Devant_ACK);

short _1_Communication_WAIT_ACK(byte wait, enum enum_ACK_Types ACK_TYPE, long Cartes_Devant_ACK);

void _1_Communication_CLEAR_ACK(void);


BaseType_t _1_Communication_Create_Trame_From_Buffer(RINGBUFF_T *RingBuff);

BaseType_t _1_Communication_Check_Rx_Adresse(struct Communication_Trame *received_trame);

BaseType_t _1_Communication_Check_Priority_Messages(struct Communication_Trame *received_trame);

void _1_Communication_Recomposition_Rx(void *pvParameters);

#endif /* 1_COMMUNICATION_1_TRAME_COMMUNICATION_H_ */

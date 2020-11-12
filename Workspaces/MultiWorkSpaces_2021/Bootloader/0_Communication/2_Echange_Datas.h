/*
 * 2_Echange_Datas.h
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#ifndef _2_ECHANGE_DATAS_H_
#define _2_ECHANGE_DATAS_H_

#include "board.h"
#include "Configuration.h"
#include "Type_Declaration.h"
#include "Communication_Typedef.h"
#include "queue.h"

//Copie des donnees d'une struct a l'autre
#define COPYDATA(X, Y)	sizeof(X) > sizeof(Y) ? sizeof(Y) : sizeof(X); memcpy(&Y, &X, sizeof(Y));
#define COPYDATA2(X, Y)	memcpy(&Y, &X, sizeof(Y));
#define COPYSTRING(X, Y) strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X); memcpy(&Y, X, strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X));


void _2_Communication_Init();

void _2_Comm_Send_Destination_Robot(struct st_DESTINATION_ROBOT* destination, enum enum_canal_communication canal);

void _2_Comm_Send_Demande_Info(uint8_t adresse_cible, enum enum_canal_communication canal);

void _2_Comm_Send_PING(uint8_t adresse_cible, enum enum_canal_communication canal);

void _2_Comm_Send_PONG(enum enum_canal_communication canal);

void _2_Comm_Send_Robot_Position(struct st_POSITION_ROBOT rob_pos, enum enum_canal_communication canal);

void _2_Comm_Send_Log_Message(char* str, enum Logger_Debug_Color color, enum enum_canal_communication canal);

void _2_Communication_Boards_Status(void* pvParameters);

void _2_Comm_Send_Servos_Destinations(struct st_Destination_Servos* destination, enum enum_canal_communication canal);

#endif /* 2_COMMUNICATION_2_ECHANGE_DATAS_H_ */
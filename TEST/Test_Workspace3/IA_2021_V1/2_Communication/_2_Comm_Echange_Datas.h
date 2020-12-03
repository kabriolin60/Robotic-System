/*
 * _2_Comm_Echange_Datas.h
 *
 *  Created on: 3 déc. 2020
 *      Author: PC-INOVA
 */

#ifndef _2_COMM_ECHANGE_DATAS_H_
#define _2_COMM_ECHANGE_DATAS_H_

void _2_Communication_Init();

void _2_Comm_Send_Demande_Info(uint8_t adresse_cible, enum enum_canal_communication canal);

void _2_Comm_Send_PING(uint8_t adresse_cible, enum enum_canal_communication canal);

void _2_Comm_Send_PONG(enum enum_canal_communication canal);

void _2_Comm_Send_Communication_Status(enum enum_canal_communication canal);


void _2_Comm_Send_Log_Message(char* str, enum Logger_Debug_Color color, uint8_t Channel, enum enum_canal_communication canal);

void _2_Communication_Boards_Status(void* pvParameters);


void _2_Comm_Send_Info_Carte_IA(enum enum_canal_communication canal);

#endif /* 2_COMM_ECHANGE_DATAS_H_ */

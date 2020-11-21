/*
 * 2_Echange_Datas_Reception.h
 *
 *  Created on: 25 avr. 2020
 *      Author: kabri
 */

#ifndef _2_ECHANGE_DATAS_RECEPTION_H_
#define _2_ECHANGE_DATAS_RECEPTION_H_

#include "Communication_Typedef.h"

void _2_Communication_RX_Init();

void _2_Communication_RX_Lectures_Messages(void *pvParameters);

void _2_Communication_Interprete_message(struct Communication_Trame* trame);

void _2_Communication_RX_Destination_Robot(struct Communication_Trame* datas);

void _2_Communication_RX_Parametres_PID(struct Communication_Trame* datas);

void _2_Comm_RX_Destination_Servos(struct Communication_Trame* datas);

void _2_Comm_RX_Id_Robot(struct Communication_Trame* datas);

byte _2_Comm_Get_Robot_ID();

void _2_Comm_RX_Simulation_Deplacement(struct Communication_Trame* datas);

void _2_Comm_RX_Motor_Power(struct Communication_Trame* datas);

void _2_Comm_RX_Set_Position_Robot(struct Communication_Trame* datas);

#endif /* 2_COMMUNICATION_2_ECHANGE_DATAS_RECEPTION_H_ */

/*
 * 0_Infos.h
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#ifndef _INFOS_H_
#define _INFOS_H_

#include "Configuration.h"
#include "Communication_Typedef.h"

#define Nb_Max_Cartes 4

void _0_Infos_Init();

void _0_Update_Card_Datas(struct Com_Reponse_Info* infos);

struct Com_Reponse_Info* _0_Get_Ptr_Card(byte number);

struct st_POSITION_ROBOT _0_Get_Robot_Position(void);

struct Com_Position_Robot _0_Get_Robot_Position_Communication(void);

struct servo_destination _0_Get_Servo_Position(byte ID);

void _0_Set_Position_SecondRobot(int16_t x, int16_t y);

struct Com_Position_Robot _0_Get_Position_SecondRobot(void);

#endif /* 0_INFOS_H_ */

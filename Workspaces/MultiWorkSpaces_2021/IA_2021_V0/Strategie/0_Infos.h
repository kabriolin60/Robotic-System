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

struct st_POSITION_ROBOT _0_Get_Ptr_Robot_Position(void);

#endif /* 0_INFOS_H_ */

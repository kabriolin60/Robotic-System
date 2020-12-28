/*
 * 2_Echange_Datas_Reception.h
 *
 *  Created on: 28 déc. 2020
 *      Author: kabri
 */

#ifndef _2_ECHANGE_DATAS_RECEPTION_H_
#define _2_ECHANGE_DATAS_RECEPTION_H_

#include "Communication_Typedef.h"

void _2_Communication_RX_Init();

void _2_Communication_Interprete_message(struct Communication_Trame* trame);


void _2_Comm_RX_Id_Robot(struct Communication_Trame* datas);

byte _2_Comm_Get_Robot_ID();

#endif /* 2_ECHANGE_DATAS_RECEPTION_H_ */

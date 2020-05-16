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

void _2_Communication_RX_Destination_Robot(struct Communication_Trame* datas);

void _2_Communication_RX_Parametres_PID(struct Communication_Trame* datas);

#endif /* 2_COMMUNICATION_2_ECHANGE_DATAS_RECEPTION_H_ */

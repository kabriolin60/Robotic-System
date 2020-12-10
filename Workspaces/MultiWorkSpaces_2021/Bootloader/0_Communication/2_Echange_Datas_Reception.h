/*
 * 2_Echange_Datas_Reception.h
 *
 *  Created on: 25 avr. 2020
 *      Author: kabri
 */

#ifndef _2_ECHANGE_DATAS_RECEPTION_H_
#define _2_ECHANGE_DATAS_RECEPTION_H_

void _2_Communication_RX_Init();

void _2_Communication_RX_Lectures_Messages(void *pvParameters);

void _2_Communication_RX_Reponse_Infos(struct Communication_Trame* datas);


#endif /* 2_COMMUNICATION_2_ECHANGE_DATAS_RECEPTION_H_ */

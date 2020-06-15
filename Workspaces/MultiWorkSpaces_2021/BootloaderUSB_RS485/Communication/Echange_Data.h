/*
 * Echange_Data.h
 *
 *  Created on: 1 févr. 2014
 *      Author: adrien
 */

#ifndef ECHANGE_DATA_H_
#define ECHANGE_DATA_H_

#include "Includes.h"
#include "Communication_NG.h"
#include "Communication_struct.h"

void Init_Echange_Data(void);

void uTask_SendUSB(void *pvParameter);

void Communication_Decodage_Trame(struct Communication_Trame *received);

void Reception_Bootloader(struct Communication_Trame *trame);

void DebugLogger(char* str, byte color, xQueueHandle QueueToAdd);

void Bootloader_Send_ACK(void);

extern xQueueHandle QueueEchangeTX_RS485;
extern xQueueHandle QueueEchangeRXTrame_RS485;

extern xQueueHandle QueueEchangeTX_USB;
extern xQueueHandle QueueEchangeRXTrame_USB;


#endif /* ECHANGE_DATA_H_ */

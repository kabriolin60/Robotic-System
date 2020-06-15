/*
 * Bootloader.h
 *
 *  Created on: 17 déc. 2012
 *      Author: Robotic System
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include "Includes.h"

void Stop_Systick(void);

void Start_Systick(void);

void Init_Bootloader(void);

void Traitement_Data(struct st_LineBootloader *line);

extern TaskHandle_t StartUserCode;

extern long Nb_Bloc;

#endif /* BOOTLOADER_H_ */

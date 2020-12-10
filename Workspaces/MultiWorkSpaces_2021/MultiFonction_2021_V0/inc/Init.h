/*
 * Init_Carte_MultiFonction.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef INIT_H_
#define INIT_H_

#include "FreeRTOS.h"
#include "task.h"

void Debug_Trace_Texte(const char * texte);

void Set_Debug_Pin_0_High(void);

void Set_Debug_Pin_0_Low(void);

void Set_Debug_Pin_1_High(void);

void Set_Debug_Pin_1_Low(void);

void Init_Carte_MultiFonctions(void);

#endif /* INIT_H_ */

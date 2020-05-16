/*
 * Init_Carte_IA.h
 *
 *  Created on: 28 avr. 2020
 *      Author: kabri
 */

#ifndef INIT_H_
#define INIT_H_

#include "FreeRTOS.h"
#include "task.h"

#include "Configuration.h"

#include "Astar.h"



void Debug_Trace_Texte(const char * texte);

void Set_Debug_Pin_0_High(void);

void Set_Debug_Pin_0_Low(void);

void Set_Debug_Pin_1_High(void);

void Set_Debug_Pin_1_Low(void);

void Init_Carte_IA(void);


#endif /* INIT_H_ */

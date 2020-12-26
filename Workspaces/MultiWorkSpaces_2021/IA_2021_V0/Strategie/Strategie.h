/*
 * Strategie.h
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#ifndef STRATEGIE_H_
#define STRATEGIE_H_

#include "1_Trame_Communication.h"
#include "stdio.h"

#include "0_Infos.h"
#include "0_Event_Group.h"

#include "2_Echange_Data_Strategie.h"

void Task_MatchTime(void *pvparameter);

byte _Strategie_Get_Robot_ID(void);

byte _Strategie_Get_Input_Strategie_Selector(void);

byte _Strategie_Get_Choosen_Strategie(void);

unsigned long _Strategie_Get_Temps_Match(void);

byte _Strategie_Get_Jack_Status(void);

byte _Strategie_Get_Color_Status(void);

byte _Strategie_Get_Switch_Status(void);

byte _Strategie_Get_User_BP_Status(void);

byte _Strategie_Get_External_LED_RED_Status(void);
void _Strategie_Set_External_LED_RED_Status(byte value);

byte _Strategie_Get_External_LED_YELLOW_Status(void);
void _Strategie_Set_External_LED_YELLOW_Status(byte value);

byte _Strategie_Get_External_LED_GREEN_Status(void);
void _Strategie_Set_External_LED_GREEN_Status(byte value);



struct Action_Datas* _Strategie_Get_Action_By_ID(byte ID);

void _Strategie_Init_Strategie_2021(void* pvparameters);

#endif /* STRATEGIE_H_ */

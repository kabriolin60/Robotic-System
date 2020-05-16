/*
 * 1_Odometrie.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef _1_ODOMETRIE_H_
#define _1_ODOMETRIE_H_

#include "board.h"
#include "Configuration.h"
#include "Type_Declaration.h"

void _1_Odometrie_Init(void);

void vTask_1_Interpolation_lineaire(void *pvParameters);

void __Interpolation_lineaire_RCVA2(struct st_POSITION_ROBOT *Position, struct st_Codeurs *Codeurs, struct st_ROBOT_PARAMETRES *r);

void _1_Odometrie_Set_Parameters(struct st_ROBOT_PARAMETRES *new_parameters);

struct st_ROBOT_PARAMETRES *_1_Odometrie_Get_Parameters(void);

struct st_POSITION_ROBOT _1_Odometrie_GetRobot_Position(void);

void _1_Odometrie_SetRobot_Position(struct st_POSITION_ROBOT *newposition);

void _1_Omodetrie_Set_Simulation(bool value);

bool _1_Omodetrie_Get_Simulation(void);

struct st_Codeurs * _1_Odometrie_Get_ptr_Codeurs_Corriges(void);

#endif /* 1_ODOMETRIE_1_ODOMETRIE_H_ */

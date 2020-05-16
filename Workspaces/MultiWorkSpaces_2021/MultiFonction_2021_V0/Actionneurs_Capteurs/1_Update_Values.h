/*
 * 1_Update_Values.h
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */

#ifndef _1_UPDATE_VALUES_H_
#define _1_UPDATE_VALUES_H_

void _1_Update_Init(void);

void _1_Update_Task(void * pvParameter);

float _1_Update_GetTensionBatterie(void);

struct Com_Reponse_Info *Get_ptr_Reponse_info(void);

#endif /* 1_UPDATE_VALUES_H_ */

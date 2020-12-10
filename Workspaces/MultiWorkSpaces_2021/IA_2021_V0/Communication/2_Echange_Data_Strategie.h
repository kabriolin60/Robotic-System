/*
 * 2_Echange_Data_Strategie.h
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#ifndef _2_ECHANGE_DATA_STRATEGIE_H_
#define _2_ECHANGE_DATA_STRATEGIE_H_

#include "Strategie_Communication_Typedef.h"
#include "Strategie_Actions.h"

#include "1_Trame_Communication.h"
#include "stdio.h"

#include "0_Infos.h"
#include "0_Event_Group.h"
#include "2_Echange_Datas.h"

void _2_Comm_Strategie_Init();

void _2_Comm_Strategie_Send_Action_Creation(struct Action_Datas* data, enum enum_canal_communication canal);

void _2_Comm_Strategie_Send_Action_State_Update(struct Action_Datas* data, char* commentaire, enum enum_canal_communication canal);


#endif /* 2_ECHANGE_DATA_STRATEGIE_H_ */

/*
 * _2_Comm_Receive_Data.h
 *
 *  Created on: 3 déc. 2020
 *      Author: PC-INOVA
 */

#ifndef _2_COMM_RECEIVE_DATA_H_
#define _2_COMM_RECEIVE_DATA_H_

#include "Lib_includes.h"

void _2_Communication_Interprete_message(struct Communication_Trame* trame);

void _2_Communication_RX_Reponse_Infos(struct Communication_Trame* datas);

#endif /* 2_COMM_RECEIVE_DATA_H_ */

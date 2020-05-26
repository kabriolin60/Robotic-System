/*
 * 0_Deplacements.h
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#ifndef _0_ACTIONNEURS_H_
#define _0_ACTIONNEURS_H_

#include "Communication_Typedef.h"
#include "2_Echange_Datas.h"

bool _0_Actionneurs_Move_1_Servo(byte ID, unsigned short desti, short torque, 
                                 unsigned short time, bool attente);

bool _0_Actionneurs_Move_2_Servo(byte ID, unsigned short desti, short torque, 
                                 byte ID_2, unsigned short desti_2, short torque_2, 
                                 unsigned short time, bool attente);

bool _0_Actionneurs_Move_3_Servo(byte ID, unsigned short desti, short torque, 
                                 byte ID_2, unsigned short desti_2, short torque_2, 
                                 byte ID_3, unsigned short desti_3, short torque_3, 
                                 unsigned short time, bool attente);

bool _0_Actionneurs_Move_4_Servo(byte ID, unsigned short desti, short torque, 
                                 byte ID_2, unsigned short desti_2, short torque_2, 
                                 byte ID_3, unsigned short desti_3, short torque_3, 
                                 byte ID_4, unsigned short desti_4, short torque_4,
                                 unsigned short time, bool attente);


#endif /* _0_ACTIONNEURS_H_ */

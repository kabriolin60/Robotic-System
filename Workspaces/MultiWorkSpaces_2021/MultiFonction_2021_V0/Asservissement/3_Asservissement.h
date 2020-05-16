/*
 * 3_Asservissement.h
 *
 *  Created on: 8 mai 2020
 *      Author: kabri
 */

#ifndef _3_ASSERVISSEMENT_H_
#define _3_ASSERVISSEMENT_H_

#include "1_Odometrie.h"
#include "1_Asservissement.h"
#include "2_Asservissement.h"

void _3_Deplacement_Init(void);


void _3_Deplacement_Distance_Brute(float distance, bool attente);

void _3_Deplacement_Rotation_Brute(float angle, bool attente);

#endif /* 3_ASSERVISSEMENT_H_ */

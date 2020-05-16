/*
 * 1_Asservissement.h
 *
 *  Created on: 25 avr. 2020
 *      Author: kabri
 */

#ifndef _1_ASSERVISSEMENT_H_
#define _1_ASSERVISSEMENT_H_

#include "board.h"
#include "Configuration.h"
#include "Type_Declaration.h"
#include "Communication_Typedef.h"

void _1_Asserv_Init_Asserv(void);

struct st_pid_filter*_1_Asserv_GetPtr_PID_Vit_Pos(void);

struct st_pid_filter*_1_Asserv_GetPtr_PID_Vit_Rot(void);

struct st_pid_filter* _1_Get_prt_PID_Vit_Droite(void);

struct st_pid_filter* _1_Get_prt_PID_Vit_Gauche(void);


void _1_Update_PID_Vitesse_Consigne_distance(long consigne);

void _1_Update_PID_Vitesse_Consigne_rotation(long consigne);

void _1_Update_PID_Consigne_Vitesse_Roue_Droite(long consigne);

void _1_Update_PID_Consigne_Vitesse_Roue_Gauche(long consigne);


void _1_Asserv_Niveau_1(void *pvParameter);

void _1_Asserv_Vitesse_Independantes(void *pvParameters);

void _1_Asserv_Boucle_Asserv_Vitesse_Polaire(void *pvParameters);

#endif /* 1_ASSERVISSEMENT_1_ASSERVISSEMENT_H_ */

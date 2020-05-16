/*
 * 2_Asservissement.h
 *
 *  Created on: 30 avr. 2020
 *      Author: kabri
 */

#ifndef _2_ASSERVISSEMENT_H_
#define _2_ASSERVISSEMENT_H_

#include "FreeRTOS.h"
#include "board.h"
#include "chip.h"

#include "Configuration.h"
#include "Type_Declaration.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"
#include "event_groups.h"
#include "ring_buffer.h"

//Taille du Buffer de points de passage
#define _2_Asserv_Buffer_Depla_size 		16





/**************************************************
Declaration de la definition de la Structure contenant les parametres pour les déplacements
 **************************************************/

struct st_DEPLACEMENT
{
	float Accel;            //Variation de la vitesse en phase d'accélération
	float Deccel;           //Variation de la vitesse en phase de déccélération
	float Vmax;             //Consigne de vitesse Max
	float Consigne;         //Consigne en Distance à parcourir

	float Coef;             //Coef appliqué sur la consigne en vitesse pour suivre le cone de déplacement

	float Vitesse;          //vitesse réelle

	unsigned char Enable;
};

/*************************************************/





void _2_Deplacement_Init(void);

struct st_pid_filter* _2_Asserv_GetPtr_PID_Pos(void);

struct st_pid_filter* _2_Asserv_GetPtr_PID_Rot(void);



int _2_Deplacement_Ajout_Point(struct st_COORDONNEES *point);

void _2_Asservissement_DestinationBuffer_Clear(void);



void _2_Update_PID_Vitesse_Consignes(long consigne_vit_distance, long consigne_vit_rotation);

void _2_Asservissement_Set_Distance_Speed_Accel(float speed, float accel, float deccel);

void _2_Asservissement_Set_Rotation_Speed_Accel(float speed, float accel, float deccel);



void _2_Asservissement_Set_Distance_Displacement_Consign(float consigne);

void _2_Asservissement_Set_Rotation_Displacement_Consign(float consigne);



void _2_Asservissement_Distance_Angle(void *pvParameters);



void _2_Asservissement_Distance_Angle_Avec_Vitesse(void);

bool _2_Asservissement_Rotation_Avance(struct st_COORDONNEES * destination, struct st_POSITION_ROBOT * Current_Robot_Position, struct st_Parametre_Deplacement * Param_Depla, struct st_ROBOT_PARAMETRES * Param_Odometrie);



int _2_Asservissement_Read_Next_Desti_Point(RINGBUFF_T * buffer, struct st_COORDONNEES * destination);

int _2_Asservissement_Read_Next_Desti_Point_extern(void);


float Trapeze_Vitesse(struct st_DEPLACEMENT *D, struct st_pid_filter *PID, enum enum_TYPE_ARRET_ arret);



float Calcul_Distance_2_points(struct st_COORDONNEES Point1, struct st_COORDONNEES Point2);

float Calcul_Distance_2_points_Simple(float X0, float Y0, float X1, float Y1);

float Calcul_Angle_2_points(struct st_COORDONNEES Point1, struct st_COORDONNEES Point2);

float Calcul_Angle_2_points_Simple(float X0, float Y0, float X1, float Y1);

float minf(float a, float b);

#endif /* 2_ASSERVISSEMENT_H_ */

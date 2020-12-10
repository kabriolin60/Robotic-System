/*
 * 0_Servos.h
 *
 *  Created on: 30 avr. 2020
 *      Author: kabri
 */

#ifndef _0_SERVOS_H_
#define _0_SERVOS_H_

#include "board.h"
#include "Configuration.h"

//#define PWM_CYCLE		1200
//#define PWM_OFFSET		200

/**************************************************
Declaration des parametres pour les Servos et AX 12
**************************************************/
#define NB_SERVO	6


struct st_SERVO
{
	unsigned short Destination;
	unsigned short Position;
	float Pos_Float;
	float Var;
};

struct st_Tableau_Servo
{
	struct st_SERVO Servo[NB_SERVO];
};



void _0_Servo_Init(void);

void _0_Servo_Power(bool power);

void _0_Servos_Calcul_Ramp(struct st_SERVO *Servo, unsigned short Temps);

void _0_Servos_MAJ_Consigne_Servo(struct st_SERVO *s);

void _0_Servos_Update_Servo(void *pvParameters);


#endif /* 0_DRIVERS_0_SERVOS_0_SERVOS_H_ */

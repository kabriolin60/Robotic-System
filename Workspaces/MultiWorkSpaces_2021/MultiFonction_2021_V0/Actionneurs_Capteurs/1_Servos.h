/*
 * 1_Servos.h
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */

#ifndef _1_SERVOS_H_
#define _1_SERVOS_H_

void _1_Servos_Init(void);
void _1_Servos_Calcul_Ramp(struct st_SERVO *Servo, unsigned short Temps);
void _1_Servos_MAJ_Consigne_Servo(struct st_SERVO *s);
void _1_Servos_Update_Servo(void *pvParameters);

struct st_Tableau_Servo _1_Servo_GetServos_Position(void);

#endif /* 1_SERVOS_H_ */

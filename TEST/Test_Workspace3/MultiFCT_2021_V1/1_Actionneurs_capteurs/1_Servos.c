/*
 * 1_Servos.c
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */


#include "1_Servos.h"

static struct st_Tableau_Servo Tableau_Servo;

/**************************************************
Fonction d'init des Servos de niveau 1
Input: None
Output: None
 **************************************************/
void _1_Servos_Init(void)
{
	(void)memset(&Tableau_Servo, 0, sizeof(struct st_Tableau_Servo));

	/* Création de la tâche de pilotage et de mise à jour des consignes des servos */
	xTaskCreate(_1_Servos_Update_Servo, "_1_Servos", 70, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
}


/**************************************************
Fonction de calcul et de mise à jour d'une nouvelle consigne pour un servo
Input: Servo ID
		Nouvelle destination
		Temps de deplacement souhaité
Output: None
 **************************************************/
void _1_Servos_Set_Destination(uint8_t id, unsigned short desti, unsigned short time)
{
	Tableau_Servo.Servo[id].Destination = desti;
	_1_Servos_Calcul_Ramp(&Tableau_Servo.Servo[id], time);
}


/**************************************************
Fonction de calcul des rampes pour l'evolution de la position des servo
Input: Pointeur sur la Structure du Servo à Mettre à jour
       Temps de déplacement souhaité du servo
Output: None
 **************************************************/

void _1_Servos_Calcul_Ramp(struct st_SERVO *Servo, unsigned short Temps)
{
	float Delta_Position = 0;
	float Nombre_Top = 0;

	if(Temps == 0)
	{
		Servo->Position = Servo->Destination;
		Servo->Pos_Float = Servo->Destination;
	}else
	{
		Nombre_Top = Temps / 20;

		Delta_Position = (Servo->Destination - Servo->Position);
		if(Delta_Position < 0) Delta_Position = -Delta_Position;

		Servo->Var = Delta_Position / Nombre_Top;
	}
}

/*************************************************/

void _1_Servos_MAJ_Consigne_Servo(struct st_SERVO *s)
{
	float variation;

	if (s->Destination > s->Pos_Float)                           /*test if the variation is positive or negative */
	{
		variation = s->Destination - s->Pos_Float;                 /* positive variation */
		if (variation < s->Var)                 /* test if the variation is too high */
			s->Pos_Float = s->Destination;                               /* variation ok return value will be in */
		else
			s->Pos_Float += s->Var;           /* variation too high so return value is filtered */
	}
	else
	{
		variation = s->Pos_Float - s->Destination;                 /* negative variation */
		if (variation < s->Var)                 /* test if the variation is too high */
			s->Pos_Float = s->Destination;                       /* variation ok return value will be in */
		else
			s->Pos_Float -= s->Var; /* variation too high so return value is filtered */
	}

	s->Position = (unsigned short)s->Pos_Float;
}

void _1_Servos_Update_Servo(void *pvParameters)
{
	Task_Delay(200);

	unsigned char id;

	Init_Timing_Tache;

	for(;;)
	{
		for(id = 0; id<6; id++)
			_1_Servos_MAJ_Consigne_Servo(&Tableau_Servo.Servo[id]);

		//MAJ des registres des servo
		LPC_PWM1->MR1 = (unsigned short)(Tableau_Servo.Servo[5].Position);  //Servo 5
		LPC_PWM1->MR2 = (unsigned short)(Tableau_Servo.Servo[4].Position);  //Servo 4
		LPC_PWM1->MR3 = (unsigned short)(Tableau_Servo.Servo[3].Position);  //Servo 3
		LPC_PWM1->MR4 = (unsigned short)(Tableau_Servo.Servo[2].Position);	//Servo 2
		LPC_PWM1->MR5 = (unsigned short)(Tableau_Servo.Servo[1].Position); 	//Servo 1
		LPC_PWM1->MR6 = (unsigned short)(Tableau_Servo.Servo[0].Position);  //Servo 0

		LPC_PWM1->LER = LER0_EN | LER1_EN | LER2_EN | LER3_EN | LER4_EN | LER5_EN | LER6_EN;

		Task_Delay_Until(20);
	}
}


/**************************************************
Fonction de lecture et de renvoi de la positon des servos
Input: None
Output: Position des servos
 **************************************************/
struct st_Tableau_Servo _1_Servo_GetServos_Position(void)
{
	return Tableau_Servo;
}

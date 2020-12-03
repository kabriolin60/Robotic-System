/*
 * Configuration.h
 *
 *  Created on: 3 déc. 2020
 *      Author: PC-INOVA
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/* Includes drivers carte multifct */
#include "0_Codeurs.h"
#include "0_Motors.h"
#include "0_Servos.h"
#include "0_AX12.h"
#include "0_ADC.h"

#include "1_Servos.h"
#include "1_AX12.h"

#include "1_Update_Values.h"

/**************************************************
Déclaration des cannaux des Codeurs
**************************************************/
#define Codeur_D_Cannal_A_Port		0
#define Codeur_D_Cannal_A_Bit		6

#define Codeur_D_Cannal_B_Port		0
#define Codeur_D_Cannal_B_Bit		7

#define Codeur_G_Cannal_A_Port		0
#define Codeur_G_Cannal_A_Bit		9

#define Codeur_G_Cannal_B_Port		0
#define Codeur_G_Cannal_B_Bit		8

/* Déclaration des Sorties et Timer des Moteurs */
#define DIR_MOTEUR_1_GAUCHE_PORT	0
#define DIR_MOTEUR_1_GAUCHE_BIT		4

#define DIR_MOTEUR_2_DROIT_PORT 	1
#define DIR_MOTEUR_2_DROIT_BIT		17


/**************************************************
Declaration des parametres pour les Servos et AX 12
**************************************************/
#define SERVO_ON_OFF_PORT			1
#define SERVO_ON_OFF_BIT			27

#define AX_12_ON_OFF_PORT			1
#define AX_12_ON_OFF_BIT			28


/**************************************************
Declaration des parametres pour la conversion ADC
**************************************************/
#define ADC_CHANNEL_0_PORT			0
#define ADC_CHANNEL_0_BIT			23

#define ADC_CHANNEL_1_PORT			0
#define ADC_CHANNEL_1_BIT			24

#define ADC_CHANNEL_2_PORT			0
#define ADC_CHANNEL_2_BIT			25

#define ADC_CHANNEL_3_PORT			0
#define ADC_CHANNEL_3_BIT			26

#define ADC_CHANNEL_7_PORT			0
#define ADC_CHANNEL_7_BIT			2

#define ADC_CHANNEL_BATTERIE_PORT	0
#define ADC_CHANNEL_BATTERIE_BIT	3


/**************************************************
Declaration des Inputs des contacteurs ToR
**************************************************/
#define FDC_0_PORT					1
#define FDC_0_BIT					30

#define FDC_1_PORT					1
#define FDC_1_BIT					31

#define CTC_0_PORT					0
#define CTC_0_BIT					30

#define CTC_1_PORT					0
#define CTC_1_BIT					29

#define CTC_2_PORT					3
#define CTC_2_BIT					25

#define CTC_3_PORT					3
#define CTC_3_BIT					26



#define IO_0_PORT					0
#define IO_0_BIT					22

#define IO_1_PORT					0
#define IO_1_BIT					21

#define IO_2_PORT					0
#define IO_2_BIT					20

#define IO_3_PORT					0
#define IO_3_BIT					19


/**************************************************
Declaration des Reglages de la RTI et des Taches a executer
**************************************************/

//#define PERIODE_INTERRUPTIONS_RTI 		(float)(1000/(float)configTICK_RATE_HZ)  //periode des interruptions RTI (ms)

//Periode d'appel de la Fonction de Mise a jour de la Position du Robot sur la Table
#define PERIODE_MISE_A_JOUR_POSITION 	10

//Periode d'appel de la Fonction de Mise a jour de la Position du Robot sur la Table
#define PERIODE_MANAGER_TRAJECTOIRE 	20

//Periode d'appel de la Fonction de PID en Distance et en Angle
#define PERIODE_PID_DISTANCE_ANGLE 		10

//Periode d'appel de la Fct de PID en Position des Moteurs Aux
#define PERIODE_PID_POS_MOTEURS_AUX 	1

//Periode d'appel du Manager deplacement des moteurs Aux
#define PERIODE_MANAGER_MOTEURS_AUX 	20

//Periode d'appel des PID en VITESSE
#define PERIODE_PID_VITESSE				5

//Periode de mise à jours des infos sur les capteurs/actionneurs
#define PERIODE_UPDATE					10

/*************************************************/

#endif /* CONFIGURATION_H_ */

/*
 * Configuration.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Type_Declaration.h"
#include "FreeRTOS.h"
#include "task.h"

#define MAJOR_RELEASE "4"
#define MINOR_RELEASE "3a"


//#define CARTE_LPCXPRESSO //Utilisation sur la Carte LPC_Xpresso

#ifdef CARTE_LPCXPRESSO
#define USE_USB
#else
#ifdef TYPE_CARTE_IA
//#define USE_USB
#endif
#endif

//Utilise la tracabilite des ISR et des niveau de buffer de communication
#ifdef DEBUG
#define config_debug_Trace_ISR_AND_Buffer_Level	1
#endif

/**************************************************
Declaration des macro des Tempo pour FreeRTOS
 **************************************************/

#define Init_Timing_Tache TickType_t xLastWakeTime; xLastWakeTime = xTaskGetTickCount ();
#define Task_Delay_Until(Time) vTaskDelayUntil(&xLastWakeTime, (float)Time /((float)(1000/(float)configTICK_RATE_HZ)));
#define Task_Delay(Time) (Time > 0? vTaskDelay((float)Time /((float)(1000/(float)configTICK_RATE_HZ))) : 0);

#define ms_to_tick(Time) (Time > 0 ? (float)Time /((float)(1000/(float)configTICK_RATE_HZ)) : 0)

#define Task_Get_Current_Handle xTaskGetCurrentTaskHandle()
#define Task_Delete_Current vTaskDelete(xTaskGetCurrentTaskHandle());
#define Task_Suspend_Current vTaskSuspend(xTaskGetCurrentTaskHandle());

#define WAIT(condition, temps)  while(condition){ Task_Delay(temps); }

/*************************************************/

#define TO_AHBS_RAM3 __attribute__((section(".bss.$RAM3")))


#define OK 1
#define NOK 0
#define OUTPUT 1
#define INPUT 0
#define ON 1
#define OFF 0

#ifdef CARTE_LPCXPRESSO
/**************************************************
Declaration des Entrees/Sorties TOR et parametres
 **************************************************/
#define LED_0_PORT					1
#define LED_0_BIT					16

#define LED_1_PORT					1
#define LED_1_BIT					15

#define LED_2_PORT					0
#define LED_2_BIT					22

#else
#define LED_0_PORT					1
#define LED_0_BIT					16

#define LED_1_PORT					1
#define LED_1_BIT					15

#define LED_2_PORT					1
#define LED_2_BIT					14
#endif

/**************************************************
Déclaration des cpins de debug
 **************************************************/
#define DEBUG_0_PORT				2
#define DEBUG_0_BIT					0

#define DEBUG_1_PORT				2
#define DEBUG_1_BIT					7


/**************************************************
Déclaration des cannaux des Codeurs
 **************************************************/
#define Codeur_D_Cannal_A_Port		0
#define Codeur_D_Cannal_A_Bit		7

#define Codeur_D_Cannal_B_Port		0
#define Codeur_D_Cannal_B_Bit		6

#define Codeur_G_Cannal_A_Port		0
#define Codeur_G_Cannal_A_Bit		8

#define Codeur_G_Cannal_B_Port		0
#define Codeur_G_Cannal_B_Bit		9

/* Déclaration des Sorties et Timer des Moteurs */
#define DIR_MOTEUR_1_GAUCHE_PORT	0
#define DIR_MOTEUR_1_GAUCHE_BIT		4

#define DIR_MOTEUR_2_DROIT_PORT 	1
#define DIR_MOTEUR_2_DROIT_BIT		17


/**************************************************
Declaration des Reglages liés à la Communication RS485
 **************************************************/
#define BAUDRATE_RS485				2000000
#define RS484_UART					LPC_UART1

#define BAUDRATE_XBEE				115200
#define XBEE_UART					LPC_UART2

#define RS485_IRQ_SELECTION 		UART1_IRQn
#define RS485_HANDLER_NAME 			UART1_IRQHandler

#define XBEE_IRQ_SELECTION 			UART2_IRQn
#define XBEE_HANDLER_NAME 			UART2_IRQHandler

#define RS485_DIR_PORT 				2
#define RS485_DIR_BIT  				3


/**************************************************
Declaration de l'adresse de la carte, en fonction de son Type ou de ses bits d'adressages
 **************************************************/
#ifdef TYPE_CARTE_IA

#define ADRESSE_CARTE 				IA_BOARD
#define ADRESSE_CARTE_CHAR 			"0" //IA_BOARD

#define GROS_ROBOT 					0
#define PETIT_ROBOT 				1

#define GROS_PETIT_PIN_PORT			2
#define GROS_PETIT_PIN_BIT			11


#define JACK_PORT					1
#define JACK_BIT					4

#define COLOR_PORT					1
#define COLOR_BIT					8

#define SWITCH_PORT					1
#define SWITCH_BIT					10

#define STRATEGIE_PORT				1
#define STRATEGIE_BIT				9

#define LED_EXTERNE_RED_PORT		2
#define LED_EXTERNE_RED_BIT			4

#define LED_EXTERNE_YELLOW_PORT		2
#define LED_EXTERNE_YELLOW_BIT		5

#define LED_EXTERNE_GREEN_PORT		2
#define LED_EXTERNE_GREEN_BIT		6

//Pin demesure de la tension 'strategie'
#define ADC_CHANNEL_7_PORT			0
#define ADC_CHANNEL_7_BIT			2

#else

#define ADRESSE_0_PORT				1
#define ADRESSE_0_BIT				8
#define ADRESSE_1_PORT				1
#define ADRESSE_1_BIT				4

#define ADRESSE_CARTE 				1 + (!Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_0_PORT, ADRESSE_0_BIT) << 1 | !Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_1_PORT, ADRESSE_1_BIT))
#define ADRESSE_CARTE_CHAR 			'1' + (!Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_0_PORT, ADRESSE_0_BIT) << 1 | !Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_1_PORT, ADRESSE_1_BIT))

#endif



/**************************************************
Declaration des Reglages de la RTI et des Taches a executer
 **************************************************/

#define PERIODE_INTERRUPTIONS_RTI 		(float)(1000/(float)configTICK_RATE_HZ)  //periode des interruptions RTI (ms)

//Periode d'appel de la Fonction de Mise a jour de la Position du Robot sur la Table
#define PERIODE_MISE_A_JOUR_POSITION 	10

//Periode d'appel de la Fonction de Mise a jour de la Position du Robot sur la Table
#define PERIODE_MANAGER_TRAJECTOIRE 	20

//Periode d'appel de la Fonction de PID en Distance et en Angle
#define PERIODE_PID_DISTANCE_ANGLE 		1

//Periode d'appel de la Fct de PID en Position des Moteurs Aux
#define PERIODE_PID_POS_MOTEURS_AUX 	1

//Periode d'appel du Manager deplacement des moteurs Aux
#define PERIODE_MANAGER_MOTEURS_AUX 	20

//Periode d'appel des PID en VITESSE
#define PERIODE_PID_VITESSE				5

/*************************************************/

#endif /* CONFIGURATION_H_ */

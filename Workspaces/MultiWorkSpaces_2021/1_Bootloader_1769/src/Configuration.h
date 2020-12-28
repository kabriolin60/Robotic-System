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

#define MAJOR_RELEASE "1"
#define MINOR_RELEASE "2a"


//#define CARTE_LPCXPRESSO //Utilisation sur la Carte LPC_Xpresso
#define TYPE_CARTE_IA


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

#define RS485_IRQ_SELECTION 		UART1_IRQn
#define RS485_HANDLER_NAME 			UART1_IRQHandler

#define RS485_DIR_PORT 				2
#define RS485_DIR_BIT  				3


/**************************************************
Declaration de l'adresse de la carte, en fonction de son Type ou de ses bits d'adressages
**************************************************/
#ifdef TYPE_CARTE_IA

#define ADRESSE_CARTE IA_BOARD
#define ADRESSE_CARTE_CHAR "0" //IA_BOARD

#define GROS_ROBOT 0
#define PETIT_ROBOT 1

#else

#define ADRESSE_0_PORT				1
#define ADRESSE_0_BIT				8
#define ADRESSE_1_PORT				1
#define ADRESSE_1_BIT				4

#define ADRESSE_CARTE 				1 + (!Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_0_PORT, ADRESSE_0_BIT) << 1 | !Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_1_PORT, ADRESSE_1_BIT))
#define ADRESSE_CARTE_CHAR 			'1' + (!Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_0_PORT, ADRESSE_0_BIT) << 1 | !Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_1_PORT, ADRESSE_1_BIT))

#endif


/**************************************************
Declaration des parametres pour les Servos et AX 12
**************************************************/
#define SERVO_ON_OFF_PORT			1
#define SERVO_ON_OFF_BIT			27

#define AX_12_ON_OFF_PORT			1
#define AX_12_ON_OFF_BIT			28



#endif /* CONFIGURATION_H_ */

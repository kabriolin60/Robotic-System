/*
 * Configuration.h
 *
 *  Created on: 27 déc. 2020
 *      Author: kabri
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#define MAJOR_RELEASE "1"
#define MINOR_RELEASE "0a"

#define CARTE_LPCXPRESSO //Utilisation sur la Carte LPC_Xpresso


#define TYPE_CARTE_PERIPHERIQUES	0

#define TYPE_CARTE 					TYPE_CARTE_PERIPHERIQUES


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
#define LED_0_PORT					0
#define LED_0_BIT					7

#define LED_1_PORT					1
#define LED_1_BIT					15

#define LED_2_PORT					0
#define LED_2_BIT					7

#else
#define LED_0_PORT					1
#define LED_0_BIT					16

#define LED_1_PORT					1
#define LED_1_BIT					15

#define LED_2_PORT					1
#define LED_2_BIT					14
#endif

/************************************************/


/**************************************************
Déclaration des cpins de debug
 **************************************************/
#define DEBUG_0_PORT				2
#define DEBUG_0_BIT					0

#define DEBUG_1_PORT				2
#define DEBUG_1_BIT					7

/************************************************/

/**************************************************
Declaration des Reglages liés à la Communication RS485
 **************************************************/
#define RS485_UART					LPC_USART

#define RS485_IRQ_SELECTION 		UART0_IRQn
#define RS485_HANDLER_NAME 			UART_IRQHandler

#define RS485_DIR					IOCON_PIO0_7
#define RS485_DIR_PORT				0
#define RS485_DIR_BIT				2

/**************************************************
Declaration de l'adresse de la carte, en fonction de son Type ou de ses bits d'adressages
 **************************************************/
#ifdef TYPE_CARTE_PERIPHERIQUES

#define ADRESSE_0_port				0
#define ADRESSE_0_bit				8
#define ADRESSE_1_port				0
#define ADRESSE_1_bit				9

#define ADRESSE_CARTE 				5 + (Chip_GPIO_GetPinState(LPC_GPIO_PORT, ADRESSE_0_port, ADRESSE_0_bit) << 1 | Chip_GPIO_GetPinState(LPC_GPIO_PORT, ADRESSE_1_port, ADRESSE_1_bit))
#define ADRESSE_CARTE_CHAR 			'5' + (Chip_GPIO_GetPinState(LPC_GPIO_PORT, ADRESSE_0_port, ADRESSE_0_bit) << 1 | Chip_GPIO_GetPinState(LPC_GPIO_PORT, ADRESSE_1_port, ADRESSE_1_bit))

#endif

#endif /* CONFIGURATION_H_ */

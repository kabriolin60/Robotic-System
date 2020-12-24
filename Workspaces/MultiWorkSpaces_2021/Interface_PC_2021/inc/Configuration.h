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
#define MINOR_RELEASE "2b"

/**************************************************
Declaration des options de Debug
 **************************************************/
#define LOG_Debug_Port 				RS485_port//Xbee_port//RS485_port

/*************************************************/

#define CARTE_LPCXPRESSO //Utilisation sur la Carte LPC_Xpresso

#ifdef CARTE_LPCXPRESSO
//#define USE_USB
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
Declaration des Reglages liés à la Communication RS485
 **************************************************/
#define RS484_UART					LPC_UART1

#define RS485_2_UART				LPC_UART3

#define BAUDRATE_XBEE				57600
#define XBEE_UART					LPC_UART2

#define RS485_IRQ_SELECTION 		UART1_IRQn
#define RS485_HANDLER_NAME 			UART1_IRQHandler

#define RS485_2_IRQ_SELECTION 		UART3_IRQn
#define RS485_2_HANDLER_NAME 		UART3_IRQHandler

#define XBEE_IRQ_SELECTION 			UART2_IRQn
#define XBEE_HANDLER_NAME 			UART2_IRQHandler

#define RS485_DIR_PORT 				2
#define RS485_DIR_BIT  				3

#define RS485_2_DIR_PORT 			1
#define RS485_2_DIR_BIT  			19


/**************************************************
Declaration de l'adresse de la carte, en fonction de son Type ou de ses bits d'adressages
 **************************************************/
#define ADRESSE_CARTE 				INTERFACE_PC
#define ADRESSE_CARTE_CHAR 			"0" //IA_BOARD

#define LED_EXTERNE_RED_PORT		2
#define LED_EXTERNE_RED_BIT			4

#define LED_EXTERNE_YELLOW_PORT		2
#define LED_EXTERNE_YELLOW_BIT		5

#define LED_EXTERNE_GREEN_PORT		2
#define LED_EXTERNE_GREEN_BIT		6

#endif /* CONFIGURATION_H_ */

/*
 * Commun_Configuration.h
 *
 *  Created on: 2 déc. 2020
 *      Author: PC-INOVA
 */

#ifndef COMMUN_CONFIGURATION_H_
#define COMMUN_CONFIGURATION_H_

#define OK 1
#define NOK 0
#define OUTPUT 1
#define INPUT 0
#define ON 1
#define OFF 0

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define AT_FILE_LINE __FILE__ ":" TOSTRING(__LINE__)
#define AT_LINE TOSTRING(__LINE__)


/**************************************************
Declaration des constantes mathematiques
**************************************************/

#define PI 	3.14159265358979F           //definition du nombre PI

/*************************************************/


/**************************************************
Declaration des macro mathematiques
**************************************************/

#define Convert_Deg_Rad(x)  x*PI/180
#define Convert_Rad_Deg(x)  x*180/PI

/*************************************************/

#define TO_AHBS_RAM3 __attribute__((section(".bss.$RAM3")))

#ifdef CARTE_LPCXPRESSO
/**************************************************
Declaration des Leds onboard
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
Déclaration des pins de debug
**************************************************/
#define DEBUG_0_PORT				2
#define DEBUG_0_BIT					0

#define DEBUG_1_PORT				2
#define DEBUG_1_BIT					7


/**************************************************
Declaration des Reglages liés à la Communication RS485
**************************************************/
#define BAUDRATE_RS485				2000000
#define RS484_UART					LPC_UART1

#define RS485_IRQ_SELECTION 		UART1_IRQn
#define RS485_HANDLER_NAME 			UART1_IRQHandler

#define RS485_DIR_PORT 				2
#define RS485_DIR_BIT  				3

#endif /* COMMUN_CONFIGURATION_H_ */

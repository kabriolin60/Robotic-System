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
Macro de mise en RAM3
**************************************************/
#define TO_AHBS_RAM3 __attribute__((section(".bss.$RAM3")))

/*************************************************/


/**************************************************
Definition de variables fixes de configuration et types de cartes
**************************************************/
#define TYPE_IA_BOARD 	IA_BOARD
#define TYPE_MULTIFCT	1

static TO_AHBS_RAM3 uint8_t static_IS_LPCXPRESSO_BOARD;
static TO_AHBS_RAM3 uint8_t static_BOARD_TYPE;

/*************************************************/


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



/**************************************************
Declaration des Leds onboard
**************************************************/
#define LED_0_PORT					1
#define LED_0_BIT					16

#define LED_1_PORT					1
#define LED_1_BIT					15

#define LED_2_PORT					1
#define LED_2_BIT					14

/**************************************************
Déclaration des pins de debug
**************************************************/
#define DEBUG_0_PORT				2
#define DEBUG_0_BIT					0

#define DEBUG_1_PORT				2
#define DEBUG_1_BIT					7


/**************************************************
Declaration des Reglages liés à la Communication
**************************************************/
#define BAUDRATE_RS485				2000000
#define RS484_UART					LPC_UART1

#define RS485_IRQ_SELECTION 		UART1_IRQn
#define RS485_HANDLER_NAME 			UART1_IRQHandler

#define RS485_DIR_PORT 				2
#define RS485_DIR_BIT  				3

#define BAUDRATE_XBEE				57600
#define XBEE_UART					LPC_UART2

#define XBEE_IRQ_SELECTION 			UART2_IRQn
#define XBEE_HANDLER_NAME 			/*static_BOARD_TYPE == TYPE_IA_BOARD? UART2_IRQHandler: */XBEE_HANDLER

/**************************************************
Declaration de l'adresse de la carte
**************************************************/
#define ADRESSE_0_PORT				1
#define ADRESSE_0_BIT				8
#define ADRESSE_1_PORT				1
#define ADRESSE_1_BIT				4

#define ADRESSE_CARTE 				(static_BOARD_TYPE == TYPE_IA_BOARD? IA_BOARD : 1 + (!Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_0_PORT, ADRESSE_0_BIT) << 1 | !Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_1_PORT, ADRESSE_1_BIT)))
#define ADRESSE_CARTE_CHAR 			(static_BOARD_TYPE == TYPE_IA_BOARD? '0' : '1' + (!Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_0_PORT, ADRESSE_0_BIT) << 1 | !Chip_GPIO_GetPinState(LPC_GPIO, ADRESSE_1_PORT, ADRESSE_1_BIT)))

#endif /* COMMUN_CONFIGURATION_H_ */

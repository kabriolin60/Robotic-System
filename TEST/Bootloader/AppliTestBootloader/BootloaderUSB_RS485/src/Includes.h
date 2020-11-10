#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "LPC17xx.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

#include "Gpio.h"
#include "Clocking.h"
#include "Init.h"


#include "Communication_NG.h"
#include "Communication_struct.h"
#include "cdc_vcom.h"

#include "Declarations.h"

#include "Echange_Data.h"
#include "Uart1.h"
#include "FIFO.h"
#include "Bootloader.h"

#include "Test.h"

/**************************************************
Declaration des Entrees/Sorties TOR et parametres
**************************************************/
#define LED_0_PORT					1
#define LED_0_BIT					16

#define LED_1_PORT					0//1
#define LED_1_BIT					22//15

#define LED_2_PORT					1
#define LED_2_BIT					14

#define RS485_DIR_PORT 				2

#ifdef CARTE_LPCXPRESSO
#define RS485_DIR_BIT  				2	//carte LPC
#define PORT_RS485_IA				2
#else
#define RS485_DIR_BIT  				3	//carte IA
#define PORT_RS485_IA				0
#endif

#define BAUDRATE_RS485				500000
#define BAUDRATE_DEBUG				2000000
#define BAUDRATE_AX_12				1000000


#define ADRESSE_CARTE IA_BOARD
#define ADRESSE_CARTE_CHAR "0" //IA_BOARD


#define QueueEchangeTX_DEBUG 	QueueEchangeTX_RS485

/*************************************************/


/**************************************************
Declaration des Reglages de la RTI et des Taches a executer
**************************************************/
   
#define PERIODE_INTERRUPTIONS_RTI 		(1000/configTICK_RATE_HZ)  //periode des interruptions RTI (ms)

#define PERIODE_CALCUL_ASTAR 			200

#define DELAY_BOUCLE_CONTROLE 			5

/*************************************************/


/**************************************************
Declaration des constantes mathematiques
**************************************************/
#define PI 	3.14159265358979F                             //definition du nombre PI

/*************************************************/


/**************************************************
Declaration des macro mathematiques
**************************************************/
#define Convert_Deg_Rad(x)  x*PI/180
#define Convert_Rad_Deg(x)  x*180/PI

/*************************************************/ 

#endif

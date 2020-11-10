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

#include "Declarations.h"

#include "Test.h"

/**************************************************
Declaration des Entrees/Sorties TOR et parametres
**************************************************/
#define LED_0_PORT					1
#define LED_0_BIT					16

#define LED_1_PORT					1
#define LED_1_BIT					15

#define LED_2_PORT					1
#define LED_2_BIT					14

#define RS485_DIR_PORT 				2
#define RS485_DIR_BIT  				3

#define BAUDRATE_RS485				500000
#define BAUDRATE_DEBUG				2000000
#define BAUDRATE_AX_12				1000000

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

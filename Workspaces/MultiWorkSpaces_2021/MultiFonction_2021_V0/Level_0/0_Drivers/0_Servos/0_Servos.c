/*
 * 0_Servos.c
 *
 *  Created on: 30 avr. 2020
 *      Author: kabri
 */

#include "0_Servos.h"





/**************************************************
Fonction d'init des Servos de niveau 0
Input: None
Output: None
 **************************************************/
void _0_Servo_Init(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, SERVO_ON_OFF_PORT, SERVO_ON_OFF_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, SERVO_ON_OFF_PORT, SERVO_ON_OFF_BIT, true);

	//Commencer par couper l'alimentation des Servos
	_0_Servo_Power(false);

	//Init du module PWM pour le pilotage des servos
	PWM_Init(26000);
	PWM_Start(1);
}


/**************************************************
Fonction de gestion de l'alimentation des servos
Input: Power ON/OFF
Output: None
 *************************************************/
void _0_Servo_Power(bool power)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, SERVO_ON_OFF_PORT, SERVO_ON_OFF_BIT, power);
}







/*
 * Init.c
 *
 *  Created on: 1 févr. 2014
 *      Author: Fixe
 */

#include "Init.h"

void Init_Robot(void)
{
	GPIOSetDir(LED_0_PORT, LED_0_BIT, 1); //Led 0
	GPIOSetDir(LED_1_PORT, LED_1_BIT, 1); //Led 1
	GPIOSetDir(LED_2_PORT, LED_2_BIT, 1); //Led 2

	Init_Echange_Data();
}

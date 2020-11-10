/*
 * Test.c
 *
 *  Created on: 9 févr. 2014
 *      Author: Fixe
 */

#include "Test.h"

#define clignotement_court GPIOSetValue(LED_1_PORT, LED_1_BIT, 1); Task_Delay(300); GPIOSetValue(LED_1_PORT, LED_1_BIT, 0); Task_Delay(300);
#define clignotement_long GPIOSetValue(LED_1_PORT, LED_1_BIT, 1); Task_Delay(600); GPIOSetValue(LED_1_PORT, LED_1_BIT, 0); Task_Delay(600);

void ClignotementLed(uint32_t data)
{
	clignotement_long;
	uint32_t nb_clig = 0;
	while(data != 0)
	{
		nb_clig = data % 10;
		while(nb_clig--)
		{
			clignotement_court;
		}
		Task_Delay(500);
		data /= 10;
	}
}

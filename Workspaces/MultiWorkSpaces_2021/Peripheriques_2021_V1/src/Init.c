/*
 * Init.c
 *
 *  Created on: 27 déc. 2020
 *      Author: kabri
 */

#include "chip.h"
#include "FreeRTOSConfig.h"

#include "Init.h"
#include "0_Communication.h"
#include "1_Trame_Communication.h"
#include "0_Event_Group.h"
#include "2_Echange_Datas.h"
#include "2_Echange_Datas_Reception.h"

void Init_Carte_Perpheriques(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, ADRESSE_0_port, ADRESSE_0_bit, IOCON_MODE_PULLUP, IOCON_FUNC0);	// GPIO
	Chip_IOCON_PinMux(LPC_IOCON, ADRESSE_1_port, ADRESSE_1_bit, IOCON_MODE_PULLUP, IOCON_FUNC0);	// GPIO
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED_2_PORT, LED_2_BIT, false); //@pin as input
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED_1_PORT, LED_1_BIT, false); //@pin as input


	/* Init du Groupe d'event de synchronisation */
	_0_Communication_Init_Event_Group();

	/* Init de la communication Niveau 0 */
	_0_Communication_Init();

	/* Init de la communication Niveau 1 */
	_1_Communication_Init();

	/* Init de la communication Niveau 2 */
	_2_Communication_Init();

	/* Init de la communication Niveau 2 Reception*/
	_2_Communication_RX_Init();
}



void vConfigureTimerForRunTimeStats( void )
{
	const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* Power up and feed the timer with a clock. */
	Chip_TIMER_Init(LPC_TIMER32_0);

	/* Reset Timer 0 */
	LPC_TIMER32_0->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIMER32_0->TCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
     but not too fast so as to overflow all the time. */
	LPC_TIMER32_0->PR =  ( configCPU_CLOCK_HZ / 50000UL ) - 1UL;

	/* Start the counter. */
	LPC_TIMER32_0->TCR = TCR_COUNT_ENABLE;
}

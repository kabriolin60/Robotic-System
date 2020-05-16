/*
 * 0_Motors.c
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#include "0_Motors.h"

/******************************************************************************
 ** Function name:		_0_GPIO_Moteurs
 **
 ** Descriptions:		Init les sorties GPIO pour les moteurs
 **
 ** parameters:			None
 ** Returned value:		None
 **
 ******************************************************************************/
__attribute__((optimize("O0"))) void _0_GPIO_Moteurs_Init(void)
{
	//Dir Moteur Gauche
	Chip_IOCON_PinMux(LPC_IOCON, DIR_MOTEUR_1_GAUCHE_PORT, DIR_MOTEUR_1_GAUCHE_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DIR_MOTEUR_1_GAUCHE_PORT, DIR_MOTEUR_1_GAUCHE_BIT, true);

	//PWM Moteur Gauche
	Chip_IOCON_PinMux(LPC_IOCON, 4, 28, IOCON_MODE_INACT, IOCON_FUNC2);

	//Dir Moteur Droit
	Chip_IOCON_PinMux(LPC_IOCON, DIR_MOTEUR_2_DROIT_PORT, DIR_MOTEUR_2_DROIT_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DIR_MOTEUR_2_DROIT_PORT, DIR_MOTEUR_2_DROIT_BIT, true);

	//PWM Moteur Droit
	Chip_IOCON_PinMux(LPC_IOCON, 4, 29, IOCON_MODE_INACT, IOCON_FUNC2);
}


/******************************************************************************
 ** Function name:		_0_Init_Timer
 **
 ** Descriptions:		Init Timers Moteurs pour les sorties PWM
 **
 ** parameters:			None
 ** Returned value:		None
 **
 ******************************************************************************/
__attribute__((optimize("O0"))) void _0_Motors_Init(void)
{
	Chip_TIMER_Init(LPC_TIMER2);/* Enable timer 2 clock */
	Chip_TIMER_Reset(LPC_TIMER2);/* reset timer */
	LPC_TIMER2->MR[0] = 0; /* set the count value */
	LPC_TIMER2->MR[1] = 0; /* set the count value */
	LPC_TIMER2->MR[2] = 2000; /* set the count value */
	LPC_TIMER2->IR = 0xff; /* reset all interrupts */
	//LPC_TIMER2->PR = 0x03;
	Chip_TIMER_MatchDisableInt(LPC_TIMER2, 0);
	Chip_TIMER_MatchDisableInt(LPC_TIMER2, 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER2, 2);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER2, 2); /* reset timer on match */
	Chip_TIMER_ExtMatchControlSet(LPC_TIMER2, 0, TIMER_EXTMATCH_CLEAR, 0);
	Chip_TIMER_ExtMatchControlSet(LPC_TIMER2, 0, TIMER_EXTMATCH_CLEAR, 1);
	Chip_TIMER_Enable(LPC_TIMER2);/* start the timer */

	NVIC_SetPriority(TIMER2_IRQn, 5);
	NVIC_EnableIRQ(TIMER2_IRQn);
}


/******************************************************************************
 ** Function name:		Timer2_IRQHandler
 **
 ** Descriptions:		Timer/Counter 2 interrupt handler
 **
 ** parameters:			None
 ** Returned value:		None
 **
 ******************************************************************************/
__attribute__((optimize("O3"))) void TIMER2_IRQHandler(void)
{
	if(LPC_TIMER2->MR[0] > 50)
		LPC_TIMER2->EMR |= (1<<0);

	if(LPC_TIMER2->MR[1] > 50)
		LPC_TIMER2->EMR |= (1<<1);

	LPC_TIMER2->IR |= 0x1<<2;		/* clear interrupt flag */
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
}

__attribute__((optimize("O0"))) void TIMER0_IRQHandler(void)
{
	NVIC_DisableIRQ(TIMER0_IRQn);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
}

__attribute__((optimize("O0"))) void TIMER1_IRQHandler(void)
{
	NVIC_DisableIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
}


/******************************************************************************
 ** Function name:		_0_Set_Motor_PWM_Value
 **
 ** Descriptions:		Set output PWM value
 **
 ** parameters:			Timer numer
 ** 					Timer output
 ** 					% * 100
 ** Returned value:		None
 **
 ******************************************************************************/
__attribute__((optimize("O0"))) void _0_Set_Motor_PWM_Value(LPC_TIMER_T *pTMP, int8_t matchnum, uint32_t matchvalue)
{
	float value = matchvalue;

	if(matchvalue > 100)
		matchvalue = 100;

	if(matchvalue < 5)
		matchvalue = 0;

	value = matchvalue * pTMP->MR[2];//pTMP->PR; //Le compteur [2] correspond à la reference 100% de PWM
	value /= 100;

	pTMP->MR[matchnum] = (uint32_t)value;
}


/******************************************************************************
 ** Function name:		_0_Set_Motor_Direction
 **
 ** Descriptions:		Set output direction
 **
 ** parameters:			dir moteur 1
 ** 					dir moteur 2
 ** Returned value:		None
 **
 ******************************************************************************/
__attribute__((optimize("O0"))) void _0_Set_Motor_Direction(bool dir_mot_1, bool dir_mot_2)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, DIR_MOTEUR_1_GAUCHE_PORT, DIR_MOTEUR_1_GAUCHE_BIT, dir_mot_1);
	Chip_GPIO_WritePortBit(LPC_GPIO, DIR_MOTEUR_2_DROIT_PORT, DIR_MOTEUR_2_DROIT_BIT, dir_mot_2);
}


/******************************************************************************
 ** Function name:		_0_Set_Motor_Power
 **
 ** Descriptions:		Envoi un PWM ou non au moteur indiqué
 **
 ** parameters:			Moteur number
 ** 					Power ON/OFF
 ** Returned value:		None
 **
 ******************************************************************************/
__attribute__((optimize("O0"))) void _0_Set_Motor_Power(uint8_t moteur_number, bool power)
{
	switch(moteur_number)
	{
	case 0: //Moteur Gauche
		if(power)
		{
			Chip_IOCON_PinMux(LPC_IOCON, 4, 28, IOCON_MODE_INACT, IOCON_FUNC2);
		}else
		{
			Chip_IOCON_PinMux(LPC_IOCON, 4, 28, IOCON_MODE_INACT, IOCON_FUNC0);
			LPC_TIMER2->MR[0] = 0;
		}
		break;

	case 1: //Moteur Droit
		if(power)
		{
			Chip_IOCON_PinMux(LPC_IOCON, 4, 29, IOCON_MODE_INACT, IOCON_FUNC2);
		}else
		{
			Chip_IOCON_PinMux(LPC_IOCON, 4, 29, IOCON_MODE_INACT, IOCON_FUNC0);
			LPC_TIMER2->MR[1] = 0;
		}
		break;

	default:
		Chip_IOCON_PinMux(LPC_IOCON, 4, 28, IOCON_MODE_INACT, IOCON_FUNC0);
		Chip_IOCON_PinMux(LPC_IOCON, 4, 29, IOCON_MODE_INACT, IOCON_FUNC0);
		LPC_TIMER2->MR[0] = 0;
		LPC_TIMER2->MR[1] = 0;
		break;
	}
}

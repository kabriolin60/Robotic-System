/*
 * 0_Codeurs.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef _0_CODEURS_H_
#define _0_CODEURS_H_

#include "board.h"
#include "Configuration.h"

#define USE_ALL_CODEURS_EGDES 1


struct Codeurs_Values
{
	int32_t Codeur_Droit;
	int32_t Codeur_Gauche;
};

struct Codeurs_Values _0_Codeurs_Get_CodeursValues();

void _0_Codeurs_Increment_Simulation(long increment_D, long increment_G);


void _0_Codeurs_Init(void);


/*****************************************************************************
 ** Function name:		GPIOSetInterrupt
 **
 ** Descriptions:
 **
 ** parameters:			port num, bit position, Rising, Falling
 ** Returned value:		None
 **
 *****************************************************************************/

static INLINE void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint8_t Rising, uint8_t Falling)
{
	/* Configure the GPIO interrupt */
	if(Falling)
		Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, portNum, 1 << bitPosi);

	if(Rising)
		Chip_GPIOINT_SetIntRising(LPC_GPIOINT, portNum, 1 << bitPosi);
}

__attribute__((optimize("O3"))) void EINT3_IRQHandler(void);

__attribute__((optimize("O3"))) void EINT0_IRQHandler(void);

__attribute__((optimize("O3"))) void EINT1_IRQHandler(void);

__attribute__((optimize("O3"))) void EINT2_IRQHandler(void);

__attribute__((optimize("O3"))) void Codeurs_Interrupts(void);

void _0_Codeurs_Reset(void);

#endif /* 0_CODEURS_0_CODEURS_H_ */

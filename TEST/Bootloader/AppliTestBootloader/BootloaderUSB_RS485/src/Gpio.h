/*
 * Gpio_It_Test.h
 *
 *  Created on: 28 sept. 2013
 *      Author: nicolas
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "LPC17xx.h"

#include "Includes.h"

#define PORT0		0
#define PORT2		2

static LPC_GPIO_TypeDef (* const LPC_GPIO[4]) = { LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3 };

void EINT3_IRQHandler(void);

void PIOINT0_IRQHandler(void);

void PIOINT2_IRQHandler(void);

void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t Rising, uint32_t Falling);

int8_t GPIOIntGetStatus( uint32_t portNum, uint32_t bitPosi, uint8_t edge);

void GPIOIntClear( uint32_t portNum, uint32_t bitPosi );

unsigned char GPIOGetValue( uint32_t portNum, uint32_t bitPosi);

void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );

void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );

void Toogle_Pin(unsigned char Port, unsigned char Pin);

#endif /* GPIO_IT_TEST_H_ */

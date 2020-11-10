/*
 * Gpio_It_Test.c
 *
 *  Created on: 28 sept. 2013
 *      Author: nicolas
 */


#include "lpc17xx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Gpio.h"


/*****************************************************************************
 ** Function name:		GPIOSetInterrupt
 **
 ** Descriptions:
 **
 ** parameters:			port num, bit position, Rising, Falling
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t Rising, uint32_t Falling)
{
	switch ( portNum )
	{
	case PORT0:
		//Enable Rising Edge
		if(Rising)
		{
			LPC_GPIOINT->IO0IntEnR |= (0x1<<bitPosi);
		}else
		{
			LPC_GPIOINT->IO0IntEnR &= ~(0x1<<bitPosi);
		}

		//Enable Falling
		if(Falling)
		{
			LPC_GPIOINT->IO0IntEnF |= (0x1<<bitPosi);
		}else
		{
			LPC_GPIOINT->IO0IntEnF &= ~(0x1<<bitPosi);
		}
		break;

	case PORT2:
		//Enable Rising Edge
		if(Rising)
		{
			LPC_GPIOINT->IO2IntEnR |= (0x1<<bitPosi);
		}else
		{
			LPC_GPIOINT->IO2IntEnR &= ~(0x1<<bitPosi);
		}

		//Enable Falling
		if(Falling)
		{
			LPC_GPIOINT->IO2IntEnF |= (0x1<<bitPosi);
		}else
		{
			LPC_GPIOINT->IO2IntEnF &= ~(0x1<<bitPosi);
		}
		break;

	default:
		break;
	}
	return;
}


/*****************************************************************************
 ** Function name:		GPIOIntGetStatus
 **
 ** Descriptions:		Get Interrupt Status for a port pin.
 **
 ** parameters:			port num, bit position, edge (1: rising, 0: falling)
 ** Returned value:		1: OK, 0: NOK
 **
 *****************************************************************************/
int8_t GPIOIntGetStatus( uint32_t portNum, uint32_t bitPosi, uint8_t edge )
{
	switch ( portNum )
	{
	case PORT0:
		if(edge)
		{
			if(LPC_GPIOINT->IO0IntStatR & (0x1<<bitPosi))
			{
				return 1;
			}else
			{
				return 0;
			}
		}else
		{
			if(LPC_GPIOINT->IO0IntStatF & (0x1<<bitPosi))
			{
				return 1;
			}else
			{
				return 0;
			}
		}
		break;

	case PORT2:
		if(edge)
		{
			if(LPC_GPIOINT->IO2IntStatR & (0x1<<bitPosi))
			{
				return 1;
			}else
			{
				return 0;
			}
		}else
		{
			if(LPC_GPIOINT->IO2IntStatF & (0x1<<bitPosi))
			{
				return 1;
			}else
			{
				return 0;
			}
		}
		break;

	default:
		break;
	}

	return 0;
}


/*****************************************************************************
 ** Function name:		GPIOIntClear
 **
 ** Descriptions:		Clear Interrupt for a port pin.
 **
 ** parameters:			port num, bit position
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi )
{
	switch ( portNum )
	{
	case PORT0:
		LPC_GPIOINT->IO0IntClr |= (0x1<<bitPosi);
		break;

	case PORT2:
		LPC_GPIOINT->IO2IntClr |= (0x1<<bitPosi);
		break;

	default:
		break;
	}
	return;
}


/*****************************************************************************
 ** Function name:		GPIOGetValue
 **
 ** Descriptions:		Get a bitvalue in a specific bit position
 **						in GPIO portX(X is the port number.)
 **
 ** parameters:			port num, bit position
 ** Returned value:		bit value
 **
 *****************************************************************************/
unsigned char GPIOGetValue( uint32_t portNum, uint32_t bitPosi)
{
	if(LPC_GPIO[portNum]->FIOPIN & (1<<bitPosi))
	{
		return 1;
	}else
	{
		return 0;
	}
}


/*****************************************************************************
 ** Function name:		GPIOSetDir
 **
 ** Descriptions:		Set the direction in GPIO port
 **
 ** parameters:			port num, bit position, direction (1 out, 0 input)
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
	if(dir)
		LPC_GPIO[portNum]->FIODIR |= 1<<bitPosi;
	else
		LPC_GPIO[portNum]->FIODIR &= ~(1<<bitPosi);
}


/*****************************************************************************
 ** Function name:		GPIOSetValue
 **
 ** Descriptions:		Set/clear a bitvalue in a specific bit position
 **						in GPIO portX(X is the port number.)
 **
 ** parameters:			port num, bit position, bit value
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
	if(bitVal)
	{
		LPC_GPIO[portNum]->FIOSET |= (1<<bitPosi);
	}else
	{
		LPC_GPIO[portNum]->FIOCLR |= (1<<bitPosi);
	}
}

void Toogle_Pin(unsigned char Port, unsigned char Pin)
{
	GPIOSetValue(Port, Pin, !GPIOGetValue(Port, Pin));
}

/*
 * 0_RS485.c
 *
 *  Created on: 24 avr. 2020
 *      Author: kabri
 */


#include "0_RS485.h"


/*****************************************************************************
 ** Function name:		_0_RS485_Init
 **
 ** Descriptions:		Fonction d'init de la pin du RS485
 **
 ** parameters:			Numéro du port ou se trouve la pin de direction du RS485
 ** 					Numéro du Bit ou se trouve la pin de direction du RS485
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_RS485_Init(uint8_t RS485_dir_port, uint8_t RS485_dir_bit)
{
	Chip_IOCON_PinMux(LPC_IOCON, RS485_dir_port, RS485_dir_bit, IOCON_MODE_INACT, IOCON_FUNC0);	// GPIO
	Chip_GPIO_WriteDirBit(LPC_GPIO, RS485_dir_port, RS485_dir_bit, true);	//dir pin en sortie
	_0_RS485_Slave_Mode(RS485_dir_port, RS485_dir_bit);
}


/*****************************************************************************
 ** Function name:		_0_RS485_Master_Mode
 **
 ** Descriptions:		Fonction de passage en mode Master du port RS485
 **
 ** parameters:			Numéro du port ou se trouve la pin de direction du RS485
 ** 					Numéro du Bit ou se trouve la pin de direction du RS485
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_RS485_Master_Mode(uint8_t RS485_dir_port, uint8_t RS485_dir_bit)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, RS485_dir_port, RS485_dir_bit, true);
}


/*****************************************************************************
 ** Function name:		_0_RS485_Slave_Mode
 **
 ** Descriptions:		Fonction de passage en mode Slave du port RS485
 **
 ** parameters:			Numéro du port ou se trouve la pin de direction du RS485
 ** 					Numéro du Bit ou se trouve la pin de direction du RS485
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_RS485_Slave_Mode(uint8_t RS485_dir_port, uint8_t RS485_dir_bit)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, RS485_dir_port, RS485_dir_bit, false);
}

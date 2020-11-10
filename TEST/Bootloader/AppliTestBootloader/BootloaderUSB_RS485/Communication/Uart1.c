/*
 * Uart1.c
 *
 *  Created on: 1 févr. 2014
 *      Author: Fixe
 */

#include "Uart1.h"

/*****************************************************************************
 ** Function name:		Init_UART_1
 **
 ** Descriptions:		Init du module de communication UART 1
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/

void Init_UART_1(unsigned long Baudrate, char port)
{
	unsigned long Fdiv;
	unsigned long pclkdiv, pclk;

	if(port == 0)
	{
		LPC_PINCON->PINSEL0 |= (1<<30);	/* Enable TxD1 P0.15 */
		LPC_PINCON->PINSEL1 |= (1<<0);	/* Enable RxD1 P0.16 */
	}

	if(port == 2)
	{
		LPC_PINCON->PINSEL4 |= (2<<0);	/* Enable TxD1 P2.0 */
		LPC_PINCON->PINSEL4 |= (2<<2);	/* Enable RxD1 P2.1 */
	}

	//UART1 Clock
	LPC_SC->PCLKSEL0 |= 1 << 8;

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 8,9 are for UART1 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
	switch ( pclkdiv )
	{
	case 0x00:
	default:
		pclk = SystemCoreClock/4;
		break;
	case 0x01:
		pclk = SystemCoreClock;
		break;
	case 0x02:
		pclk = SystemCoreClock/2;
		break;
	case 0x03:
		pclk = SystemCoreClock/8;
		break;
	}

	LPC_UART1->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	Fdiv = ( pclk / 16 ) / Baudrate ;	/*baud rate */
	LPC_UART1->DLM = Fdiv / 256;
	LPC_UART1->DLL = Fdiv % 256;
	LPC_UART1->LCR = 0x03;		/* DLAB = 0 */

	LPC_UART1->LCR |= 0x04;	//2 stops bits
	LPC_UART1->LCR |= 0x08;	//parity enabled

	LPC_UART1->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

	NVIC_EnableIRQ(UART1_IRQn);
}

/****************************************************************************/

/*****************************************************************************
 ** Function name:		UART_1_Read_Byte
 **
 ** Descriptions:		Read a byte to the UART 1 port based
 **
 ** parameters:			None
 ** Returned value:		Read byte or 0
 **
 *****************************************************************************/

unsigned char UART_1_Read_Byte(void)
{
	uint8_t IIRValue, LSRValue;

	IIRValue = LPC_UART1->IIR;

	IIRValue >>= 1;			/* skip pending bit in IIR */
	IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
	if ( IIRValue == IIR_RLS )		/* Receive Line Status */
	{
		LSRValue = LPC_UART1->LSR;
		/* Receive Line Status */
		if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
		{
			/* There are errors or break interrupt */
			/* Read LSR will clear the interrupt */
			return LPC_UART1->RBR;		/* Dummy read on RX to clear
									interrupt, then bail out */		}
		if ( LSRValue & LSR_RDR )	/* Receive Data Ready */
		{
			/* If no error on RLS, normal ready, save into the data buffer. */
			/* Note: read RBR will clear the interrupt */
			return LPC_UART1->RBR;
		}
	}
	else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
	{
		/* Receive Data Available */
		return LPC_UART1->RBR;
	}
	else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
	{
		return LPC_UART1->RBR;
	}
	else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
	{
		/* THRE interrupt */
		LSRValue = LPC_UART1->LSR;		/* Check status in the LSR to see if
									valid data in U0THR or not */
		return LPC_UART1->RBR;
	}

	return 0;
}


/*****************************************************************************
 ** Function name:		UART_1_Send_Byte
 **
 ** Descriptions:		Send a byte to the UART 1 port based
 **
 ** parameters:			data
 ** Returned value:		None
 **
 *****************************************************************************/
void UART_1_Send_Byte(unsigned char Data)
{
	LPC_UART1->THR = Data;
	while(!(LPC_UART1->LSR & 1<<6));
}

/****************************************************************************/


/*****************************************************************************
 ** Function name:		UART_1_Enable_Interrupts
 **
 ** Descriptions:		Activation des interruptions sur UART 1
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/

void UART_1_Enable_Interrupts(void)
{
	(void) LPC_UART1->IIR;
	(void) LPC_UART1->LSR;
	(void) LPC_UART1->RBR;		/* Dummy read on RX to clear interrupt, then bail out */
	LPC_UART1->IER = IER_RBR;// | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
}

/****************************************************************************/


/*****************************************************************************
 ** Function name:		UART_1_Disable_Interrupts
 **
 ** Descriptions:		Desactivation des interruptions sur UART 1
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/

void UART_1_Disable_Interrupts(void)
{
	LPC_UART1->IER = 0;//IER_THRE | IER_RLS;			/* Disable RBR */
}

/****************************************************************************/


/*****************************************************************************
 ** Function name:		UART1_IRQHandler
 **
 ** Descriptions:		UART1 interrupt handler
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
/*
void UART1_IRQHandler (void)
{
	NVIC_ClearPendingIRQ(UART1_IRQn);
}
 */

/****************************************************************************/


void Fct_NULL(void)
{

}


void RS485_Init_IO(void)
{
	GPIOSetDir( RS485_DIR_PORT, RS485_DIR_BIT, 1);		/* P2.3 defined as Outputs */
	GPIOSetValue( RS485_DIR_PORT, RS485_DIR_BIT, 0 );
}


void RS485_Master_Mode(void)
{
	GPIOSetValue( RS485_DIR_PORT, RS485_DIR_BIT, 1 );
}


void RS485_Slave_Mode(void)
{
	GPIOSetValue( RS485_DIR_PORT, RS485_DIR_BIT, 0 );
}

/*
 * 0_Codeurs.c
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */
#include "0_Codeurs.h"
#include "0_Motors.h"

static volatile struct Codeurs_Values Valeurs_Codeurs;
static uint32_t mask_interrupt_Codeur_D, mask_interrupt_Codeur_G;

/*****************************************************************************
 ** Function name:		_0_Codeurs_Init
 **
 ** Descriptions:		Init Codeurs
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Codeurs_Init(void)
{
	//Gpio always ON

	//Select Modes:
	//GPIO0.6 as GPIO
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, Codeur_D_Cannal_B_Port, Codeur_D_Cannal_B_Bit);
	Chip_IOCON_PinMux(LPC_IOCON, Codeur_D_Cannal_B_Port, Codeur_D_Cannal_B_Bit, IOCON_MODE_PULLUP, IOCON_FUNC0);

	//GPIO0.7 as GPIO
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, Codeur_D_Cannal_A_Port, Codeur_D_Cannal_A_Bit);
	Chip_IOCON_PinMux(LPC_IOCON, Codeur_D_Cannal_A_Port, Codeur_D_Cannal_A_Bit, IOCON_MODE_PULLUP, IOCON_FUNC0);

	//GPIO0.8 as GPIO
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, Codeur_G_Cannal_A_Port, Codeur_G_Cannal_A_Bit);
	Chip_IOCON_PinMux(LPC_IOCON, Codeur_G_Cannal_A_Port, Codeur_G_Cannal_A_Bit, IOCON_MODE_PULLUP, IOCON_FUNC0);

	//GPIO0.9 as GPIO
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, Codeur_G_Cannal_B_Port, Codeur_G_Cannal_B_Bit);
	Chip_IOCON_PinMux(LPC_IOCON, Codeur_G_Cannal_B_Port, Codeur_G_Cannal_B_Bit, IOCON_MODE_PULLUP, IOCON_FUNC0);

	mask_interrupt_Codeur_D = 1 << Codeur_D_Cannal_A_Bit | 1 << Codeur_D_Cannal_B_Bit;
	mask_interrupt_Codeur_G = 1 << Codeur_G_Cannal_A_Bit | 1 << Codeur_G_Cannal_B_Bit;


	//Set Interrupts on Edges
	GPIOSetInterrupt(Codeur_D_Cannal_A_Port, Codeur_D_Cannal_A_Bit, 1, 1);
#if USE_ALL_CODEURS_EGDES == 1
	GPIOSetInterrupt(Codeur_D_Cannal_B_Port, Codeur_D_Cannal_B_Bit, 1, 1);
#endif

	GPIOSetInterrupt(Codeur_G_Cannal_A_Port, Codeur_G_Cannal_A_Bit, 1, 1);
#if USE_ALL_CODEURS_EGDES == 1
	GPIOSetInterrupt(Codeur_G_Cannal_B_Port, Codeur_G_Cannal_B_Bit, 1, 1);
#endif

	//Clear Interrupts
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_D_Cannal_A_Port, mask_interrupt_Codeur_D & mask_interrupt_Codeur_G);

	//Enable Interrupts
	NVIC_SetPriority(EINT3_IRQn, 1);
	NVIC_ClearPendingIRQ(EINT3_IRQn);
	NVIC_EnableIRQ(EINT3_IRQn);
}


/*****************************************************************************
 ** Function name:		_0_Codeurs_Get_CodeursValues
 **
 ** Descriptions:		Return Codeurs Values
 **
 ** parameters:			None
 ** Returned value:		struct Codeurs_Values
 **
 *****************************************************************************/
struct Codeurs_Values _0_Codeurs_Get_CodeursValues()
{
	return Valeurs_Codeurs;
}


/*****************************************************************************
 ** Function name:		_0_Codeurs_Increment_CodeursValues
 **
 ** Descriptions:		Incrémente les codeurs pour la simulation uniquement
 **
 ** parameters:			Valeur codeur Droit à incrémenter
 ** 					Valeur codeur Gauche à incrémenter
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Codeurs_Increment_Simulation(long increment_D, long increment_G)
{
	if(_0_Get_Motor_Power() == true)
	{
		Valeurs_Codeurs.Codeur_Droit += increment_D;
		Valeurs_Codeurs.Codeur_Gauche += increment_G;
	}
}


/*****************************************************************************
 ** Function name:		EINT3_IRQHandler
 **
 ** Descriptions:		Interrupteur d'entrée GPIO Port 0 et 2
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
__attribute__((optimize("O3"))) void EINT3_IRQHandler(void)
{
	//if at least it is pending on GPIO0
	if(Chip_GPIOINT_IsIntPending(LPC_GPIOINT, GPIOINT_PORT0))
	{
		//Codeur IT
		Codeurs_Interrupts();

		_0_Codeurs_Increment_Simulation(0,0);
	}
}

__attribute__((optimize("O3"))) void EINT0_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(EINT0_IRQn);
}

__attribute__((optimize("O3"))) void EINT1_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(EINT1_IRQn);
}

__attribute__((optimize("O3"))) void EINT2_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(EINT2_IRQn);
}


/*****************************************************************************
 ** Function name:		Codeurs_Interrupts
 **
 ** Descriptions:		Use one GPIO pin(port0) as interrupt source
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
__attribute__((optimize("O3")))void Codeurs_Interrupts_420cy(void)
{
	//Si front montant du codeur Droit cannal A
	if(LPC_GPIOINT->IO0.STATR & 1 << Codeur_D_Cannal_A_Bit)
	{
		//Clear l'interrupt de ce cannal
		Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_D_Cannal_A_Port, Codeur_D_Cannal_A_Port);

		//Si le Cannal B est Low
		if(!((LPC_GPIO[Codeur_D_Cannal_B_Port].PIN >> Codeur_D_Cannal_B_Bit) & 1))
		{
			//cas 1
			//Incrémente le codeur
			Valeurs_Codeurs.Codeur_Droit++;
		}else
		{
			//cas 3
			//Decrémente le codeur
			Valeurs_Codeurs.Codeur_Droit--;
		}
	}else
	{
		//Si front descendant du codeur Droit cannal A
		if(LPC_GPIOINT->IO0.STATF & 1 << Codeur_D_Cannal_A_Bit)
		{
			//Clear l'interrupt de ce cannal
			Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_D_Cannal_A_Port, Codeur_D_Cannal_A_Port);

			//Si le Cannal B est Low
			if(!((LPC_GPIO[Codeur_D_Cannal_B_Port].PIN >> Codeur_D_Cannal_B_Bit) & 1))
			{
				//cas 5
				//Décrémente le codeur
				Valeurs_Codeurs.Codeur_Droit--;
			}else
			{
				//cas 7
				//Incrémente le codeur
				Valeurs_Codeurs.Codeur_Droit++;
			}
		}
	}

	//Si front montant du codeur Droit cannal B
	if(LPC_GPIOINT->IO0.STATR & 1 << Codeur_D_Cannal_B_Bit)
	{
		//Clear l'interrupt de ce cannal
		Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_D_Cannal_B_Port, Codeur_D_Cannal_B_Port);

		//Si le Cannal A est High
		if(((LPC_GPIO[Codeur_D_Cannal_A_Port].PIN >> Codeur_D_Cannal_A_Bit) & 1))
		{
			//cas 2
			//Incrémente le codeur
			Valeurs_Codeurs.Codeur_Droit++;
		}else
		{
			//cas 4
			//Decrémente le codeur
			Valeurs_Codeurs.Codeur_Droit--;
		}
	}else
	{
		//Si front descendant du codeur Droit cannal B
		if(LPC_GPIOINT->IO0.STATF & 1 << Codeur_D_Cannal_B_Bit)
		{
			//Clear l'interrupt de ce cannal
			Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_D_Cannal_B_Port, Codeur_D_Cannal_B_Port);

			//Si le Cannal A est High
			if(((LPC_GPIO[Codeur_D_Cannal_A_Port].PIN >> Codeur_D_Cannal_A_Bit) & 1))
			{
				//cas 6
				//Décrémente le codeur
				Valeurs_Codeurs.Codeur_Droit--;
			}else
			{
				//cas 8
				//Incrémente le codeur
				Valeurs_Codeurs.Codeur_Droit++;
			}
		}
	}



	//Si front montant du codeur Gauche cannal A
	if(LPC_GPIOINT->IO0.STATR & 1 << Codeur_G_Cannal_A_Bit)
	{
		//Clear l'interrupt de ce cannal
		Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_G_Cannal_A_Port, Codeur_G_Cannal_A_Port);

		//Si le Cannal B est Low
		if(!((LPC_GPIO[Codeur_G_Cannal_B_Port].PIN >> Codeur_G_Cannal_B_Bit) & 1))
		{
			//cas 1
			//Incrémente le codeur
			Valeurs_Codeurs.Codeur_Gauche++;
		}else
		{
			//cas 3
			//Decrémente le codeur
			Valeurs_Codeurs.Codeur_Gauche--;
		}
	}else
	{
		//Si front descendant du codeur Droit cannal A
		if(LPC_GPIOINT->IO0.STATF & 1 << Codeur_G_Cannal_A_Bit)
		{
			//Clear l'interrupt de ce cannal
			Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_G_Cannal_A_Port, Codeur_G_Cannal_A_Port);

			//Si le Cannal B est Low
			if(!((LPC_GPIO[Codeur_G_Cannal_B_Port].PIN >> Codeur_G_Cannal_B_Bit) & 1))
			{
				//cas 5
				//Décrémente le codeur
				Valeurs_Codeurs.Codeur_Gauche--;
			}else
			{
				//cas 7
				//Incrémente le codeur
				Valeurs_Codeurs.Codeur_Gauche++;
			}
		}
	}

	//Si front montant du codeur Droit cannal B
	if(LPC_GPIOINT->IO0.STATR & 1 << Codeur_G_Cannal_B_Bit)
	{
		//Clear l'interrupt de ce cannal
		Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_G_Cannal_B_Port, Codeur_G_Cannal_B_Port);

		//Si le Cannal A est High
		if(((LPC_GPIO[Codeur_G_Cannal_A_Port].PIN >> Codeur_G_Cannal_A_Bit) & 1))
		{
			//cas 2
			//Incrémente le codeur
			Valeurs_Codeurs.Codeur_Gauche++;
		}else
		{
			//cas 4
			//Decrémente le codeur
			Valeurs_Codeurs.Codeur_Gauche--;
		}
	}else
	{
		//Si front descendant du codeur Droit cannal B
		if(LPC_GPIOINT->IO0.STATF & 1 << Codeur_G_Cannal_B_Bit)
		{
			//Clear l'interrupt de ce cannal
			Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_G_Cannal_B_Port, Codeur_G_Cannal_B_Port);

			//Si le Cannal A est High
			if(((LPC_GPIO[Codeur_G_Cannal_A_Port].PIN >> Codeur_G_Cannal_A_Bit) & 1))
			{
				//cas 6
				//Décrémente le codeur
				Valeurs_Codeurs.Codeur_Gauche--;
			}else
			{
				//cas 8
				//Incrémente le codeur
				Valeurs_Codeurs.Codeur_Gauche++;
			}
		}
	}
}


__attribute__((optimize("O3")))void Codeurs_Interrupts(void)
{
	static unsigned char enc_DROIT_last = 0;
	static unsigned char enc_GAUCHE_last = 0;

	static unsigned char enc_DROIT_now = 0;
	static unsigned char enc_GAUCHE_now = 0;

	uint32_t IntStatus_Codeur;

	IntStatus_Codeur = LPC_GPIOINT->IO0.STATF | LPC_GPIOINT->IO0.STATR;

	if(IntStatus_Codeur & mask_interrupt_Codeur_D)
	{
		Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_D_Cannal_A_Port, mask_interrupt_Codeur_D);

		enc_DROIT_now = ((LPC_GPIO[Codeur_D_Cannal_A_Port].PIN >> Codeur_D_Cannal_A_Bit) & 1) + (((LPC_GPIO[Codeur_D_Cannal_A_Port].PIN >> Codeur_D_Cannal_B_Bit) & 1)<<1);

		if(enc_DROIT_now != enc_DROIT_last)
		{
			if((enc_DROIT_last & 1) ^ ((enc_DROIT_now & 2) >> 1))
			{
#if USE_ALL_CODEURS_EGDES == 1
				Valeurs_Codeurs.Codeur_Droit--;
#else
				Valeurs_Codeurs.Codeur_Droit-=2;
#endif
			}else
			{
#if USE_ALL_CODEURS_EGDES == 1
				Valeurs_Codeurs.Codeur_Droit++;
#else
				Valeurs_Codeurs.Codeur_Droit+=2;
#endif
			}
			enc_DROIT_last = enc_DROIT_now;
		}
	}

	if(IntStatus_Codeur & mask_interrupt_Codeur_G)
	{
		Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, Codeur_G_Cannal_A_Port, mask_interrupt_Codeur_G);

		enc_GAUCHE_now = ((LPC_GPIO[Codeur_G_Cannal_A_Port].PIN >> Codeur_G_Cannal_A_Bit) & 1) + (((LPC_GPIO[Codeur_G_Cannal_A_Port].PIN >> Codeur_G_Cannal_B_Bit) & 1)<<1);

		if(enc_GAUCHE_now != enc_GAUCHE_last)
		{
			if((enc_GAUCHE_last & 1) ^ ((enc_GAUCHE_now & 2) >> 1))
			{
#if USE_ALL_CODEURS_EGDES == 1
				Valeurs_Codeurs.Codeur_Gauche--;
#else
				Valeurs_Codeurs.Codeur_Gauche-=2;
#endif
			}else
			{
#if USE_ALL_CODEURS_EGDES == 1
				Valeurs_Codeurs.Codeur_Gauche++;
#else
				Valeurs_Codeurs.Codeur_Gauche+=2;
#endif
			}
			enc_GAUCHE_last = enc_GAUCHE_now;
		}
	}
}


/*****************************************************************************
 ** Function name:		Reset_Codeurs
 **
 ** Descriptions:		Reset_Codeurs
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Codeurs_Reset(void)
{
	Valeurs_Codeurs.Codeur_Droit = 0;
	Valeurs_Codeurs.Codeur_Gauche = 0;
}



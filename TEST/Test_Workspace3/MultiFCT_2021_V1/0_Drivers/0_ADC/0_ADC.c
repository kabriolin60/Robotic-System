/*
 * 0_ADC.c
 *
 *  Created on: 5 mai 2020
 *      Author: kabri
 */

#include "Configuration.h"
#include "chip.h"
#include "0_ADC.h"

static ADC_CLOCK_SETUP_T ADCSetup;
static volatile uint8_t Burst_Mode_Flag = 0;

/**************************************************
Fonction d'init du module ADC de niveau 0
Input: None
Output: None
 **************************************************/
void _0_ADC_Init(void)
{
	Chip_ADC_Init(_LPC_ADC_ID, &ADCSetup);

	Chip_IOCON_PinMux(LPC_IOCON, ADC_CHANNEL_0_PORT, ADC_CHANNEL_0_BIT, IOCON_MODE_INACT, IOCON_FUNC1);
	Chip_IOCON_PinMux(LPC_IOCON, ADC_CHANNEL_1_PORT, ADC_CHANNEL_1_BIT, IOCON_MODE_INACT, IOCON_FUNC1);
	Chip_IOCON_PinMux(LPC_IOCON, ADC_CHANNEL_2_PORT, ADC_CHANNEL_2_BIT, IOCON_MODE_INACT, IOCON_FUNC1);
	Chip_IOCON_PinMux(LPC_IOCON, ADC_CHANNEL_3_PORT, ADC_CHANNEL_3_BIT, IOCON_MODE_INACT, IOCON_FUNC1);

	Chip_IOCON_PinMux(LPC_IOCON, ADC_CHANNEL_BATTERIE_PORT, ADC_CHANNEL_BATTERIE_BIT, IOCON_MODE_INACT, IOCON_FUNC2);
}



/**************************************************
Fonction de lecture d'une mesure Analogique niveau 0
Input: Channel à lire (0 à 3)
Output: None
 **************************************************/
uint16_t _0_ADC_Read_Channel(uint8_t canal)
{
	uint16_t dataADC;
	ADC_CHANNEL_T channel;

	switch(canal)
	{
	case 0:
		channel = ADC_CH0;
		break;
	case 1:
		channel = ADC_CH1;
		break;
	case 2:
		channel = ADC_CH2;
		break;
	case 3:
		channel = ADC_CH3;
		break;
	case 4:
		channel = ADC_CH4;
		break;
	case 5:
		channel = ADC_CH5;
		break;
	case 6:
		channel = ADC_CH6;
		break;
	case 7:
		channel = ADC_CH7;
		break;

	default:
		return 0;
	}

	Chip_ADC_EnableChannel(_LPC_ADC_ID, channel, ENABLE);

	/* Select using burst mode or not */
	if (Burst_Mode_Flag) {
		Chip_ADC_SetBurstCmd(_LPC_ADC_ID, ENABLE);
	}
	else {
		Chip_ADC_SetBurstCmd(_LPC_ADC_ID, DISABLE);
	}

	/* Start A/D conversion if not using burst mode */
	if (!Burst_Mode_Flag) {
		Chip_ADC_SetStartMode(_LPC_ADC_ID, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	}
	/* Waiting for A/D conversion complete */
	while (Chip_ADC_ReadStatus(_LPC_ADC_ID, channel, ADC_DR_DONE_STAT) != SET) {}
	/* Read ADC value */
	Chip_ADC_ReadValue(_LPC_ADC_ID, channel, &dataADC);

	/* Disable burst mode, if any */
	if (Burst_Mode_Flag) {
		Chip_ADC_SetBurstCmd(_LPC_ADC_ID, DISABLE);
	}

	Chip_ADC_EnableChannel(_LPC_ADC_ID, channel, DISABLE);

	return dataADC;
}

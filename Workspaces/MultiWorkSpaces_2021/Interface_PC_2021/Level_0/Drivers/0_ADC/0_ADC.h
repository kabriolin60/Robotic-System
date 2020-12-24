/*
 * 0_ADC.h
 *
 *  Created on: 5 mai 2020
 *      Author: kabri
 */

#ifndef _0_ADC_H_
#define _0_ADC_H_

#define _LPC_ADC_ID LPC_ADC
#define _LPC_ADC_IRQ ADC_IRQn

void _0_ADC_Init(void);

uint16_t _0_ADC_Read_Channel(ADC_CHANNEL_T channel);

#endif /* 0_DRIVERS_0_ADC_0_ADC_H_ */

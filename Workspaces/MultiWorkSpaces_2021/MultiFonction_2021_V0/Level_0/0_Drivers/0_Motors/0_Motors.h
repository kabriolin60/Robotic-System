/*
 * 0_Motors.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef _0_MOTORS_H_
#define _0_MOTORS_H_

#include "board.h"
#include "Configuration.h"

/* TIME_INTERVALmS is a value to load the timer match register with
   to get a 1 mS delay */
#define TIME_INTERVALmS	10000

__attribute__((optimize("O0"))) void _0_GPIO_Moteurs_Init(void);

__attribute__((optimize("O0"))) void _0_Motors_Init(void);

__attribute__((optimize("O0"))) void TIMER2_IRQHandler(void);

__attribute__((optimize("O0"))) void TIMER0_IRQHandler(void);

__attribute__((optimize("O0"))) void TIMER1_IRQHandler(void);

__attribute__((optimize("O0"))) void _0_Set_Motor_PWM_Value(LPC_TIMER_T *pTMP, int8_t matchnum, uint32_t matchvalue);

__attribute__((optimize("O0"))) void _0_Set_Motor_Direction(bool dir_mot_1, bool dir_mot_2);

__attribute__((optimize("O0"))) void _0_Set_Motor_Power(uint8_t moteur_number, bool power);

__attribute__((optimize("O0"))) bool _0_Get_Motor_Power(void);

#endif /* 0_MOTORS_0_MOTORS_H_ */

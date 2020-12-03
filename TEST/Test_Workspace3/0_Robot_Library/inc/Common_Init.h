/*
 * Common_Init.h
 *
 *  Created on: 2 déc. 2020
 *      Author: kabri
 */

#ifndef COMMON_INIT_H_
#define COMMON_INIT_H_

void vConfigureTimerForRunTimeStats( void );

void vTask_HartBeat(void *pvParameters);

void prvSetupHardware(uint8_t Use_LPCXPRESSO_BOARD);


void Set_Debug_Pin_0_High(void);

void Set_Debug_Pin_0_Low(void);

void Set_Debug_Pin_1_High(void);

void Set_Debug_Pin_1_Low(void);


void Common_Communication_Init(uint8_t Use_LPCXPRESSO_BOARD, uint8_t Carte_type);

#endif /* COMMON_INIT_H_ */

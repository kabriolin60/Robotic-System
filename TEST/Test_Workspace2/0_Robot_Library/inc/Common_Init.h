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

#endif /* COMMON_INIT_H_ */

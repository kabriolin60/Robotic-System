/*
 * Declarations.h
 *
 *  Created on: 29 sept. 2013
 *      Author: nicolas
 */

#ifndef DECLARATIONS_H_
#define DECLARATIONS_H_

#include "stdint.h"

#include "Includes.h"

/**************************************************
Declaration des macro des Tempo pour FreeRTOS
 **************************************************/

#define Init_Timing_Tache portTickType xLastWakeTime; xLastWakeTime = xTaskGetTickCount ();
#define Task_Delay_Until(Time) vTaskDelayUntil(&xLastWakeTime, (float)Time /((float)(1000/(float)configTICK_RATE_HZ)));
#define Task_Delay(Time) vTaskDelay((float)Time /((float)(1000/(float)configTICK_RATE_HZ)));

#define Task_Get_Current_Handle xTaskGetCurrentTaskHandle()
#define Task_Delete_Current vTaskDelete(xTaskGetCurrentTaskHandle());
#define Task_Suspend_Current vTaskSuspend(xTaskGetCurrentTaskHandle());

#define WAIT(condition, temps)  while(condition){ Task_Delay(temps); }

/*************************************************/

#define OK 1
#define NOK 0
#define OUTPUT 1
#define INPUT 0
#define ON 1
#define OFF 0

#endif /* DECLARATIONS_H_ */

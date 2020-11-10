/*
 * main.c
 *
 *  Created on: 7 nov. 2020
 *      Author: pi
 */


#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"


#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "tiny_gpio.h"





void vApplicationIdleHook(void);

void vTask1(void*);
void vTask2(void*);
void vTask3(void*);
void vTask4(void*);
void vTask_LED(void *parameter);


int main(void) {

	/*int i;

	if (gpioInitialise() < 0) return 1;

	for (i=0; i<54; i++)
	{
		printf("gpio=%d mode=%d level=%d\n", i, gpioGetMode(i), gpioRead(i));
	}*/


	/* Creating Two Task Same Priorities and Delay*/
	xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL );
	xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
	/* Creating Two Task Same Priorities and DelayUntil*/
	xTaskCreate( vTask3, "Task 3", 1000, NULL, 1, NULL );
	xTaskCreate( vTask4, "Task 4", 1000, NULL, 1, NULL );

	//xTaskCreate( vTask_LED, "Task LED", 1000, NULL, 1, NULL );



	vTaskStartScheduler();
	return 0;
}


void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
	taskENTER_CRITICAL();
	{
		printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
		fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}


void vTask1(void* parameter)
{
	while(1){
		printf("Task 1\n");
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}
void vTask2(void* parameter)
{
	while(1){
		printf("Task 2\n");
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}
void vTask3(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
	while(1){
		printf("Task 3 with 250ms\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(250));
	}
}
void vTask4(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
	while(1){
		printf("Task 4 with 500ms\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(500));
	}
}

void vTask_LED(void *parameter)
{
	vTaskDelay(pdMS_TO_TICKS(250));
	gpioSetMode(7, 1);

	for(;;)
	{
		gpioWrite(7,0);
		printf("gpio=%d mode=%d level=%d\n", 7, gpioGetMode(7), gpioRead(7));
		vTaskDelay(pdMS_TO_TICKS(2500));              // mS


		gpioWrite(7,1);			//OFF
		printf("gpio=%d mode=%d level=%d\n", 7, gpioGetMode(7), gpioRead(7));
		vTaskDelay(pdMS_TO_TICKS(2500));
	}
}



void vApplicationIdleHook(void)
{
	//	printf("Idle\r\n");
}



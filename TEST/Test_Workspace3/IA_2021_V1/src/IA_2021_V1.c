/*
===============================================================================
 Name        : IA_2021_V1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#include "Lib_includes.h"


int main(void) {

	Common_Communication_Init(1, TYPE_IA_BOARD);

	/* LED1 toggle thread */
	xTaskCreate(vTask_HartBeat, (char *) "vTask_HartBeat", 70, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();


	while(1) {
		// "Dummy" NOP to allow source level single
		// stepping of tight while() loop
		__asm volatile ("nop");
	}
	return 0 ;
}

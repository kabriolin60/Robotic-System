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

	prvSetupHardware(1);

	_0_Communication_Init();

	/* LED1 toggle thread */
	xTaskCreate(vTask_HartBeat, (char *) "vTask_HartBeat", 70, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);


	/* Start the scheduler */
	vTaskStartScheduler();


	// Force the counter to be placed into memory
	volatile static int i = 0 ;
	// Enter an infinite loop, just incrementing a counter
	while(1) {
		i++ ;
		// "Dummy" NOP to allow source level single
		// stepping of tight while() loop
		__asm volatile ("nop");
	}
	return 0 ;
}

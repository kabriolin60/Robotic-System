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

#include "Configuration.h"


void Init_MultiFct(void)
{
	/* Init Codeurs Input / Interrupts */
	_0_Codeurs_Init();

	/* Init Timer des Moteurs */
	_0_Motors_Init();

	/* Init des sorties GPIO de direction des moteurs */
	_0_GPIO_Moteurs_Init();

	/* Init des sorties Servos Niveau 0*/
	_0_Servo_Init();

	/* Init des sorties Servos Niveau 1*/
	_1_Servos_Init();

	/* Init du module ADC niveau 0 */
	_0_ADC_Init();

	/* Init des AX 12 Niveau 0 */
	_0_Init_AX12();

	/* Init des AX 12 Niveau 1 */
	_1_Init_AX12();
}


int main(void) {

	Common_Communication_Init(1, TYPE_MULTIFCT);

	//Init Spécifique au type de carte
	Init_MultiFct();

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

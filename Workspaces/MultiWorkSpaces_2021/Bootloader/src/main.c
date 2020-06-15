/*
===============================================================================
 Name        : Test_FreeRTOS2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */


#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cdc_usb_main.h"
#include "cdc_vcom.h"

#include "Configuration.h"
#include "Bootloader.h"


#include <cr_section_macros.h>


/* Sets up system hardware */
static void prvSetupHardware(void)
{
#ifdef CARTE_LPCXPRESSO
	Chip_SetupXtalClocking(configCPU_CLOCK_HZ, 12000000);
	Set_Clock_in(12000000);
#else
	Chip_SetupXtalClocking(configCPU_CLOCK_HZ, 16000000);
	Set_Clock_in(16000000);
#endif

	SystemCoreClockUpdate();
	Board_Init();
}

/* LED1 toggle thread */
static void vTask_HartBeat(void *pvParameters) {
	bool LedState = false;

	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, true); //Led as Output
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, true); //Led as Output
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_0_PORT, LED_0_BIT, true); //Led as Output

	Chip_GPIO_WritePortBit(LPC_GPIO, LED_0_PORT, LED_0_BIT, false);
	Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, false);
	Chip_GPIO_WritePortBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, false);

	while (1) {
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, LedState);
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, LedState);
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_0_PORT, LED_0_BIT, LedState);
		LedState = (bool) !LedState;

		/* About a 5Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 10);
	}
}



static void vTask_LunchUser_Application(void *pvParameters) {
	//Wait 5 seconds
	vTaskDelay(5000);

	//Si user code présent:
		if(user_code_present())
		{
			//execute user code
			execute_user_code();
		}


	bool LedState = false;


	while (1) {
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, LedState);
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, LedState);
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_0_PORT, LED_0_BIT, LedState);
		LedState = (bool) !LedState;

		/* About a 10Hz on/off toggle rate */
		//ERROR
		vTaskDelay(configTICK_RATE_HZ / 20);
	}
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{
	prvSetupHardware();

	/* LED1 toggle thread */
	xTaskCreate(vTask_HartBeat, (char *) "vTask_HartBeat", 50, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
	xTaskCreate(vTask_LunchUser_Application, (char *) "vTask_LunchUser_Application", 1000, NULL, (tskIDLE_PRIORITY + 1UL) | portPRIVILEGE_BIT, (xTaskHandle *) NULL);


	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

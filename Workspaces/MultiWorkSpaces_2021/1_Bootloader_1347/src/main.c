/*
 * @brief FreeRTOS blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Configuration.h"
#include "Init.h"
#include "Bootloader.h"
#include "Bootloader_Data.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

TaskHandle_t Run_Application_Handler = NULL;

extern uint8_t is_RunApplication_running;

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/* Sets up system hardware */
static void prvSetupHardware(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();
}

/* LED1 toggle thread */
static void vTask_HartBeat(void *pvParameters) {
	bool LedState = false;

	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED_2_PORT, LED_2_BIT, true); //Led as Output
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED_1_PORT, LED_1_BIT, true); //Led as Output
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, LED_0_PORT, LED_0_BIT, true); //Led as Output

	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED_0_PORT, LED_0_BIT, false);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED_1_PORT, LED_1_BIT, false);
	Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED_2_PORT, LED_2_BIT, false);

	while (1)
	{
		if(is_RunApplication_running)
		{
			Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED_2_PORT, LED_2_BIT, LedState);
			Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED_1_PORT, LED_1_BIT, LedState);
			Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED_0_PORT, LED_0_BIT, LedState);
		}else
		{
			Chip_GPIO_WritePortBit(LPC_GPIO_PORT, LED_2_PORT, LED_2_BIT, LedState);
		}
		LedState = (bool) !LedState;

		/* About a 5Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 10);
	}
}


static void vTask_LunchUser_Application(void *pvParameters) {
	//Wait 5 seconds
	Task_Delay(9000);

	//disable les receptions
	NVIC_DisableIRQ(RS485_IRQ_SELECTION);
#ifdef USE_USB
	NVIC_DisableIRQ(USB_IRQn);
#endif

	Task_Delay(1000);

	//Si user code présent:
	if(user_code_present())
	{
		//execute user code
		execute_user_code();
	}
}

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{
	prvSetupHardware();

	Init_bootloader();

	Init_Carte_Perpheriques();

	/* LED1 toggle thread */
	xTaskCreate(vTask_HartBeat, (char *) "vTask_HartBeat", 60, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);

	/* Bootloader Application Launcher */
	xTaskCreate(vTask_LunchUser_Application, (char *) "vTask_LunchUser_App", 60, NULL, (tskIDLE_PRIORITY + 1UL) | portPRIVILEGE_BIT, &Run_Application_Handler);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}


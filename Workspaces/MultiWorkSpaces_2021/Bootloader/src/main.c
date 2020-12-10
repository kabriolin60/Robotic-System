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

#include "0_Event_Group.h"
#include "0_Communication.h"
#include "1_Trame_Communication.h"
#include "2_Echange_Datas.h"
#include "2_Echange_Datas_Reception.h"


#include <cr_section_macros.h>

TaskHandle_t Run_Application_Handler = NULL;

extern uint8_t is_RunApplication_running;


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

	while (1)
	{
		if(is_RunApplication_running)
		{
			Chip_GPIO_WritePortBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, LedState);
			Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, LedState);
			Chip_GPIO_WritePortBit(LPC_GPIO, LED_0_PORT, LED_0_BIT, LedState);
		}else
		{
			Chip_GPIO_WritePortBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, LedState);
		}
		LedState = (bool) !LedState;

		/* About a 5Hz on/off toggle rate */
		vTaskDelay(configTICK_RATE_HZ / 10);
	}
}



static void vTask_LunchUser_Application(void *pvParameters) {
	//Wait 5 seconds
	vTaskDelay(9000);

	//disable les receptions
	NVIC_DisableIRQ(RS485_IRQ_SELECTION);
#ifdef USE_USB
	NVIC_DisableIRQ(USB_IRQn);
#endif

	vTaskDelay(1000);

	//Si user code présent:
	if(user_code_present())
	{
		//execute user code
		execute_user_code();
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

#ifdef USE_USB
	usb_main();
#endif

#ifndef TYPE_CARTE_IA
	//Turn Servo power OFF
	Chip_IOCON_PinMux(LPC_IOCON, SERVO_ON_OFF_PORT, SERVO_ON_OFF_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, SERVO_ON_OFF_PORT, SERVO_ON_OFF_BIT, true);

	//Commencer par couper l'alimentation des Servos
	Chip_GPIO_WritePortBit(LPC_GPIO, SERVO_ON_OFF_PORT, SERVO_ON_OFF_BIT, 0);


	//Turn AX-12 power OFF
	Chip_IOCON_PinMux(LPC_IOCON, AX_12_ON_OFF_PORT, AX_12_ON_OFF_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, AX_12_ON_OFF_PORT, AX_12_ON_OFF_BIT, true);

	//Commencer par couper l'alimentation des AX12
	Chip_GPIO_WritePortBit(LPC_GPIO, AX_12_ON_OFF_PORT, AX_12_ON_OFF_BIT, 0);


	//Dir Moteur Gauche
	Chip_IOCON_PinMux(LPC_IOCON, DIR_MOTEUR_1_GAUCHE_PORT, DIR_MOTEUR_1_GAUCHE_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DIR_MOTEUR_1_GAUCHE_PORT, DIR_MOTEUR_1_GAUCHE_BIT, true);

	//PWM Moteur Gauche
	Chip_IOCON_PinMux(LPC_IOCON, 4, 28, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 4, 28, true);
	Chip_GPIO_WritePortBit(LPC_GPIO, 4, 28, 0);


	//Dir Moteur Droit
	Chip_IOCON_PinMux(LPC_IOCON, DIR_MOTEUR_2_DROIT_PORT, DIR_MOTEUR_2_DROIT_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DIR_MOTEUR_2_DROIT_PORT, DIR_MOTEUR_2_DROIT_BIT, true);

	//PWM Moteur Droit
	Chip_IOCON_PinMux(LPC_IOCON, 4, 29, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 4, 29, true);
	Chip_GPIO_WritePortBit(LPC_GPIO, 4, 29, 0);

#endif

	/* Init Bootloader */
	Init_bootloader();

	/* Init du Groupe d'event de synchronisation */
	_0_Communication_Init_Event_Group();

	/* Init de la communication Niveau 0 */
	_0_Communication_Init();

	/* Init de la communication Niveau 1 */
	_1_Communication_Init();

	/* Init de la communication Niveau 2 */
	_2_Communication_Init();

	/* Init de la communication de Reception Niveau 2 */
	_2_Communication_RX_Init();



	/* LED1 toggle thread */
	xTaskCreate(vTask_HartBeat, (char *) "vTask_HartBeat", 50, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
	xTaskCreate(vTask_LunchUser_Application, (char *) "vTask_LunchUser_Application", 1000, NULL, (tskIDLE_PRIORITY + 1UL) | portPRIVILEGE_BIT, &Run_Application_Handler);


	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * Name        : main.c
 * Version     :
 * Description : main definition for FreeRTOS application
 */

/*
 * FreeRTOS includes
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "Clocking.h"
#include "Includes.h"

volatile long Boucles = 0;

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE

void __error__(char *pcFilename, unsigned long ulLine) {
}

static void setupHardware(void) {
	// TODO: Put hardware configuration and initialisation in here
	LPC_SetPLL0(configCPU_CLOCK_HZ);
}




#if defined (DEBUG)
signed char pcWriteBuffer[700];
#endif

void vUserTask_KernelDEBUG(void *pvParameters)
{
	Init_Timing_Tache;

	Task_Delay(200);

	for(;;)
	{
#if defined (DEBUG)
		//Generation des stats
		vTaskList(&pcWriteBuffer);
		printf("\rIA");
		printf(&pcWriteBuffer);

		Task_Delay(200);

		vTaskGetRunTimeStats(&pcWriteBuffer);
		printf(&pcWriteBuffer);
#endif

		Task_Delay_Until(1500);
	}
}


/**
 * Simple task that just toggles between to states
 */
void vUserTask1(void *pvParameters) {
	static int iState = 0;

	while (1) {
		if (iState == 0) {
			iState = 1;
		} else {
			iState = 0;
		}

		GPIOSetValue(1, 15, iState);
		GPIOSetValue(0, 22, iState);
		Task_Delay(50);
	}
}



/**
 * Program entry point 
 */
int main(void) {
	setupHardware();

	Init_Robot();

	/*
	 * LED task
	 */
	xTaskCreate( vUserTask1, "Hart beat", 150, NULL, tskIDLE_PRIORITY, NULL );


	/*
	 * Kernel Debug task
	 */
	//xTaskCreate( vUserTask_KernelDEBUG, "KernelDebug", 384, NULL, tskIDLE_PRIORITY, NULL );

	/* 
	 * Start the scheduler. 
	 */
	vTaskStartScheduler();

	/* 
	 * Will only get here if there was insufficient memory to create the idle task. 
	 */
	return 1;
}


void vConfigureTimerForRunTimeStats( void )
{
	const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* Power up and feed the timer with a clock. */
	LPC_SC->PCONP |= 0x02UL;
	LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

	/* Reset Timer 0 */
	LPC_TIM0->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIM0->TCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
    but not too fast so as to overflow all the time. */
	LPC_TIM0->PR =  ( configCPU_CLOCK_HZ / 50000UL ) - 1UL;

	/* Start the counter. */
	LPC_TIM0->TCR = TCR_COUNT_ENABLE;
}

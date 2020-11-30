/*
===============================================================================
 Name        : IA_2021_V0.c
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
#include "queue.h"
#include "FreeRTOSConfig.h"

#include "cdc_usb_main.h"
#include "cdc_vcom.h"

#include "Configuration.h"

#include <Init.h>
#include "test.h"

#include "0_RS485.h"

#include <cr_section_macros.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
static __attribute__ ((used,section(".bss.$RAM3."))) uint8_t heap_sram_lower[configTOTAL_HEAP_SIZE]; // placed in in no_init section inside SRAM_LOWER

static HeapRegion_t xHeapRegions[] =
{
		{
				&heap_sram_lower[0], sizeof(heap_sram_lower)
		},
		{
				NULL, 0 // << Terminates the array.
		}
};



/*****************************************************************************
 * Private functions
 ****************************************************************************/

void vConfigureTimerForRunTimeStats( void )
{
	const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* Power up and feed the timer with a clock. */
	Chip_TIMER_Init(LPC_TIMER0);

	/* Reset Timer 0 */
	LPC_TIMER0->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIMER0->TCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
     but not too fast so as to overflow all the time. */
	LPC_TIMER0->PR =  ( configCPU_CLOCK_HZ / 50000UL ) - 1UL;

	/* Start the counter. */
	LPC_TIMER0->TCR = TCR_COUNT_ENABLE;
}


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

	vPortDefineHeapRegions(xHeapRegions); // Pass the array into vPortDefineHeapRegions(). Must be called first!
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
		LedState = (bool) !LedState;

		/* About a 3Hz on/off toggle rate */
		Task_Delay(500);
	}
}


#if configGENERATE_RUN_TIME_STATS == 1
char task_stat[1024];
static void vTask_Stats(void *pvParameters)
{
	Task_Delay(300000);
	vTaskGetRunTimeStats(&task_stat);


	_0_RS485_Master_Mode(RS485_DIR_PORT, RS485_DIR_BIT);

	for(int i = 0; i < 3; i++)
	{
		Chip_UART_SendByte(RS484_UART, '\r');
		while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
		{
			for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
		}
		Chip_UART_SendByte(RS484_UART, '\n');
		while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
		{
			for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
		}
	}

	int index = 0;
	while (index < 1000)
	{
		Chip_UART_SendByte(RS484_UART, task_stat[index++]);
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );

		while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
		{
			for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
		}
	}

	for(int i = 0; i < 100; i++)__asm volatile( "nop" );

	//Passe en RX
	_0_RS485_Slave_Mode(RS485_DIR_PORT, RS485_DIR_BIT);

	Task_Delay(10);
	vTaskSuspendAll();

	for(;;)
	{

	}
}
#endif




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
	vTraceEnable( TRC_INIT );

	Init_Carte_IA();

#ifdef USE_USB
	usb_main();
#endif

	/* LED1 toggle thread */
	xTaskCreate(vTask_HartBeat, (char *) "vTask_HartBeat", 70, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);

#if configGENERATE_RUN_TIME_STATS == 1
	xTaskCreate(vTask_Stats, (char *) "vTask_Stats", 320, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
#endif

	//TEST
	TEST_init_parametres();

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}



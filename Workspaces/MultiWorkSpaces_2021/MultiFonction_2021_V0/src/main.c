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
#include "queue.h"
#include "cdc_usb_main.h"
#include "cdc_vcom.h"
#include "Configuration.h"
#include "FreeRTOSConfig.h"

#include <Init.h>

#include "0_Codeurs.h"
#include "0_Motors.h"
#include "0_RS485.h"

#include "1_Trame_Communication.h"

#include "Test.h"

#include <cr_section_macros.h>



/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
static TO_AHBS_RAM3 uint8_t heap_sram_lower[configTOTAL_HEAP_SIZE]; // placed in in no_init section inside SRAM_LOWER

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
		vTaskDelay(configTICK_RATE_HZ / 1);
	}
}


#if configGENERATE_RUN_TIME_STATS == 1
char task_stat[1024];
static void vTask_Stats(void *pvParameters)
{
	Task_Delay(300000);
	vTaskGetRunTimeStats(&task_stat);

	vTaskSuspendAll();
	Task_Delay(1000);

	Task_Delay(ADRESSE_CARTE*100);

	_0_RS485_Master_Mode(RS485_DIR_PORT, RS485_DIR_BIT);

	for(int i = 0; i < 16; i++)	__asm volatile( "nop" );

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

	Chip_UART_SendByte(RS484_UART, 'M');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, 'u');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, 'ul');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, 't');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, 'i');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, 'F');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, 'c');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, 't');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, '@');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, ADRESSE_CARTE_CHAR);
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, ':');
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		for(int i = 0; i < 16; i++)	__asm volatile( "nop" );
	}
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

	Init_Carte_MultiFonctions();

#if configGENERATE_RUN_TIME_STATS == 1
	xTaskCreate(vTask_Stats, (char *) "vTask_Stats", 320, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
#endif

	/* LED1 toggle thread */
	xTaskCreate(vTask_HartBeat, (char *) "vTask_HartBeat", 50, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);

	//TEST
	TEST_init_parametres();

	__enable_irq();

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

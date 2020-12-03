/*
 * Common_Init.c
 *
 *  Created on: 2 déc. 2020
 *      Author: kabri
 */

#include "Lib_includes.h"


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

/* LED1 toggle thread */
void vTask_HartBeat(void *pvParameters) {
	/*bool LedState = false;

	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, true); //Led as Output
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, true); //Led as Output
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_0_PORT, LED_0_BIT, true); //Led as Output

	Chip_GPIO_WritePortBit(LPC_GPIO, LED_0_PORT, LED_0_BIT, false);
	Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, false);
	Chip_GPIO_WritePortBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, false);

	while (1) {
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_2_PORT, LED_2_BIT, LedState);
		LedState = (bool) !LedState;

		// About a 3Hz on/off toggle rate
		Task_Delay(500);
	}*/

	while (1) {

	}
}

/* Sets up system hardware */
void prvSetupHardware(uint8_t Use_LPCXPRESSO_BOARD){

	if(Use_LPCXPRESSO_BOARD == 1)
	{
		Chip_SetupXtalClocking(configCPU_CLOCK_HZ, 12000000);
		Set_Clock_in(12000000);
	}else
	{
		Chip_SetupXtalClocking(configCPU_CLOCK_HZ, 16000000);
		Set_Clock_in(16000000);
	}

	SystemCoreClockUpdate();
	Board_Init();

	vPortDefineHeapRegions(xHeapRegions); // Pass the array into vPortDefineHeapRegions(). Must be called first!
}


void Set_Debug_Pin_0_High(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_0_PORT, DEBUG_0_BIT, true);
}


void Set_Debug_Pin_0_Low(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_0_PORT, DEBUG_0_BIT, false);
}


void Set_Debug_Pin_1_High(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_1_PORT, DEBUG_1_BIT, true);
}


void Set_Debug_Pin_1_Low(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_1_PORT, DEBUG_1_BIT, false);
}



void Common_Communication_Init(uint8_t Use_LPCXPRESSO_BOARD, uint8_t Carte_type)
{
	static_IS_LPCXPRESSO_BOARD = Use_LPCXPRESSO_BOARD;
	static_BOARD_TYPE = Carte_type;

	//CPU Init
	prvSetupHardware(static_IS_LPCXPRESSO_BOARD);

	/* Init Debug Output pins */
	Chip_IOCON_PinMux(LPC_IOCON, DEBUG_0_PORT, DEBUG_0_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_IOCON_PinMux(LPC_IOCON, DEBUG_1_PORT, DEBUG_1_BIT, IOCON_MODE_INACT, IOCON_FUNC0);

	Chip_GPIO_WriteDirBit(LPC_GPIO, DEBUG_0_PORT, DEBUG_0_BIT, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DEBUG_1_PORT, DEBUG_1_BIT, true);

	Set_Debug_Pin_0_Low();
	Set_Debug_Pin_1_Low();

	/* Init du Groupe d'event de synchronisation */
	_0_Communication_Init_Event_Group();

	/* Init de la communication Niveau 0 */
	_0_Communication_Init();

	/* Init de la communication Niveau 1 */
	_1_Communication_Init();

	/* Init de la communication Niveau 2 */
	_2_Communication_Init();
}



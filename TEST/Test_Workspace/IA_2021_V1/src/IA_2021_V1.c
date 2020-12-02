/*
 * IA_2021_V1.c
 *
 *  Created on: 2 déc. 2020
 *      Author: PC-INOVA
 */

#include "Lib_Includes.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
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

	/* Should never arrive here */
	return 1;
}

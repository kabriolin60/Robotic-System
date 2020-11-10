/*
 * clocking.c: Oscillator and PLL related functions for lpc17xx
 *
 * Copyright (C) 2011, MyVoice CAD/CAM Services
 * All Rights reserved.
 *
 * LPCXpresso users are granted unlimited use of this (and only this) piece of code.
 * Feel free to use it in any commercial or non-commercial application.
 * If you make any enhancements or bug fixes, feel free to inform me of this
 * (updates are highly appreciated) via email: rob@myvoice.nl
 *
 * History
 * 2011-07-03	v 1.00	Preliminary version, first release
 * 2011-07-08   v 1.10  Changed PLL values to save power on INTRC_OSC
 *                      Verified PLL values to result in correct frequencies
 *
 *
 * Clocking functions
 *
 * Software License Agreement
 *
 * The software is owned by MyVoice CAD/CAM Services and/or its suppliers, and is
 * protected under applicable copyright laws.  All rights are reserved.  Any
 * use in violation of the foregoing restrictions may subject the user to criminal
 * sanctions under applicable laws, as well as to civil liability for the breach
 * of the terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
 * TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
 * MYVOICE CAD/CAM SERVICES.
 *
 */


#include "Clocking.h"
/*
 * This is the global variable SystemCoreClock
 * the same name as in the CMSIS - but since we do not link against CMSIS
 * it's OK to use the same name - for compatibility with software that is
 * using this global variable to determine the clock.
 *
 * Also define the frequencies for the different clocks
 */
/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemFrequency = IRC_OSC; /*!< System Clock Frequency (Core Clock)  */
uint32_t SystemCoreClock;

#define FREQ_MAIN_OSC  16000000
#define FREQ_INTRC_OSC  4000000
#define FREQ_RTC_OSC      32768

/*
 * RTC_AUX bit values
 * 	- RTC_OSCF: is set when a problem with the RTC oscillator was detected
 * 	            (oscillator does not run at all or was stopped temporarily
 * 	             due to power failues)
 */
#define RTC_OSCF (1<<4)

/*
 * PLL0 configuration table
 *
 * These tables contains the values that are needed to program PLL0 for different
 * output frequencies (for each of the oscillators)
 */
typedef struct {
	uint32_t freq;   /* The requested frequency (in Hz) */
	uint32_t msel;   /*  MSEL                           */
	uint32_t nsel;   /* and NSEL values for the PLL     */
	uint32_t clkdiv; /* Clock divider (post PLL div)    */
} PLLconf;

static PLLconf MainOscPLL0conf_12[] = {
		{ 144000000, 11, 0, 1 }, /* 144 MHz: M=12,  N=1, cclkcfg=2,  Fcco = 288 MHz */
		{ 128000000, 15, 0, 2 }, /* 128 MHz: M=16,  N=1, cclkcfg=3,  Fcco = 384 MHz */
		{ 120000000, 14, 0, 2 }, /* 120 MHz: M=15,  N=1, cclkcfg=3,  Fcco = 360 MHz */
		{ 100000000, 99, 5, 3 }, /* 100 MHz: M=100, N=6, cclkcfg=4,  Fcco = 400 MHz */
		{  90000000, 14, 0, 3 }, /*  90 MHz: M=15,  N=1, cclkcfg=4,  Fcco = 360 MHz */
		{  80000000, 99, 5, 4 }, /*  80 MHz: M=100, N=6, cclkcfg=5,  Fcco = 400 MHz */
		{  70000000, 34, 2, 3 }, /*  70 MHz: M=35,  N=3, cclkcfg=4,  Fcco = 280 MHz */
		{  65000000, 26, 0, 4 }, /*  65 MHz: M=13,  N=1, cclkcfg=5,  Fcco = 325 MHz */
		{  60000000, 14, 0, 5 }, /*  60 MHz: M=15,  N=1, cclkcfg=6,  Fcco = 360 MHz */
		{  50000000, 99, 5, 7 }, /*  50 MHz: M=100, N=6, cclkcfg=8,  Fcco = 400 MHz */
		{  40000000, 99, 5, 9 }, /*  40 MHz: M=100, N=6, cclkcfg=10, Fcco = 400 MHz */
		{  30000000, 14, 0,11 }, /*  30 MHz: M=15,  N=1, cclkcfg=12, Fcco = 360 MHz */
		{  20000000, 14, 0,17 }, /*  20 MHz: M=15,  N=1, cclkcfg=18, Fcco = 360 MHz */
		{  10000000, 34, 2,27 }, /*  10 MHz: M=35,  N=3, cclkcfg=28, Fcco = 280 MHz */
		{         0,  0, 0, 0 }
};

static PLLconf MainOscPLL0conf_16[] = {
		{ 144000000, 8, 0, 1 },  /* 144 MHz: M=9,   N=1, cclkcfg=2,  Fcco = 288 MHz */
		{ 128000000, 11, 0, 2 }, /* 128 MHz: M=12,  N=1, cclkcfg=3,  Fcco = 384 MHz */
		{ 122666666, 11, 0, 2 }, /* 122 MHz: M=11,  N=1, cclkcfg=3,  Fcco = 368 MHz */
		{ 100000000, 24, 1, 3 }, /* 100 MHz: M=25,  N=2, cclkcfg=4,  Fcco = 400 MHz */
		{  96000000, 8, 0, 2 },  /*  96 MHz: M=9,   N=1, cclkcfg=3,  Fcco = 288 MHz */
		{  80000000, 24, 1, 4 }, /*  80 MHz: M=25,  N=2, cclkcfg=5,  Fcco = 400 MHz */
		{  64000000, 11, 1, 2 }, /*  64 MHz: M=12,  N=2, cclkcfg=3,  Fcco = 192 MHz */
		{  48000000, 8, 1, 2 },  /*  48 MHz: M=9,   N=2, cclkcfg=3,  Fcco = 144 MHz */
		{  32000000, 11, 3, 2 }, /*  32 MHz: M=12,  N=4, cclkcfg=3,  Fcco = 192 MHz */
		{  16000000, 0, 3, 1 },  /*  16 MHz: M=1,   N=1, cclkcfg=2,  Fcco = 32 MHz */
		{         0,  0, 0, 0 }
};

static PLLconf IntRCOscPLL0conf[] = {
		{ 120000000, 44, 0, 2 }, /* 120 MHz: M=45,  N=1, cclkcfg=2,  Fcco = 360 MHz */
		{ 100000000, 74, 1, 2 }, /* 100 MHz: M=75,  N=2, cclkcfg=3,  Fcco = 300 MHz */
		{  90000000, 89, 1, 3 }, /*  90 MHz: M=90,  N=2, cclkcfg=4,  Fcco = 360 MHz */
		{  80000000, 99, 1, 4 }, /*  80 MHz: M=100, N=2, cclkcfg=5,  Fcco = 400 MHz */
		{  70000000, 69, 1, 3 }, /*  70 MHz: M=70,  N=2, cclkcfg=4,  Fcco = 280 MHz */
		{  60000000, 89, 1, 5 }, /*  60 MHz: M=90,  N=2, cclkcfg=6,  Fcco = 360 MHz */
		{  50000000, 99, 1, 7 }, /*  50 MHz: M=100, N=2, cclkcfg=8,  Fcco = 400 MHz */
		{  40000000, 99, 1, 9 }, /*  40 MHz: M=100, N=2, cclkcfg=10, Fcco = 400 MHz */
		{  30000000, 89, 1,11 }, /*  30 MHz: M=90,  N=2, cclkcfg=12, Fcco = 360 MHz */
		{  20000000, 69, 1,13 }, /*  20 MHz: M=70,  N=2, cclkcfg=14, Fcco = 280 MHz */
		{  10000000, 69, 1,27 }, /*  10 MHz: M=70,  N=2, cclkcfg=28, Fcco = 280 MHz */
		{         0,  0, 0, 0 }
};

#ifdef LPC_CLOCK_FROM_RTC
static PLLconf RTCOscPLL0conf[] = {
		{  70000000, 8544, 1, 3 }, /*  70.000640 MHz: M=8545,  N=2, cclkcfg=4,  Fcco = 280.00256 MHz */
		{  60000000,10986, 1, 5 }, /*  59,998208 MHz: M=10986, N=2, cclkcfg=6,  Fcco = 359.9892 MHz  */
		{  50000000,18310, 2, 7 }, /*  50.001237 MHz: M=18311, N=3, cclkcfg=8,  Fcco = 400.0098 MHz  */
		{  40000000,18310, 2, 9 }, /*  40.000989 MHz: M=18311, N=3, cclkcfg=10, Fcco = 400.0098 MHz  */
		{  30000000,10985, 1,11 }, /*  29.999104 MHz: M=10986, N=2, cclkcfg=12, Fcco = 359.9892 MHz  */
		{  20000000,10985, 1,17 }, /*  19.999402 MHz: M=10986, N=2, cclkcfg=18, Fcco = 359.9892 MHz  */
		{  10000000, 8544, 1,27},  /*  10.000091 MHz: M=8545,  N=2, cclkcfg=28, Fcco = 280.00256 MHz */
		{         0,  0, 0, 0 }
};
#endif

/*
 * LPC_UseOscillator(oscillator)
 *
 * Switch to the base frequency of the requested oscillator, disabling PLL0
 */
int LPC_UseOscillator(uint8_t osc)
{
	volatile int count;
	uint32_t systick_control;

	if(LPC_SC->PLL0STAT & (PLLE0_STAT | PLLC0_STAT))
	{
		/*
		 * Disable PLL
		 * 	- First disconnect PLL0
		 * 	- Then stop PLL0
		 */
		LPC_SC->PLL0CON = PLLE0;
		LPC_SC->PLL0FEED = 0xAA;
		LPC_SC->PLL0FEED = 0x55;
		LPC_SC->PLL0CON = 0;
		LPC_SC->PLL0FEED = 0xAA;
		LPC_SC->PLL0FEED = 0x55;
	}

#if 0
	/*
	 * Check if the oscillator being asked for is already used
	 * If it is, we only need to update systemCoreClock and return.
	 */
	if((LPC_SC->CLKSRCSEL & 3) == osc)
	{
		switch(osc)
		{
		case LPC_MAIN_OSC:  SystemCoreClock = FREQ_MAIN_OSC;  break;
		case LPC_INTRC_OSC: SystemCoreClock = FREQ_INTRC_OSC; break;
#ifdef LPC_CLOCK_FROM_RTC
		case LPC_RTC_OSC:   SystemCoreClock = FREQ_RTC_OSC;   break;
#endif
		}
		return 1;
	}
#endif

	/*
	 * Stop the SysTick
	 */
	systick_control = SysTick->CTRL;
	SysTick->CTRL = 4; /* Stop Systick and interrupt */
	SysTick->VAL  = 0;

	/*
	 * Switch to new oscillator
	 */

	switch(osc)
	{
	case LPC_MAIN_OSC:
		/*
		 * Switch power to main crystal oscillator on
		 */
		LPC_SC->SCS = OSCEN;

		count=100000;
		while (count-- && ((LPC_SC->SCS & OSCSTAT) == 0));

		if(count == 0)
		{
			return 0;
		}

		LPC_SC->CLKSRCSEL = LPC_MAIN_OSC;
		LPC_SC->CCLKCFG   = 0;
		SystemCoreClock = FREQ_MAIN_OSC;

		break;
#ifdef LPC_CLOCK_FROM_RTC
	case LPC_RTC_OSC:
		/*
		 * Check if the RTC oscillator is running
		 * Do this by checking the RTC Oscillator Fail flag in RTC_AUX
		 * we do not try to reset this flag to see if the RTC is running
		 * It is up to the application to properly initialize and control
		 * the RTC
		 */
		if(LPC_RTC->RTC_AUX & RTC_OSCF)
		{
			return 0;
		}
		LPC_SC->CLKSRCSEL = LPC_RTC_OSC;
		LPC_SC->CCLKCFG   = 0;
		SystemCoreClock = FREQ_RTC_OSC;

		/*
		 * Now it is safe to disable the main oscillator
		 * (in case it was running)
		 */
		LPC_SC->SCS = 0;

		break;
#endif
	case LPC_INTRC_OSC:
		/*
		 * The internal RC oscillator is the easy one
		 * It is always running correctly (or the chip is not working at all)
		 * so we can just switch to it
		 * and disable the main oscillator if it is not used by PLL1
		 */
		LPC_SC->CLKSRCSEL = LPC_INTRC_OSC;
		LPC_SC->CCLKCFG   = 0;
		SystemCoreClock   = FREQ_INTRC_OSC;

		if((LPC_SC->PLL1STAT & PLLE1_STAT) == 0)
			LPC_SC->SCS       = 0;

		break;
	default:
		return 0;
	}

	/*
	 * Perform post switching operations:
	 * 	- Reprogram and start SysTick again
	 * 	- Set the Flash Accelerator timing
	 */
#ifdef LPC_CLOCK_FROM_RTC
	if(osc == LPC_RTC_OSC)
	{
		SysTick->LOAD = ( SystemCoreClock / 10 ) - 1;
		SysTick->CTRL = systick_control; /* Start again */
	} else
#endif
	{
		SysTick->LOAD = ( SystemCoreClock / configTICK_RATE_HZ ) - 1;
		SysTick->CTRL = systick_control; /* Start again */
	}

	LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | 0x00004000;

	return 1;
}

/*
 * LPC_SetPLL0(cclk)
 *
 * Set PLL0 to get the wanted frequency.
 * Check which oscillator is currently used and set the PLL depending on this value.
 *
 * Make sure that the FLASHCFG register has a safe value before switching
 * to the new clock.
 */
int LPC_SetPLL0(long cclk)
{
	int index = 0;
	uint32_t freq, msel, nsel, clkdiv;
	uint32_t systick_control;

	LPC_UseOscillator(LPC_MAIN_OSC);

	switch(LPC_SC->CLKSRCSEL & 3)
	{
	case LPC_MAIN_OSC:
		/*
		 * For Main Osc only !!!!
		 */
		if(FREQ_MAIN_OSC == 12000000)
		{
			while((MainOscPLL0conf_12[index].freq != cclk) && MainOscPLL0conf_12[index].freq) index++;
			if(MainOscPLL0conf_12[index].freq == 0)
			{
				return 0;
			}
			freq   = MainOscPLL0conf_12[index].freq;
			msel   = MainOscPLL0conf_12[index].msel;
			nsel   = MainOscPLL0conf_12[index].nsel;
			clkdiv = MainOscPLL0conf_12[index].clkdiv;
			break;
		}

		if(FREQ_MAIN_OSC == 16000000)
		{
			while((MainOscPLL0conf_16[index].freq != cclk) && MainOscPLL0conf_16[index].freq) index++;
			if(MainOscPLL0conf_16[index].freq == 0)
			{
				return 0;
			}
			freq   = MainOscPLL0conf_16[index].freq;
			msel   = MainOscPLL0conf_16[index].msel;
			nsel   = MainOscPLL0conf_16[index].nsel;
			clkdiv = MainOscPLL0conf_16[index].clkdiv;
			break;
		}


	case LPC_INTRC_OSC:
		/*
		 * For Main Osc only !!!!
		 */
		while((IntRCOscPLL0conf[index].freq != cclk) && IntRCOscPLL0conf[index].freq) index++;

		if(IntRCOscPLL0conf[index].freq == 0)
		{
			return 0;
		}
		freq   = IntRCOscPLL0conf[index].freq;
		msel   = IntRCOscPLL0conf[index].msel;
		nsel   = IntRCOscPLL0conf[index].nsel;
		clkdiv = IntRCOscPLL0conf[index].clkdiv;
		break;
#ifdef LPC_CLOCK_FROM_RTC
	case LPC_RTC_OSC:
		/*
		 * For Main Osc only !!!!
		 */
		while((IntRCOscPLL0conf[index].freq != cclk) && IntRCOscPLL0conf[index].freq) index++;

		if(IntRCOscPLL0conf[index].freq == 0)
		{
			return 0;
		}
		freq   = RTCOscPLL0conf[index].freq;
		msel   = RTCOscPLL0conf[index].msel;
		nsel   = RTCOscPLL0conf[index].nsel;
		clkdiv = RTCOscPLL0conf[index].clkdiv;
		break;
#endif
	default:
		return 0;
	}

	/*
	 * If the PLL is currently enabled, stop PLL0
	 */
	if(LPC_SC->PLL0STAT & (PLLE0_STAT | PLLC0_STAT))
	{
		/*
		 * 	- First disconnect PLL0
		 * 	- Then stop PLL0
		 */
		LPC_SC->PLL0CON = PLLE0;
		LPC_SC->PLL0FEED = PLLFEED1;
		LPC_SC->PLL0FEED = PLLFEED2;
		LPC_SC->PLL0CON = 0;
		LPC_SC->PLL0FEED = PLLFEED1;
		LPC_SC->PLL0FEED = PLLFEED2;
	}

	/*
	 * Program the PLL with the requested values taking care of the order
	 * in which to use and enable PLL, CPU clock divider and SysTick.
	 * Note that the cclkcfg (CPU clock divider values) are set in such a way
	 * that this will never result in a cclk higher that 100 MHz
	 * (max. Fcco = 400 MHz, min. cclkcfg = 4).
	 * Cclkcfg is programmed before enabling the PLL
	 * 		When switching from 50 to 60 MHz the PLL goes from 400 to 360 MHz
	 * 		and cclkcfg from 8 to 6.
	 * 		First cclkcfg is changed resulting in an intermediate 400/6=66 MHz
	 * 		before switching to 60 MHz.
	 *
	 * Steps done:
	 *
	 * 	- Program PLL0
	 * 	- Wait for PLL0 to lock
	 * 	- Stop the systick timer, reprogram it
	 * 	- Pogram the CCLKCFG post PLL divider
	 * 	  need to do this before connecting the PLL, otherwise the CCLK
	 * 	  would start to run at the Fcco frequency which is too high.
	 * 	- Program FLASHCFG with a safe value
	 * 	- Connect PLL0
	 * 	- Program FLASHCFG with the correct value
	 * 	- Enable systick
	 *
	 * 	ToDo: use a timeout on the wait for lock
	 * 	ToDo: check the while loop waiting for status to reflect connect status in PLL0STAT
	 * 	      this is copied from LPC17xx.c but I have no idea why it's there ...
	 */
	LPC_SC->PLL0CFG = msel + (nsel << 16);			/* Program PLL  */
	LPC_SC->PLL0FEED = 0xAA;
	LPC_SC->PLL0FEED = 0x55;

	LPC_SC->PLL0CON = PLLE0;  						/* Enable PLL   */
	LPC_SC->PLL0FEED = 0xAA;
	LPC_SC->PLL0FEED = 0x55;

	while(!(LPC_SC->PLL0STAT & PLOCK0));			/* Locked ?     */

	/*
	 * Stop and reprogram systick
	 */
	/*
	systick_control = SysTick->CTRL;
	SysTick->CTRL = 4; 								// Stop Systick
	SysTick->VAL  = 0;
	SysTick->LOAD = ( cclk / configTICK_RATE_HZ ) - 1;
	SysTick->CTRL = systick_control; // Start systick (when it was on)
	 */

	LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | 0x00004000;

	LPC_SC->CCLKCFG = clkdiv; /* set CCLK divider */
	LPC_SC->PLL0CON = PLLE0 | PLLC0;				/* Connect      */
	LPC_SC->PLL0FEED = 0xAA;
	LPC_SC->PLL0FEED = 0x55;

	while(!(LPC_SC->PLL0STAT & (PLLE0_STAT | PLLC0_STAT)));

	if(cclk > 80000000)
		LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | 0x00004000;
	else if(cclk > 60000000)
		LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | 0x00003000;
	else if(cclk > 40000000)
		LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | 0x00002000;
	else if(cclk > 20000000)
		LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | 0x00001000;
	else
		LPC_SC->FLASHCFG  = (LPC_SC->FLASHCFG & ~0x0000F000) | 0x00000000;

	SystemFrequency = cclk;
	SystemCoreClock = SystemFrequency;

	return 1;
}





/**
 * Determine the CPU Clock Frequency (SystemFrequency)
 *
 * @param none
 * @return none
 *
 * @brief	Function determines the microcontroller's CPU clock frequency
 *			It populates this value into the "SystemFrequeny" variable.
 *			Recommended to call this function in main()
 */
void SystemClockUpdate (void)
{
	/* Determine clock frequency according to clock register values             */
	if (((LPC_SC->PLL0STAT >> 24)&3)==3) {/* If PLL0 enabled and connected      */
		switch (LPC_SC->CLKSRCSEL & 0x03) {
		case 0:                           /* Internal RC oscillator => PLL0     */
		case 3:                           /* Reserved, default to Internal RC   */
			SystemFrequency = (IRC_OSC *
					(((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
							(((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /
							((LPC_SC->CCLKCFG & 0xFF)+ 1));
			break;
		case 1:                           /* Main oscillator => PLL0            */
			SystemFrequency = (OSC_CLK *
					(((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
							(((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /
							((LPC_SC->CCLKCFG & 0xFF)+ 1));
			break;
		case 2:                           /* RTC oscillator => PLL0             */
			SystemFrequency = (RTC_CLK *
					(((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
							(((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /
							((LPC_SC->CCLKCFG & 0xFF)+ 1));
			break;
		}

	} else {
		switch (LPC_SC->CLKSRCSEL & 0x03) {
		case 0:                           /* Internal RC oscillator => PLL0     */
		case 3:                           /* Reserved, default to Internal RC   */
			SystemFrequency = IRC_OSC / ((LPC_SC->CCLKCFG & 0xFF)+ 1);
			break;
		case 1:                           /* Main oscillator => PLL0            */
			SystemFrequency = OSC_CLK / ((LPC_SC->CCLKCFG & 0xFF)+ 1);
			break;
		case 2:                           /* RTC oscillator => PLL0             */
			SystemFrequency = RTC_CLK / ((LPC_SC->CCLKCFG & 0xFF)+ 1);
			break;
		}
	}

}

#define SYSCTL_PLL_ENABLE   (1 << 0)/*!< PLL enable flag */
#define SYSCTL_PLL0STS_ENABLED   (1 << 24)	/*!< PLL0 enable flag */
#define SYSCTL_PLL0STS_CONNECTED (1 << 25)	/*!< PLL0 connect flag */
#define SYSCTL_PLL0STS_LOCKED    (1 << 26)	/*!< PLL0 connect flag */
#define SYSCTL_PLL1STS_ENABLED   (1 << 8)	/*!< PLL1 enable flag */
#define SYSCTL_PLL1STS_CONNECTED (1 << 9)	/*!< PLL1 connect flag */
#define SYSCTL_PLL1STS_LOCKED    (1 << 10)	/*!< PLL1 connect flag */
#define SYSCTL_PLL_CONNECT (1 << 1)	/*!< PLL connect flag only applies to 175x/6x */


#define PLL1CFG_Val           0x00000023

int LPC_EnablePLL1(void)
{
	/*
	 * Switch power to main crystal oscillator on
	 */
	LPC_SC->SCS = OSCEN;

	while ((LPC_SC->SCS & OSCSTAT) == 0);

	LPC_SC->PLL1CFG   = PLL1CFG_Val;
	LPC_SC->PLL1FEED  = PLLFEED1;
	LPC_SC->PLL1FEED  = PLLFEED2;

	LPC_SC->PLL1CON   = PLLE1;            /* PLL1 Enable                        */
	LPC_SC->PLL1FEED  = PLLFEED1;
	LPC_SC->PLL1FEED  = PLLFEED2;
	while (!(LPC_SC->PLL1STAT & PLOCK1)); /* Wait for PLOCK1                    */

	LPC_SC->PLL1CON   = PLLE1 | PLLC1;    /* PLL1 Enable & Connect              */
	LPC_SC->PLL1FEED  = PLLFEED1;
	LPC_SC->PLL1FEED  = PLLFEED2;

	while (!(LPC_SC->PLL1STAT & (PLLC1_STAT | PLLE1_STAT)));/* Wait for PLLC1_STAT & PLLE1_STAT */


	if(FREQ_MAIN_OSC == 12000000)
	{
		/* Setup USB PLL1 for a 48MHz clock
	   Input clock rate (FIN) is main oscillator = 12MHz
	   PLL1 Output = USBCLK = 48MHz = FIN * MSEL, so MSEL = 4.
	   FCCO = USBCLK = USBCLK * 2 * P. It must be between 156 MHz to 320 MHz.
	   so P = 2 and FCCO = 48MHz * 2 * 2 = 192MHz */
		Chip_Clock_SetupPLL(SYSCTL_USB_PLL, 3, 1);	/* Multiply by 4, Divide by 2 */
	}

	if(FREQ_MAIN_OSC == 16000000)
	{
		/* Setup USB PLL1 for a 48MHz clock
		   Input clock rate (FIN) is main oscillator = 12MHz
		   PLL1 Output = USBCLK = 48MHz = FIN * MSEL, so MSEL = 4.
		   FCCO = USBCLK = USBCLK * 2 * P. It must be between 156 MHz to 320 MHz.
		   so P = 2 and FCCO = 48MHz * 2 * 2 = 192MHz */
		Chip_Clock_SetupPLL(SYSCTL_USB_PLL, 2, 0);	/* Multiply by 3, Divide by 1 */
	}

	/* Use PLL1 output as USB Clock Source */
	/* Enable PLL1 */
	Chip_Clock_EnablePLL(SYSCTL_USB_PLL, SYSCTL_PLL_ENABLE);

	/* Wait for PLL1 to lock */
	while (!Chip_Clock_IsUSBPLLLocked()) {}

	/* Connect PLL1 */
	Chip_Clock_EnablePLL(SYSCTL_USB_PLL, SYSCTL_PLL_ENABLE | SYSCTL_PLL_CONNECT);

	/* Wait for PLL1 to be connected */
	while (!Chip_Clock_IsUSBPLLConnected()) {}

	/* Enable AHB clock to the USB block and USB RAM. */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_USB_loc);

	return 1;
}

#define LPC_SYSCTL_BASE           0x400FC000
#define LPC_SYSCTL                ((LPC_SYSCTL_T           *) LPC_SYSCTL_BASE)


inline char Chip_Clock_IsUSBPLLLocked(void)
{
	return (char) ((LPC_SYSCTL->PLL[1].PLLSTAT & SYSCTL_PLL1STS_LOCKED) != 0);
}

inline void Chip_Clock_FeedPLL(CHIP_SYSCTL_PLL_T PLLNum)
{
	LPC_SYSCTL->PLL[PLLNum].PLLFEED = 0xAA;
	LPC_SYSCTL->PLL[PLLNum].PLLFEED = 0x55;
}


void Chip_Clock_EnablePLL(CHIP_SYSCTL_PLL_T PLLNum, uint32_t flags)
{
	uint32_t temp;

	temp = LPC_SYSCTL->PLL[PLLNum].PLLCON;
	temp |= flags;
	LPC_SYSCTL->PLL[PLLNum].PLLCON = temp;
	Chip_Clock_FeedPLL(PLLNum);
}

void Chip_Clock_SetupPLL(CHIP_SYSCTL_PLL_T PLLNum, uint32_t msel, uint32_t psel)
{
	uint32_t PLLcfg;

	/* PLL0 and PLL1 are slightly different */
	if (PLLNum == SYSCTL_MAIN_PLL) {
		PLLcfg = (msel) | (psel << 16);
	}
	else {
		PLLcfg = (msel) | (psel << 5);
	}


	LPC_SYSCTL->PLL[PLLNum].PLLCFG = PLLcfg;
	LPC_SYSCTL->PLL[PLLNum].PLLCON = 0x1;
	Chip_Clock_FeedPLL(PLLNum);
}


/* Enables power and clocking for a peripheral */
void Chip_Clock_EnablePeriphClock(CHIP_SYSCTL_CLOCK_T_loc clk)
{
	uint32_t bs = (uint32_t) clk;

#if defined(CHIP_LPC40XX)
	if (bs >= 32) {
		LPC_SYSCTL->PCONP1 |= (1 << (bs - 32));
	}
	else {
		LPC_SYSCTL->PCONP |= (1 << bs);
	}
#else
	LPC_SYSCTL->PCONP |= (1 << bs);
#endif
}

char Chip_Clock_IsUSBPLLConnected(void)
{
	return (char) ((LPC_SYSCTL->PLL[1].PLLSTAT & SYSCTL_PLL1STS_CONNECTED) != 0);
}



//Fonction de réglage de la "RTI"
//Reçoit la période en ms
void Init_Systick(unsigned int Periode)
{
	SysTick_Config(SystemFrequency / 1000 * Periode);
}

/*
 * LPC_DisablePLL1()
 *
 * Disable PLL1 and stop the main oscillator if it is not in use as sysclk.
 * The PLL is disabled  but stays connected.
 * In this way the USB block is not clocked saving approx. 4 mA supply current.
 * See AN10915 "Using the LPC1700 power modes".
 */
int LPC_DisablePLL1(void)
{
	LPC_SC->PLL1CON = PLLC1;
	LPC_SC->PLL1FEED = PLLFEED1;
	LPC_SC->PLL1FEED = PLLFEED2;
	while(((LPC_SC->PLL1STAT & PLLC1_STAT) == 0));

	if((LPC_SC->CLKSRCSEL & 3) != LPC_MAIN_OSC)
	{
		LPC_SC->SCS = 0;
	}
	return 1;
}

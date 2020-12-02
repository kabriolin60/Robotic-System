/*
 * @brief LPC17xx/40xx Chip specific SystemInit
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
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

#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/*#define CARTE_LPCXPRESSO

#ifdef CARTE_LPCXPRESSO
#define FREQ_MAIN_OSC  12000000
#else
#define FREQ_MAIN_OSC  16000000
#endif
#define FREQ_INTRC_OSC  4000000
#define FREQ_RTC_OSC      32768
*/

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

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/
#if defined(CHIP_LPC175X_6X)
void Chip_SetupIrcClocking(void)
{
	/*int index = 0;
	uint32_t freq, msel, nsel, clkdiv;

	if(FREQ_MAIN_OSC == 12000000)
	{
		while((MainOscPLL0conf_12[index].freq != cclk) && MainOscPLL0conf_12[index].freq) index++;
		if(MainOscPLL0conf_12[index].freq == 0)
		{
			return;
		}
		freq   = MainOscPLL0conf_12[index].freq;
		msel   = MainOscPLL0conf_12[index].msel;
		nsel   = MainOscPLL0conf_12[index].nsel;
		clkdiv = MainOscPLL0conf_12[index].clkdiv;
	}else if(FREQ_MAIN_OSC == 16000000)
	{
		while((MainOscPLL0conf_16[index].freq != cclk) && MainOscPLL0conf_16[index].freq) index++;
		if(MainOscPLL0conf_16[index].freq == 0)
		{
			return;
		}
		freq   = MainOscPLL0conf_16[index].freq;
		msel   = MainOscPLL0conf_16[index].msel;
		nsel   = MainOscPLL0conf_16[index].nsel;
		clkdiv = MainOscPLL0conf_16[index].clkdiv;
	}*/

	/* Disconnect the Main PLL if it is connected already */
	if (Chip_Clock_IsMainPLLConnected()) {
		Chip_Clock_DisablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
	}

	/* Disable the PLL if it is enabled */
	if (Chip_Clock_IsMainPLLEnabled()) {
		Chip_Clock_DisablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	}

	Chip_Clock_SetCPUClockDiv(0);
	Chip_Clock_SetMainPLLSource(SYSCTL_PLLCLKSRC_IRC);

	/* FCCO = ((44+1) * 2 * 4MHz) / (0+1) = 360MHz */
	Chip_Clock_SetupPLL(SYSCTL_MAIN_PLL, 44, 0);

	Chip_Clock_EnablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);

	Chip_Clock_SetCPUClockDiv(2);
	while (!Chip_Clock_IsMainPLLLocked()) {} /* Wait for the PLL to Lock */

	Chip_Clock_EnablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
}



void Chip_SetupXtalClocking(long cclk, uint32_t XTALFREQ)
{
	int index = 0;
	uint32_t  msel, nsel, clkdiv;


	if(XTALFREQ == 12000000)
	{
		while((MainOscPLL0conf_12[index].freq != cclk) && MainOscPLL0conf_12[index].freq) index++;
		if(MainOscPLL0conf_12[index].freq == 0)
		{
			return;
		}
		//freq   = MainOscPLL0conf_12[index].freq;
		msel   = MainOscPLL0conf_12[index].msel;
		nsel   = MainOscPLL0conf_12[index].nsel;
		clkdiv = MainOscPLL0conf_12[index].clkdiv;
	}else if(XTALFREQ == 16000000)
	{
		while((MainOscPLL0conf_16[index].freq != cclk) && MainOscPLL0conf_16[index].freq) index++;
		if(MainOscPLL0conf_16[index].freq == 0)
		{
			return;
		}
		//freq   = MainOscPLL0conf_16[index].freq;
		msel   = MainOscPLL0conf_16[index].msel;
		nsel   = MainOscPLL0conf_16[index].nsel;
		clkdiv = MainOscPLL0conf_16[index].clkdiv;
	}

	/* Disconnect the Main PLL if it is connected already */
	if (Chip_Clock_IsMainPLLConnected()) {
		Chip_Clock_DisablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
	}

	/* Disable the PLL if it is enabled */
	if (Chip_Clock_IsMainPLLEnabled()) {
		Chip_Clock_DisablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	}

	/* Enable the crystal */
	if (!Chip_Clock_IsCrystalEnabled())
		Chip_Clock_EnableCrystal();
	while(!Chip_Clock_IsCrystalEnabled()) {}

	/* Set PLL0 Source to Crystal Oscillator */
	Chip_Clock_SetCPUClockDiv(0);
	Chip_Clock_SetMainPLLSource(SYSCTL_PLLCLKSRC_MAINOSC);

	/* FCCO = ((15+1) * 2 * 12MHz) / (0+1) = 384MHz */
	Chip_Clock_SetupPLL(SYSCTL_MAIN_PLL, msel, nsel);

	Chip_Clock_EnablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);

	/* 384MHz / (3+1) = 96MHz */
	Chip_Clock_SetCPUClockDiv(clkdiv);
	while (!Chip_Clock_IsMainPLLLocked()) {} /* Wait for the PLL to Lock */

	Chip_Clock_EnablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
}
#endif

#if (defined(CHIP_LPC177X_8X) | defined(CHIP_LPC40XX))
/* Clock and PLL initialization based on the internal oscillator */
void Chip_SetupIrcClocking(void)
{
	/* Clock the CPU from SYSCLK, in case if it is clocked by PLL0 */
	Chip_Clock_SetCPUClockSource(SYSCTL_CCLKSRC_SYSCLK);

	/* Disable the PLL if it is enabled */
	if (Chip_Clock_IsMainPLLEnabled()) {
		Chip_Clock_DisablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	}

	/* It is safe to switch the PLL Source to IRC */
	Chip_Clock_SetMainPLLSource(SYSCTL_PLLCLKSRC_IRC);

	/* FCCO = 12MHz * (9+1) * 2 * (0+1) = 240MHz */
	/* Fout = FCCO / ((0+1) * 2) = 120MHz */
	Chip_Clock_SetupPLL(SYSCTL_MAIN_PLL, 9, 0);

	Chip_Clock_EnablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	Chip_Clock_SetCPUClockDiv(1);
	while (!Chip_Clock_IsMainPLLLocked()) {} /* Wait for the PLL to Lock */
	Chip_Clock_SetCPUClockSource(SYSCTL_CCLKSRC_MAINPLL);

	/* Peripheral clocking will be derived from PLL0 with a divider of 2 (60MHz) */
	Chip_Clock_SetPCLKDiv(2);
}

/* Clock and PLL initialization based on the external oscillator */
void Chip_SetupXtalClocking(void)
{
	/* Enable the crystal */
	if (!Chip_Clock_IsCrystalEnabled())
		Chip_Clock_EnableCrystal();

	while(!Chip_Clock_IsCrystalEnabled()) {}

	/* Clock the CPU from SYSCLK, in case if it is clocked by PLL0 */
	Chip_Clock_SetCPUClockSource(SYSCTL_CCLKSRC_SYSCLK);

	/* Disable the PLL if it is enabled */
	if (Chip_Clock_IsMainPLLEnabled()) {
		Chip_Clock_DisablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	}

	/* It is safe to switch the PLL Source to Crystal Oscillator */
	Chip_Clock_SetMainPLLSource(SYSCTL_PLLCLKSRC_MAINOSC);

	/* FCCO = 12MHz * (9+1) * 2 * (0+1) = 240MHz */
	/* Fout = FCCO / ((0+1) * 2) = 120MHz */
	Chip_Clock_SetupPLL(SYSCTL_MAIN_PLL, 9, 0);

	Chip_Clock_EnablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	Chip_Clock_SetCPUClockDiv(1);

	while (!Chip_Clock_IsMainPLLLocked()) {} /* Wait for the PLL to Lock */
	Chip_Clock_SetCPUClockSource(SYSCTL_CCLKSRC_MAINPLL);

	/* Peripheral clocking will be derived from PLL0 with a divider of 2 (60MHz) */
	Chip_Clock_SetPCLKDiv(2);
}
#endif

/* Set up and initialize hardware prior to call to main */
void Chip_SystemInit(void)
{
	/* Setup Chip clocking */
	Chip_SetupIrcClocking();
}



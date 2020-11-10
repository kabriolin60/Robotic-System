/*
 * clocking.h: Oscillator and PLL related functions for lpc17xx
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

/*
 * Can't get clocking from the RTC to work with debugger.
 * make sure that this is not compiled ...
 */

#include "LPC17xx.h"

#undef LPC_CLOCK_FROM_RTC

#define LPC_INTRC_OSC 0
#define LPC_MAIN_OSC  1
#ifdef LPC_CLOCK_FROM_RTC
	#define LPC_RTC_OSC   2
#endif


#ifndef OSCRANGE

/*
 * SCS bit values
 */
#define OSCRANGE (1<<4)
#define OSCEN    (1<<5)
#define OSCSTAT  (1<<6)

/*
 * PCON bit values
 */
#define PM0        (1<< 0)
#define PM1        (1<< 1)
#define BODRPM     (1<< 2)
#define BOGD       (1<< 3)
#define BORD       (1<< 4)
#define SMFLAG     (1<< 8)
#define DSFLAG     (1<< 9)
#define PDFLAG     (1<<10)
#define DPDFLAG    (1<<11)
/*
 * PCONP bit values
 * Setting these bits in PCONP will enable the clock to the given peripheral
 */
#define PCTIM0     (1<< 1)
#define PCTIM1     (1<< 2)
#define PCUART0    (1<< 3)
#define PCUART1    (1<< 4)
#define PCPWM1     (1<< 6)
#define PCI2C0     (1<< 7)
#define PCSPI      (1<< 8)
#define PCRTC      (1<< 9)
#define PCSSP1     (1<<10)
#define PCADC      (1<<12)
#define PCCAN1     (1<<13)
#define PCCAN2     (1<<14)
#define PCGPIO     (1<<15)
#define PCRIT      (1<<16)
#define PCMPWM     (1<<17)
#define PCQEI      (1<<18)
#define PCI2C1     (1<<19)
#define PCSSP0     (1<<21)
#define PCTIM2     (1<<22)
#define PCTIM3     (1<<23)
#define PCUART2    (1<<24)
#define PCUART3    (1<<25)
#define PCI2C2     (1<<26)
#define PCI2S      (1<<27)
#define PCGPDMA    (1<<29)
#define PCENET     (1<<30)
#define PCUSB      (1<<31)

/*
 * PLL0CON bit values
 */
#define PLLE0 0x01
#define PLLC0 0x03

/*
 * PLL0STAT bit values
 */
#define PLLE0_STAT (1<<24)
#define PLLC0_STAT (1<<25)
#define PLOCK0     (1<<26)


/*
 * PLL1CON bit values
 */
#define PLLE1      (1<< 0)
#define PLLC1      (1<< 1)

/*
 * PLL1STAT bit values
 */
#define PLLE1_STAT (1<< 8)
#define PLLC1_STAT (1<< 9)
#define PLOCK1     (1<<10)

/*
 * PLL Feed constants
 */
#define PLLFEED1   0xAA
#define PLLFEED2   0x55

#endif

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define XTAL        (12000000UL)        /* Oscillator frequency               */
#define OSC_CLK     (      XTAL)        /* Main oscillator frequency          */
#define RTC_CLK     (   32000UL)        /* RTC oscillator frequency           */
#define IRC_OSC     ( 4000000UL)        /* Internal RC oscillator frequency   */

/*
 * Wahhh... this should really be defined only in FreeRTOSConfig.h !!!
 */
#ifndef configTICK_RATE_HZ
#define configTICK_RATE_HZ 200
#endif

/*
 * Set which oscillator to use
 */

void SystemClockUpdate (void);
int LPC_UseOscillator(unsigned char osc);
int LPC_SetPLL0(long cclk);
int LPC_EnablePLL1(void);
int LPC_DisablePLL1(void);
void Init_Systick(unsigned int Periode);

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
#ifndef _clocking_h_
#define _clocking_h_

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

#define CHIP_LPC175X_6X
#define __IO volatile
#define __O volatile
#define __O volatile

typedef struct {
	__IO uint32_t PLLCON;					/*!< (R/W)  PLL Control Register */
	__IO uint32_t PLLCFG;					/*!< (R/W)  PLL Configuration Register */
	__I  uint32_t PLLSTAT;					/*!< (R/ )  PLL Status Register */
	__O  uint32_t PLLFEED;					/*!< ( /W)  PLL Feed Register */
	uint32_t RESERVED1[4];
} SYSCTL_PLL_REGS_T;

typedef enum {
	SYSCTL_MAIN_PLL,			/*!< Main PLL (PLL0) */
	SYSCTL_USB_PLL,				/*!< USB PLL (PLL1) */
} CHIP_SYSCTL_PLL_T;


/**
 * @brief LPC17XX/40XX Clock and Power register block structure
 */

typedef struct {
	__IO uint32_t FLASHCFG;					/*!< Offset: 0x000 (R/W)  Flash Accelerator Configuration Register */
	uint32_t RESERVED0[15];
	__IO uint32_t MEMMAP;					/*!< Offset: 0x000 (R/W)  Flash Accelerator Configuration Register */
	uint32_t RESERVED1[15];
	SYSCTL_PLL_REGS_T PLL[SYSCTL_USB_PLL + 1];		/*!< Offset: 0x080: PLL0 and PLL1 */
	__IO uint32_t PCON;						/*!< Offset: 0x0C0 (R/W)  Power Control Register */
	__IO uint32_t PCONP;					/*!< Offset: 0x0C4 (R/W)  Power Control for Peripherals Register */
#if defined(CHIP_LPC175X_6X)
	uint32_t RESERVED2[15];
#elif defined(CHIP_LPC177X_8X)
	uint32_t RESERVED2[14];
	__IO uint32_t EMCCLKSEL;				/*!< Offset: 0x100 (R/W)  External Memory Controller Clock Selection Register */
#else
	__IO uint32_t PCONP1;					/*!< Offset: 0x0C8 (R/W)  Power Control 1 for Peripherals Register */
	uint32_t RESERVED2[13];
	__IO uint32_t EMCCLKSEL;				/*!< Offset: 0x100 (R/W)  External Memory Controller Clock Selection Register */
#endif
	__IO uint32_t CCLKSEL;					/*!< Offset: 0x104 (R/W)  CPU Clock Selection Register */
	__IO uint32_t USBCLKSEL;				/*!< Offset: 0x108 (R/W)  USB Clock Selection Register */
	__IO uint32_t CLKSRCSEL;				/*!< Offset: 0x10C (R/W)  Clock Source Select Register */
	__IO uint32_t CANSLEEPCLR;				/*!< Offset: 0x110 (R/W)  CAN Sleep Clear Register */
	__IO uint32_t CANWAKEFLAGS;				/*!< Offset: 0x114 (R/W)  CAN Wake-up Flags Register */
	uint32_t RESERVED3[10];
	__IO uint32_t EXTINT;					/*!< Offset: 0x140 (R/W)  External Interrupt Flag Register */
	uint32_t RESERVED4;
	__IO uint32_t EXTMODE;					/*!< Offset: 0x148 (R/W)  External Interrupt Mode Register */
	__IO uint32_t EXTPOLAR;					/*!< Offset: 0x14C (R/W)  External Interrupt Polarity Register */
	uint32_t RESERVED5[12];
	__IO uint32_t RSID;						/*!< Offset: 0x180 (R/W)  Reset Source Identification Register */
#if defined(CHIP_LPC175X_6X) || defined(CHIP_LPC40XX)
	uint32_t RESERVED6[7];
#elif defined(CHIP_LPC177X_8X)
	uint32_t RESERVED6;
	uint32_t MATRIXARB;
	uint32_t RESERVED6A[5];
#endif
	__IO uint32_t SCS;						/*!< Offset: 0x1A0 (R/W)  System Controls and Status Register */
	__IO uint32_t RESERVED7;
#if defined(CHIP_LPC175X_6X)
	__IO uint32_t PCLKSEL[2];				/*!< Offset: 0x1A8 (R/W)  Peripheral Clock Selection Register */
	uint32_t RESERVED8[4];
#else
	__IO uint32_t PCLKSEL;				/*!< Offset: 0x1A8 (R/W)  Peripheral Clock Selection Register */
	uint32_t RESERVED9;
	__IO uint32_t PBOOST;					/*!< Offset: 0x1B0 (R/W)  Power Boost control register */
	__IO uint32_t SPIFICLKSEL;
	__IO uint32_t LCD_CFG;					/*!< Offset: 0x1B8 (R/W)  LCD Configuration and clocking control Register */
	uint32_t RESERVED10;
#endif
	__IO uint32_t USBIntSt;					/*!< Offset: 0x1C0 (R/W)  USB Interrupt Status Register */
	__IO uint32_t DMAREQSEL;				/*!< Offset: 0x1C4 (R/W)  DMA Request Select Register */
	__IO uint32_t CLKOUTCFG;				/*!< Offset: 0x1C8 (R/W)  Clock Output Configuration Register */
#if defined(CHIP_LPC175X_6X)
	uint32_t RESERVED11[6];
#else
	__IO uint32_t RSTCON[2];				/*!< Offset: 0x1CC (R/W)  RESET Control0/1 Registers */
	uint32_t RESERVED11[2];
	__IO uint32_t EMCDLYCTL;				/*!< Offset: 0x1DC (R/W) SDRAM programmable delays          */
	__IO uint32_t EMCCAL;					/*!< Offset: 0x1E0 (R/W) Calibration of programmable delays */
#endif
} LPC_SYSCTL_T;


/**
 * Power control for peripherals
 */
typedef enum CHIP_SYSCTL_CLOCK_loc {
	SYSCTL_CLOCK_RSVD0_loc,
	SYSCTL_CLOCK_TIMER0_loc,			/*!< Timer 0 clock */
	SYSCTL_CLOCK_TIMER1_loc,			/*!< Timer 1 clock */
	SYSCTL_CLOCK_UART0_loc,				/*!< UART 0 clock */
	SYSCTL_CLOCK_UART1_loc,				/*!< UART 1 clock */
	SYSCTL_CLOCK_RSVD5_loc,
	SYSCTL_CLOCK_PWM1_loc,				/*!< PWM1 clock */
	SYSCTL_CLOCK_I2C0_loc,				/*!< I2C0 clock */
	SYSCTL_CLOCK_SPI_loc,				/*!< SPI clock */
	SYSCTL_CLOCK_RTC_loc,				/*!< RTC clock */
	SYSCTL_CLOCK_SSP1_loc,				/*!< SSP1 clock */
	SYSCTL_CLOCK_RSVD11_loc,
	SYSCTL_CLOCK_ADC_loc,				/*!< ADC clock */
	SYSCTL_CLOCK_CAN1_loc,				/*!< CAN1 clock */
	SYSCTL_CLOCK_CAN2_loc,				/*!< CAN2 clock */
	SYSCTL_CLOCK_GPIO_loc,				/*!< GPIO clock */
	SYSCTL_CLOCK_RIT_loc,				/*!< RIT clock */
	SYSCTL_CLOCK_MCPWM_loc,				/*!< MCPWM clock */
	SYSCTL_CLOCK_QEI_loc,				/*!< QEI clock */
	SYSCTL_CLOCK_I2C1_loc,				/*!< I2C1 clock */
	SYSCTL_CLOCK_RSVD20_loc,
	SYSCTL_CLOCK_SSP0_loc,				/*!< SSP0 clock */
	SYSCTL_CLOCK_TIMER2_loc,			/*!< Timer 2 clock */
	SYSCTL_CLOCK_TIMER3_loc,			/*!< Timer 3 clock */
	SYSCTL_CLOCK_UART2_loc,				/*!< UART 2 clock */
	SYSCTL_CLOCK_UART3_loc,				/*!< UART 3 clock */
	SYSCTL_CLOCK_I2C2_loc,				/*!< I2C2 clock */
	SYSCTL_CLOCK_I2S_loc,				/*!< I2S clock */
	SYSCTL_CLOCK_RSVD28_loc,
	SYSCTL_CLOCK_GPDMA_loc,				/*!< GP DMA clock */
	SYSCTL_CLOCK_ENET_loc,				/*!< EMAC/Ethernet clock */
	SYSCTL_CLOCK_USB_loc,				/*!< USB clock */
	SYSCTL_CLOCK_RSVD32_loc,
	SYSCTL_CLOCK_RSVD33_loc,
	SYSCTL_CLOCK_RSVD34_loc,
	SYSCTL_CLOCK_RSVD35_loc,
} CHIP_SYSCTL_CLOCK_T_loc;
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

inline char Chip_Clock_IsUSBPLLLocked(void);
inline void Chip_Clock_FeedPLL(CHIP_SYSCTL_PLL_T PLLNum);
void Chip_Clock_EnablePLL(CHIP_SYSCTL_PLL_T PLLNum, uint32_t flags);
void Chip_Clock_SetupPLL(CHIP_SYSCTL_PLL_T PLLNum, uint32_t msel, uint32_t psel);
void Chip_Clock_EnablePeriphClock(CHIP_SYSCTL_CLOCK_T_loc clk);
char Chip_Clock_IsUSBPLLConnected(void);

#endif

/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2020
 * Generated linker script file for LPC1769
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v11.1.1 [Build 3241] [2020-03-02] on 31 mai 2020 18:24:19
 */

MEMORY
{
  /* Define each memory region */
  MFlash512 (rx) : ORIGIN = 0x10000, LENGTH = 0x70000 /* 448K bytes (alias Flash) */  
  RamLoc32 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x8000 /* 32K bytes (alias RAM) */  
  RamAHB32 (rwx) : ORIGIN = 0x2007c000, LENGTH = 0x800 /* 2K bytes (alias RAM2) */  
  RAM3 (rwx) : ORIGIN = 0x2007c800, LENGTH = 0x7800 /* 30K bytes (alias RAM3) */  
}

  /* Define a symbol for the top of each memory region */
  __base_MFlash512 = 0x10000  ; /* MFlash512 */  
  __base_Flash = 0x10000 ; /* Flash */  
  __top_MFlash512 = 0x10000 + 0x70000 ; /* 448K bytes */  
  __top_Flash = 0x10000 + 0x70000 ; /* 448K bytes */  
  __base_RamLoc32 = 0x10000000  ; /* RamLoc32 */  
  __base_RAM = 0x10000000 ; /* RAM */  
  __top_RamLoc32 = 0x10000000 + 0x8000 ; /* 32K bytes */  
  __top_RAM = 0x10000000 + 0x8000 ; /* 32K bytes */  
  __base_RamAHB32 = 0x2007c000  ; /* RamAHB32 */  
  __base_RAM2 = 0x2007c000 ; /* RAM2 */  
  __top_RamAHB32 = 0x2007c000 + 0x800 ; /* 2K bytes */  
  __top_RAM2 = 0x2007c000 + 0x800 ; /* 2K bytes */  
  __base_RAM3 = 0x2007c800  ; /* RAM3 */  
  __base_RAM3 = 0x2007c800 ; /* RAM3 */  
  __top_RAM3 = 0x2007c800 + 0x7800 ; /* 30K bytes */  
  __top_RAM3 = 0x2007c800 + 0x7800 ; /* 30K bytes */  

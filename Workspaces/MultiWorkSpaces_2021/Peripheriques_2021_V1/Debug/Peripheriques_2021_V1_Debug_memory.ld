/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2020
 * Generated linker script file for LPC1347
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v11.2.1 [Build 4149] [2020-10-07] on 28 déc. 2020 13:43:35
 */

MEMORY
{
  /* Define each memory region */
  MFlash64 (rx) : ORIGIN = 0x4000, LENGTH = 0xc000 /* 48K bytes (alias Flash) */  
  RamLoc8 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x2000 /* 8K bytes (alias RAM) */  
  RamUsb2 (rwx) : ORIGIN = 0x20004000, LENGTH = 0x800 /* 2K bytes (alias RAM2) */  
  RamPeriph2 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x800 /* 2K bytes (alias RAM3) */  
}

  /* Define a symbol for the top of each memory region */
  __base_MFlash64 = 0x4000  ; /* MFlash64 */  
  __base_Flash = 0x4000 ; /* Flash */  
  __top_MFlash64 = 0x4000 + 0xc000 ; /* 48K bytes */  
  __top_Flash = 0x4000 + 0xc000 ; /* 48K bytes */  
  __base_RamLoc8 = 0x10000000  ; /* RamLoc8 */  
  __base_RAM = 0x10000000 ; /* RAM */  
  __top_RamLoc8 = 0x10000000 + 0x2000 ; /* 8K bytes */  
  __top_RAM = 0x10000000 + 0x2000 ; /* 8K bytes */  
  __base_RamUsb2 = 0x20004000  ; /* RamUsb2 */  
  __base_RAM2 = 0x20004000 ; /* RAM2 */  
  __top_RamUsb2 = 0x20004000 + 0x800 ; /* 2K bytes */  
  __top_RAM2 = 0x20004000 + 0x800 ; /* 2K bytes */  
  __base_RamPeriph2 = 0x20000000  ; /* RamPeriph2 */  
  __base_RAM3 = 0x20000000 ; /* RAM3 */  
  __top_RamPeriph2 = 0x20000000 + 0x800 ; /* 2K bytes */  
  __top_RAM3 = 0x20000000 + 0x800 ; /* 2K bytes */  

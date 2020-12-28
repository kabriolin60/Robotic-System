/*
 * Flash_iap.h
 *
 *  Created on: 30 mai 2020
 *      Author: kabri
 */

#ifndef FLASH_IAP_H_
#define FLASH_IAP_H_

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
void Flash_Write_Block(uint32_t destadrr, uint32_t *srcAdd, uint32_t byteswrt);

#endif /* FLASH_IAP_H_ */

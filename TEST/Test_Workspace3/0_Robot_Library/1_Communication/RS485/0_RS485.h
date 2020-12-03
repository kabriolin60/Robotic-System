/*
 * RS485.h
 *
 *  Created on: 24 avr. 2020
 *      Author: kabri
 */

#ifndef _0_RS485_RS485_H_
#define _0_RS485_RS485_H_

#include "Lib_includes.h"

void _0_RS485_Init(uint8_t RS485_dir_port, uint8_t RS485_dir_bit);

void _0_RS485_Master_Mode(uint8_t RS485_dir_port, uint8_t RS485_dir_bit);

void _0_RS485_Slave_Mode(uint8_t RS485_dir_port, uint8_t RS485_dir_bit);

#endif /* 0_COMMUNICATION_RS485_RS485_H_ */

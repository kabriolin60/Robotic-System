/*
 * 1_AX12.h
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */

#ifndef _1_AX12_H_
#define _1_AX12_H_


#include "0_AX12.h"
#include "board.h"
#include "Configuration.h"
#include "Type_Declaration.h"
#include "queue.h"
#include "semphr.h"


struct st_AX_12
{
	unsigned char Numero;
	unsigned short Position;
	short Speed;
	short Torque;
	unsigned short Max_Torque;
	unsigned short Speed_Consigne;
	unsigned short Goal;
	unsigned char Slope;
	unsigned char Margin;
	unsigned char Temperature;
	unsigned char Voltage;
	unsigned char LED:1;
	unsigned char Torque_Enable:1;
};


void _1_Init_AX12(void);

void _1_AX12_Interpretation_RX(void *pvParameters);

void _1_AX12_Recomposition_Rx(void *pvParameters);

void _1_AX12_WAIT_FOR_ACK(struct st_AX_12_Trame *trame, bool wait);


__attribute__((optimize("O0"))) void AX_12_SET_POSITION(unsigned char ID, unsigned int position, unsigned char Attente_Synchro);

__attribute__((optimize("O0"))) unsigned short AX_12_READ_Position(unsigned char ID);

__attribute__((optimize("O0"))) void AX_12_DO_RESET(unsigned char ID);

__attribute__((optimize("O0"))) void AX_12_RETURN_DELAY(unsigned char ID, unsigned char Value);

__attribute__((optimize("O0"))) void AX_12_TORQUE_ENABLE(unsigned char ID, unsigned char Attente_Synchro);

__attribute__((optimize("O0"))) void AX_12_TORQUE_DISABLE(unsigned char ID, unsigned char Attente_Synchro);

__attribute__((optimize("O0"))) void AX_12_SET_TORQUE(unsigned char ID, unsigned int torque, unsigned char Attente_Synchro);

__attribute__((optimize("O0"))) void AX_12_CHANGE_BAUDRATE(unsigned char ID, long Baudrate);

__attribute__((optimize("O0"))) void AX_12_CHANGE_ID(unsigned char Old_ID, unsigned char New_ID);

__attribute__((optimize("O0"))) void AX_12_SYNCHRO_ACTION(void);

__attribute__((optimize("O0"))) void AX_12_SET_SPEED(unsigned char ID, unsigned int speed, unsigned char Attente_Synchro);

__attribute__((optimize("O0"))) int AX_12_TWO_CHAR_TO_INT(unsigned char Input1, unsigned char Input2);

__attribute__((optimize("O0"))) unsigned char AX_12_READ_VALUES(unsigned char ID, struct st_AX_12 *Loc_AX_12);

__attribute__((optimize("O0"))) void AX_12_LED_ON(unsigned char ID);

__attribute__((optimize("O0"))) void AX_12_LED_OFF(unsigned char ID);

#endif /* 1_AX12_H_ */

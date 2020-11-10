/*
 * Communication_NG.h
 *
 *  Created on: 1 févr. 2014
 *      Author: Fixe
 */

#ifndef COMMUNICATION_NG_H_
#define COMMUNICATION_NG_H_

#include "Includes.h"
#include "Communication_struct.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef OK
#define OK 1
#define NOK 0
#define ERREUR -1
#endif

//Copie des donnees d'une struct a l'autre
#define COPYDATA(X, Y)	sizeof(X) > sizeof(Y) ? sizeof(Y) : sizeof(X); memcpy(&Y, &X, sizeof(Y));
#define COPYDATA2(X, Y)	memcpy(&Y, &X, sizeof(Y));
#define COPYSTRING(X, Y) strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X); memcpy(&Y, X, strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X));


#define COMMUNICATION_TRAME_MAX_DATA 64

#define BALISE_BOARD -3
#define PC -2
#define ALL_CARDS -1
#define IA_BOARD  0

struct Communication_Trame
{
	enum Com_Instruction Instruction;					//Instruction a echanger
	signed char Slave_Adresse;							//Adresse de la carte ciblée //ALL_CARDS (-1) pour toutes les cartes
	unsigned char Length;								//Nombre d'octets dans le tableau de data
	unsigned char Data[COMMUNICATION_TRAME_MAX_DATA];
	unsigned short XBEE_DEST_ADDR;						//Adresse de la cible Xbee en mode API
};

#define ADD_TRAME_RS485(data) Add_Trame_ToTXQueue(QueueEchangeTX_RS485, data, 50);

void Add_Trame_ToTXQueue(xQueueHandle QueueToAdd, struct Communication_Trame *send, TickType_t xTicksToWait);

void Communication_Send_Trame(struct Communication_Trame send, byte *SendFct(byte));

void uTask_ReceptionDatas(void *pvParameter);

void Communication_Reception_Data(void *pvParameters);

void Communication_Reception_Data_USB(void *pvParameters);

void Communication_Lecture_Trames(void *pvParameters);

void Communication_Lecture_Trames_USB(void *pvParameters);

void Communication_Traitement_Reception_Trame(void *pvParameters);

byte SendRS485Byte(byte data);

byte SendXbeeByte(byte data);

void Communication_Envoie_Nombre_Messages(void);

extern volatile struct st_FIFO Buffer_UART1;
extern volatile struct st_FIFO Buffer_UART_USB;

extern UInt32 MissedData;


#endif /* COMMUNICATION_NG_H_ */

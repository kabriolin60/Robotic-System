/*
 * 0_Event_Groupe.h
 *
 *  Created on: 24 avr. 2020
 *      Author: kabri
 */

#ifndef _0_EVENT_GROUP_H_
#define _0_EVENT_GROUP_H_

#include "event_groups.h"

#define eGROUP_SYNCH_TxTrameDispo 			( 1 << 0 ) 	//Flag indiquant que la trame permettant d'envoyer un message est dispo
#define eGROUP_SYNCH_RxTrameDispo 			( 1 << 1 ) 	//Flag indiquant que le trame permettant de recevoir un message est dispo

#define eGROUP_SYNCH_RS485_Rx_Data_Avail 	( 1 << 2 ) 	//Flag indiquant que des datas sont dispo dans le Buffer RX du canal RS485
#define eGROUP_SYNCH_USB_Rx_Data_Avail 		( 1 << 3 ) 	//Flag indiquant que des datas sont dispo dans le Buffer RX du canal USB
#define eGROUP_SYNCH_XBEE_Rx_Data_Avail 	( 1 << 4 ) 	//Flag indiquant que des datas sont dispo dans le Buffer RX du XBEE

#define eGROUP_SYNCH_STRATEGIE_TxTrameDispo ( 1 << 5 ) 	//Flag indiquant que la trame (séparée pour la stratégie) permettant d'envoyer un message est dispo depuis la page de strategie

#define eGROUP_SYNCH_COMMUNICATION_TxDispo 	( 1 << 6 ) 	//Flag indiquant que la fonction d'envoi un message est disponible

#define eGROUP_SYNCH_TxClearance			( 1 << 7 ) 	//Flag indiquant que la carte a reçu l'autorisation d'envoyer un message


extern EventGroupHandle_t _0_Comm_EventGroup;


void _0_Communication_Init_Event_Group();



#endif /* 0_COMMUNICATION_0_EVENT_GROUPE_H_ */

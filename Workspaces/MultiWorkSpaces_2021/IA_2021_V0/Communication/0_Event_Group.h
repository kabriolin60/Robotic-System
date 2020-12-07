/*
 * 0_Event_Groupe.h
 *
 *  Created on: 24 avr. 2020
 *      Author: kabri
 */

#ifndef _0_EVENT_GROUP_H_
#define _0_EVENT_GROUP_H_

#include "event_groups.h"

#define eGROUP_SYNCH_TxTrameDispo ( 1 << 0 ) 		//Flag indiquant que la trame permettant d'envoyer un message est dispo
#define eGROUP_SYNCH_RxTrameDispo ( 1 << 1 ) 		//Flag indiquant que le trame permettant de recevoir un message est dispo

#define eGROUP_SYNCH_RS485_Rx_Data_Avail ( 1 << 2 ) //Flag indiquant que des datas sont dispo dans le Buffer RX du canal RS485
#define eGROUP_SYNCH_USB_Rx_Data_Avail ( 1 << 3 ) 	//Flag indiquant que des datas sont dispo dans le Buffer RX du canal USB
#define eGROUP_SYNCH_XBEE_Rx_Data_Avail ( 1 << 4 ) 	//Flag indiquant que des datas sont dispo dans le Buffer RX du XBEE

#define eGROUP_SYNCH_STRATEGIE_TxTrameDispo ( 1 << 5 ) 		//Flag indiquant que la trame (séparée pour la stratégie) permettant d'envoyer un message est dispo depuis la page de strategie


extern EventGroupHandle_t _0_Comm_EventGroup;


/*
 * EventGroupe de Flags de présence des cartes sur le bus
 */

#define eGROUP_STATUS_CARTE_IA			( 1 << 0 )		//Flag indiquant la presence de la carte IA sur le bus
#define eGROUP_STATUS_CARTE_MultiFCT_1	( 1 << 1 )		//Flag indiquant la presence de la carte multiFCT 1 sur le bus
#define eGROUP_STATUS_CARTE_MultiFCT_2	( 2 << 1 )		//Flag indiquant la presence de la carte multiFCT 2 sur le bus
#define eGROUP_STATUS_CARTE_MultiFCT_3	( 3 << 1 )		//Flag indiquant la presence de la carte multiFCT 3 sur le bus
#define eGROUP_STATUS_CARTE_MultiFCT_4	( 4 << 1 )		//Flag indiquant la presence de la carte multiFCT 4 sur le bus
//A ajouter pour chaque carte possible

#define eGROUP_STATUS_CARTES_MULTI_FCT_ATTENDUES	eGROUP_STATUS_CARTE_MultiFCT_1
#define eGROUP_STATUS_CARTES_AUTRES_ATTENDUES		0
#define eGROUP_STATUS_CARTES_SERVO_ATTENDUES		eGROUP_STATUS_CARTES_MULTI_FCT_ATTENDUES

#define eGROUP_STATUS_CARTES_ATTENDUES eGROUP_STATUS_CARTE_IA | eGROUP_STATUS_CARTES_MULTI_FCT_ATTENDUES | eGROUP_STATUS_CARTES_AUTRES_ATTENDUES

extern EventGroupHandle_t _0_Status_EventGroup;


/*
 * EventGroup de flags des ACK de communication
 */
/* Event_Group d'indication du type d'ACK de communication reçu */
extern EventGroupHandle_t _0_ACK_Type_EventGroup;

/* Event_Group d'indication des adresses des cartes ayant renvoyé un ACK */
extern EventGroupHandle_t _0_ACK_Adresses;


void _0_Communication_Init_Event_Group();



#endif /* 0_COMMUNICATION_0_EVENT_GROUPE_H_ */

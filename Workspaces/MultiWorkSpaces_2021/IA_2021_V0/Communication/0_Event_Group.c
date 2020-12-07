/*
 * 0_Event_Groupe.c
 *
 *  Created on: 24 avr. 2020
 *      Author: kabri
 */

#include "FreeRTOS.h"
#include "event_groups.h"
#include <0_Event_Group.h>


/* Event_Group de synchronisation */
EventGroupHandle_t _0_Comm_EventGroup;

/* Event_Group de Status des cartes sur le bus */
EventGroupHandle_t _0_Status_EventGroup;

/* Event_Group d'indication du type d'ACK de communication reçu */
EventGroupHandle_t _0_ACK_Type_EventGroup;

/* Event_Group d'indication des adresses des cartes ayant renvoyé un ACK */
EventGroupHandle_t _0_ACK_Adresses;


/*****************************************************************************
 ** Function name:		_0_Communication_Init_Event_Group
 **
 ** Descriptions:		Fonction d'initialisation de l'Event Group de Synchronisation
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Communication_Init_Event_Group(void)
{
	//Init de l'event Group lié à la communication
	_0_Comm_EventGroup = xEventGroupCreate();
	vTraceSetEventGroupName( _0_Comm_EventGroup, "_0_Comm_EventGroup");

	//Init de l'event Group lié au status des cartes sur le bus
	_0_Status_EventGroup = xEventGroupCreate();
	vTraceSetEventGroupName( _0_Status_EventGroup, "_0_Status_EventGroup");

	//Init de l'event Group lié aux ACK de communication
	_0_ACK_Type_EventGroup = xEventGroupCreate();
		vTraceSetEventGroupName( _0_ACK_Type_EventGroup, "_0_ACK_Type_EventGroup");

	//Init de l'event Group lié aux adresses des cartes ayant renvoyé un ACK
		_0_ACK_Adresses = xEventGroupCreate();
			vTraceSetEventGroupName( _0_ACK_Adresses, "_0_ACK_Adresses");
}

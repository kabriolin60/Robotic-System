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
}

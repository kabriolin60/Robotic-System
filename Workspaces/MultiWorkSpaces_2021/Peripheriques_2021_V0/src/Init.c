/*
 * Init.c
 *
 *  Created on: 27 déc. 2020
 *      Author: kabri
 */
#include "Init.h"
//#include "0_Communication.h"
//#include "0_Event_Group.h"

void Init_Carte_Perpheriques(void)
{
	/* Init du Groupe d'event de synchronisation */
	_0_Communication_Init_Event_Group();

	/* Init de la communication Niveau 0 */
	_0_Communication_Init();
}

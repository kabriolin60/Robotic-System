/*
 * Strategie_Communication_Typedef.h
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#ifndef STRATEGIE_COMMUNICATION_TYPEDEF_H_
#define STRATEGIE_COMMUNICATION_TYPEDEF_H_

#include "Strategie_Actions.h"




struct Communication_Action_Datas
{
	byte ID;

	byte Name_Length;

	char Name[Max_Action_Name_Length];

	enum Action_State State;

	byte Step;

	byte Priority;

	byte Points;

	enum Qui Qui_Fait;

	enum Qui Qui_Peut;

	unsigned short Temps_mini;

	unsigned short Temps_maxi;
};



#endif /* STRATEGIE_COMMUNICATION_TYPEDEF_H_ */

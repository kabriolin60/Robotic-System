/*
 * Strategie_Actions.h
 *
 *  Created on: 28 nov. 2020
 *      Author: kabri
 */

#ifndef STRATEGIE_LEVEL0_STRATEGIE_ACTIONS_H_
#define STRATEGIE_LEVEL0_STRATEGIE_ACTIONS_H_

#include "Configuration.h"
#include "Communication_Typedef.h"


enum Action_State
{
	En_Attente,
	En_cours,
	Terminee,
	Annulee,
	Impossible
};

enum Qui
{
	Action_Personne,
	Action_Gros_Robot,
	Action_Petit_Robot,
	Action_Deux_Robots
};

#define Max_Action_Name_Length	40


struct Action_Datas
{
	byte ID;							//ID de l'action

	byte Name_Length;					//Longueur du nom

	char Name[Max_Action_Name_Length];	//Nom

	enum Action_State State;			//Status

	byte Step;							//Avancement

	byte Priority;						//Priorité

	byte Points;						//Nombre de points rapportés par cette action

	enum Qui Qui_Fait;					//Qui réalise cette action

	enum Qui Qui_Peut;					//Qui peut réaliser cette action

	ushort Temps_mini;					//Tps mini pour débuter l'action

	ushort Temps_maxi;					//Tps maxi pour débuter l'action

	void (*Fct)(byte);					//Pointeur vers l'action
	void *Param;						//Pointeur vers un parametre utilise par l'action

	ushort StartPoint_X;				//Coordonnées du point de départ de l'action sur X
	ushort StartPoint_Y;				//Coordonnées du point de départ de l'action sur Y
};

#endif /* STRATEGIE_LEVEL0_STRATEGIE_ACTIONS_H_ */

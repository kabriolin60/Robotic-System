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
	Action_En_Attente,
	Action_En_cours,
	Action_Terminee,
	Action_Annulee,
	Action_Impossible
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

	char* Name;							//Nom

	enum Action_State State;			//Status

	byte Step;							//Avancement

	byte Priority;						//Priorité

	byte Points;						//Nombre de points rapportés par cette action

	enum Qui Qui_Fait;					//Qui réalise cette action

	enum Qui Qui_Peut;					//Qui peut réaliser cette action

	long Temps_mini;					//Tps mini pour débuter l'action

	long Temps_maxi;					//Tps maxi pour débuter l'action

	void (*Fct)(byte);					//Pointeur vers l'action
	void *Param;						//Pointeur vers un parametre utilise par l'action

	ushort StartPoint_X;				//Coordonnées du point de départ de l'action sur X
	ushort StartPoint_Y;				//Coordonnées du point de départ de l'action sur Y
};


struct Actions_List
{
	struct Action_Datas Actions[16];
	byte Nombre_Actions;
};



void _Strategie_Change_Action_State(struct Action_Datas* action_to_change, enum Action_State new_state, char* commentaire);

void _Strategie_Change_Action_Step(struct Action_Datas* action_to_change, byte new_step, char* commentaire);

#endif /* STRATEGIE_LEVEL0_STRATEGIE_ACTIONS_H_ */

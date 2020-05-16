/*
 * 1_Odometrie.c
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#include "0_Codeurs.h"
#include "1_Odometrie.h"

#include <cr_section_macros.h>

static struct st_ROBOT_PARAMETRES _1_Odometrie_Parameter;                // structure des parametres du robot

static struct st_POSITION_ROBOT _1_Odometrie_Position_Robot;             // structure contenant la position du Robot

static struct st_Codeurs _1_Odometrie_Codeurs;                           // structure contenant les valeurs des codeurs


/*****************************************************************************
 ** Function name:		_1_Odometrie_Init
 **
 ** Descriptions:		Fonction d'initialisation
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Odometrie_Init(void)
{
	(void)memset(&_1_Odometrie_Parameter, 0, sizeof(struct st_ROBOT_PARAMETRES));
	(void)memset(&_1_Odometrie_Position_Robot, 0, sizeof(struct st_POSITION_ROBOT));
	(void)memset(&_1_Odometrie_Codeurs, 0, sizeof(struct st_Codeurs));

	/* Init Task Odometrie */
	xTaskCreate(vTask_1_Interpolation_lineaire, (char *) "vTask_Odometrie",	80, NULL, (tskIDLE_PRIORITY + 4UL), (xTaskHandle *) NULL);
}


/*****************************************************************************
 ** Function name:		vTask_1_Interpolation_lineaire
 **
 ** Descriptions:		Fonction d'interpolation lineaire pour la mise a jour de la position du robot sur la table
 **
 ** parameters:			pointeur sur la structure contenant les parametres du robot
 ** Returned value:		None
 **
 *****************************************************************************/
void vTask_1_Interpolation_lineaire(void *pvParameters)
{
	Init_Timing_Tache;
	struct Codeurs_Values Valeurs_Codeurs;

	for(;;)
	{
		switch(_1_Odometrie_Parameter._1_Odometrie_Type_Asserv)
		{
		default:
		case Polaire_Tourne_Avance_point_unique:
		case Vitesse_Droite_Vitesse_Gauche_Indep:
		case Vitesse_D_G__Distance_Angle:
			//Va chercher la valeur des codeurs brutes
			Valeurs_Codeurs = _0_Codeurs_Get_CodeursValues();

			//Applique la correction sur les diamètres des roues
			_1_Odometrie_Codeurs.Codeur_Droit = Valeurs_Codeurs.Codeur_Droit * _1_Odometrie_Codeurs.Coef_Correction_codeur_Droit;
			_1_Odometrie_Codeurs.Codeur_Gauche = Valeurs_Codeurs.Codeur_Gauche* _1_Odometrie_Codeurs.Coef_Correction_codeur_Gauche;

			__Interpolation_lineaire_RCVA2(&_1_Odometrie_Position_Robot, &_1_Odometrie_Codeurs, &_1_Odometrie_Parameter);
			break;


			//cas spé des moteurs aux
		case Moteurs_Aux:
			break;
		}

		Task_Delay_Until(PERIODE_MISE_A_JOUR_POSITION);
	}
}


/**************************************************
Fonction d'interpolation lineaire pour la mise a jour de la position du robot sur la table version RCVA
Input: pointeur sur la structure contenant la position du robot
       pointeur sur la structure contenant les valeurs des codeurs
       pointeur sur la structure contenant les parametres du robot
Output: None
 **************************************************/

//#define CONV_RD (1 / Robot.ROBOT.COEF_ROT)
#define LINEAIRE 1
#define CIRCULAIRE 2
#define ALGO_UTILISE CIRCULAIRE
#define CORFUGE 0  //0.0002F

__attribute__((optimize("O0"))) void __Interpolation_lineaire_RCVA2(struct st_POSITION_ROBOT *Position, struct st_Codeurs *Codeurs, struct st_ROBOT_PARAMETRES *r)
{
	/********* Definition des variables globales *********/

	static long gauche = 0, droite = 0, distance = 0, distance_precedente = 0; // variables entieres sur32 bits
	static long orient = 0, orient_precedente = 0;

	long delta_d, delta_orient;
	long orient_moy ;
	float orient_moy_radian, delta_orient_radian;
	float K, dx,dy;
	float Derive_x, Derive_y;

	droite = Codeurs->Codeur_Droit;
	gauche = Codeurs->Codeur_Gauche;


	// Correspondance avec notation du document theorique
	distance = (droite + gauche)/2 ; // distance en pas parcourue a tn
	orient = Position->orient_init + (droite - gauche) ; //correspond a qn mais en pas
	delta_d = distance - distance_precedente; // correspond a L mais en pas
	delta_orient = orient -  orient_precedente ; // correspond a Dqn mais en pas
	orient_moy = (orient + orient_precedente)/2 ; // correspond a qmoy en pas
	delta_orient_radian = delta_orient / r->COEF_ROT; // correspond a Dqn en rd
	orient_moy_radian = orient_moy /r->COEF_ROT; // correspond a qmoy en rd

	if (ALGO_UTILISE == LINEAIRE)
		K = 1;
	else // Choix de l'algo circulaire
	{
		if (delta_orient == 0) // Pour eviter la division par zero
			K = 1;
		else
			K =( sinf(delta_orient_radian /2)) / (delta_orient_radian /2);
	}

	dx = K * delta_d * cosf(orient_moy_radian);
	dy = K * delta_d * sinf(orient_moy_radian);

	Derive_x =  CORFUGE * delta_orient * dy;
	Derive_y = -CORFUGE * delta_orient * dx;

	dx += Derive_x;
	dy += Derive_y;

	//MAJ de la Structure contenant la position reelle du Robot
	Position->Position_X += dx / r->COEF_D;
	Position->Position_Y += dy / r->COEF_D;

	Position->Angle_rad = orient_moy_radian;

	while(Position->Angle_rad > PI) Position->Angle_rad -= 2*PI;
	while(Position->Angle_rad < -PI) Position->Angle_rad += 2*PI;

	Position->Angle_Deg = Convert_Rad_Deg(Position->Angle_rad);

	orient_precedente = orient; // actualisation de qn-1
	distance_precedente = distance; //actualisation de Dn-1
}


/**************************************************
Fonction de réglage des parametres d'odometrie
Input:  pointeur sur la structure contenant les parametres du robot
Output: None
 **************************************************/
void _1_Odometrie_Set_Parameters(struct st_ROBOT_PARAMETRES *new_parameters)
{
	_1_Odometrie_Parameter = *new_parameters;

	//Mise à jour des parametres de deplacement du Robot
	_1_Odometrie_Codeurs.Coef_Correction_codeur_Droit = 1 + _1_Odometrie_Parameter.COEF_CORRECTION_DIAMETRES / 2;
	_1_Odometrie_Codeurs.Coef_Correction_codeur_Gauche = 1 - _1_Odometrie_Parameter.COEF_CORRECTION_DIAMETRES / 2;
}


/**************************************************
Fonction de réglage des parametres d'odometrie
Input:  None
Output: None
 **************************************************/
struct st_ROBOT_PARAMETRES* _1_Odometrie_Get_Parameters(void)
{
	return &_1_Odometrie_Parameter;
}


/**************************************************
Fonction de lecture de la position actuelle
Input:  None
Output: Structure de position du Robot
 **************************************************/
struct st_POSITION_ROBOT _1_Odometrie_GetRobot_Position(void)
{
	return _1_Odometrie_Position_Robot;
}


/**************************************************
Fonction de Réglage de la position actuelle
Input:  Pointeur Structure de position du Robot
Output: None
 **************************************************/
void _1_Odometrie_SetRobot_Position(struct st_POSITION_ROBOT *newposition)
{
	_1_Odometrie_Position_Robot = *newposition;

	_1_Odometrie_Position_Robot.orient_init = _1_Odometrie_Position_Robot.Angle_rad * _1_Odometrie_Parameter.COEF_ROT - (_1_Odometrie_Codeurs.Codeur_Droit - _1_Odometrie_Codeurs.Codeur_Gauche);
}


/**************************************************
Fonction de passage en mode simulation ou non
Input:  Oui/Non
Output: None
 **************************************************/
void _1_Omodetrie_Set_Simulation(bool value)
{
	_1_Odometrie_Parameter.SIMULATION = value;
}


/**************************************************
Fonction de lecture du mode simulation ou non
Input:  None
Output: Oui/Non
 **************************************************/
bool _1_Omodetrie_Get_Simulation(void)
{
	return _1_Odometrie_Parameter.SIMULATION;
}




struct st_Codeurs * _1_Odometrie_Get_ptr_Codeurs_Corriges(void)
{
	return &_1_Odometrie_Codeurs;
}

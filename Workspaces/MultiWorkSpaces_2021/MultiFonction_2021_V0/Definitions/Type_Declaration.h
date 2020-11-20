/*
 * Type_Declaration.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef TYPE_DECLARATION_H_
#define TYPE_DECLARATION_H_

typedef uint8_t byte;

/**************************************************
Declaration des constantes mathematiques
**************************************************/

#define PI 	3.14159265358979F           //definition du nombre PI

/*************************************************/


/**************************************************
Declaration des macro mathematiques
**************************************************/

#define Convert_Deg_Rad(x)  x*PI/180
#define Convert_Rad_Deg(x)  x*180/PI

/*************************************************/


/**************************************************
Declaration des types d'asservissements possibles
 **************************************************/
enum _enum_Type_Asserv
{
	Polaire_Tourne_Avance_point_unique,		//Asserv avec boucle de vitesse en distance, vitesse en angle, distance, angle
	Vitesse_Droite_Vitesse_Gauche_Indep, 	//Uniquement un asserv en vitesse à droite et vitesse à Gauche
	Vitesse_D_G__Distance_Angle,			//Asserv en vitesse à D et à G, plus Asserv en distance et/ou angle (sans coordonnées)
	Moteurs_Aux,
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant la position du Robot à mettre à jour
 **************************************************/
struct st_POSITION_ROBOT
{
	float Position_X;
	float Position_Y;
	float Angle_rad;
	float Angle_Deg;

	float orient_init;
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant les caracteristiques du robot
 **************************************************/

struct st_ROBOT_PARAMETRES
{
	float COEF_ROT;
	float COEF_D;
	float COEF_CORRECTION_DIAMETRES;

	byte Coef_Multiplicateur_Periode_asserv;	//0 = 1ms, 1 = 2ms, 2 = 3ms...

	byte SIMULATION;		//Indique si les déplacements sont en simulation ou en réel

	enum _enum_Type_Asserv _1_Odometrie_Type_Asserv;		//Type d'asservissement
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant les valeurs des Codeurs
 **************************************************/
struct st_Codeurs
{
	long Codeur_Gauche;                                  //valeur lue du codeur gauche
	long Codeur_Droit;                                   //valeur lue du codeur droit

	int Codeur_Moteur_1;                                 //valeur lue du codeur du moteur 3
	int Codeur_Moteur_2;                                 //valeur lue du codeur du moteur 4

	float Coef_Correction_codeur_Gauche;                 //coef permetant de corriger des defauts de position/diametre des roues codeuses
	float Coef_Correction_codeur_Droit;                  //coef permetant de corriger des defauts de position/diametre des roues codeuses
};

/*************************************************/


#include "Communication_Typedef.h"

#endif /* TYPE_DECLARATION_H_ */

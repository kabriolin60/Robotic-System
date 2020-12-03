/*
 * Lib_includes.h
 *
 *  Created on: 2 déc. 2020
 *      Author: kabri
 */

#ifndef LIB_INCLUDES_H_
#define LIB_INCLUDES_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

/* Librairies */
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cr_section_macros.h>
#include <math.h>

/* Includes FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "ring_buffer.h"

/* Includes déclarations communes */
#include "Common_Init.h"
#include "Commun_Configuration.h"
#include "Communication_Structures.h"

/* Includes sources communes */
#include "0_Communication.h"
#include "0_Event_Group.h"
#include "1_Trame_Communication.h"

/* Includes drivers */
#include "0_RS485.h"

/*#include "1_Odometrie.h"
#include "1_PID.h"
#include "1_Asservissement.h"
#include "2_Asservissement.h"
#include "2_Spline.h"
#include "3_Asservissement.h"*/


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
Declaration des enum contenant les types de déplacements
 **************************************************/

enum enum_TYPE_MOVE_
{
	aucun_mouvement,             //Aucun mouvement
	xy_tour_av_avant,            //Déplacement standard en marche avant
	xy_tour_av_arriere,          //Déplacement standard en marche arriere
	tourne_vers_cap_rad,         //Oriente le robot selon l'angle donné en absolu
	deplacement_libre,           //Permet de faire des tests en déplacement linéaires
	rotation_libre,              //Permet de faire des tests en rotaion
};

/*************************************************/


/**************************************************
Declaration des enum contenant les parametres de déplacement
 **************************************************/

enum enum_TYPE_ARRET_
{
	depla_AVEC_freinage,	//Le robot freine et s'arret au point cible
	depla_SANS_freinage	//le robot ne freine pas et ne s'arrete pas au point cible
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant les parametres pour les déplacements
 **************************************************/

struct st_Parametre_Deplacement
{
	uint16_t Distance_Detection_Fin_Trajectoire;           //distance separant le robot de son objectif avant passage au point suivant * 100
	uint8_t Angle_Detection_Fin_Trajectoire;               //angle separant le robot de son objectif avant passage au point suivant pour les rotations * 100
	uint8_t Angle_Avant_Debut_Avance;                      //angle separant le robot de son objectif pour qu'il commence à avancer * 100
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant des coordonnees
 **************************************************/

struct st_COORDONNEES
{
	short X;                     //coordonnee X
	short Y;                     //coordonnee Y
	short Angle;                 //angle pour les rotations vers un cap absolu * 100
	short Distance;              //Indique la distance a parcourir pour les deplacements en distance libre, ou le rayon des cercles

	enum enum_TYPE_MOVE_ Type_Deplacement; 	//choix du type de deplacement

	enum enum_TYPE_ARRET_ Type_Arret;		//Type d'arret au point cible

	struct st_Parametre_Deplacement ptrParameters;
};

/*************************************************/

#endif /* LIB_INCLUDES_H_ */

/*
 * spline.h
 *
 *  Created on: 7 déc. 2012
 *      Author: Robotic System
 */

#ifndef SPLINE_H_
#define SPLINE_H_

#define SPLINE_MAX_POINTS 200

//Definition de la Taille de la Carte sur l'axe X
#define ASTAR_MAP_SIZE_X 2000

//Definition de la Taille de la Carte sur l'axe Y
#define ASTAR_MAP_SIZE_Y 3000

typedef struct {
	float x;
	float y;
} spline_point;


typedef struct {
	unsigned char Degre_Courbe;
	spline_point P[4];
	unsigned char Nombre_Points_Calcules;
	unsigned char Distance_Entre_Points;
	unsigned char AVANT_ARRIERE; //0= Marche avant, 1= Marche arriere
} Bezier;


/*********************************************
Fonction de convertion des coordonnees d'un point entre 0 et 1
Input:  NULL
Output: NULL
 *********************************************/

void Spline_Convert_Point(spline_point *p);

/********************************************/


/*********************************************
Fonction de calcul des points de passage d'une courbe de bezier
Input:  Pointeur vers la structure contenant les parametres de la courbe à calculer
Output: 1: OK
		0: Erreur
 *********************************************/

void SPLINE_MARCHE_AVANT(Bezier *b);

/********************************************/


/*********************************************
Fonction de calcul des points de passage d'une courbe de bezier
Input:  Pointeur vers la structure contenant les parametres de la courbe à calculer
Output: 1: OK
		0: Erreur
 *********************************************/

void SPLINE_MARCHE_ARRIERE(Bezier *b);

/********************************************/


void CubicSpline_Process(struct CubicSpline *spline);

float CubicSpline_Point_Processing(float p0, float m0, float p1, float m1, float t);


#endif /* SPLINE_H_ */

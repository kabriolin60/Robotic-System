#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "2_Asservissement.h"
#include "2_Spline.h"


/*********************************************
Fonction de convertion des coordonnees d'un point entre 0 et 1
Input:  Pointeur vers le point à convertir
Output: NULL
 *********************************************/

void Spline_Convert_Point(spline_point *p)
{
	p->x /= ASTAR_MAP_SIZE_X;
	p->y /= ASTAR_MAP_SIZE_Y;
}

/********************************************/


/*********************************************
Fonction de calcul des points de passage d'une courbe de bezier
Input:  Pointeur vers la structure contenant les parametres de la courbe à calculer
Output: 1: OK
		0: Erreur
 *********************************************/

void SPLINE_MARCHE_AVANT(Bezier *b)
{
	float Distante_Totale = 0;
	float index_point = 0;
	float Nb_Points = 0;
	float Fraction_Point = 0;

	spline_point Solution;

	//Structure locale contenant les points calcules
	struct st_COORDONNEES dest = { 0 };

	//Calcul la somme des longueurs des tronçons de la courbe
	for(index_point = 0; index_point < b->Degre_Courbe - 1; index_point++)
	{
		Distante_Totale += Calcul_Distance_2_points_Simple(b->P[(unsigned char)index_point].x, b->P[(unsigned char)index_point].y, b->P[(unsigned char)index_point + 1].x, b->P[(unsigned char)index_point + 1].y);
	}

	//Calcul du nombre de points à effectuer sur la courbe
	Nb_Points = (unsigned short)(Distante_Totale / b->Distance_Entre_Points);

	//Si nombre de points > SPLINE_MAX_POINTS, limite à SPLINE_MAX_POINTS points
	if(Nb_Points > SPLINE_MAX_POINTS) Nb_Points = SPLINE_MAX_POINTS;

	//Convertion des points de parametres de la courbe avec des coordonnées entre 0 et 1
	for(index_point = 0; index_point < b->Degre_Courbe; index_point++)
	{
		Spline_Convert_Point(&b->P[(unsigned char)index_point]);
	}

	dest.Type_Deplacement = TYPE_MOVE_xy_tour_av_avant;

	b->Nombre_Points_Calcules = Nb_Points;

	//Calcul des Coordonnées des points de la Courbe
	//Pour chaque point de la Courbe
	if(b->Degre_Courbe == 3)
	{
		for(index_point = 0; index_point < Nb_Points - 1; index_point++)
		{
			Fraction_Point = index_point / Nb_Points;

			Solution.x = b->P[0].x * pow((1 - Fraction_Point), 2);
			Solution.x += 2.0F * b->P[1].x * Fraction_Point * (1 - Fraction_Point);
			Solution.x += b->P[2].x * pow(Fraction_Point, 2);

			Solution.x *= ASTAR_MAP_SIZE_X;

			Solution.y = b->P[0].y * pow((1 - Fraction_Point), 2);
			Solution.y += 2.0F * b->P[1].y * Fraction_Point * (1 - Fraction_Point);
			Solution.y += b->P[2].y * pow(Fraction_Point, 2);

			Solution.y *= ASTAR_MAP_SIZE_Y;

			dest.X = (short)(Solution.x);
			dest.Y = (short)(Solution.y);

			//Envoie des points intermediaires, sans attente, qui ne sera activée si besoin que sur le dernier point du trajer
			//xQueueSend(xQueue_TRAJECTOIRE, &dest, ( TickType_t ) portMAX_DELAY );
		}
	}

	if(b->Degre_Courbe == 4)
	{
		for(index_point = 0; index_point < Nb_Points - 1; index_point++)
		{
			Fraction_Point = index_point / Nb_Points;

			Solution.x = b->P[0].x * pow((1 - Fraction_Point), 3);
			Solution.x += 3.0F * b->P[1].x * Fraction_Point * pow((1 - Fraction_Point), 2);
			Solution.x += 3.0F * b->P[2].x * pow(Fraction_Point, 2) * (1 - Fraction_Point);
			Solution.x += b->P[3].x * pow(Fraction_Point, 3);

			Solution.x *= ASTAR_MAP_SIZE_X;

			Solution.y = b->P[0].y * pow((1 - Fraction_Point), 3);
			Solution.y += 3.0F * b->P[1].y * Fraction_Point * pow((1 - Fraction_Point), 2);
			Solution.y += 3.0F * b->P[2].y * pow(Fraction_Point, 2) * (1 - Fraction_Point);
			Solution.y += b->P[3].y * pow(Fraction_Point, 3);

			Solution.y *= ASTAR_MAP_SIZE_Y;

			dest.X = (short)(Solution.x);
			dest.Y = (short)(Solution.y);

			//Envoie des points intermediaires, sans attente, qui ne sera activée si besoin que sur le dernier point du trajer
			//xQueueSend(xQueue_TRAJECTOIRE, &dest, ( TickType_t ) portMAX_DELAY );
		}

		//Envoie du dernier points, sans attente, qui ne sera activée si besoin que sur le dernier point du trajet
		dest.X = (short)(b->P[3].x * ASTAR_MAP_SIZE_X);
		dest.Y = (short)(b->P[3].y * ASTAR_MAP_SIZE_Y);

		//xQueueSend(xQueue_TRAJECTOIRE, &dest, ( TickType_t ) portMAX_DELAY );
	}
}

/********************************************/


/*********************************************
Fonction de calcul des points de passage d'une courbe de bezier
Input:  Pointeur vers la structure contenant les parametres de la courbe à calculer
Output: 1: OK
		0: Erreur
 *********************************************/

void SPLINE_MARCHE_ARRIERE(Bezier *b)
{
	float Distante_Totale = 0;
	float index_point = 0;
	float Nb_Points = 0;
	float Fraction_Point = 0;

	spline_point Solution;

	//Sructure locale contenant les points calcules
	struct st_COORDONNEES dest = { 0 };

	//Calcul la somme des longueurs des tronçons de la courbe
	for(index_point = 0; index_point < b->Degre_Courbe - 1; index_point++)
	{
		Distante_Totale += Calcul_Distance_2_points_Simple(b->P[(unsigned char)index_point].x, b->P[(unsigned char)index_point].y, b->P[(unsigned char)index_point + 1].x, b->P[(unsigned char)index_point + 1].y);
	}

	//Calcul du nombre de points à effectuer sur la courbe
	Nb_Points = (unsigned short)(Distante_Totale / b->Distance_Entre_Points);

	//Si nombre de points > SPLINE_MAX_POINTS, limite à SPLINE_MAX_POINTS points
	if(Nb_Points > SPLINE_MAX_POINTS) Nb_Points = SPLINE_MAX_POINTS;

	//Convertion des points de parametres de la courbe avec des coordonnées entre 0 et 1
	for(index_point = 0; index_point < b->Degre_Courbe; index_point++)
	{
		Spline_Convert_Point(&b->P[(unsigned char)index_point]);
	}

	dest.Type_Deplacement = TYPE_MOVE_xy_tour_av_arriere;

	b->Nombre_Points_Calcules = Nb_Points;

	//Calcul des Coordonnées des points de la Courbe
	//Pour chaque point de la Courbe
	if(b->Degre_Courbe == 3)
	{
		for(index_point = 0; index_point < Nb_Points - 1; index_point++)
		{
			Fraction_Point = index_point / Nb_Points;

			Solution.x = b->P[0].x * pow((1 - Fraction_Point), 2);
			Solution.x += 2.0F * b->P[1].x * Fraction_Point * (1 - Fraction_Point);
			Solution.x += b->P[2].x * pow(Fraction_Point, 2);

			Solution.x *= ASTAR_MAP_SIZE_X;

			Solution.y = b->P[0].y * pow((1 - Fraction_Point), 2);
			Solution.y += 2.0F * b->P[1].y * Fraction_Point * (1 - Fraction_Point);
			Solution.y += b->P[2].y * pow(Fraction_Point, 2);

			Solution.y *= ASTAR_MAP_SIZE_Y;

			dest.X = (short)(Solution.x);
			dest.Y = (short)(Solution.y);

			//Envoie des points intermediaires, sans attente, qui ne sera activée si besoin que sur le dernier point du trajer
			//xQueueSend(xQueue_TRAJECTOIRE, &dest, ( TickType_t ) portMAX_DELAY );
		}
	}

	if(b->Degre_Courbe == 4)
	{
		for(index_point = 0; index_point < Nb_Points - 1; index_point++)
		{
			Fraction_Point = index_point / Nb_Points;

			Solution.x = b->P[0].x * pow((1 - Fraction_Point), 3);
			Solution.x += 3.0F * b->P[1].x * Fraction_Point * pow((1 - Fraction_Point), 2);
			Solution.x += 3.0F * b->P[2].x * pow(Fraction_Point, 2) * (1 - Fraction_Point);
			Solution.x += b->P[3].x * pow(Fraction_Point, 3);

			Solution.x *= ASTAR_MAP_SIZE_X;

			Solution.y = b->P[0].y * pow((1 - Fraction_Point), 3);
			Solution.y += 3.0F * b->P[1].y * Fraction_Point * pow((1 - Fraction_Point), 2);
			Solution.y += 3.0F * b->P[2].y * pow(Fraction_Point, 2) * (1 - Fraction_Point);
			Solution.y += b->P[3].y * pow(Fraction_Point, 3);

			Solution.y *= ASTAR_MAP_SIZE_Y;

			dest.X = (short)(Solution.x);
			dest.Y = (short)(Solution.y);

			//Envoie des points intermediaires, sans attente, qui ne sera activée si besoin que sur le dernier point du trajer
			//xQueueSend(xQueue_TRAJECTOIRE, &dest, ( TickType_t ) portMAX_DELAY );
		}

		//Envoie du dernier points, sans attente, qui ne sera activée si besoin que sur le dernier point du trajet
		dest.X = (short)(b->P[3].x * ASTAR_MAP_SIZE_X);
		dest.Y = (short)(b->P[3].y * ASTAR_MAP_SIZE_Y);

		//xQueueSend(xQueue_TRAJECTOIRE, &dest, ( TickType_t ) portMAX_DELAY );
	}
}

/********************************************/



/*********************************************
Fonction de calcul des points de passage d'une spline cubique
Input:  Pointeur vers la structure contenant les parametres de la spline cubique
Output: 1: OK
		0: Erreur
 *********************************************/
/*void CubicSpline_Receive(struct Communication_Trame *received)
{
	struct CubicSpline spline;
	COPYDATA2(received->Data, spline);

	CubicSpline_Process(&spline);
}*/




struct Astar_smoothing_vector_multiFCT vectors_spline;


void CubicSpline_Process(struct CubicSpline *spline)
{
	float t = 0;
	float Result_X = 0, Result_Y = 0;

	float P0_x, P0_y,  M0_x, M0_y;
	float P1_x, P1_y,  M1_x, M1_y;

	//Structure locale contenant les points calcules
	struct st_COORDONNEES dest = { 0 };



	//Conversion des points de passage entre 0 et 1
	P0_x = spline->P0.X ;
	P0_x /= spline->Taille_Terrain.X;
	P0_y = spline->P0.Y;
	P0_y /= spline->Taille_Terrain.Y;

	//Tangente initiale
	//Utilisation des valeurs de M0 fournies par l’IA
	M0_x = spline->M0.X;
	M0_x /= spline->Taille_Terrain.X;
	M0_y = spline->M0.Y;
	M0_y /= spline->Taille_Terrain.Y;


	//Point d’arrivée
	P1_x = spline->P1.X ;
	P1_x /= spline->Taille_Terrain.X;
	P1_y = spline->P1.Y;
	P1_y /= spline->Taille_Terrain.Y;

	//Tangente d’arrivée
	M1_y = spline->M1.Y ;
	M1_y /= spline->Taille_Terrain.Y;
	M1_x = spline->M1.X ;
	M1_x /= spline->Taille_Terrain.X;

	float pas = spline->Nombre_Points;
	pas = 1/pas;

	if(spline->Direction == 0)
	{
		dest.Type_Deplacement = TYPE_MOVE_Spline_avant;
	}else
	{
		dest.Type_Deplacement = TYPE_MOVE_Spline_arriere;
	}

	struct st_Parametre_Deplacement parameters2;
	parameters2.Angle_Avant_Debut_Avance = spline->ptrParameters.Angle_Avant_Debut_Avance;
	parameters2.Distance_Detection_Fin_Trajectoire = spline->ptrParameters.Distance_Detection_Fin_Trajectoire;

	dest.ptrParameters = parameters2;

	dest.Final_X = spline->P1.X;
	dest.Final_Y = spline->P1.Y;

	vectors_spline.Nb_Vectors = 0;
	vectors_spline.Vectors[vectors_spline.Nb_Vectors].Start_Point.x = P0_x*spline->Taille_Terrain.X;
	vectors_spline.Vectors[vectors_spline.Nb_Vectors].Start_Point.y = P0_y*spline->Taille_Terrain.Y;

	dest.Type_Arret = depla_SANS_freinage;
	//Pour chaque point de la trajectoire à calculer
	for(t = pas ; t < 1 ; t+= pas)
	{
		Result_X = CubicSpline_Point_Processing(P0_x, M0_x, P1_x, M1_x, t);
		Result_X *= spline->Taille_Terrain.X;

		Result_Y = CubicSpline_Point_Processing(P0_y, M0_y, P1_y, M1_y, t);
		Result_Y *= spline->Taille_Terrain.Y;

		//Envoie des points calculés de la trajectoire vers l’objectif
		dest.X = (short)Result_X;
		dest.Y = (short)Result_Y;
		_2_Deplacement_Ajout_Point(&dest);


		vectors_spline.Vectors[vectors_spline.Nb_Vectors].End_Point.x = dest.X;
		vectors_spline.Vectors[vectors_spline.Nb_Vectors].End_Point.y = dest.Y;

		vectors_spline.Vectors[vectors_spline.Nb_Vectors].Color = Astar_Vector_Color_Green_multiFCT;

		vectors_spline.Nb_Vectors++;

		vectors_spline.Vectors[vectors_spline.Nb_Vectors].Start_Point.x = dest.X;
		vectors_spline.Vectors[vectors_spline.Nb_Vectors].Start_Point.y = dest.Y;
	}

	t = 1;
	{
		//Dernier point avec Freinage
		dest.Type_Arret = depla_AVEC_freinage;

		Result_X = CubicSpline_Point_Processing(P0_x, M0_x, P1_x, M1_x, t);
		Result_X *= spline->Taille_Terrain.X;

		Result_Y = CubicSpline_Point_Processing(P0_y, M0_y, P1_y, M1_y, t);
		Result_Y *= spline->Taille_Terrain.Y;

		//Envoie des points calculés de la trajectoire vers l’objectif
		dest.X = (short)Result_X;
		dest.Y = (short)Result_Y;
		_2_Deplacement_Ajout_Point(&dest);

		vectors_spline.Vectors[vectors_spline.Nb_Vectors].Start_Point.x = dest.X;
		vectors_spline.Vectors[vectors_spline.Nb_Vectors].Start_Point.y = dest.Y;
	}
}


float CubicSpline_Point_Processing(float p0, float m0, float p1, float m1, float t)
{
	float result = 0;

	result = p0*(1-t)*(1-t)*(1-t);
	result += 3*m0*t*(1-t)*(1-t);
	result += 3*m1*t*t*(1-t);
	result += p1*t*t*t;

	return result;

	result = p0*(2*t*t*t-3*t*t+1);
	result += m0*(t*t*t-2*t*t+t);
	result += p1*(-2*t*t*t+3*t*t);
	result += m1*(t*t*t-t*t);

	return result;
}


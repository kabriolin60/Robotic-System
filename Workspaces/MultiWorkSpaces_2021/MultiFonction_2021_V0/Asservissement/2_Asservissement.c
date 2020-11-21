/*
 * 2_Asservissement.c
 *
 *  Created on: 30 avr. 2020
 *      Author: kabri
 */

#include "0_Codeurs.h"
#include "2_Asservissement.h"
#include "1_Asservissement.h"
#include "1_PID.h"
#include "1_Odometrie.h"
#include "Init.h"


//Structures des PID en distance et en Angle
static struct st_pid_filter PID_Position;
static struct st_pid_filter PID_Rotation;

//Structure des Deplacement pour rampes en trapezes
static struct st_DEPLACEMENT Deplacement_Distance;
static struct st_DEPLACEMENT Deplacement_Rotation;


//Handler sur la tâche d'asserv en position et en Rotation
static TaskHandle_t xHandle_Asserv_Position_Rotation;


/* Ring buffer de deplacement*/
static RINGBUFF_T rb_Deplacement;
/* Receive buffer for deplacement*/
static struct st_COORDONNEES deplacement_buffer[_2_Asserv_Buffer_Depla_size];

//Stock ici les coordonnées de la destination actuelle
static struct st_COORDONNEES Current_Destination;


/*****************************************************************************
 ** Function name:		_2_Deplacement_Init
 **
 ** Descriptions:		Initialisation des deplacements
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Deplacement_Init(void)
{
	//Init à 0 des PID en position et rotation
	(void)memset(&PID_Position, 0, sizeof(struct st_pid_filter));
	(void)memset(&PID_Rotation, 0, sizeof(struct st_pid_filter));

	//Init à 0 la destination actuelle

	Deplacement_Distance.Enable = 1;
	Deplacement_Rotation.Enable = 1;


	(void)memset(&Current_Destination, 0, sizeof(struct st_COORDONNEES));

	/* Before using the ring buffers, initialize them using the ring buffer init function */
	RingBuffer_Init(&rb_Deplacement, deplacement_buffer, sizeof(struct st_COORDONNEES), _2_Asserv_Buffer_Depla_size);

	//Création de la tâche d'asservissement en position et en rotation
	xTaskCreate(_2_Asservissement_Distance_Angle, (char *) "vTask_Asserv_Pos_Rot",	100, NULL, (tskIDLE_PRIORITY + 3UL), (xTaskHandle *) &xHandle_Asserv_Position_Rotation);
}


/*****************************************************************************
 ** Function name:		_2_Enable_Asserv
 **
 ** Descriptions:		Active l'asservissement de niveau 2
 ** 					Utile pour le réglage des PIDs par exemple
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Enable_Asserv(void)
{
	vTaskResume(xHandle_Asserv_Position_Rotation);
}


/*****************************************************************************
 ** Function name:		_2_Disable_Asserv
 **
 ** Descriptions:		Desactive l'asservissement de niveau 2
 ** 					Utile pour le réglage des PIDs par exemple
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Disable_Asserv(void)
{
	vTaskSuspend(xHandle_Asserv_Position_Rotation);
}


/*****************************************************************************
 ** Function name:		_2_Get_prt_PID_Position
 **
 ** Descriptions:		get pointeurs des PID
 **
 ** parameters:			None
 ** Returned value:		pointeur vers le PID en Position
 **
 *****************************************************************************/
struct st_pid_filter* _2_Asserv_GetPtr_PID_Pos(void)
{
	return &PID_Position;
}


/*****************************************************************************
 ** Function name:		_2_Get_prt_PID_Rotation
 **
 ** Descriptions:		get pointeurs des PID
 **
 ** parameters:			None
 ** Returned value:		pointeur vers le PID en Rotation
 **
 *****************************************************************************/
struct st_pid_filter* _2_Asserv_GetPtr_PID_Rot(void)
{
	return &PID_Rotation;
}


/*****************************************************************************
 ** Function name:		_2_Deplacement_Ajout_Point
 **
 ** Descriptions:		Ajoute un point de destination
 **
 ** parameters:			pointeur vers la structure du point à ajouter
 ** Returned value:		1: ajout réussi
 ** 					0: buffer plein
 **
 *****************************************************************************/
int _2_Deplacement_Ajout_Point(struct st_COORDONNEES *point)
{
	return RingBuffer_Insert(&rb_Deplacement, point);
}


/**************************************************
Fonction d'effacement du buffer de destination
Input: 	None
Output: None
 **************************************************/
void _2_Asservissement_DestinationBuffer_Clear(void)
{
	RingBuffer_Clear(&rb_Deplacement);
	Current_Destination.Type_Deplacement = aucun_mouvement;
}


/*****************************************************************************
 ** Function name:		_2_Update_PID_Vitesse_Consignes
 **
 ** Descriptions:		Met à jour les consignes de vitesse en position et en rotation
 **
 ** parameters:			pointeur vers la structure du point à ajouter
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Update_PID_Vitesse_Consignes(long consigne_vit_distance, long consigne_vit_rotation)
{
	_1_Update_PID_Vitesse_Consigne_distance(consigne_vit_distance);
	_1_Update_PID_Vitesse_Consigne_rotation(consigne_vit_rotation);
}


/*****************************************************************************
 ** Function name:		_2_Asservissement_Set_Distance_Speed_Accel
 **
 ** Descriptions:		Reglages des vitesses et acceleration en distance
 **
 ** parameters:			vitesse, acceleration, decceleration
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Asservissement_Set_Distance_Speed_Accel(float speed, float accel, float deccel)
{
	Deplacement_Distance.Accel = accel;
	Deplacement_Distance.Deccel = deccel;
	Deplacement_Distance.Vmax = speed;
}


/*****************************************************************************
 ** Function name:		_2_Asservissement_Set_Rotation_Speed_Accel
 **
 ** Descriptions:		Reglages des vitesses et acceleration en rotation
 **
 ** parameters:			vitesse, acceleration, decceleration
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Asservissement_Set_Rotation_Speed_Accel(float speed, float accel, float deccel)
{
	Deplacement_Rotation.Accel = accel;
	Deplacement_Rotation.Deccel = deccel;
	Deplacement_Rotation.Vmax = speed;
}



/*****************************************************************************
 ** Function name:		_2_Asservissement_Set_Distance_Displacement_Consign
 **
 ** Descriptions:		Donne la consigne au trapeze en distance
 **
 ** parameters:			Consigne
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Asservissement_Set_Distance_Displacement_Consign(float consigne)
{
	Deplacement_Distance.Consigne = consigne;
}


/*****************************************************************************
 ** Function name:		_2_Asservissement_Set_Rotation_Displacement_Consign
 **
 ** Descriptions:		Donne la consigne au trapeze en rotation
 **
 ** parameters:			Consigne
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Asservissement_Set_Rotation_Displacement_Consign(float consigne)
{
	Deplacement_Rotation.Consigne = consigne;
}



/*****************************************************************************
 ** Function name:		_2_Asservissement_Distance_Angle
 **
 ** Descriptions:		Boucle d'asservissement en distance et en Angle
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _2_Asservissement_Distance_Angle(void *pvParameters)
{
	Task_Delay(5);
	Init_Timing_Tache;

	bool Arrive = false; //Indique si le Robot est arrive à destination (dans le cas d'un asservissement "Tourne-Avance"

	//Position actuelle du Robot
	struct st_POSITION_ROBOT Current_Robot_Position;

	for(;;)
	{
		Task_Delay_Until(PERIODE_PID_DISTANCE_ANGLE);

		switch(_1_Odometrie_Get_Parameters()->_1_Odometrie_Type_Asserv)
		{
		case Polaire_Tourne_Avance_point_unique:
			//Asservissement polaire sur des coordonnées

			//Récupère la position actuelle du Robot
			Current_Robot_Position = _1_Odometrie_GetRobot_Position();

			//Asservissement vers le point cible (Rotation puis deplacement)
			if(Current_Destination.Type_Deplacement == xy_tour_av_avant || Current_Destination.Type_Deplacement == xy_tour_av_arriere)
			{
				Arrive = _2_Asservissement_Rotation_Avance(&Current_Destination, &Current_Robot_Position, &Current_Destination.ptrParameters, _1_Odometrie_Get_Parameters());
			}

			//Si on est arrivé, lecture du prochain point de destination
			if(Arrive || Current_Destination.Type_Deplacement == aucun_mouvement)
			{
				//On est arrivée ou alors aucun mouvement n'est demandé, alors on s'arrete

				//Mise à jour des consignes en distance et en angle
				_2_Asservissement_Set_Distance_Displacement_Consign(0); // en pas
				_2_Asservissement_Set_Rotation_Displacement_Consign(0); // en pas

				Deplacement_Distance.Coef = 0;
				Deplacement_Rotation.Coef = 0;			

				//Gestion des accelerations et freinages
				Trapeze_Vitesse(&Deplacement_Distance, _2_Asserv_GetPtr_PID_Pos(), depla_AVEC_freinage);
				Trapeze_Vitesse(&Deplacement_Rotation, _2_Asserv_GetPtr_PID_Rot(), depla_AVEC_freinage);

				//Effectue le calcul des PID
				pid_do_filter(_2_Asserv_GetPtr_PID_Pos());
				pid_do_filter(_2_Asserv_GetPtr_PID_Rot());

				//La sortie de ces PIDs corresponds à des consignes en vitesses (distance et Rotation)

				//Met à jour les consignes des PID en vitesse position et rotation à partir des sorties des PID en position et rotation
				_2_Update_PID_Vitesse_Consignes(_2_Asserv_GetPtr_PID_Pos()->Commande, _2_Asserv_GetPtr_PID_Rot()->Commande);

				_2_Asservissement_Read_Next_Desti_Point(&rb_Deplacement, &Current_Destination);
			}
			break;

		case Vitesse_D_G__Distance_Angle:
			//Asservissement en distance et en Angle avec boucle de vitesse, sans notion de coordonnées
			_2_Asservissement_Distance_Angle_Avec_Vitesse();
			break;

		case Vitesse_Droite_Vitesse_Gauche_Indep:
			//Asservissement uniquement en vitesse Droite et Gauche indépendant
			//Pas d'asserv de niveau 2
			break;

		default:
			break;
		}
	}
}


/**************************************************
Fonction d'asservissement en distance en angle avec boucle de vitesse
Input: 	None
Output: 1: on est arrivés
		0: Le robot n'a pas encore atteint sa destination
 **************************************************/
void _2_Asservissement_Distance_Angle_Avec_Vitesse(void)
{
	struct st_Codeurs* Valeurs_Codeurs;
	Valeurs_Codeurs = (struct st_Codeurs*)_1_Odometrie_Get_ptr_Codeurs_Corriges();

	/*
	 * Step 1: Mise à jour des positions actuelles
	 */

	//Mise à jour des currents values des PID distances et Angles
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Pos(), (Valeurs_Codeurs->Codeur_Droit + Valeurs_Codeurs->Codeur_Gauche) >> 1);
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Rot(), (Valeurs_Codeurs->Codeur_Droit - Valeurs_Codeurs->Codeur_Gauche));

	/*
	 * Step 2: Calcule les rampes de vitesses
	 */
	Deplacement_Distance.Coef = 1;
	Deplacement_Rotation.Coef = 1;

	//Gestion des accelerations et freinages
	Trapeze_Vitesse(&Deplacement_Distance, _2_Asserv_GetPtr_PID_Pos(), depla_AVEC_freinage);
	Trapeze_Vitesse(&Deplacement_Rotation, _2_Asserv_GetPtr_PID_Rot(), depla_AVEC_freinage);

	/*
	 * Step 3: Asservissement sur les erreurs en distances et en angle
	 */

	//Effectue le calcul des PID
	pid_do_filter(_2_Asserv_GetPtr_PID_Pos());
	pid_do_filter(_2_Asserv_GetPtr_PID_Rot());

	//La sortie de ces PIDs corresponds à des consignes en vitesses pour le roue Droite et la roue Gauche
	_1_Update_PID_Consigne_Vitesse_Roue_Droite(_2_Asserv_GetPtr_PID_Pos()->Commande + _2_Asserv_GetPtr_PID_Rot()->Commande);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(_2_Asserv_GetPtr_PID_Pos()->Commande - _2_Asserv_GetPtr_PID_Rot()->Commande);
}



/**************************************************
Fonction d'execution de l'asservissement vers un point
Input: 	Pointeur sur la Structure contenant le point cible
		Pointeur sur la position actuelle du Robot
		Pointeur sur les parametres de deplacement
Output: 1: on est arrivés
		0: Le robot n'a pas encore atteint sa destination
 **************************************************/
bool _2_Asservissement_Rotation_Avance(struct st_COORDONNEES * destination, struct st_POSITION_ROBOT * Current_Robot_Position, struct st_Parametre_Deplacement * Param_Depla, struct st_ROBOT_PARAMETRES * Param_Odometrie)
{
	bool arrive = false;


	float Erreur_Distance = 0;
	float Erreur_Angle = 0;

	/*
	 * Step 0: calculer les erreurs entre la position du Robot et la cible
	 */

	//Calcul les erreurs entre la position actuelle du Robot et sa destination actuelle
	Erreur_Distance = Calcul_Distance_2_points_Simple(Current_Robot_Position->Position_X, Current_Robot_Position->Position_Y, destination->X, destination->Y);
	Erreur_Angle = Calcul_Angle_2_points_Simple(Current_Robot_Position->Position_X, Current_Robot_Position->Position_Y, destination->X, destination->Y) - Current_Robot_Position->Angle_rad;

	if(Erreur_Angle > PI ) Erreur_Angle -= 2*PI;
	else if( Erreur_Angle < - PI) Erreur_Angle += 2*PI;

	switch(destination->Type_Deplacement)
	{
	//Si marche arriere
	case xy_tour_av_arriere:
		//Modifie l'erreur pour faire faire demi-tour au robot
		if(Erreur_Angle > PI/2 || Erreur_Angle < -PI/2)
		{
			//Le point se trouve derrière nous
			Erreur_Distance = -Erreur_Distance;
		}
		Erreur_Angle += PI;
		if(Erreur_Angle > PI) Erreur_Angle -= 2*PI;
		if(Erreur_Angle < -PI) Erreur_Angle += 2*PI;

		break;

	case aucun_mouvement:
		//Mise à jour des consignes en distance et en angle
		_2_Asservissement_Set_Distance_Displacement_Consign(0); // en pas
		_2_Asservissement_Set_Rotation_Displacement_Consign(0); // en pas

		//Effectue le calcul des PID
		pid_do_filter(_2_Asserv_GetPtr_PID_Pos());
		pid_do_filter(_2_Asserv_GetPtr_PID_Rot());

		//La sortie de ces PIDs corresponds à des consignes en vitesses (distance et Rotation)

		//Met à jour les consignes des PID en vitesse position et rotation à partir des sorties des PID en position et rotation
		_2_Update_PID_Vitesse_Consignes(_2_Asserv_GetPtr_PID_Pos()->Commande, _2_Asserv_GetPtr_PID_Rot()->Commande);
		return false;

	default:
		break;
	}


	/*
	 * Step 1: vérifier si le robot peut être considére comme arrivé à destination
	 */

	//Si le robot est proche de sa destination
	if(fabsf(Erreur_Distance) < ((float)(Param_Depla->Distance_Detection_Fin_Trajectoire))/100)
	{
		//On est arrivés

		//La vitesse en rotation et en distance deviennent nulles pour ne pas sateliser
		//Deplacement_Distance.Coef = 0;
		Deplacement_Rotation.Coef = 0;
		arrive = true;

		//Mise à jour des consignes en distance et en angle
		//_2_Asservissement_Set_Distance_Displacement_Consign(0); // en pas
		_2_Asservissement_Set_Rotation_Displacement_Consign(0); // en pas

	}else if(fabsf(Erreur_Distance) < 3 * ((float)(Param_Depla->Distance_Detection_Fin_Trajectoire))/100)
	{
		/*
		 * Si l'erreur est faible et que le robot a depassé le point
		 */
		switch(destination->Type_Deplacement)
		{
		case xy_tour_av_avant:
			//Modifie l'erreur pour ne pas faire faire demi-tour au robot
			if(Erreur_Angle > PI/2 || Erreur_Angle < -PI/2)
			{
				//Le point se trouve derrière nous
				Erreur_Distance = -Erreur_Distance;

				Erreur_Angle += PI;
				if(Erreur_Angle > PI) Erreur_Angle -= 2*PI;
				if(Erreur_Angle < -PI) Erreur_Angle += 2*PI;
			}
			break;

		case xy_tour_av_arriere:
			//Modifie l'erreur pour ne pas faire faire demi-tour au robot
			if(Erreur_Angle > PI/2 || Erreur_Angle < -PI/2)
			{
				//Le point se trouve devant nous
				Erreur_Distance = -Erreur_Distance;

				Erreur_Angle += PI;
				if(Erreur_Angle > PI) Erreur_Angle -= 2*PI;
				if(Erreur_Angle < -PI) Erreur_Angle += 2*PI;
			}
			break;

		default:
			break;
		}
		Erreur_Angle = 0;
	}else
	{
		arrive = false;
	}

	/*
	 * Step 1.5: Si le robot n'est pas arrivé, adapte les coef sur les vitesses pour lui permettre de commencer à s'orienter vers sa cible avant d'avancer
	 */

	if(!arrive)
	{
		//Le robot n'a pas encore atteint sa destination

		//Mise à jour des consignes en distance et en angle
		_2_Asservissement_Set_Distance_Displacement_Consign(Erreur_Distance * Param_Odometrie->COEF_D); // en pas
		_2_Asservissement_Set_Rotation_Displacement_Consign(Erreur_Angle * Param_Odometrie->COEF_ROT); // en pas

		//Calcule les coef sur les rampes de vitesses

		float Angle_Avant_Debut_Avance = Param_Depla->Angle_Avant_Debut_Avance;
		Angle_Avant_Debut_Avance /= 100;

		//ajustage de la vitesse d'avance en fonction de l'erreur en orientation
		if(fabsf(Erreur_Angle) <= Angle_Avant_Debut_Avance)
		{
			//Si l'erreur en angle est inférieur au seuil, le robot est considéré comme aligné avec sa cible, il peut alors avancer
			Deplacement_Distance.Coef = (Angle_Avant_Debut_Avance - fabsf(Erreur_Angle)) / Angle_Avant_Debut_Avance;
			Deplacement_Rotation.Coef = Erreur_Angle / Angle_Avant_Debut_Avance;

			if(Deplacement_Distance.Coef < 0) Deplacement_Distance.Coef = -Deplacement_Distance.Coef;
			if(Deplacement_Rotation.Coef < 0) Deplacement_Rotation.Coef = -Deplacement_Rotation.Coef;
		}else
		{
			//Le robot n'est pas aligné, seule la rotation est activée
			Deplacement_Distance.Coef = 0;
			Deplacement_Rotation.Coef = 1;
		}
	}


	/*
	 * Step 2: Calcule les rampes de vitesses
	 */

	//Dans ce mode d'asserv, les current Values des PID distance et angles doivent être à 0
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Pos(), 0);
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Rot(), 0);

	//Gestion des accelerations et freinages
	Trapeze_Vitesse(&Deplacement_Distance, _2_Asserv_GetPtr_PID_Pos(), destination->Type_Arret);
	Trapeze_Vitesse(&Deplacement_Rotation, _2_Asserv_GetPtr_PID_Rot(), destination->Type_Arret);

	/*
	 * Step 3: Asservissement sur les erreurs en distances et en angle
	 */

	//Effectue le calcul des PID
	pid_do_filter(_2_Asserv_GetPtr_PID_Pos());
	pid_do_filter(_2_Asserv_GetPtr_PID_Rot());

	//La sortie de ces PIDs corresponds à des consignes en vitesses (distance et Rotation)

	//Met à jour les consignes des PID en vitesse position et rotation à partir des sorties des PID en position et rotation
	_2_Update_PID_Vitesse_Consignes(_2_Asserv_GetPtr_PID_Pos()->Commande, _2_Asserv_GetPtr_PID_Rot()->Commande);

	return arrive;
}


/**************************************************
Fonction de lecture du prochain point de destination, si plusieurs points sont dispo dans le buffer
Input: 	Pointeur sur le buffer contenant les points de destination
		Pointeur sur la structure qui va contenir le resultat de la lecture (si possible)
Output: 1: un point a été pris dans le buffer
		0: aucun point disponible dans le buffer
 **************************************************/
int _2_Asservissement_Read_Next_Desti_Point(RINGBUFF_T * buffer, struct st_COORDONNEES * destination)
{
	return RingBuffer_Pop(buffer, destination);
}



/**************************************************
Fonction de lecture du prochain point de destination, si plusieurs points sont dispo dans le buffer
Input: 	None
Output: 1: un point a été pris dans le buffer
		0: aucun point disponible dans le buffer
 **************************************************/
int _2_Asservissement_Read_Next_Desti_Point_extern(void)
{
	return RingBuffer_Pop(&rb_Deplacement, &Current_Destination);
}



/**************************************************
Fonction d'execution et de calcul des vitesses en forme de trapeze
Input: Pointeur sur la Structure du depacement
       Pointeur sur la Structure du PID a qui faire suivre ce type de mouvement
Output: La vitesse resultante du trapeze
 **************************************************/
float Trapeze_Vitesse(struct st_DEPLACEMENT *D, struct st_pid_filter *PID, enum enum_TYPE_ARRET_ arret)
{
	if(D->Enable == 0)
		return 0;

	float Vaccel, Vdecel;

	//Calcul de vitesse en acceleration
	Vaccel = D->Vitesse + D->Accel;

	//Calcul de la vitesse max en comptant l'erreur et la decceleration
	if(arret == depla_AVEC_freinage)
	{
		Vdecel = sqrtf(2 * D->Deccel * fabsf(D->Consigne - PID->Current_Value));
	}else
	{
		//Si l'arret n'est pas demandé au point, on continue à vitesse max
		Vdecel = D->Vmax;
	}

	//Prends la valeur mini entre les 3 courbes
	D->Vitesse = minf(Vaccel, D->Vmax);
	D->Vitesse = minf(D->Vitesse, Vdecel);

	PID_update_Consign(PID, D->Consigne);
	PID_set_Max_Min(PID, D->Vitesse * D->Coef);

	return D->Vitesse * D->Coef;
}

/*************************************************/


/**************************************************
Fonction de calcul de la distance entre 2 points
Input: structures de coordonnees des 2 points
Output: Distance entre ces deux points (mm)
 **************************************************/

float Calcul_Distance_2_points(struct st_COORDONNEES Point1, struct st_COORDONNEES Point2)
{
	float DeltaX = 0, DeltaY = 0;

	DeltaX = Point2.X - Point1.X;
	DeltaX *= DeltaX;

	DeltaY = Point2.Y - Point1.Y;
	DeltaY *= DeltaY;

	return (float)sqrtf(DeltaX + DeltaY);
}

/*************************************************/


/**************************************************
Fonction de calcul de la distance entre 2 points
Input: Coordonnees des 2 points
Output: Distance entre ces deux points (mm)
 **************************************************/

float Calcul_Distance_2_points_Simple(float X0, float Y0, float X1, float Y1)
{
	float DeltaX = 0, DeltaY = 0;

	DeltaX = X1 - X0;
	DeltaX *= DeltaX;

	DeltaY = Y1 - Y0;
	DeltaY *= DeltaY;

	return (float)sqrtf(DeltaX + DeltaY);
}

/*************************************************/


/**************************************************
Fonction de calcul de l'angle entre 2 points
Input: structures de coordonnees des 2 points
Output: angle entre ces deux points    (rad)
 **************************************************/

float Calcul_Angle_2_points(struct st_COORDONNEES Point1, struct st_COORDONNEES Point2)
{
	float DeltaX = 0, DeltaY = 0;
	float Angle = 0;

	DeltaX = Point2.X - Point1.X;

	DeltaY = Point2.Y - Point1.Y;

	Angle = (float)atan2((double)DeltaY, (double)DeltaX);

	if(Angle > PI)Angle = -2*PI+Angle;
	else if(Angle < -PI) Angle = 2*PI+Angle;

	return Angle;
}

float Calcul_Angle_2_points_Simple(float X0, float Y0, float X1, float Y1)
{
	float DeltaX = 0, DeltaY = 0;
	float Angle = 0;

	DeltaX = X1 - X0;

	DeltaY = Y1 - Y0;

	Angle = (float)atan2((double)DeltaY, (double)DeltaX);

	if(Angle > PI)Angle = -2*PI+Angle;
	else if(Angle < -PI) Angle = 2*PI+Angle;

	return Angle;
}

/*************************************************/


float minf(float a, float b)
{
	if(a < b)
		return a;

	return b;
}

/*
 * 1_Asservissement.c
 *
 *  Created on: 25 avr. 2020
 *      Author: kabri
 */
#include "0_Codeurs.h"
#include "0_Motors.h"

#include "1_Odometrie.h"
#include "1_Asservissement.h"
#include "1_PID.h"

static struct st_pid_filter PID_Vitesse_Position;
static struct st_pid_filter PID_Vitesse_Rotation;


static struct st_pid_filter PID_Vitesse_Roue_Droite;
static struct st_pid_filter PID_Vitesse_Roue_Gauche;


/*****************************************************************************
 ** Function name:		_1_Asserv_Init_Asserv
 **
 ** Descriptions:		Init de l'asserv Niveau 1
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Asserv_Init_Asserv(void)
{
	//Pour l'asserv Polaire
	(void)memset(&PID_Vitesse_Position, 0, sizeof(struct st_pid_filter));
	(void)memset(&PID_Vitesse_Rotation, 0, sizeof(struct st_pid_filter));

	//Pour des asserv en vitesse séparés
	(void)memset(&PID_Vitesse_Roue_Droite, 0, sizeof(struct st_pid_filter));
	(void)memset(&PID_Vitesse_Roue_Gauche, 0, sizeof(struct st_pid_filter));

	/* Init Task Asserv en vitesse */
	xTaskCreate(_1_Asserv_Niveau_1, (char *) "vTask_Asserv_1",	120, NULL, (tskIDLE_PRIORITY + 3UL), (xTaskHandle *) NULL);
}


/*****************************************************************************
 ** Function name:		_1_Get_prt_PID_Vit_Position
 **
 ** Descriptions:		get pointeurs des PID
 **
 ** parameters:			None
 ** Returned value:		pointeur vers le PID en Vitesse de Position
 **
 *****************************************************************************/
struct st_pid_filter* _1_Asserv_GetPtr_PID_Vit_Pos(void)
{
	return &PID_Vitesse_Position;
}


/*****************************************************************************
 ** Function name:		_1_Get_prt_PID_Vit_Rotation
 **
 ** Descriptions:		get pointeurs des PID
 **
 ** parameters:			None
 ** Returned value:		pointeur vers le PID en Vitesse de Rotation
 **
 *****************************************************************************/
struct st_pid_filter* _1_Asserv_GetPtr_PID_Vit_Rot(void)
{
	return &PID_Vitesse_Rotation;
}


/*****************************************************************************
 ** Function name:		_1_Get_prt_PID_Vit_Droite
 **
 ** Descriptions:		get pointeurs des PID
 **
 ** parameters:			None
 ** Returned value:		pointeur vers le PID en Vitesse de Position
 **
 *****************************************************************************/
struct st_pid_filter* _1_Get_prt_PID_Vit_Droite(void)
{
	return &PID_Vitesse_Roue_Droite;
}


/*****************************************************************************
 ** Function name:		_1_Get_prt_PID_Vit_Gauche
 **
 ** Descriptions:		get pointeurs des PID
 **
 ** parameters:			None
 ** Returned value:		pointeur vers le PID en Vitesse de Rotation
 **
 *****************************************************************************/
struct st_pid_filter* _1_Get_prt_PID_Vit_Gauche(void)
{
	return &PID_Vitesse_Roue_Gauche;
}




/*****************************************************************************
 ** Function name:		_1_Update_PID_Vitesse_Consigne_distance
 **
 ** Descriptions:		Met à jour la consigne de vitesse en position
 **
 ** parameters:			consigne
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Update_PID_Vitesse_Consigne_distance(long consigne)
{
	PID_update_Consign(&PID_Vitesse_Position, consigne);
}


/*****************************************************************************
 ** Function name:		_1_Update_PID_Vitesse_Consigne_rotation
 **
 ** Descriptions:		Met à jour la consigne de vitesse en rotation
 **
 ** parameters:			consigne
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Update_PID_Vitesse_Consigne_rotation(long consigne)
{
	PID_update_Consign(&PID_Vitesse_Rotation, consigne);
}


/*****************************************************************************
 ** Function name:		_1_Update_PID_Consigne_Vitesse_Roue_Droite
 **
 ** Descriptions:		Met à jour la consigne de vitesse de la roue Droite
 **
 ** parameters:			consigne
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Update_PID_Consigne_Vitesse_Roue_Droite(long consigne)
{
	PID_update_Consign(&PID_Vitesse_Roue_Droite, consigne);
}


/*****************************************************************************
 ** Function name:		_1_Update_PID_Vitesse_Roue_Gauche
 **
 ** Descriptions:		Met à jour la consigne de vitesse de la roue Gauche
 **
 ** parameters:			consigne
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Update_PID_Consigne_Vitesse_Roue_Gauche(long consigne)
{
	PID_update_Consign(&PID_Vitesse_Roue_Gauche, consigne);
}




/*****************************************************************************
 ** Function name:		_1_Asserv_Niveau_1
 **
 ** Descriptions:		Task d'asservissement de niveau, en fonction du type d'asserv
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Asserv_Niveau_1(void *pvParameter)
{
	Init_Timing_Tache;

	for(;;)
	{
		switch(_1_Odometrie_Get_Parameters()->_1_Odometrie_Type_Asserv)
		{
		case Polaire_Tourne_Avance_point_unique:
			//Dans le cas d'un asserv polaire (vitesse distance, vitesse angle, distance, angle)
			_1_Asserv_Boucle_Asserv_Vitesse_Polaire(pvParameter);
			Task_Delay_Until(PERIODE_PID_VITESSE);
			break;


		case Vitesse_Droite_Vitesse_Gauche_Indep:
		case Vitesse_D_G__Distance_Angle:
			//Dans le cas d'un asserv en vitesse des roues à Droite et à Gauche
			_1_Asserv_Vitesse_Independantes(pvParameter);
			Task_Delay_Until(PERIODE_PID_VITESSE);
			break;

		default:
			Task_Delay_Until(PERIODE_PID_VITESSE);
			break;
		}
	}
}


/*****************************************************************************
 ** Function name:		_1_Asserv_Vitesse_Independantes
 **
 ** Descriptions:		Realiser un asservissement en vitesse à droite et à gauche, de façon indépendante
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Asserv_Vitesse_Independantes(void *pvParameters)
{
	static long prev_Codeur_Gauche = 0;                             //valeur lue du codeur gauche
	static long prev_Codeur_Droit = 0;                              //valeur lue du codeur droit

	long deplacement_Droit, deplacement_Gauche;
	struct Codeurs_Values Valeurs_Codeurs;

	float consigne_perc_moteur_Droit, consigne_perc_moteur_Gauche;

	Valeurs_Codeurs = _0_Codeurs_Get_CodeursValues();
	deplacement_Droit = Valeurs_Codeurs.Codeur_Droit - prev_Codeur_Droit; //= vitesse réelle de la roue droite
	deplacement_Gauche = Valeurs_Codeurs.Codeur_Gauche - prev_Codeur_Gauche; //= vitesse réelle de la roue gauche

	prev_Codeur_Droit = Valeurs_Codeurs.Codeur_Droit;
	prev_Codeur_Gauche = Valeurs_Codeurs.Codeur_Gauche;


	//Mise à jour des current Value des PID
	PID_update_Current_Value(&PID_Vitesse_Roue_Droite, deplacement_Droit);
	//Execute le PID d'asserv en vitesse de la roue droite
	pid_do_filter(&PID_Vitesse_Roue_Droite);

	PID_update_Current_Value(&PID_Vitesse_Roue_Gauche, deplacement_Gauche);
	//Execute le PID d'asserv en vitesse de la roue gauche
	pid_do_filter(&PID_Vitesse_Roue_Gauche);


	//Envoie les consignes vers les PWM pour les moteurs
	if(!_1_Omodetrie_Get_Simulation())
	{
		//Calcul les consignes de chaque moteur
		consigne_perc_moteur_Droit = PID_Vitesse_Roue_Droite.Commande;
		consigne_perc_moteur_Gauche = PID_Vitesse_Roue_Gauche.Commande;

		//Evite le depassement de 100% de PWM
		if(consigne_perc_moteur_Droit > 100)
		{
			consigne_perc_moteur_Droit = 100;
		}

		if(consigne_perc_moteur_Droit < -100)
		{
			consigne_perc_moteur_Droit = -100;
		}

		if(consigne_perc_moteur_Gauche > 100)
		{
			consigne_perc_moteur_Gauche = 100;
		}

		if(consigne_perc_moteur_Gauche < -100)
		{
			consigne_perc_moteur_Gauche = -100;
		}


		//Moteur Gauche
		_0_Set_Motor_PWM_Value(LPC_TIMER2, 0, fabsf(consigne_perc_moteur_Gauche));

		//Moteur Droit
		_0_Set_Motor_PWM_Value(LPC_TIMER2, 1, fabsf(consigne_perc_moteur_Droit));

		//Applique le sens de rotation aux moteurs
		_0_Set_Motor_Direction(consigne_perc_moteur_Gauche >= 0 ? false : true, consigne_perc_moteur_Droit >= 0 ? true : false);
	}else
	{
		//Mise à jour des codeurs virtuels pour la simulation
		_0_Codeurs_Increment_Simulation(PID_Vitesse_Roue_Droite.Commande, PID_Vitesse_Roue_Gauche.Commande);
	}
}



/*****************************************************************************
 ** Function name:		_1_Asserv_Boucle_Asserv_Vitesse_Polaire
 **
 ** Descriptions:		Task d'asservissement polaire en vitesse
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_Asserv_Boucle_Asserv_Vitesse_Polaire(void *pvParameters)
{
	static long prev_Codeur_Gauche = 0;                             //valeur lue du codeur gauche
	static long prev_Codeur_Droit = 0;                              //valeur lue du codeur droit

	long deplacement_Droit, deplacement_Gauche;
	short deplacement_distance;

	float consigne_perc_moteur_Droit, consigne_perc_moteur_Gauche;

	struct Codeurs_Values Valeurs_Codeurs;

	Valeurs_Codeurs = _0_Codeurs_Get_CodeursValues();
	deplacement_Droit = Valeurs_Codeurs.Codeur_Droit - prev_Codeur_Droit;
	deplacement_Gauche = Valeurs_Codeurs.Codeur_Gauche - prev_Codeur_Gauche;
	deplacement_distance = (deplacement_Droit+deplacement_Gauche) >> 1;

	prev_Codeur_Droit = Valeurs_Codeurs.Codeur_Droit;
	prev_Codeur_Gauche = Valeurs_Codeurs.Codeur_Gauche;

	//Mise à jour des current Value du PID en position
	PID_update_Current_Value(&PID_Vitesse_Position, deplacement_distance);
	//Execute le PID d'asserv en vitesse POSITION
	pid_do_filter2(&PID_Vitesse_Position);

	//Mise à jour des current Value du PID en rotation
	PID_update_Current_Value(&PID_Vitesse_Rotation, deplacement_Droit - deplacement_Gauche);
	//Execute le PID d'asserv en vitesse ROTATION
	pid_do_filter2(&PID_Vitesse_Rotation);

	//Envoie les consignes vers les PWM pour les moteurs
	if(!_1_Omodetrie_Get_Simulation())
	{
		//Calcul les consignes de chaque moteur
		consigne_perc_moteur_Droit = PID_Vitesse_Position.Commande + PID_Vitesse_Rotation.Commande;
		consigne_perc_moteur_Gauche = PID_Vitesse_Position.Commande - PID_Vitesse_Rotation.Commande;

		//Evite le depassement de 100% de PWM
		if(consigne_perc_moteur_Droit > 100)
		{
			consigne_perc_moteur_Droit = 100;
		}

		if(consigne_perc_moteur_Droit < -100)
		{
			consigne_perc_moteur_Droit = -100;
		}

		if(consigne_perc_moteur_Gauche > 100)
		{
			consigne_perc_moteur_Gauche = 100;
		}

		if(consigne_perc_moteur_Gauche < -100)
		{
			consigne_perc_moteur_Gauche = -100;
		}

		//Moteur Gauche
		_0_Set_Motor_PWM_Value(LPC_TIMER2, 0, fabsf(consigne_perc_moteur_Gauche));

		//Moteur Droit
		_0_Set_Motor_PWM_Value(LPC_TIMER2, 1, fabsf(consigne_perc_moteur_Droit));

		//Applique le sens de rotation aux moteurs
		_0_Set_Motor_Direction(consigne_perc_moteur_Gauche >= 0 ? true : false, consigne_perc_moteur_Droit >= 0 ? false : true);
	}else
	{
		//Calcul les consignes de chaque moteur
		consigne_perc_moteur_Droit = PID_Vitesse_Position.Commande + PID_Vitesse_Rotation.Commande;
		consigne_perc_moteur_Gauche = PID_Vitesse_Position.Commande - PID_Vitesse_Rotation.Commande;

		//Mise à jour des codeurs virtuels pour la simulation
		_0_Codeurs_Increment_Simulation(consigne_perc_moteur_Droit / 2, consigne_perc_moteur_Gauche / 2);
	}
}

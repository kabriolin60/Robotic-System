#include "1_PID.h"

/**************************************************
Fonction de mise a jour de la consigne d'un PID
Input: Pointeur sur la structure du PID a MAJ
       Valeur a mettre dans la consigne
Output: None
 **************************************************/

void PID_update_Consign(struct st_pid_filter *p, long Consigne)
{
	p->Consigne = Consigne;
}

/*************************************************/


/**************************************************
Fonction de mise a jour de la valeur courante d'un PID
Input: Pointeur sur la structure du PID a MAJ
       Valeur a mettre dans la Current_Value
Output: None
 **************************************************/

void PID_update_Current_Value(struct st_pid_filter *p, long Current)
{
	p->Current_Value = Current;
}

/*************************************************/


/**************************************************
Fonction de mise a jour de la consigne d'un PID
Input: Pointeur sur la structure du PID a MAJ
       Valeur a mettre dans la commande max et commande min
Output: None
 **************************************************/

void PID_set_Max_Min(struct st_pid_filter *p, float MaxMin)
{
	p->commande_max = MaxMin;
	p->commande_min = -MaxMin;
}

/*************************************************/


/**************************************************
Fonction initialisation des PID
Input: Pointeur sur la structure Robot comportant les PID a initialiser
Output: None
 **************************************************/

void Init_PID(void)
{

}

/*************************************************/


/**************************************************
Fonction d'activation d'un PID
Input: Pointeur sur la structure PID a activer
Output: None
 **************************************************/

void pid_Activation(struct st_pid_filter *p)
{
	p->Enable = 1;   //activation du PID
}

/*************************************************/


/**************************************************
Fonction de desactivation d'un PID
Input: Pointeur sur la structure PID a desactiver
Output: None
 **************************************************/

void pid_Desactivation(struct st_pid_filter *p)
{
	p->Enable = 0;   //desactivation du PID
}

/*************************************************/


/**************************************************
Fonction PID
Input: Pointeur sur la structure PID a r�guler
Output: None
 **************************************************/

void pid_do_filter2(struct st_pid_filter *p)
{
	float En = 0, Dn = 0;
	float commande = 0;

	En = p->Consigne - p->Current_Value;     //Calcul de l'erreur en Position
	Dn = p->Current_Value - p->prev_value;    //Calcul de l'erreur Derivee

	p->Erreur_Cumul += En;

	//Gain Kp
	commande = (p->gain_P * En);

	//Gain Kd
	commande += (p->gain_D * Dn);

	//Gain Ki
	commande += (p->gain_I * p->Erreur_Cumul);


	if(!p->Enable) commande = 0;

	p->Commande = commande;           //envoie de la nouvelle commande

	pid_seuillage(p);

	p->prev_value = p->Current_Value; //sauvegarde de la position actuelle
}


void pid_do_filter(struct st_pid_filter *p)
{
	float En = 0, Dn = 0;
	float commande = 0;

	En = p->Consigne - p->Current_Value;     //Calcul de l'erreur en Position

	Dn = (p->prev_value) - (p->Current_Value);    //Calcul de l'erreur Derivee

	p->Erreur_Cumul += En;
	if(p->Erreur_Cumul > p->Max_Erreur_Cumul) p->Erreur_Cumul = p->Max_Erreur_Cumul;
	if(p->Erreur_Cumul < -p->Max_Erreur_Cumul) p->Erreur_Cumul = -p->Max_Erreur_Cumul;

	if(En > 0)
	{
		if(p->Erreur_Cumul < 0)
		{
			p->Erreur_Cumul = 0;
		}
	}else if(En < 0)
	{
		if(p->Erreur_Cumul > 0)
		{
			p->Erreur_Cumul = 0;
		}
	}

	/*commande = (p->gain_P * En) + (p->gain_D * Dn) + (p->Erreur_Cumul);   //Calcul de la commande*/
    if(p->Sommation_Sortie) commande += p->Commande;

	commande = (p->gain_P * En) + (p->gain_D * Dn) + (p->Sommation_Sortie * p->Commande);   //Calcul de la commande
	commande += p->gain_I * p->Erreur_Cumul; //int�gration de l'erreur

	if(!p->Enable) commande = 0;

	if(commande > p->commande_max)
	{
		commande = p->commande_max;   //seuillages
	} else if(commande < p->commande_min)
	{
		commande = p->commande_min;
	}

	p->Commande = commande;           //envoie de la nouvelle commande
	p->prev_value = p->Current_Value; //sauvegarde de la position actuelle
}

/*************************************************/


/**************************************************
Fonction PID seuillage
Input: Pointeur sur la structure PID a seuiller
Output: None
 **************************************************/

void pid_seuillage(struct st_pid_filter *p)
{
	if(p->Commande > p->commande_max) p->Commande = p->commande_max;  //seuillages
	if(p->Commande < p->commande_min) p->Commande = p->commande_min;
}

/*************************************************/ 


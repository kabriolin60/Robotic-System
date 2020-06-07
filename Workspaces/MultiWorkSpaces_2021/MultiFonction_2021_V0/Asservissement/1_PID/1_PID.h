#ifndef _PID_H_
#define _PID_H_

/**************************************************
Declaration de la definition de la Structure contenant les parametres de PID sur des floats
 **************************************************/

struct st_pid_filter
{
	float Consigne;   //Consigne à respecter
	float Current_Value; //valeur actuelle
	float gain_P; /**< Gain of Proportionnal module */
	float gain_I; /**< Gain of Integral module */
	float gain_D; /**< Gain of Derivate module */
	float Erreur_Cumul;
	float Max_Erreur_Cumul;

	unsigned char Sommation_Sortie; /**< Gain indiquant l'addition de la sortie précedente à la nouvelle commande */
	float commande_max;
	float commande_min;
	float prev_value;
	float prev_commande;
	float variation_erreur; //Valeur dont a varié l'erreur entre 2 itérations
	unsigned char Enable;
	float Commande;    //Resultat du filtre
};

/*************************************************/

/**************************************************
Fonction de mise a jour de la consigne d'un PID
Input: Pointeur sur la structure du PID a MAJ
       Valeur a mettre dans la consigne
Output: None
**************************************************/

void PID_update_Consign(struct st_pid_filter *p, long Consigne);

/*************************************************/


/**************************************************
Fonction de mise a jour de la valeur courante d'un PID
Input: Pointeur sur la structure du PID a MAJ
       Valeur a mettre dans la Current_Value
Output: None
**************************************************/

void PID_update_Current_Value(struct st_pid_filter *p, long Current);

/*************************************************/


/**************************************************
Fonction de mise a jour de la consigne d'un PID
Input: Pointeur sur la structure du PID a MAJ
       Valeur a mettre dans la commande max et commande min
Output: None
**************************************************/

void PID_set_Max_Min(struct st_pid_filter *p, float MaxMin);

/*************************************************/


/**************************************************
Fonction initialisation des PID
Input: Pointeur sur la structure Robot comportant les PID a initialiser
Output: None
**************************************************/

void Init_PID(void);

/*************************************************/


/**************************************************
Fonction d'activation d'un PID
Input: Pointeur sur la structure PID a activer
Output: None
**************************************************/

void pid_Activation(struct st_pid_filter *p);

/*************************************************/


/**************************************************
Fonction de desactivation d'un PID
Input: Pointeur sur la structure PID a desactiver
Output: None
**************************************************/

void pid_Desactivation(struct st_pid_filter *p);

/*************************************************/

 
/**************************************************
Fonction PID
Input: Pointeur sur la structure PID a reguler
Output: None
**************************************************/ 
void pid_do_filter2(struct st_pid_filter *p);
void pid_do_filter(struct st_pid_filter *p);

/*************************************************/


/**************************************************
Fonction PID seuillage
Input: Pointeur sur la structure PID a reguler
Output: None
**************************************************/

void pid_seuillage(struct st_pid_filter *p);

/*************************************************/

#endif

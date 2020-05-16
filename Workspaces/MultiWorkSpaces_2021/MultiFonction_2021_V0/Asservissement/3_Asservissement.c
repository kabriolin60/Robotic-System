/*
 * 3_Asservissement.c
 *
 *  Created on: 8 mai 2020
 *      Author: kabri
 */

#include "1_PID.h"
#include "3_Asservissement.h"



/*****************************************************************************
 ** Function name:		_3_Deplacement_Init
 **
 ** Descriptions:		Initialisation des deplacements
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _3_Deplacement_Init(void)
{


}



/*****************************************************************************
 ** Function name:		_3_Deplacement_Distance_Brute
 **
 ** Descriptions:		Force un deplacement d'une distance donnée
 **
 ** parameters:			Distance à parcourir en mm
 ** 					Attente de la fin de trajectoire ou non
 ** Returned value:		None
 **
 *****************************************************************************/
void _3_Deplacement_Distance_Brute(float distance, bool attente)
{
	/*
	 * Step 1: Passer en mode d'asservissement en distance + angle brutes + boucles de vitesses D et G
	 */
	_1_Odometrie_Get_Parameters()->_1_Odometrie_Type_Asserv = Vitesse_D_G__Distance_Angle;


	/*
	 * Step 2: applique la consigne en distance
	 */
	_2_Asservissement_Set_Distance_Displacement_Consign(distance * _1_Odometrie_Get_Parameters()->COEF_D + _2_Asserv_GetPtr_PID_Pos()->Current_Value); // en pas

	/*
	 * Step 3:
	 */
	if(attente)
	{
		Task_Delay(PERIODE_PID_DISTANCE_ANGLE * 2);
		//S'il a été demandé d'attendre la fin de la trajectoire
		while(fabsf(_2_Asserv_GetPtr_PID_Pos()->Consigne - _2_Asserv_GetPtr_PID_Pos()->Current_Value) > 3 *_1_Odometrie_Get_Parameters()->COEF_D)
		{
			Task_Delay(10);
		}
	}
}


/*****************************************************************************
 ** Function name:		_3_Deplacement_Rotation_Brute
 **
 ** Descriptions:		Force un deplacement d'une rotation donnée
 **
 ** parameters:			Rotation à parcourir en radians
 ** 					Attente de la fin de trajectoire ou non
 ** Returned value:		None
 **
 *****************************************************************************/
void _3_Deplacement_Rotation_Brute(float angle, bool attente)
{
	/*
	 * Step 1: Passer en mode d'asservissement en distance + angle brutes + boucles de vitesses D et G
	 */
	_1_Odometrie_Get_Parameters()->_1_Odometrie_Type_Asserv = Vitesse_D_G__Distance_Angle;


	/*
	 * Step 2: applique la consigne en angle
	 */
	_2_Asservissement_Set_Rotation_Displacement_Consign(angle * _1_Odometrie_Get_Parameters()->COEF_ROT + _2_Asserv_GetPtr_PID_Rot()->Current_Value); // en pas

	/*
	 * Step 3:
	 */
	if(attente)
	{
		Task_Delay(PERIODE_PID_DISTANCE_ANGLE * 2);
		//S'il a été demandé d'attendre la fin de la trajectoire
		while(fabsf(_2_Asserv_GetPtr_PID_Rot()->Consigne - _2_Asserv_GetPtr_PID_Rot()->Current_Value) > Convert_Deg_Rad(2) *_1_Odometrie_Get_Parameters()->COEF_ROT)
		{
			Task_Delay(10);
		}
	}
}

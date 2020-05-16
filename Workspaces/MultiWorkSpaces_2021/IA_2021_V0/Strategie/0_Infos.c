/*
 * 0_Infos.c
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#include "0_Infos.h"


static TO_AHBS_RAM3 struct Com_Reponse_Info Infos_Cartes[Nb_Max_Cartes];


/*****************************************************************************
 ** Function name:		_0_Infos_Init
 **
 ** Descriptions:		Init des cartes
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Infos_Init()
{
	memset(&Infos_Cartes, 0, sizeof(struct Com_Reponse_Info)*Nb_Max_Cartes);
}



/*****************************************************************************
 ** Function name:		_0_Update_Card_Datas
 **
 ** Descriptions:		Updates cards datas
 **
 ** parameters:			pointer to the Datas
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Update_Card_Datas(struct Com_Reponse_Info* infos)
{
	Infos_Cartes[infos->Numero_Carte - 1] = *infos;
}



/*****************************************************************************
 ** Function name:		_0_Get_Ptr_Card
 **
 ** Descriptions:		Get a pointer to a specific card
 **
 ** parameters:			Card number
 ** Returned value:		pointer to a specific card
 **
 *****************************************************************************/
struct Com_Reponse_Info* _0_Get_Ptr_Card(byte number)
{
	return &Infos_Cartes[number - 1];
}



/*****************************************************************************
 ** Function name:		_0_Get_Robot_Position
 **
 ** Descriptions:		Get a ptr to a Robot Position (on Card @1)
 **
 ** parameters:			None
 ** Returned value:		pointer to the Robot Position
 **
 *****************************************************************************/
struct st_POSITION_ROBOT _0_Get_Ptr_Robot_Position(void)
{
	struct st_POSITION_ROBOT position;

	position.Position_X = Infos_Cartes[0].PositionRobot.Position_X;
	position.Position_X /= 10;
	position.Position_Y = Infos_Cartes[0].PositionRobot.Position_Y;
	position.Position_Y /= 10;

	position.Angle_rad = Infos_Cartes[0].PositionRobot.Angle;
	position.Angle_rad /= 100;

	return position;
}

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
 ** Descriptions:		Get the Robot Position (on Card @1)
 **
 ** parameters:			None
 ** Returned value:		Robot Position
 **
 *****************************************************************************/
struct st_POSITION_ROBOT _0_Get_Robot_Position(void)
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


/*****************************************************************************
 ** Function name:		_0_Get_Servo_Position
 **
 ** Descriptions:		Get servo Values (position / torque if AX12)
 **
 ** parameters:			Servo Global ID
 ** Returned value:		Servos Values
 **
 *****************************************************************************/
struct servo_destination _0_Get_Servo_Position(byte ID)
{
	struct servo_destination values;

	//Servo or AX12?
	if(ID % 10 <= 5)
	{
		//servo
		values.ID = ID;
		values.Destination = Infos_Cartes[ID / 10 - 1].Position_Servos.Position[ID % 10];
	}else
	{
		//AX12	
		values.ID = ID;
		values.Destination = Infos_Cartes[ID / 10 - 1].Position_AX12.Position[ID % 10];
		values.Torque = Infos_Cartes[ID / 10 - 1].Position_AX12.Torque[ID % 10];
	}

	return values;
}

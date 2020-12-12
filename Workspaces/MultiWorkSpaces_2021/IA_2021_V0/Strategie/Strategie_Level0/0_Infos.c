/*
 * 0_Infos.c
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#include "0_Infos.h"
#include "semphr.h"


static TO_AHBS_RAM3 struct Com_Reponse_Info Infos_Cartes[Nb_Max_Cartes];

static TO_AHBS_RAM3 struct Com_Position_Robot PositionSecondRobot;

SemaphoreHandle_t xMutex_Protection;


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

	xMutex_Protection = xSemaphoreCreateMutex();

	PositionSecondRobot.Position_X = -1000;
	PositionSecondRobot.Position_Y = -1000;
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
	if( xSemaphoreTake( xMutex_Protection, ms_to_tick(5) ) == pdTRUE )
		Infos_Cartes[infos->Numero_Carte - 1] = *infos;

	xSemaphoreGive( xMutex_Protection );
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

	if( xSemaphoreTake( xMutex_Protection, ms_to_tick(5) ) == pdTRUE )
	{
		position.Position_X = Infos_Cartes[0].PositionRobot.Position_X;
		position.Position_X /= 10;
		position.Position_Y = Infos_Cartes[0].PositionRobot.Position_Y;
		position.Position_Y /= 10;

		position.Angle_rad = Infos_Cartes[0].PositionRobot.Angle;
		position.Angle_rad /= 100;

		position.Bloquage_Deplacement = Infos_Cartes[0].PositionRobot.Bloquage;
	}

	xSemaphoreGive( xMutex_Protection );
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

	if( xSemaphoreTake( xMutex_Protection, ms_to_tick(5) ) == pdTRUE )
	{
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
			values.Destination = Infos_Cartes[ID / 10 - 1].Position_AX12.Position[ID % 10 - 6];
			values.Torque = Infos_Cartes[ID / 10 - 1].Position_AX12.Torque[ID % 10 - 6];
		}
	}

	xSemaphoreGive( xMutex_Protection );
	return values;
}



/*****************************************************************************
 ** Function name:		_0_Set_Position_SecondRobot
 **
 ** Descriptions:		position du second Robot
 **
 ** parameters:			position X, Y
 ** Returned value:		None
 **
 *****************************************************************************/
void _0_Set_Position_SecondRobot(int16_t x, int16_t y)
{
	PositionSecondRobot.Position_X = x / 10;
	PositionSecondRobot.Position_Y = y / 10;
}


/*****************************************************************************
 ** Function name:		_0_Get_Position_SecondRobot
 **
 ** Descriptions:		position du second Robot
 **
 ** parameters:			None
 ** Returned value:		Position du Second Robot
 **
 *****************************************************************************/
struct Com_Position_Robot _0_Get_Position_SecondRobot(void)
{
	return PositionSecondRobot;
}


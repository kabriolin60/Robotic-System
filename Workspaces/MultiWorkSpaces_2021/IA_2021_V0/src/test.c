/*
 * test.c
 *
 *  Created on: 2 mai 2020
 *      Author: kabri
 */

#include "Test.h"
#include "Init.h"

#include "2_Echange_Datas.h"
#include "Type_Declaration.h"

#include "Astar.h"
#include "0_Infos.h"
#include "0_Deplacements.h"
#include "0_Actionneurs.h"

#include "1_Obstacles.h"


long Nb_Ping_Envoyes = 0;
extern long Nb_PONG_recus;

__attribute__((optimize("O0"))) void TEST_init_parametres(void)
{
	xTaskCreate(TEST_envoi_Ping, (char *) "Ping", 120, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
	/*struct Point founded_destination;

	Set_Debug_Pin_0_High();

	Astar_Map_Init(Astar_Get_Map(), Astar_Get_Vector_Map(), 255, 300, 2585, 1722);

	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 0, 2950, 180, false);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 1820, 2950, 1950, false);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 0, 180, 1950, false);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 2820, 0, 2950, 1950, false);

	Astar_Add_Obstacle_Vector(Astar_Get_Vector_Map(),
			180, 180,
			2820, 1820);

	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 1000, 800, 1600, 1400, true);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 2000, 600, 2600, 1200, true);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 500, 1100, 1100, 1700, true);

	//Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 200, 500, 2900, 550);

	/*Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 50, 550, 2650, 600);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 420, 320 - 320, 490, 420);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 420, 490, 450);

	Astar_Find_Path(Astar_Get_Map(), Astar_Get_Vector_Map());
	Astar_Debug_Display_Map(Astar_Get_Map());



	founded_destination = Astar_Get_Map()->First_Destination;

	while(founded_destination.x != Astar_Get_Map()->Start_End_Vector.End_Point.x && founded_destination.y != Astar_Get_Map()->Start_End_Vector.End_Point.y)
	{
		Astar_Map_Init(Astar_Get_Map(), Astar_Get_Vector_Map(), founded_destination.x, founded_destination.y, 2585, 1722);

		Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 0, 2950, 180, false);
		Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 1820, 2950, 1950, false);
		Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 0, 180, 1950, false);
		Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 2820, 0, 2950, 1950, false);

		Astar_Add_Obstacle_Vector(Astar_Get_Vector_Map(),
				180, 180,
				2820, 1820);

		Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 1000, 800, 1600, 1400, true);
		Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 2000, 600, 2600, 1200, true);
		Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 500, 1100, 1100, 1700, true);

		Astar_Find_Path(Astar_Get_Map(), Astar_Get_Vector_Map());
		Astar_Debug_Display_Map(Astar_Get_Map());

		founded_destination = Astar_Get_Map()->First_Destination;
	}

	Set_Debug_Pin_0_Low();

	Astar_Debug_Display_Map(Astar_Get_Map());
	 */

	//xTaskCreate(TEST_Deplacement, (char *) "test_deplace", 240, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
}


void TEST_Deplacement(void * pvParameter)
{
	static char str[70];


	_2_Comm_Demande_Simulation(TRUE, RS485_port);
	_2_Comm_Demande_Motor_Power(TRUE, RS485_port);

	Task_Delay(2000);

	_2_Comm_Set_Robot_Position(900, 250, 90, RS485_port);

	while(1)
	{
		Task_Delay(2000);

		//TRAPPE_HAUT;
		_0_Actionneurs_Move_1_Servo(10, 2000, 0, 1000, true);

		GOTO_XY_AVANT_WAIT(600, 460);

		//TRAPPE_BAS;
		_0_Actionneurs_Move_1_Servo(10, 500, 0, 1000, true);

		GOTO_XY_AVANT_WAIT(430, 780);

		//TRAPPE_HAUT;
		_0_Actionneurs_Move_1_Servo(10, 2000, 0, 1000, true);

		GOTO_XY_AVANT_WAIT(370, 910);

		//TRAPPE_BAS;
		_0_Actionneurs_Move_1_Servo(10, 500, 0, 1000, true);

		GOTO_XY_ARRIERE_WAIT(730, 260);

		GOTO_XY_AVANT_WAIT(570, 260);

		//TRAPPE_HAUT;
		_0_Actionneurs_Move_1_Servo(10, 2000, 0, 1000, true);

		GOTO_XY_ARRIERE_WAIT(800, 260);




		GOTO_XY_AVANT_WAIT(1000, 450);

		//TRAPPE_BAS;
		_0_Actionneurs_Move_1_Servo(10, 500, 0, 1000, true);

		GOTO_XY_AVANT_WAIT(877, 942);

		//TRAPPE_HAUT;
		_0_Actionneurs_Move_1_Servo(10, 2000, 0, 1000, true);

		GOTO_XY_AVANT_WAIT(848, 1060);

		//TRAPPE_BAS;
		_0_Actionneurs_Move_1_Servo(10, 500, 0, 1000, true);

		GOTO_XY_ARRIERE_WAIT(800, 260);

		GOTO_XY_AVANT_WAIT(1020, 260);

		//TRAPPE_HAUT;
		_0_Actionneurs_Move_1_Servo(10, 2000, 0, 1000, true);

		GOTO_XY_ARRIERE_WAIT(800, 260);
	}

	while(1)
	{
		GOTO_XY_AVANT_WAIT(500, 500);

		sprintf(str, "Test strategie: 1 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_AVANT_ASTAR_WAIT(1722, 2585, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 2 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_AVANT_ASTAR_WAIT(1000, 600, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 3 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_AVANT_ASTAR_WAIT(1000, 2700, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 4 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_AVANT_WAIT(1000, 2900);

		sprintf(str, "Test strategie: 5 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_ARRIERE_WAIT(1000, 2700);

		sprintf(str, "Test strategie: 6 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_ARRIERE_ASTAR_WAIT(1000, 600, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 7 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);
	}

	Task_Delete_Current;
}



void TEST_envoi_Ping(void *pvParameters)
{
	Task_Delay(20);
	Init_Timing_Tache;

	while(true)
	{
		_2_Comm_Send_Demande_Info(1, RS485_port);

		Task_Delay_Until(2.0F);

		/*_2_Comm_Send_Demande_Info(2, RS485_port);

		Task_Delay_Until(2.0F);

		_2_Comm_Send_Demande_Info(3, RS485_port);

		Task_Delay_Until(2.0F);

		_2_Comm_Send_Demande_Info(4, RS485_port);

		Task_Delay_Until(2.0F);
		Task_Delay_Until(2.0F);/**/
	}

	Task_Delete_Current;
}

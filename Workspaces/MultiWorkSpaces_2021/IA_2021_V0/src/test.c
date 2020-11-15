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
#include "1_Obstacles.h"


long Nb_Ping_Envoyes = 0;
extern long Nb_PONG_recus;

__attribute__((optimize("O0"))) void TEST_init_parametres(void)
{
	xTaskCreate(TEST_envoi_Ping, (char *) "Ping", 80, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
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
	GOTO_XY_AVANT_WAIT(500, 500);

	GOTO_XY_AVANT_ASTAR_WAIT(2585, 1722, &_1_Obstacles_Create_Terrain_Border);

	GOTO_XY_AVANT_ASTAR_WAIT(600, 1000, &_1_Obstacles_Create_Terrain_Border);

	GOTO_XY_AVANT_ASTAR_WAIT(2700, 1000, &_1_Obstacles_Create_Terrain_Border);

	GOTO_XY_AVANT_WAIT(2900, 1000);

	GOTO_XY_ARRIERE_WAIT(2700, 1000);

	GOTO_XY_ARRIERE_ASTAR_WAIT(600, 1000, &_1_Obstacles_Create_Terrain_Border);

	Task_Delete_Current;
}




void TEST_envoi_Ping(void *pvParameters)
{
	Task_Delay(20);
	Init_Timing_Tache;
	vTraceEnable( TRC_START );

	while(Nb_Ping_Envoyes < 100000)
	{
		Set_Debug_Pin_0_High();

		_2_Comm_Send_PING(1, RS485_port);
		//_2_Comm_Send_Demande_Info(1, RS485_port);
		Nb_Ping_Envoyes++;

		Task_Delay_Until(5.0F);
	}

	Task_Delay_Until(20);

	if(Nb_PONG_recus == Nb_Ping_Envoyes)
	{
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, true);
	}

	while(1)
	{
		Task_Delay_Until(20);
	}
}

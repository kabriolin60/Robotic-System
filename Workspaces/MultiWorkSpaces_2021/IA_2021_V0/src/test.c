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
	//xTaskCreate(TEST_envoi_Ping, (char *) "Ping", 120, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
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

	//xTaskCreate(TEST_Deplacement, (char *) "test_deplace", 240, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
	xTaskCreate(TEST_Deplacement_Reel, (char *) "test_deplace reel", 320, NULL, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);
}



void TEST_Deplacement_Reel(void * pvParameter)
{
	static char str[70];
	Task_Delay(520);

	_2_Comm_Demande_Simulation(TRUE, RS485_port);
	_2_Comm_Demande_Motor_Power(TRUE, RS485_port);

	/*
	 * Recallage bordure
	 */

	if(!_0_Deplacement_Get_Simulation())
	{
		//Set les 2 PIDs de vitesses des roues independantes
		_2_Comm_Send_Robot_PID(PID_Id_vitesse_roues_independantes, 0.085f, 0, 0.55f, 50, 0, 1, 1, RS485_port);

		//Set le PID de distance
		_2_Comm_Send_Robot_PID(PID_Id_distance, 0.04f, 0, 0.0f, 20, 0, 0, 1, RS485_port);

		//Set le PID de rotation
		_2_Comm_Send_Robot_PID(PID_Id_orientation, 0.006f, 0, 0.0f, 20, 0, 0, 1, RS485_port);
	}

	Task_Delay(3000);
	struct st_POSITION_ROBOT Robot_Position = _0_Get_Robot_Position();

	/*GOTO_TO_SPLINE_AVANT_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y, 	//P0
			Robot_Position.Position_X+200, Robot_Position.Position_Y,					//M0
			Robot_Position.Position_X+400, Robot_Position.Position_Y+200, 				//M1
			Robot_Position.Position_X+600, Robot_Position.Position_Y+200);				//P1

	Task_Delay(3000);
	GOTO_TO_SPLINE_ARRIERE_WAIT(Robot_Position.Position_X+600, Robot_Position.Position_Y+200, 	//P0
			Robot_Position.Position_X+400, Robot_Position.Position_Y+200,					//M0
			Robot_Position.Position_X+200, Robot_Position.Position_Y, 						//M1
			Robot_Position.Position_X, Robot_Position.Position_Y);							//P1

	Task_Delay(3000);
	Robot_Position = _0_Get_Robot_Position();
	GOTO_TO_SPLINE_AVANT_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y, 	//P0
			750, 1000,
			1600, 0,
			1600, 300);				//P1

	Task_Delay(3000);
	Robot_Position = _0_Get_Robot_Position();
	GOTO_TO_SPLINE_ARRIERE_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y, 	//P0
			1600, 100,					//M0
			1600, 100, 						//M1
			1600, 75);							//P1

	Task_Delay(3000);
	Robot_Position = _0_Get_Robot_Position();
	GOTO_TO_SPLINE_AVANT_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y, 	//P0
			1600, 575,
			1050, 1800,
			1800, 1800);*/

	/*Task_Delay(3000);
	Robot_Position = _0_Get_Robot_Position();
	GOTO_TO_SPLINE_AVANT_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y, 	//P0
			Robot_Position.Position_X-500, Robot_Position.Position_Y,					//M0
			Robot_Position.Position_X-500, Robot_Position.Position_Y+500, 						//M1
			Robot_Position.Position_X, Robot_Position.Position_Y+500);*/



	//Task_Delay(3000);
	Robot_Position = _0_Get_Robot_Position();
	bool MOVE_RESULT;
	sprintf(str, "Test strategie: 1 OK\n");_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);
	MOVE_RESULT = GOTO_TO_SPLINE_AVANT_ASTAR_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y,
			Robot_Position.Position_X+500, Robot_Position.Position_Y,
			Robot_Position.Position_X+500, Robot_Position.Position_Y+500,
			Robot_Position.Position_X, Robot_Position.Position_Y+500,
			&_1_Obstacles_Create_Terrain_Border);

	if(MOVE_RESULT){
		sprintf(str, "Test strategie: 2 OK\n");_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);
		Robot_Position = _0_Get_Robot_Position();
		MOVE_RESULT = GOTO_TO_SPLINE_AVANT_ASTAR_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y,
				Robot_Position.Position_X+500, Robot_Position.Position_Y,
				Robot_Position.Position_X+500, Robot_Position.Position_Y+500,
				Robot_Position.Position_X, Robot_Position.Position_Y+500,
				&_1_Obstacles_Create_Terrain_Border);

		if(MOVE_RESULT)
		{
			sprintf(str, "Test strategie: 3 OK\n");_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);
			Robot_Position = _0_Get_Robot_Position();
			GOTO_TO_SPLINE_AVANT_ASTAR_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y,
					Robot_Position.Position_X+500, Robot_Position.Position_Y,
					Robot_Position.Position_X+800, Robot_Position.Position_Y+300,
					Robot_Position.Position_X+1000, Robot_Position.Position_Y+300,
					&_1_Obstacles_Create_Terrain_Border);
		}else
		{
			sprintf(str, "Test strategie: 4 OK\n");_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);
			Robot_Position = _0_Get_Robot_Position();
			GOTO_TO_SPLINE_ARRIERE_ASTAR_WAIT(Robot_Position.Position_X, Robot_Position.Position_Y,
					Robot_Position.Position_X-100, Robot_Position.Position_Y-500,
					Robot_Position.Position_X, Robot_Position.Position_Y-800,
					Robot_Position.Position_X+100, 230,
					&_1_Obstacles_Create_Terrain_Border);
		}
	}

	Task_Delay(1000);
	for(;;)
	{
		Task_Delay(1000);
	}

	RECALAGE_BORDURE_ARRIERE_WAIT(10);
	_2_Comm_Set_Robot_Position(1000, 75, 90, RS485_port);

	_2_Comm_Send_Robot_Speed(0.1f,0.3f,0.3f,	0.2f,0.3f,0.3f,RS485_port);
	GOTO_XY_AVANT_WAIT(_0_Get_Robot_Position().Position_X, _0_Get_Robot_Position().Position_Y + 150);

	GOTO_XY_ARRIERE_WAIT(800, _0_Get_Robot_Position().Position_Y);

	RECALAGE_BORDURE_ARRIERE_WAIT(10);
	_2_Comm_Set_Robot_Position(75, _0_Get_Robot_Position().Position_Y, 0, RS485_port);

	Task_Delay(1000);

	GOTO_XY_AVANT_WAIT(_0_Get_Robot_Position().Position_X + 200, _0_Get_Robot_Position().Position_Y);
	_2_Comm_Set_Robot_Position(1000, 250, _0_Get_Robot_Position().Angle_Deg, RS485_port);

	Task_Delay(3000);
	_2_Comm_Send_Robot_Speed(0.2f,0.5f,0.5f,	0.5f,0.5f,0.5f,RS485_port);

	Task_Delay(3000);

	while(1)
	{
		GOTO_XY_AVANT_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(1000);

		GOTO_XY_ARRIERE_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(3000);


		//_2_Comm_Send_Robot_Speed(0.2f,0.5f,0.5f,	0.5f,0.5f,0.5f,RS485_port);

		GOTO_XY_AVANT_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(1000);

		GOTO_XY_ARRIERE_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(3000);


		//_2_Comm_Send_Robot_Speed(0.3f,0.5f,0.5f,	0.5f,0.5f,0.5f,RS485_port);

		GOTO_XY_AVANT_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(1000);
		GOTO_XY_ARRIERE_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(3000);


		//_2_Comm_Send_Robot_Speed(0.3f,0.5f,0.5f,	0.5f,0.5f,0.5f,RS485_port);

		GOTO_XY_AVANT_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(1000);

		GOTO_XY_ARRIERE_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(3000);


		//_2_Comm_Send_Robot_Speed(0.4f,0.5f,0.5f,	0.5f,0.5f,0.5f,RS485_port);

		GOTO_XY_AVANT_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(1000);

		GOTO_XY_ARRIERE_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(3000);


		//_2_Comm_Send_Robot_Speed(0.5f,0.5f,0.5f,	0.5f,0.5f,0.5f,RS485_port);

		GOTO_XY_AVANT_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(1000);

		GOTO_XY_ARRIERE_WAIT(1000, 800);

		Task_Delay(1000);

		GOTO_XY_AVANT_WAIT(1000, 250);

		Task_Delay(3000);

	}


}





void TEST_Deplacement(void * pvParameter)
{
	static char str[70];

	Task_Delay(520);


	_2_Comm_Demande_Simulation(TRUE, RS485_port);
	_2_Comm_Demande_Motor_Power(TRUE, RS485_port);

	Task_Delay(2000);

	_2_Comm_Set_Robot_Position(900, 250, 90, RS485_port);

	/*
	//while(1)
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
	}/**/

	/**/
	while(1)
	{
		GOTO_XY_AVANT_WAIT(500, 500);

		sprintf(str, "Test strategie: 1 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_AVANT_ASTAR_WAIT(1722, 2585, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 2 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		_2_Comm_Send_Robot_Speed(1,1,1,1,1,1,RS485_port);

		GOTO_XY_AVANT_ASTAR_WAIT(1000, 600, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 3 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_AVANT_ASTAR_WAIT(1000, 2380, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 4 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		_2_Comm_Send_Robot_Speed(0.5f,1,1,1,1,1,RS485_port);

		GOTO_XY_AVANT_WAIT(1000, 2900);

		sprintf(str, "Test strategie: 5 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_ARRIERE_WAIT(1000, 2380);

		sprintf(str, "Test strategie: 6 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);

		GOTO_XY_ARRIERE_ASTAR_WAIT(1000, 600, &_1_Obstacles_Create_Terrain_Border);

		sprintf(str, "Test strategie: 7 OK\n");
		_2_Comm_Send_Log_Message(str, Color_Blue, Channel_Debug_Test, RS485_port);
	}/**/

	Task_Delete_Current;
}


void TEST_envoi_Ping(void *pvParameters)
{
	Task_Delay(20);
	Init_Timing_Tache;

	long nb_to_send = 100000;

	for(long i = 0; i < nb_to_send; i++)
	{
		_2_Comm_Send_Demande_Info(1, Xbee_port);

		Task_Delay_Until(2000.0F);
	}

	if(Nb_PONG_recus >= nb_to_send)
	{
		Chip_GPIO_WritePortBit(LPC_GPIO, LED_1_PORT, LED_1_BIT, 1);
	}
	Task_Delete_Current;
}




void TEST_envoi_Ping2(void *pvParameters)
{
	Task_Delay(20);
	Init_Timing_Tache;

	while(true)
	{
		_2_Comm_Send_Demande_Info(1, RS485_port);

		Task_Delay_Until(20.0F);

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





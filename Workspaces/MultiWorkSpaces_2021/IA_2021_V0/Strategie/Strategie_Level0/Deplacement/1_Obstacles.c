/*
 * 1_Obstacles.c
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#include "Astar.h"

void _1_TEST_Obstacles_Opponent_Robots(void)
{
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 1000, 800, 1600, 1400, true, Astar_Vector_Color_Red);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 500, 1100, 1100, 1700, true, Astar_Vector_Color_Red);
}


void _1_Obstacles_Create_Terrain_Border(void)
{
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 0, 180, 2950, false, Astar_Vector_Color_Blue);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 2780, 1950, 2950, false, Astar_Vector_Color_Blue);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 0, 0, 1950, 170, false, Astar_Vector_Color_Blue);
	Astar_Add_Rectangular_Obstacle(Astar_Get_Map(), Astar_Get_Vector_Map(), 1820, 0, 1950, 2950, false, Astar_Vector_Color_Blue);

	Astar_Add_Obstacle_Vector(Astar_Get_Vector_Map(), 180, 180,	1820, 2820, Astar_Vector_Color_Blue);

	_1_TEST_Obstacles_Opponent_Robots();
}

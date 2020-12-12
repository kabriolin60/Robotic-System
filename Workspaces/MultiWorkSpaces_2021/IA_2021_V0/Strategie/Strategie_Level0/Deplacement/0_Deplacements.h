/*
 * 0_Deplacements.h
 *
 *  Created on: 13 mai 2020
 *      Author: kabri
 */

#ifndef _0_DEPLACEMENTS_H_
#define _0_DEPLACEMENTS_H_

#include "Communication_Typedef.h"
#include "2_Echange_Datas.h"

/*
 * extern MACRO for deplacement
 */
#define GOTO_XY_AVANT_WAIT(x, y)					_0_Deplacement_Tourne_Avance(x, y, false, true, 0);
#define GOTO_XY_AVANT_ASTAR_WAIT(x, y, obstacles)	_0_Deplacement_Tourne_Avance_ASTAR(x, y, true, 0, obstacles);

#define GOTO_XY_ARRIERE_WAIT(x, y)					_0_Deplacement_Tourne_Avance(x, y, false, true, 1);
#define GOTO_XY_ARRIERE_ASTAR_WAIT(x, y, obstacles)	_0_Deplacement_Tourne_Avance_ASTAR(x, y, true, 1, obstacles);

#define RECALLAGE_BORDURE_AVANT_WAIT(speed)			_0_Deplacement_Recallage_Bordure(true, speed, 6000);
#define RECALLAGE_BORDURE_ARRIERE_WAIT(speed)		_0_Deplacement_Recallage_Bordure(false, speed, 6000);

#define ARRET_DEPLACEMENT()							_0_Deplacement_STOP();



#define Astar_Recherche_Periode						150



#define eGROUP_DEPLA_pathFOUND 		( 1 << 0 ) 		//Flag indicating that a path to the destination has been found
#define eGROUP_DEPLA_path_NOT_FOUND ( 1 << 1 ) 		//Flag indicating that NO path to the destination has been found
#define eGROUP_DEPLA_ARRIVED 		( 1 << 2 ) 		//Flag indicating that the robot is arrived at it's destination
#define eGROUP_DEPLA_NOT_ARRIVED 	( 1 << 3 ) 		//Flag indicating that the robot can not reach the destination
#define eGROUP_DEPLA_BLOQUAGE		( 1 << 4 )		//Flag indiquant qu'un bloquage est détecté
#define eGROUP_DEPLA_SIMULATION		( 1 << 5 )		//Flag indiquant un deplacement simulé


struct Astar_deplacement
{
	void (*obstacle_creation_fct)(void);	//pointer to the Pathfinding obstacles creation fonction
	struct st_DESTINATION_ROBOT destination;	//Destination information
};


void _0_Deplacement_Init(void);

struct st_DESTINATION_ROBOT* _0_Deplacement_Get_ptr_Current_Destination(void);

bool _0_Deplacement_Wait_For_Arrival(struct st_COORDONNEES* coord);

float Distance_To_Destination(struct st_POSITION_ROBOT position, struct st_COORDONNEES* dest);

float Distance_Two_Points(short xa, short ya, short xb, short yb);




bool _0_Deplacement_Tourne_Avance(short X, short Y, bool remplacement, bool Attente, bool direction);

bool _0_Deplacement_Tourne_Avance_ASTAR(short X, short Y, bool Attente, bool direction, void* obstacle_creation_fct);

bool _0_Deplacement_Recallage_Bordure(bool direction, short speed, short TIMEOUT);

void _0_Deplacement_STOP(void);

void _0_Deplacement_ASTAR(void* pvParameter);


#endif /* 0_DEPLACEMENT_0_DEPLACEMENTS_H_ */

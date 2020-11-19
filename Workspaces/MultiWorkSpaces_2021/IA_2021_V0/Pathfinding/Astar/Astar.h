/*
 * Astar.h
 *
 *  Created on: 10 mai 2020
 *      Author: kabri
 */

#ifndef ASTAR_ASTAR_H_
#define ASTAR_ASTAR_H_

#include "stdint.h"
#include "string.h"
#include "math.h"
#include "stdbool.h"


#define Map_size_X						2000
#define Map_size_Y						3000

#define Map_Resolution					50

//Definition du nombre de cases sur l'axe X
#define Astar_Node_Nb_X 				Map_size_X / Map_Resolution

//Definition du nombre de cases sur l'axe Y
#define Astar_Node_Nb_Y 				Map_size_Y / Map_Resolution


#define COST_SIMPLE 					10		//Cout des noeuds "droits"
#define COST_DIAGONAL 					14		//Cout des noeuds en "diagonale"
#define ASTAR_COST_LIMITED_ACCES_NODE 	2		//Cost * coef when walking on a limited access node

#define H_COST_GAIN						1.5F		//Gain on the cost to the destination


struct Point
{
	short x;       			       //Coordonnee X du point
	short y;       			       //Coordonnee Y du point
};


enum _ASTAR_NODE_STATES
{
	Not_Processed,
	Open,
	Closed,
	Path,
	Path_Lisse
};

enum _Astar_NODE_ACCESS
{
	Walkable,
	Limited,
	Blocked,
	Trajectory,
	Start_Node,
	End_Node
};


/**************************************************
Astar Node definition
 **************************************************/
struct Astar_Node
{
	enum _ASTAR_NODE_STATES Astar_Node_State:3;			//0 = non visited; 1 = open; 2 = closed
	enum _Astar_NODE_ACCESS Astar_Node_Access:3;	//0 = walkable; 1 = limited; 2 = blocked

	uint16_t G_cost : 12;				//Cost from the start Node to the current Node
	uint16_t H_cost : 12;				//Cost from the current Node to the Final Node

	uint8_t Parent_Node_index_X;		//Index on X axis of the Parent Node
	uint8_t Parent_Node_index_Y;		//Index on Y axis of the Parent Node
};


/**************************************************
Astar Index
 **************************************************/
struct Astar_Index
{
	uint8_t x;
	uint8_t y;
};


/**************************************************
Astar_Vector
 **************************************************/
struct Astar_Vector
{
	struct Point Start_Point;
	struct Point End_Point;
};


/**************************************************
Astar Map definition
 **************************************************/
struct Astar_Map
{
	struct Astar_Node Nodes[Astar_Node_Nb_X][Astar_Node_Nb_Y];

	struct Astar_Vector Start_End_Vector;

	struct Astar_Index  Start_Node_index; 	//Index of the Starting Node
	struct Astar_Index  End_Node_index; 	//Index of the End Node

	struct Point First_Destination;			//First point to go
};



#define NB_SOMMETS_OBSTACLES_MAX 10
#define NB_OBSTACLES_MAX 10
#define Astar_Vector_Max_Nb (NB_OBSTACLES_MAX * NB_SOMMETS_OBSTACLES_MAX + 5)
/**************************************************
Smoothing Vector definition
 **************************************************/
struct Astar_smoothing_vector
{
	//Smoothing vectors array
	struct Astar_Vector Vectors[Astar_Vector_Max_Nb];
	//Number of vector in the array
	unsigned char Nb_Vectors:7;
};



void Astar_Map_Init(struct Astar_Map* map, struct Astar_smoothing_vector* Astar_Vector, short Start_X_position, short Start_Y_position, short End_X_position, short End_Y_position);

struct Astar_Map* Astar_Get_Map(void);

struct Astar_smoothing_vector* Astar_Get_Vector_Map(void);


short Astar_Find_Path(struct Astar_Map* map, struct Astar_smoothing_vector* vectors_map);

short Astar_Generate_Trajectory(struct Astar_Map* map, struct Astar_smoothing_vector* vectors_map);

int8_t Astar_Find_Best_Node_To_Compute(struct Astar_Map* map, short * best_index_x, short * best_index_y);


bool Astar_Compute_Surronding_Nodes(struct Astar_Map* map, uint8_t index_x, uint8_t index_y);

bool Astar_Compute_Node(struct Astar_Map* map, struct Astar_Node* calling_node, struct Astar_Node* new_node, uint8_t cost_to_go, short index_x_calling, short index_y_calling, short index_x_new, short index_y_new);



struct Point Astar_Smoothing(struct Astar_Map* map, struct Astar_smoothing_vector* vectors_map);

short Dijkstra_Intersect_Any_Segment(struct Astar_Vector* tested_vector, struct Astar_smoothing_vector* vectors_map);

unsigned char Dijkstra_intersect_segment(struct Point *s1, struct Point *s2, struct Point *t1, struct Point *t2, struct Point *p);



void Astar_Debug_Display_Map(struct Astar_Map* map);


void Astar_DEBUG_Display_Map_Node_Status(struct Astar_Map* map);

void Astar_debug_Display_Node_Status(struct Astar_Node* node);



void Astar_DEBUG_Display_Map_Node_Access(struct Astar_Map* map);

void Astar_debug_Display_Node_Access(struct Astar_Node* node);



void Astar_Debug_Send_String(char* str);

void Astar_Debug_Send_char(char ch);



static inline void Astar_Set_Node_as_Blocked(struct Astar_Node* node)
{
	node->Astar_Node_State = Closed;
	node->Astar_Node_Access = Blocked;
}


static inline void Astar_Set_Node_as_Closed(struct Astar_Node* node)
{
	node->Astar_Node_State = Closed;
}


static inline void Astar_Set_Node_as_Opened(struct Astar_Node* node)
{
	node->Astar_Node_State = Open;
}


static inline void Astar_Set_Node_as_Start(struct Astar_Node* node)
{
	node->Astar_Node_Access = Start_Node;

	//Used to identify this node during smoothing
	node->Parent_Node_index_X = Astar_Node_Nb_X + 1;
	node->Parent_Node_index_Y = Astar_Node_Nb_Y + 1;
}

static inline void Astar_Set_Node_as_End(struct Astar_Node* node)
{
	node->Astar_Node_Access = End_Node;

	//Used to identify this node during smoothing
	node->Parent_Node_index_X = Astar_Node_Nb_X + 1;
	node->Parent_Node_index_Y = Astar_Node_Nb_Y + 1;
}

static inline void Astar_Set_Node_as_Trajectory(struct Astar_Node* node)
{
	node->Astar_Node_Access = Trajectory;
}



void Astar_Add_Rectangular_Obstacle(struct Astar_Map* map, struct Astar_smoothing_vector* vectors_map, short X_min, short Y_min, short X_max, short Y_max, bool use_vectors);

void Astar_Add_Obstacle_Vector(struct Astar_smoothing_vector* vectors_map, short X_min, short Y_min, short X_max, short Y_max);






static inline short Astar_Compute_H_cost(short index_a_x, short index_a_y, short index_b_x, short index_b_y)
{
	short delta_x, delta_y;

	delta_x = index_b_x - index_a_x;
	delta_y = index_b_y - index_a_y;

	return (short)(sqrtf((delta_x*delta_x)+(delta_y*delta_y)) * COST_SIMPLE * H_COST_GAIN);
}



static inline short smin(short a, short b)
{
	if(a<b)
		return a;

	return b;
}

static inline short smax(short a, short b)
{
	if(a>b)
		return a;

	return b;
}


static inline short Astar_Coord_To_Index_X(short coord)
{
	return smax(0, smin(coord, Map_size_X)) / Map_Resolution;
}


static inline short Astar_Coord_To_Index_Y(short coord)
{
	return smax(0, smin(coord, Map_size_Y)) / Map_Resolution;
}



#endif /* ASTAR_ASTAR_H_ */

/*
 * Astar.c
 *
 *  Created on: 10 mai 2020
 *      Author: kabri
 */


#include "Astar.h"

#include "Configuration.h"



static struct Astar_Map Map;
static /*TO_AHBS_RAM3*/ struct Astar_smoothing_vector Astar_Vector;

/*****************************************************************************
 ** Function name:		Astar_Map_Init
 **
 ** Descriptions:		Astar Map initialisation
 **
 ** parameters:			Pointer to the map
 ** 					Pointer to the Vector Map
 ** 					Start_X_position (mm)
 ** 					Start_Y_position (mm)
 ** 					End_X_position (mm)
 ** 					End_Y_position (mm)
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_Map_Init(struct Astar_Map* map, struct Astar_smoothing_vector* Astar_Vector, short Start_X_position, short Start_Y_position, short End_X_position, short End_Y_position)
{
	memset(map, 0, sizeof(struct Astar_Map));
	memset(Astar_Vector, 0, sizeof(struct Astar_smoothing_vector));

	//Set Start and End nodes
	uint8_t index_Start_X_position;
	uint8_t index_Start_Y_position;
	uint8_t index_End_X_position;
	uint8_t index_End_Y_position;

	//Transform Coord to index
	index_Start_X_position = Astar_Coord_To_Index_X(Start_X_position);
	index_Start_Y_position = Astar_Coord_To_Index_Y(Start_Y_position);

	index_End_X_position = Astar_Coord_To_Index_X(End_X_position);
	index_End_Y_position = Astar_Coord_To_Index_Y(End_Y_position);

	//Set Start and End Nodes
	Astar_Set_Node_as_Start(&map->Nodes[index_Start_X_position][index_Start_Y_position]);
	Astar_Set_Node_as_End(&map->Nodes[index_End_X_position][index_End_Y_position]);

	//Keep Start and End coord safe
	map->Start_End_Vector.Start_Point.x = Start_X_position;
	map->Start_End_Vector.Start_Point.y = Start_Y_position;

	map->Start_Node_index.x = index_Start_X_position;
	map->Start_Node_index.y = index_Start_Y_position;

	map->Start_End_Vector.End_Point.x = End_X_position;
	map->Start_End_Vector.End_Point.y = End_Y_position;

	map->End_Node_index.x = index_End_X_position;
	map->End_Node_index.y = index_End_Y_position;
}



/*****************************************************************************
 ** Function name:		Astar_Get_Map
 **
 ** Descriptions:		Get the Astar map pointer
 **
 ** parameters:			None
 ** Returned value:		Pointer to the Map
 **
 *****************************************************************************/
struct Astar_Map* Astar_Get_Map(void)
{
	return &Map;
}


/*****************************************************************************
 ** Function name:		Astar_Get_Vector_Map
 **
 ** Descriptions:		Get the Vector map pointer
 **
 ** parameters:			None
 ** Returned value:		Pointer to the Vector Map
 **
 *****************************************************************************/
struct Astar_smoothing_vector* Astar_Get_Vector_Map(void)
{
	return &Astar_Vector;
}


/*****************************************************************************
 ** Function name:		Astar_Find_Path
 **
 ** Descriptions:		Astar process
 **
 ** parameters:			Pointer to the map
 ** 					Pointer to the Vectors map
 ** Returned value:		Cost to destination
 ** 					-1 if no path found
 ** 					-2 Start Blocked
 ** 					-3 End Blocked
 **
 *****************************************************************************/
#include "2_Echange_Datas.h"
__attribute__((optimize("O0"))) short Astar_Find_Path(struct Astar_Map* m, struct Astar_smoothing_vector* vect)
{
	struct Astar_Map* map = m;
	struct Astar_smoothing_vector* vectors_map = vect;

	static byte Already_Blocked = 0;

	//Check if Starting point is not in a blocked node
	if(map->Nodes[map->Start_Node_index.x][map->Start_Node_index.y].Astar_Node_Access == Blocked)
	{
		//Starting node blocked
		Already_Blocked = 1;
		if(!Already_Blocked)
			_2_Comm_Send_Log_Message("ASTAR: Start node Blocked!\n", Channel_Debug_ASTAR, Color_Red, RS485_port);

		map->Nodes[map->Start_Node_index.x][map->Start_Node_index.y].Astar_Node_Access = Walkable;
		//return -2; //Le point de départ, le robot y est, donc il est considéré comme accessible
	}
	else
	{
		Already_Blocked = 0;
	}

	if(map->Nodes[map->End_Node_index.x][map->End_Node_index.y].Astar_Node_Access == Blocked)
	{
		//End node blocked, no path possible
		_2_Comm_Send_Log_Message("ASTAR: End node Blocked!\n", Channel_Debug_ASTAR, Color_Red, RS485_port);
		return -3;
	}

	//First, compute at the starting point
	Astar_Compute_Node(map,
			&map->Nodes[map->Start_Node_index.x][map->Start_Node_index.y],
			&map->Nodes[map->Start_Node_index.x][map->Start_Node_index.y],
			map->Start_Node_index.x,
			map->Start_Node_index.y,
			map->Start_Node_index.x,
			map->Start_Node_index.y,
			0);
	Astar_Compute_Surronding_Nodes(map, map->Start_Node_index.x, map->Start_Node_index.y);
	//Astar_DEBUG_Display_Map_Node_Status(map);

	short best_index_x, best_index_y;

	int8_t result = 0;
	//While we haven't found the End Node
	while(result == 0)
	{
		if(Astar_Find_Best_Node_To_Compute(map, &best_index_x, &best_index_y) == -1)
		{
			//No solution found, no more nodes to explore
			return -1;
		}
		result = Astar_Compute_Surronding_Nodes(map, best_index_x, best_index_y);

		//Debug purpose, display map details at each iteration
		//Astar_DEBUG_Display_Map_Node_Status(map);
	}

	//End of mapping
	if(result == 1)
	{
		//We have found the End Node
		Astar_Generate_Trajectory(map, vectors_map);

		//Smooth the path
		map->First_Destination = Astar_Smoothing(map, vectors_map);

		//we reached the End node
		return map->Nodes[map->End_Node_index.x][map->End_Node_index.y].G_cost; //travel cost
	}

	return -1; // no path found
}


/*****************************************************************************
 ** Function name:		Astar_Generate_Trajectory
 **
 ** Descriptions:		Create the list of nodes to pass through
 **
 ** parameters:			Pointer to the Map
 ** 					Pointer to the vectors map
 **
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_Generate_Trajectory(struct Astar_Map* map, struct Astar_smoothing_vector* vectors_map)
{
	struct Astar_Node* node = &map->Nodes[map->End_Node_index.x][map->End_Node_index.y];
	struct Astar_Node* parent_node = &map->Nodes[node->Parent_Node_index_X][node->Parent_Node_index_Y];

	for(int i = 0; i < Astar_Node_Nb_X * Astar_Node_Nb_Y; i++)
	{
		//Mark this node as Trajectory
		Astar_Set_Node_as_Trajectory(parent_node);

		if(parent_node->Parent_Node_index_X >= Astar_Node_Nb_X || parent_node->Parent_Node_index_Y >= Astar_Node_Nb_Y)
		{
			break;
		}

		if(parent_node->Parent_Node_index_X == map->Start_Node_index.x && parent_node->Parent_Node_index_Y == map->Start_Node_index.y)
		{
			//If the parent of a point is = to the Start point of the trajectory
			break;
		}else
		{
			node = parent_node;
			parent_node = &map->Nodes[node->Parent_Node_index_X][node->Parent_Node_index_Y];
		}
	}
}



/*****************************************************************************
 ** Function name:		Astar_Find_Best_Node_To_Compute
 **
 ** Descriptions:		Find the next Node to Compute
 **
 ** parameters:			Pointer to the Map
 ** 					Pointer to the next Node index X
 ** 					Pointer to the next Node index Y
 ** Returned value:		-1: we haven't fount the end node, and there no more nodes to explore, no path possible
 ** 					0: we haven't found yet the End Node but there is still node to explore
 ** 					1: we have found the End Node
 **
 *****************************************************************************/
__attribute__((optimize("O0"))) int8_t Astar_Find_Best_Node_To_Compute(struct Astar_Map* map, short * best_index_x, short * best_index_y)
{
	short Best_F_cost = 0x7FFF; //maximum as begining
	short Best_H_cost = 0X7FFF;

	uint8_t best_node_index_x = 0;
	uint8_t best_node_index_y = 0;

	struct Astar_Node* checked_node;
	short nb_of_opended_nodes = 0;

	//for each node on X axis
	for(int x = 0; x < Astar_Node_Nb_X; x++)
	{
		//For each node on Y axis
		for(int y = 0; y < Astar_Node_Nb_Y; y++)
		{
			checked_node = &map->Nodes[x][y];

			//If the node is opended
			if(checked_node->Astar_Node_State == Open)
			{
				nb_of_opended_nodes++;

				uint16_t new_F_cost = checked_node->G_cost + checked_node->H_cost;
				//If the cost to go to this node + the cost to go from this node to the End Node = F_cost = G_cost + H_Cost <= previous best_F_cost
				if(new_F_cost < Best_F_cost)
				{
					//This new node is better, keep is index in memory
					Best_F_cost = new_F_cost;
					Best_H_cost = checked_node->H_cost;
					best_node_index_x = x;
					best_node_index_y = y;
				}else if(new_F_cost == Best_F_cost)
				{
					//If the new F_cost and last Best F_cost are =, compare only with the H_cost
					if(checked_node->H_cost < Best_H_cost)
					{
						//This new node is better, keep is index in memory
						Best_F_cost = new_F_cost;
						Best_H_cost = checked_node->H_cost;
						best_node_index_x = x;
						best_node_index_y = y;
					}
				}
			}
		}
	}

	//return the next best node to compute
	*best_index_x = best_node_index_x;
	*best_index_y = best_node_index_y;

	if(nb_of_opended_nodes > 0)
		return 0; //we haven't yet found the End node but there is still nodes to explore

	return -1; //we haven't yet found the End node, but there is no more nodes to explore
}


/*****************************************************************************
 ** Function name:		Astar_Compute_Surronding_Nodes
 **
 ** Descriptions:		Compute new costs for the 8 nodes arround the current one
 **
 ** parameters:			Pointer to the Map
 ** 					Index X of the current Node
 ** 					Index Y of the current Node
 ** Returned value:		False: no End node found arround this node
 ** 					True: we've found the End Node
 **
 *****************************************************************************/
__attribute__((optimize("O0"))) bool Astar_Compute_Surronding_Nodes(struct Astar_Map* map, uint8_t index_x, uint8_t index_y)
{
	//Get a pointer on the current Node
	struct Astar_Node* Current_node = &map->Nodes[index_x][index_y];
	struct Astar_Node* tested_node = NULL;

	//This node has been computed, set as Closed
	Astar_Set_Node_as_Closed(Current_node);

	//Compute the North-Ouest Node if exist
	if(index_x > 0 && index_y < Astar_Node_Nb_Y - 1)
	{
		tested_node= &map->Nodes[index_x - 1][index_y + 1];

		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_DIAGONAL, index_x, index_y, index_x - 1, index_y + 1);

		if(map->Nodes[index_x - 1][index_y + 1].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	//Compute the North Node if exist
	if(index_y < Astar_Node_Nb_Y - 1)
	{
		tested_node= &map->Nodes[index_x][index_y + 1];

		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_SIMPLE, index_x, index_y, index_x, index_y + 1);

		if(map->Nodes[index_x][index_y + 1].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	//Compute the North-East Node if exist
	if(index_x < Astar_Node_Nb_X - 1 && index_y < Astar_Node_Nb_Y - 1)
	{
		tested_node= &map->Nodes[index_x + 1][index_y + 1];
		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_DIAGONAL, index_x, index_y, index_x + 1, index_y + 1);

		if(map->Nodes[index_x + 1][index_y + 1].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	//Compute the East Node if exist
	if(index_x < Astar_Node_Nb_X - 1)
	{
		tested_node= &map->Nodes[index_x + 1][index_y];

		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_SIMPLE, index_x, index_y, index_x + 1, index_y);

		if(map->Nodes[index_x + 1][index_y].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	//Compute the South-East Node if exist
	if(index_x < Astar_Node_Nb_X - 1 && index_y > 0)
	{
		tested_node= &map->Nodes[index_x + 1][index_y - 1];
		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_DIAGONAL, index_x, index_y, index_x + 1, index_y - 1);

		if(map->Nodes[index_x + 1][index_y - 1].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	//Compute the South Node if exist
	if(index_y > 0)
	{
		tested_node= &map->Nodes[index_x][index_y - 1];

		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_SIMPLE, index_x, index_y, index_x, index_y - 1);

		if(map->Nodes[index_x][index_y - 1].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	//Compute the South-West Node if exist
	if(index_x > 0 && index_y > 0)
	{
		tested_node= &map->Nodes[index_x - 1][index_y - 1];
		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_DIAGONAL, index_x, index_y, index_x - 1, index_y - 1);

		if(map->Nodes[index_x - 1][index_y - 1].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	//Compute the West Node if exist
	if(index_x > 0)
	{
		tested_node= &map->Nodes[index_x - 1][index_y];

		//This Node is on the map
		//Compute his new cost
		Astar_Compute_Node(map, Current_node, tested_node, COST_SIMPLE, index_x, index_y, index_x - 1, index_y);

		if(map->Nodes[index_x - 1][index_y].Astar_Node_Access == End_Node)
			return true; // End node found
	}

	return false; // no End node found
}


/*****************************************************************************
 ** Function name:		Astar_Compute_Surronding_Nodes
 **
 ** Descriptions:		Compute new costs for the 8 nodes arround the current one
 **
 ** parameters:			Pointer to the Node
 ** 					pointer to the Calling Node
 ** 					Cost to go from the previus node to this one
 ** 					Index X of the calling Node
 ** 					Index Y of the calling Node
 ** 					Index X of the new Node
 ** 					Index Y of the new Node
 ** Returned value:		True: still accesible
 ** 					False: node is blocked or closed, no comptation done
 **
 *****************************************************************************/
__attribute__((optimize("O0"))) bool Astar_Compute_Node(struct Astar_Map* map, struct Astar_Node* calling_node, struct Astar_Node* new_node, uint8_t cost_to_go, short index_x_calling, short index_y_calling, short index_x_new, short index_y_new)
{
	//Check if this node is in Block Acces
	if(new_node->Astar_Node_Access == Blocked)
	{
		//No Computation to do
		return false;
	}

	//Check if this node is in Closes States
	if(new_node->Astar_Node_State == Closed)
	{
		//No Computation to do
		return false;
	}

	//Check if this node is the starting Node
	if(new_node->Astar_Node_Access == Start_Node)
	{
		return true;
	}

	//If this Node is a limited access Node
	if(new_node->Astar_Node_Access == Limited)
		cost_to_go *= ASTAR_COST_LIMITED_ACCES_NODE;

	//Compute the new G_cost of this Node
	uint16_t new_G_cost = calling_node->G_cost + cost_to_go;

	//Compute the new H_Cost of this Node = distance between the two nodes
	if(new_node->H_cost == 0)
	{
		new_node->H_cost = Astar_Compute_H_cost(index_x_new, index_y_new, map->End_Node_index.x, map->End_Node_index.y);
	}


	//If the new G_Cost < previous G_Cost, this link is better than the previous saved_one
	if(new_G_cost < new_node->G_cost || new_node->Astar_Node_State == Not_Processed)
	{
		//Set the new G_cost
		new_node->G_cost = new_G_cost;

		//And the new parent, because we found a faster path to this node
		new_node->Parent_Node_index_X = index_x_calling;
		new_node->Parent_Node_index_Y = index_y_calling;

		//Define this node as Open
		Astar_Set_Node_as_Opened(new_node);
	} //else keep the previous G_cost and parent

	return true;
}



/*****************************************************************************
 ** Function name:		Astar_Smoothing
 **
 ** Descriptions:		Trajectory smoothing
 **
 ** parameters:			Pointer to the Map
 **
 ** Returned value:		Next destination
 **
 *****************************************************************************/
struct Point Astar_Smoothing(struct Astar_Map* map, struct Astar_smoothing_vector* vectors_map)
{
	struct Astar_Vector tested_vector;
	struct Astar_Node* tested_node;

	//Test intersection vectors

	//Check for the longest vector as possible from the (real) starting point
	//start with the (real) destination
	tested_vector.Start_Point = map->Start_End_Vector.Start_Point;
	tested_vector.End_Point = map->Start_End_Vector.End_Point;

	tested_node = &map->Nodes[map->End_Node_index.x][map->End_Node_index.y];

	while(Dijkstra_Intersect_Any_Segment(&tested_vector, vectors_map) && tested_node->Parent_Node_index_X != Astar_Node_Nb_X + 1 && tested_node->Parent_Node_index_Y != Astar_Node_Nb_Y + 1)
	{
		//In case of Intersection
		//Try with a shorter vector
		//His new ends = parent node of the previous tested node

		tested_vector.End_Point.x = (tested_node->Parent_Node_index_X * Map_Resolution) + (Map_Resolution >> 1);
		tested_vector.End_Point.y = (tested_node->Parent_Node_index_Y * Map_Resolution) + (Map_Resolution >> 1);

		//If vector too small, keep the previous one
		if(Dijkstra_Vector_Length(tested_vector.Start_Point.x, tested_vector.Start_Point.y, tested_vector.End_Point.x, tested_vector.End_Point.y) < Map_Resolution*2)
		{
			tested_node = &map->Nodes[tested_node->Parent_Node_index_X][tested_node->Parent_Node_index_Y];
			return tested_vector.End_Point;
		}

		tested_node = &map->Nodes[tested_node->Parent_Node_index_X][tested_node->Parent_Node_index_Y];
	}

	//2 case: We've found the longuest vector possible OR the next point == the closest node
	//The next possible node is the closest one
	return tested_vector.End_Point;
}


/*****************************************************************************
 ** Function name:		Dijkstra_Intersect_Any_Segment
 **
 ** Descriptions:		Check if a vector intersect any vector in the map
 **
 ** parameters:			Pointer to the tested vector
 ** 					Pointer to the Vectors map
 ** Returned value:		false: no intersection
 ** 					true: intersect an other vector
 **
 *****************************************************************************/
short Dijkstra_Intersect_Any_Segment(struct Astar_Vector* tested_vector, struct Astar_smoothing_vector* vectors_map)
{
	short Index_Liste_Vecteur = 0;
	struct Point PointIntersection;

	//for each vector in the map
	for(Index_Liste_Vecteur = 0; Index_Liste_Vecteur < vectors_map->Nb_Vectors; Index_Liste_Vecteur++)
	{
		//Check if the tested vector intersect any other one in the map
		if(Dijkstra_intersect_segment(&tested_vector->Start_Point,
				&tested_vector->End_Point,
				&vectors_map->Vectors[Index_Liste_Vecteur].Start_Point,
				&vectors_map->Vectors[Index_Liste_Vecteur].End_Point,
				&PointIntersection))
		{
			/*static char str[70];
			sprintf(str, "Vect Intersect, X= %d, Y= %d; to X= %d, Y= %d \n",
					(short)tested_vector->Start_Point.x,
					(short)tested_vector->Start_Point.y,
					(short)tested_vector->End_Point.x,
					(short)tested_vector->End_Point.y);
			_2_Comm_Send_Log_Message(str, Color_Red, RS485_port);
			sprintf(str, "With Vect, X= %d, Y= %d; to X= %d, Y= %d \n",
					(short)vectors_map->Vectors[Index_Liste_Vecteur].Start_Point.x,
					(short)vectors_map->Vectors[Index_Liste_Vecteur].Start_Point.y,
					(short)vectors_map->Vectors[Index_Liste_Vecteur].End_Point.x,
					(short)vectors_map->Vectors[Index_Liste_Vecteur].End_Point.y);
			_2_Comm_Send_Log_Message(str, Color_Red, RS485_port);
			sprintf(str, "At, X= %d, Y= %d \n",
					(short)PointIntersection.x,
					(short)PointIntersection.y);
			_2_Comm_Send_Log_Message(str, Color_Red, RS485_port);*/


			//Arrete de boucler
			return true;
		}
	}

	//No Intersection
	return false;
}


float Dijkstra_Vector_Length(short start_x, short start_y, short end_x, short end_y)
{
	short delta_x, delta_y;

	delta_x = start_x - end_x;
	delta_y = start_y - end_y;

	return sqrtf((delta_x*delta_x)+(delta_y*delta_y));
}


/*****************************************************************************
 ** Function name:		Dijkstra_intersect_segment
 **
 ** Descriptions:		Check if a vector intersect an other Vector
 **
 ** parameters:			Pointer to the start point of vector 1
 ** 					Pointer to the end point of vector 1
 ** 					Pointer to the start point of vector 2
 ** 					Pointer to the end point of vector 2
 ** 					Pointer to the coord of the Intersection point (if exists)
 ** Returned value:		0: OK, no intertsection
 ** 					1: Intersection
 ** 					2: Commoun extremity
 **
 *****************************************************************************/
unsigned char Dijkstra_intersect_segment(struct Point *s1, struct Point *s2, struct Point *t1, struct Point *t2, struct Point *p)
{
	struct Point A = { 0 };
	struct Point B = { 0 };
	struct Point C = { 0 };
	struct Point D = { 0 };

	float Coef_Directeur_Vecteur_1 = 0;
	float Coef_Directeur_Vecteur_2 = 0;
	float Ordonne_Origine_Vecteur_1 = 0;
	float Ordonne_Origine_Vecteur_2 = 0;

	/* if points egual */
	if (s1->x == t1->x && s1->y == t1->y) {
		*p = *s1;
		return 2;
	}
	if (s1->x == t2->x && s1->y == t2->y) {
		*p = *s1;
		return 2;
	}
	if (s2->x == t1->x && s2->y == t1->y) {
		*p = *s2;
		return 2;
	}
	if (s2->x == t2->x && s2->y == t2->y) {
		*p = *s2;
		return 2;
	}


	//Classement par Ordre des X Croissants
	if(s1->x >= s2->x)
	{
		A = *s2;
		B = *s1;
	}else
	{
		A = *s1;
		B = *s2;
	}


	if(t1->x >= t2->x)
	{
		C = *t2;
		D = *t1;
	}else
	{
		C = *t1;
		D = *t2;
	}


	//Calcul des Parametres des Vecteurs
	//Si AB non vertical
	if(A.x != B.x)
	{
		Coef_Directeur_Vecteur_1 = (float)((B.y - A.y)) / (float)((B.x - A.x));
	}else
	{
		Coef_Directeur_Vecteur_1 = 0xFFFFFFFF;
	}


	//Si CD non vertical
	if(C.x != D.x)
	{
		Coef_Directeur_Vecteur_2 = (float)((D.y - C.y)) / (float)((D.x - C.x));
	}else
	{
		Coef_Directeur_Vecteur_2 = 0xFFFFFFFF;
	}


	//Si AB et CD verticaux
	if(A.x == B.x && C.x == D.x)
	{
		if(A.x != C.x)
		{
			return 0;
		}

		if(A.y < B.y)
		{
			if(C.y > A.y && C.y < B.y)
			{
				return 1;
			}
		}else
		{
			if(C.y < A.y && C.y > B.y)
			{
				return 1;
			}
		}
	}


	//Si les Segments sont // et non verticaux
	if(Coef_Directeur_Vecteur_1 == Coef_Directeur_Vecteur_2)
	{
		if(A.y != C.y)
		{
			return 0;
		}
		if(A.x < B.x)
		{
			if(C.x > A.x && C.x < B.x)
			{
				return 1;
			}
		}else
		{
			if(C.x < A.x && C.x > B.x)
			{
				return 1;
			}
		}
	}


	//Calcul des Ordonnees a l'origine
	//Si AB non vertical
	if(A.x != B.x)
	{
		Ordonne_Origine_Vecteur_1 = A.y - Coef_Directeur_Vecteur_1 * A.x;
	}else
	{
		Ordonne_Origine_Vecteur_1 = 0xFFFFFFFF;
	}


	//Si CD non vertical
	if(C.x != D.x)
	{
		Ordonne_Origine_Vecteur_2 = C.y - Coef_Directeur_Vecteur_2 * C.x;
	}else
	{
		Ordonne_Origine_Vecteur_2 = 0xFFFFFFFF;
	}


	//Calcul du point d'intersection entre les vecteurs
	//Si AB et CD non verticaux
	if(A.x != B.x && C.x != D.x)
	{
		p->x = (Ordonne_Origine_Vecteur_2 - Ordonne_Origine_Vecteur_1) / (Coef_Directeur_Vecteur_1 - Coef_Directeur_Vecteur_2);
		p->y = Coef_Directeur_Vecteur_1 * p->x + Ordonne_Origine_Vecteur_1;
	}


	//Si AB vertical et CD non vertical
	if(A.x == B.x)
	{
		if(C.x != D.x)
		{
			p->x = A.x;
			p->y = Coef_Directeur_Vecteur_2 * p->x + Ordonne_Origine_Vecteur_2;
		}
	}else
	{
		//Si AB non vertical et CD vertical
		if(C.x == D.x)
		{
			p->x = C.x;
			p->y = Coef_Directeur_Vecteur_1 * p->x + Ordonne_Origine_Vecteur_1;
		}
	}

	//Si AB horizontal
	if(A.y == B.y)
	{
		p->y = A.y;
	}

	//Si CD horizontal
	if(C.y == D.y)
	{
		p->y = C.y;
	}

	//Si le point d'intersection est compris entre A et B et entre C et D
	if(p->x >= A.x && p->x <= B.x)
	{
		if(p->x >= C.x && p->x <= D.x)
		{
			//Classement par ordre des Y croissants
			if(s1->y >= s2->y)
			{
				A = *s2;
				B = *s1;
			}else
			{
				A = *s1;
				B = *s2;
			}

			if(t1->y >= t2->y)
			{
				C = *t2;
				D = *t1;
			}else
			{
				C = *t1;
				D = *t2;
			}

			if(p->y >= A.y && p->y <= B.y)
			{
				if(p->y >= C.y && p->y <= D.y)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

/*************************************************/



/*short ASTAR_LISSAGE(struct ASTAR_MAP *MAP)
{
	if(MAP->Nb_Passage_Points_Found < 2)
	{
		return OK;
	}

	short Index_Resultat_Lisse = 0;
	short Index_Trajet = 0;
	char Delta_X_Depart = 0;
	char Delta_Y_Depart = 0;
	char Delta_X_Arrivee = 0;
	char Delta_Y_Arrivee = 0;

	short Index_X_Current = 0, Index_Y_Current = 0;
	short New_X, New_Y;

	//Resultat du Astar : un tableau de menant vers la liste des noeuds a passer pour rejoindre l'arrivee
	static unsigned short Result_Lisse[(ASTAR_NODE_NUMBER_X * ASTAR_NODE_NUMBER_Y)/3];

	//Le resultat commencera pareil que je trajet d'origine
	Result_Lisse[Index_Resultat_Lisse++] = MAP->Result[Index_Trajet];

	//Calcul des indices des noeuds du point d'origine
	ASTAR_INDEX_TO_NOEUD(MAP->Result[Index_Trajet++], &Index_X_Current, &Index_Y_Current);

	//Boucle jusqu'a etre arrive au point d'arrive
	do
	{
		//Calcul des indices des noeuds du point suivant
		ASTAR_INDEX_TO_NOEUD(MAP->Result[Index_Trajet], &New_X, &New_Y);

		//Calcul des variation de deplacement entre les 2 noeuds
		Delta_X_Depart = New_X - Index_X_Current;
		Delta_Y_Depart = New_Y - Index_Y_Current;

		//Pour chacun des points suivant de la trajectoire calculee
		//Temps que la variation est constance
		for(; Index_Trajet < MAP->Nb_Passage_Points_Found;)
		{
			//Calcul des indices des noeuds du point d'origine
			ASTAR_INDEX_TO_NOEUD(MAP->Result[Index_Trajet], &Index_X_Current, &Index_Y_Current);

			//Calcul des indices des noeuds du point suivant
			ASTAR_INDEX_TO_NOEUD(MAP->Result[++Index_Trajet], &New_X, &New_Y);

			//Calcul la nouvelle Variation
			Delta_X_Arrivee = New_X - Index_X_Current;
			Delta_Y_Arrivee = New_Y - Index_Y_Current;

			//Si les variations sont constantes
			if(Delta_X_Depart == Delta_X_Arrivee && Delta_Y_Depart == Delta_Y_Arrivee)
			{
				//Continue la boucle, et teste le point suivant
				//Le point Courant est passe en "Path_Lisse" pour le Debug
				MAP->NODE_LIST[MAP->Result[Index_Trajet - 1]].State = Path_Lisse;
			}
			else
			{
				//Si il y a changement de l'angle entre 2 points se suivants
				//Ajoute le point courant (index_Trajet - 1) a la liste des resultats lisses
				Result_Lisse[Index_Resultat_Lisse++] = MAP->Result[Index_Trajet - 1];

				//Prends le point courant comme nouveau point de depart
				//Calcul des indices des noeuds du point d'origine
				ASTAR_INDEX_TO_NOEUD(MAP->Result[Index_Trajet - 1], &Index_X_Current, &Index_Y_Current);
				//Stop la boucle
				break;
			}
		}
	}while(Index_Trajet < MAP->Nb_Passage_Points_Found);

	//Au final, donne le nombre de nouveaux pts de passage trouve
	MAP->Nb_Passage_Points_Found = Index_Resultat_Lisse;
	//Copie la nouvelle trajectoire dans le resultat
	for(Index_Trajet = 0; Index_Trajet < MAP->Nb_Passage_Points_Found; Index_Trajet++)
	{
		MAP->Result[Index_Trajet] = Result_Lisse[Index_Trajet];
	}

	return OK;
}*/



/*************************************************/





/*****************************************************************************
 ** Function name:		Astar_Show_Map_Console
 **
 ** Descriptions:		Display the Map in the console
 **
 ** parameters:			Pointer to the Map
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_Debug_Display_Map(struct Astar_Map* map)
{
	vTaskSuspendAll();

	//Passe en TX
	_0_RS485_Master_Mode(RS485_DIR_PORT, RS485_DIR_BIT);


	Astar_DEBUG_Display_Map_Node_Status(map);
	Astar_DEBUG_Display_Map_Node_Access(map);

	//Passe en RX
	_0_RS485_Slave_Mode(RS485_DIR_PORT, RS485_DIR_BIT);

	xTaskResumeAll();
}


/*****************************************************************************
 ** Function name:		Astar_DEBUG_Display_Map_Node_Status
 **
 ** Descriptions:		Show the map nodes status in the console
 **
 ** parameters:			Ptr to the map
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_DEBUG_Display_Map_Node_Status(struct Astar_Map* map)
{
	//For each Node of the Map

	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
	Astar_Debug_Send_String("Astar Nodes States");
	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');

	//Start with a '#' line as upper terrain bondarie
	for(int x = 0; x < Astar_Node_Nb_X + 2; x++)
	{
		Astar_Debug_Send_char('#');
	}
	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');

	//Starting from the Upper raw
	//For each raw of the map
	for(int y = Astar_Node_Nb_Y - 1; y >= 0; y--)
	{
		//Each line starts with a '#' as bondarie
		Astar_Debug_Send_char('#');

		//For each colum of the map
		for(int x = 0; x < Astar_Node_Nb_X; x++)
		{
			//Display the Node state in the console
			Astar_debug_Display_Node_Status(&map->Nodes[x][y]);
		}

		//Each line ends with a '#' as bondarie
		Astar_Debug_Send_char('#');
		//New line
		Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
	}

	//End with a '#' line as Lower terrain bondarie
	for(int x = 0; x < Astar_Node_Nb_X + 2; x++)
	{
		Astar_Debug_Send_char('#');
	}

	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
}


/*****************************************************************************
 ** Function name:		Astar_debug_Display_Node_Status
 **
 ** Descriptions:		Display the status of ONE node in the console
 **
 ** parameters:			Ptr to the node
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_debug_Display_Node_Status(struct Astar_Node* node)
{
	char state;
	switch(node->Astar_Node_State)
	{
	case Not_Processed:
		state = ' ';
		break;
	case Open:
		state = 'O';
		break;
	case Closed:
		state = 'C';
		break;
	case Path:
		state = 'P';
		break;
	case Path_Lisse:
		state = 'L';
		break;

	default:
		state = ' ';
		break;
	}

	switch(node->Astar_Node_Access)
	{
	case Start_Node:
		state = 'S'; //End and Start ans inverted
		break;
	case End_Node:
		state = 'E'; //End and Start ans inverted
		break;
	case Blocked:
		state = 'X';
		break;
	case Limited:
		state = 'x';
		break;
	default:
		break;
	}
	Astar_Debug_Send_char(state);
}


/*****************************************************************************
 ** Function name:		Astar_DEBUG_Display_Map_Node_Access
 **
 ** Descriptions:		Show the map nodes acces in the console
 **
 ** parameters:			Ptr to the map
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_DEBUG_Display_Map_Node_Access(struct Astar_Map* map)
{
	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
	Astar_Debug_Send_String("Astar Nodes Access");
	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');

	//Start with a '#' line as upper terrain bondarie
	for(int x = 0; x < Astar_Node_Nb_X + 2; x++)
	{
		Astar_Debug_Send_char('#');
	}
	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');

	//Starting from the Upper raw
	//For each raw of the map
	for(int y = Astar_Node_Nb_Y - 1; y >= 0; y--)
	{
		//Each line starts with a '#' as bondarie
		Astar_Debug_Send_char('#');

		//For each colum of the map
		for(int x = 0; x < Astar_Node_Nb_X; x++)
		{
			//Display the Node state in the console
			Astar_debug_Display_Node_Access(&map->Nodes[x][y]);
		}

		//Each line ends with a '#' as bondarie
		Astar_Debug_Send_char('#');
		//New line
		Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
	}

	//End with a '#' line as Lower terrain bondarie
	for(int x = 0; x < Astar_Node_Nb_X + 2; x++)
	{
		Astar_Debug_Send_char('#');
	}

	//New line
	Astar_Debug_Send_char('\n');Astar_Debug_Send_char('\r');
}


/*****************************************************************************
 ** Function name:		Astar_debug_Display_Node_Access
 **
 ** Descriptions:		Display the acces of ONE node in the console
 **
 ** parameters:			Ptr to the node
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_debug_Display_Node_Access(struct Astar_Node* node)
{
	char state;
	switch(node->Astar_Node_Access)
	{
	case Walkable:
		state = ' ';
		break;
	case Limited:
		state = 'x';
		break;
	case Blocked:
		state = 'X';
		break;
	case Trajectory:
		state = '#';
		break;
	case Start_Node:
		state = 'S'; //End and Start ans inverted
		break;
	case End_Node:
		state = 'E'; //End and Start ans inverted
		break;

	default:
		state = ' ';
		break;
	}
	Astar_Debug_Send_char(state);
}


/*****************************************************************************
 ** Function name:		Astar_Debug_Send_string
 **
 ** Descriptions:		send a string to the console
 **
 ** parameters:			string
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_Debug_Send_String(char* str)
{
	int i = 0;
	while(str[i] != 0)
	{
		Astar_Debug_Send_char(str[i++]);
	}
}

/*****************************************************************************
 ** Function name:		Astar_Debug_Send_char
 **
 ** Descriptions:		send a value to the console
 **
 ** parameters:			value to display
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_Debug_Send_char(char ch)
{
	while((Chip_UART_ReadLineStatus(RS484_UART) & (UART_LSR_THRE | UART_LSR_OE | UART_LSR_PE)) == 0)
	{
		__asm volatile( "nop" );
	}
	Chip_UART_SendByte(RS484_UART, ch);
}


/*****************************************************************************
 ** Function name:		Astar_Add_Rectangular_Obstacle
 **
 ** Descriptions:		Set some Nodes add "Blocked" because they are inside an obstacle
 **
 ** parameters:			Ptr to the map
 ** 					Pointer to the vectors map
 ** 					Obstacle X min position (mm)
 ** 					Obstacle Y min position (mm)
 ** 					Obstacle X max position (mm)
 ** 					Obstacle Y max position (mm)
 ** 					Add corresponding vectors (bool)
 ** 					Couleur des vecteurs
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_Add_Rectangular_Obstacle(struct Astar_Map* map, struct Astar_smoothing_vector* vectors_map, short X_min, short Y_min, short X_max, short Y_max, bool use_vectors, enum Astar_Vector_Color color)
{
	uint8_t index_x_mini;
	uint8_t index_y_mini;
	uint8_t index_x_max;
	uint8_t index_y_max;

	if(X_min == 0)
		X_min = 1;

	if(X_max >= Map_size_X)
		X_max = Map_size_X -1;

	if(Y_min == 0)
		Y_min = 1;

	if(Y_max >= Map_size_Y)
		Y_max = Map_size_Y -1;

	index_x_mini = Astar_Coord_To_Index_X(X_min);
	index_y_mini = Astar_Coord_To_Index_Y(Y_min);

	index_x_max = Astar_Coord_To_Index_X(X_max);
	index_y_max = Astar_Coord_To_Index_Y(Y_max);

	if(use_vectors)
		Astar_Add_Obstacle_Vector(vectors_map,
				X_min,
				Y_min,
				X_max,
				Y_max, color);

	//for each node from index X_min to index X_max
	for(int x = index_x_mini; x <= index_x_max; x++)
	{
		//for each node from index Y_min to index Y_max
		for(int y = index_y_mini; y <= index_y_max; y++)
		{
			Astar_Set_Node_as_Blocked(&map->Nodes[x][y]);
		}
	}

}


/*****************************************************************************
 ** Function name:		Astar_Add_Obstacle_Vector
 **
 ** Descriptions:		Add some Obstacles vectors for smoothing
 **
 ** parameters:			Pointer to the vectors map
 ** 					Obstacle X min position (mm)
 ** 					Obstacle Y min position (mm)
 ** 					Obstacle X max position (mm)
 ** 					Obstacle Y max position (mm)
 ** 					Couleur des vecteurs
 ** Returned value:		None
 **
 *****************************************************************************/
void Astar_Add_Obstacle_Vector(struct Astar_smoothing_vector* vectors_map, short X_min, short Y_min, short X_max, short Y_max, enum Astar_Vector_Color color)
{
	struct Astar_Vector Vect;

	if(vectors_map->Nb_Vectors + 4 >= Astar_Vector_Max_Nb)
		return;

	Vect.Color = color;

	Vect.Start_Point.x = (uint16_t)X_min;
	Vect.Start_Point.y = (uint16_t)Y_min;
	Vect.End_Point.x = (uint16_t)X_max;
	Vect.End_Point.y = (uint16_t)Y_min;
	vectors_map->Vectors[vectors_map->Nb_Vectors++] = Vect;

	Vect.Start_Point.x = X_max;
	Vect.Start_Point.y = Y_min;
	Vect.End_Point.x = X_max;
	Vect.End_Point.y = Y_max;
	vectors_map->Vectors[vectors_map->Nb_Vectors++] = Vect;

	Vect.Start_Point.x = X_max;
	Vect.Start_Point.y = Y_max;
	Vect.End_Point.x = X_min;
	Vect.End_Point.y = Y_max;
	vectors_map->Vectors[vectors_map->Nb_Vectors++] = Vect;

	Vect.Start_Point.x = X_min;
	Vect.Start_Point.y = Y_max;
	Vect.End_Point.x = X_min;
	Vect.End_Point.y = Y_min;
	vectors_map->Vectors[vectors_map->Nb_Vectors++] = Vect;
}

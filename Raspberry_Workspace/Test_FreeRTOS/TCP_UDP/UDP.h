/*
 * UDP.h
 *
 *  Created on: 11 nov. 2020
 *      Author: pi
 */

#ifndef UDP_H_
#define UDP_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#define COPYDATA(X, Y)	sizeof(X) > sizeof(Y) ? sizeof(Y) : sizeof(X); memcpy(&Y, &X, sizeof(Y));
#define COPYDATA2(X, Y)	memcpy(&Y, &X, sizeof(Y));
#define COPYSTRING(X, Y) strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X); memcpy(&Y, X, strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X));



void Init_UDP(int);


void Task_UDP(void* pvparameter);

/**************************************************
Definition des commandes de communication
 **************************************************/
enum Com_Instruction
{
	//Destination Robot
	DESTINATION_ROBOT = 0,
	DEPLACEMENT_SPLINE,
	DEPLACEMENT_CERCLE,
	FLUSH_DEPLACEMENT,
	DEPLACEMENT_SPLINE_CUBIQUE,

	//Position Robot
	SET_ROBOT_POSITION = 8,
	DEMANDE_ROBOT_POSITION,
	REPONSE_ROBOT_POSITION,
	REPONSE_AUTRE_ROBOT_POSITION,

	//Robot Adverse
	DEMANDE_POSITION_ROBOT_ADVERSE = 13,
	REPONSE_POSITION_ROBOT_ADVERSE,

	//Moteurs Aux
	DESTINATION_MOTEURS_AUX,
	DEMANDE_POSITION_MOTEURS_AUX,
	REPONDE_POSITION_MOTEURS_AUX,
	RECALAGE_MOTEUR_AUX,

	DESTINATION_SERVOS_AND_AX12 = 20
};


/**************************************************
Declaration de la definition de la Structure contenant la position du Robot à mettre à jour
 **************************************************/
struct st_POSITION_ROBOT
{
	float Position_X;
	float Position_Y;
	float Angle_rad;
	float Angle_Deg;

	long orient_init;
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter la position du Robot vers la carte IA ou vers le PC
 **************************************************/
struct Com_Position_Robot
{
	//Instruction 10		//7 octets + alignement = 8
	//Position du Robot

	uint8_t Numero_Robot;		//0 = PR, 1 = GR
	int16_t Position_X;			//*10
	int16_t Position_Y;			//*10
	int16_t Angle;				//*100 //Orientation du Robot (° *100)
	//byte Bloquage;
	//byte Fin_Deplacement;
};

/*************************************************/


/**************************************************
Definition du nombre maximum de donnees dans une communication
 **************************************************/
#define COMMUNICATION_TRAME_MAX_DATA  64


/**************************************************
Definition des adresses des modules Xbee
 **************************************************/
enum Adress_Xbee
{
	XBee_PC,
	Gros_Robot,
	Petit_Robot,
	Balise_Gros_Robot,
	Balise_Petit_Robot,
	ALL_XBEE = 0xFFFF
};


/**************************************************
Definition des adresses des escalves
 **************************************************/
enum Slave_Adresses
{
	BALISE_BOARD = -3,
	PC = -2,
	ALL_CARDS = -1,
	IA_BOARD = 0
};


/**************************************************
Declaration de la definition de la Structure de communication contenant un message
 **************************************************/
struct Communication_Trame
{
	enum Com_Instruction Instruction;                           //Instruction a echanger

	enum Slave_Adresses Slave_Adresse;                          //Adresse de la carte ciblée //ALL_CARDS (-1) pour toutes les cartes

	uint8_t Length;                                             	//Nombre d'octets dans le tableau de data

	uint8_t Data[COMMUNICATION_TRAME_MAX_DATA];

	enum Adress_Xbee XBEE_DEST_ADDR;							//Adresse de la cible Xbee en mode API
};


struct Communication_Message
{
	uint8_t length;												//Longueur du message à envoyer
	uint8_t Data[COMMUNICATION_TRAME_MAX_DATA + 11];				//Data = en-tête + trame ("Communication_Trame") + pied
};


void _2_Comm_Send_Robot_Position(struct st_POSITION_ROBOT rob_pos);

void _1_Communication_Create_Trame(struct Communication_Trame *pMessage_to_send);


#endif /* UDP_H_ */

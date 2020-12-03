/*
 * Communication_Structures.h
 *
 *  Created on: 2 déc. 2020
 *      Author: PC-INOVA
 */

#ifndef COMMUNICATION_STRUCTURES_H_
#define COMMUNICATION_STRUCTURES_H_

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

	//servo move
	DESTINATION_SERVOS_AND_AX12 = 20,

	//Info sur la carte
	DEFINITION_ID_ROBOT = 34,		//Permet à la carte ID de donner l'ID du robot sur les autres cartes

	DEMANDE_INFO = 35,
	REPONSE_INFO,                   //Toutes les infos d'une carte dans un seul message
	REPONSE_INFO_IA,

	//Power
	DEMANDE_MOTEURS_POWER = 38,
	DEMANDE_MOTEURS_AUX_POWER,
	DEMANDE_SERVO_POWER,
	DEMANDE_AX_12_POWER,
	DEMANDE_SIMULATION_MOTEURS,
	DEMANDE_SIMULATION_MOTEURS_AUX,

	//Parametres
	PARAMETRES_ODOMETRIE = 45,
	PARAMETRES_PID,
	VITESSE_ROBOT,
	VITESSE_MOTEURS_AUX,
	TYPE_ASSERV,                    //type d'asservissement: 0=Polaire; 1=Moteurs Aux

	//Bloc 50 a 54 reserve pour la strategie
	STRATEGIE_CHANGEMENT_ETAT = 51,

	//Logger
	LOGGER_DEBUG = 56,

	//ASTAR
	ASTAR_CONTENU,
	ASTAR_VECTEURS,

	//Bootloader
	BOOTLOADER,
	BOOTLOADER_ACK,

	//Ping
	PING = 252,
	PONG,

	//End Communication
	END_COMMUNICATION = 254,

	//Test
	TEST_COMMUNICATION = 255,
};


/**************************************************
Definition du nombre maximum de donnees dans une communication
 **************************************************/
#define COMMUNICATION_TRAME_MAX_DATA  64


/**************************************************
Definition des adresses des modules Xbee
 **************************************************/
enum Adress_Xbee
{
	Xbee_address_Gros_Robot = 0x01,
	Xbee_address_Petit_Robot,
	Xbee_address_Balise_Gros_Robot,
	Xbee_address_Balise_Petit_Robot,
	XBee_PC = 0x0A,
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

	uint8_t Length;                                             //Nombre d'octets dans le tableau de data

	uint8_t Data[COMMUNICATION_TRAME_MAX_DATA];

	enum Adress_Xbee XBEE_DEST_ADDR;							//Adresse de la cible Xbee en mode API
};

enum enum_canal_communication
{
	USB_port,
	RS485_port,
	RS485_Debug_port,
	Xbee_port
};


struct Communication_Message
{
	uint8_t length;												//Longueur du message à envoyer
	uint8_t Data[COMMUNICATION_TRAME_MAX_DATA + 11];			//Data = en-tête + trame ("Communication_Trame") + pied
	enum enum_canal_communication canal_communication;			//Indique quel port de communication utiliser pour envoyer ce message
};


#endif /* COMMUNICATION_STRUCTURES_H_ */

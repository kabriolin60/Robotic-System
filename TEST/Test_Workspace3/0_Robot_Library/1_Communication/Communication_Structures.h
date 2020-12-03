/*
 * Communication_Structures.h
 *
 *  Created on: 2 déc. 2020
 *      Author: PC-INOVA
 */

#ifndef COMMUNICATION_STRUCTURES_H_
#define COMMUNICATION_STRUCTURES_H_


//Copie des donnees d'une struct a l'autre
#define COPYDATA(X, Y)	sizeof(X) > sizeof(Y) ? sizeof(Y) : sizeof(X); memcpy(&Y, &X, sizeof(Y));
#define COPYDATA2(X, Y)	memcpy(&Y, &X, sizeof(Y));
#define COPYSTRING(X, Y) strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X); memcpy(&Y, X, strlen(X)> sizeof(Y) ? sizeof(Y) : strlen(X));


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



/**************************************************
Declaration de la definition de la Structure de communication permettant d'envoyer un message de log
 **************************************************/
enum Logger_Debug_Color
{
	Color_Black,
	Color_Blue,
	Color_Red
};

enum Channel_Debug
{
	Channel_Debug_Communication,
	Channel_Debug_Deplacement,
	Channel_Debug_ASTAR,
	Channel_Debug_Test,
	Channel_Debug_Divers,
	TBD,
	Channel_Debug_Error
};


#define Max_Char_per_Log 59
struct Logger_Debug_Data
{
	uint8_t Nombre_Carateres;								//Nombre d'octets de la chaine de commentaires à suivre						//1 octet

	uint8_t Channel;										//Cannal de Debug utilisé													//1 octet

	enum Logger_Debug_Color Color;                      //Couleur du texte à afficher												//1 octet

	char Text[Max_Char_per_Log];						//Chaine de commentaire limitée au nombre ci-dessus, maximum 60 caratcers 	//60 octets
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

	float orient_init;
};

/*************************************************/




#define NB_SERVO	6
#define NB_AX_12	4
#define NB_MES_ANA	4


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter la position du Robot vers la carte IA ou vers le PC
 **************************************************/
struct Com_Position_Robot
{
	//Instruction 10		//7 octets + alignement = 8
	//Position du Robot

	int16_t Position_X;			//*10
	int16_t Position_Y;			//*10
	int16_t Angle;				//*100 //Orientation du Robot (° *100)
	//byte Bloquage;
	//byte Fin_Deplacement;
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter les positions des servos
 **************************************************/
struct Com_Position_Servos
{
	unsigned short Position[NB_SERVO];
};
/*************************************************/


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter les positions des AX-12
 **************************************************/
struct Com_Position_AX12
{
	//4 Octets par AX_12

	unsigned short Position[NB_AX_12];
	short Torque[NB_AX_12];
};
/*************************************************/


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter les mesures analogiques
 **************************************************/
struct Com_Mesures_Analogiques
{
	//Instruction 52	//2 Octets par mesure, soit 8 octets
	//Reponse aux mesures analogiques

	unsigned short Mesure[NB_MES_ANA];
};



/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter l'ensemble des infos de la carte vers la carte IA ou vers le PC
 **************************************************/
struct Com_Reponse_Info
{
	//instruction 36		//58 + alignement = 60 octets

	uint8_t Numero_Robot;											//0 = PR, 1 = GR

	uint8_t Numero_Carte;											//Numero de la carte emettant le message // 1 octet

	uint8_t Etat_Alim; //0= motor power; 1 = motor aux power; 2 = servos power; 3 = ax12 power; 4 = AUX 1 power; 5 = Aux 2 power					 //1 octet

	uint8_t Etat_Contacteurs; //0= FDC 0; 1 = FDC 1; 2 = CTC 0; 3 = CTC 1; 4 = CTC 2; 5 = CTC 3					 //1 octet

	uint8_t Etat_IO;          //0= IO 0; 1 = IO 1; 2 = IO 2; 3 = IO 3

	struct Com_Position_Robot PositionRobot;					//8 octets

	struct Com_Position_Servos Position_Servos;					//12 octets

	struct Com_Position_AX12 Position_AX12;						//16 octets

	struct Com_Mesures_Analogiques Mesures_Analogiques;			//8 octets

	unsigned short Tension_Batterie;							//Tension * 100		//2 octets
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter l'ensemble des infos de la carte IA vers le PC
 **************************************************/

struct Com_Reponse_Info_IA
{
	//instruction 	37
	uint8_t Numero_Robot;

	uint8_t Strategie;

	unsigned short Temps_Match;                             //Temps /10		//2 octets

	uint8_t Etat_Inputs; //0= Jack; 1 = Color; 2 = Switchs; 3 = LED Red; 4 = LED Yellow; 5 = LED Green					 //1 octet
};

/*************************************************/

#endif /* COMMUNICATION_STRUCTURES_H_ */

/*
 * Communication_Typedef.h
 *
 *  Created on: 16 avr. 2020
 *      Author: kabri
 */

#ifndef COMMUNICATION_TYPEDEF_H_
#define COMMUNICATION_TYPEDEF_H_

#include "FreeRTOS.h"
#include "Type_Declaration.h"

#include "stdbool.h"


#define BAUDRATE_RS485		2000000

#define	BAUDRATE_RS485_2	2000000



#define NB_SERVO		6
#define NB_AX_12		4
#define NB_MES_ANA		4

/**************************************************
Definition des commandes de communication
 **************************************************/
enum Com_Instruction
{
	ACKNOWLEDGE = 0,

	//Destination Robot
	DESTINATION_ROBOT = 10,
	DESTINATION_ROBOT_SPLINE_CUBIQUE,

	//Position Robot
	SET_ROBOT_POSITION = 20,
	POSITION_AUTRE_ROBOT,

	//Servos et AX12
	DESTINATION_SERVOS_AND_AX12 = 30,

	//Info sur la carte
	DEFINITION_ID_ROBOT = 40,       //Permet à la carte ID de donner l'ID du robot sur les autres cartes
	DEMANDE_INFO,					//Demande les infos d'une carte
	REPONSE_INFO_Multi_FCT,         //Toutes les infos d'une carte dans un seul message
	REPONSE_INFO_IA,                //Toutes les infos de la carte IA

	//Power
	DEMANDE_MOTEURS_POWER = 50,
	DEMANDE_MOTEURS_AUX_POWER,
	DEMANDE_SERVO_POWER,
	DEMANDE_AX_12_POWER,
	DEMANDE_SIMULATION_MOTEURS,
	DEMANDE_SIMULATION_MOTEURS_AUX,

	//Parametres
	PARAMETRES_ODOMETRIE = 60,
	PARAMETRES_PID,

	//Vitesse du robot
	VITESSE_ROBOT = 70,
	VITESSE_MOTEURS_AUX,

	//Bloc 70 reserve pour la strategie
	STRATEGIE_CHANGEMENT_ETAT = 80,
	//STRATEGIE_ENSEMBLE_ACTIONS,
	//STRATEGIE_DIALOGUE_ENTREROBOTS,
	//STRATEGIE_ETAT_INTERNE,

	//Logger
	LOGGER_DEBUG = 90,

	//ASTAR
	ASTAR_CONTENU = 100,
	ASTAR_VECTEURS,

	//Graphiques
	GRAPHIQUES_ADD_DATA = 110,
	GRAPHIQUES_CLEAR,

	//Balises
	BALISE_START = 200,         //Start moteur et lasers
	BALISE_POSITION,            //Position du robot donnant la position centrale de la balise
	BALISE_POSITION_ADVERSAIRES,       //Retour sur la position des adversaires detectée par la balise

	//Bootloader
	BOOTLOADER = 240,
	BOOTLOADER_ACK,

	//Lecture en Flash
	FLASH_CHANGE_FILE = 250,    //Demande un changement de fichier à l'IHM lors de l'enregistrement

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
	INTERFACE_PC = -4,
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

	byte Length;                                             	//Nombre d'octets dans le tableau de data

	byte Data[COMMUNICATION_TRAME_MAX_DATA];

	enum Adress_Xbee XBEE_DEST_ADDR;							//Adresse de la cible Xbee en mode API
};

enum enum_canal_communication
{
	USB_port,
	RS485_port,
	RS485_2_port,
	Xbee_port
};


/**************************************************
Declaration de la definition de la Structure de communication
 **************************************************/
struct Communication_Message
{
	byte length;												//Longueur du message à envoyer
	byte Data[COMMUNICATION_TRAME_MAX_DATA + 11];				//Data = en-tête + trame ("Communication_Trame") + pied
	enum enum_canal_communication canal_communication;			//Indique quel port de communication utiliser pour envoyer ce message
};


enum enum_ACK_Types
{
	ACK_POSITION_ROBOT,
	ACK_POWER_MOTOR,
	ACK_POWER_SERVO,
	ACK_POWER_AX_12,
	ACK_DEPLACEMENT,
	ACK_MOVE_SERVOS,
	ACK_DEMANDE_SIMULATION_MOTEURS,
	ACK_PARAMETRES_ODOMETRIE,
	ACK_PARAMETRES_PID,
	ACK_VITESSE_ROBOT,
	ACK_DEFINITION_ID_ROBOT,
};

/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter un ACK à un ordre reçu
 **************************************************/
struct Communication_ACK
{
	byte Adresse;		//Adresse de la carte envoyant l'ACK

	enum enum_ACK_Types ACK_TYPE;	//Quel a été le type de message reçu auquel répondre
};


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter un PONG
 **************************************************/
struct Communication_PONG
{
	byte Adresse;		//Adresse de la carte envoyant le PONG
	long Nombre_Messages_Recus;		//Infos sur l'aspect communication de cette carte
	long Nombre_Erreurs_Communication;
};


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter la position du Robot vers la carte IA ou vers le PC
 **************************************************/
/*enum Communication_Ports
{
	RS485_1,
	RS485_2,
	I2C_1,
	I2C_2,
	USB,
	XBEE
};*/



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
	byte Bloquage;
};


struct Com_Position_Robot_Inter_Robots
{
	//Instruction 10		//7 octets + alignement = 8
	//Position du Robot

	byte Robot_ID;

	struct Com_Position_Robot Position;
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
Declaration de la definition de la Structure de communication permettant de définir l'ID du robot (IA vers autres cartes)
 **************************************************/
struct Com_Def_Id_Robot
{
	//instruction 34		//58 + alignement = 60 octets

	byte Numero_Robot;											//0 = PR, 1 = GR
};

/*************************************************/



/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter l'ensemble des infos de la carte vers la carte IA ou vers le PC
 **************************************************/
struct Com_Reponse_Info
{
	//instruction 36		//58 + alignement = 60 octets

	byte Numero_Robot;											//0 = GR, 1 = PR

	byte Numero_Carte;											//Numero de la carte emettant le message // 1 octet

	byte Etat_Alim; //0= motor power; 1 = motor aux power; 2 = servos power; 3 = ax12 power; 4 = AUX 1 power; 5 = Aux 2 power					 //1 octet

	byte Etat_Contacteurs; //0= FDC 0; 1 = FDC 1; 2 = CTC 0; 3 = CTC 1; 4 = CTC 2; 5 = CTC 3					 //1 octet

	byte Etat_IO;          //0= IO 0; 1 = IO 1; 2 = IO 2; 3 = IO 3

	struct Com_Position_Robot PositionRobot;					//8 octets

	struct Com_Position_Servos Position_Servos;					//12 octets

	struct Com_Position_AX12 Position_AX12;						//16 octets

	struct Com_Mesures_Analogiques Mesures_Analogiques;			//8 octets

	unsigned short Tension_Batterie;							//Tension * 100		//2 octets

	//struct Com_Position_Moteurs_Aux_Light PositionMoteursAux;	//4 octets

};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter l'ensemble des infos de la carte IA vers le PC
 **************************************************/

struct Com_Reponse_Info_IA
{
	//instruction 	37
	byte Numero_Robot;

	byte Strategie;

	unsigned short Temps_Match;                             //Temps /10		//2 octets

	byte Etat_Inputs; //0= Jack; 1 = Color; 2 = Switchs; 3 = LED Red; 4 = LED Yellow; 5 = LED Green					 //1 octet

	long Boards_Comm_Status;			//== valeur _0_Status_EventGroup == présence des cartes sur le bus par leur adresses

	unsigned short Tension_Batterie;							//Tension * 100		//2 octets
};

/*************************************************/


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
	Channel_Debug_Match,
	Channel_Debug_Error
};


#define Max_Char_per_Log 59
struct Logger_Debug_Data
{
	byte Nombre_Carateres;								//Nombre d'octets de la chaine de commentaires à suivre						//1 octet

	byte Channel;										//Cannal de Debug utilisé													//1 octet

	enum Logger_Debug_Color Color;                      //Couleur du texte à afficher												//1 octet

	char Text[Max_Char_per_Log];						//Chaine de commentaire limitée au nombre ci-dessus, maximum 60 caratcers 	//60 octets
};


/**************************************************
Declaration des enum contenant les types de déplacements
 **************************************************/

enum enum_TYPE_MOVE_
{
	TYPE_MOVE_aucun_mouvement,             //Aucun mouvement
	TYPE_MOVE_xy_tour_av_avant,            //Déplacement standard en marche avant
	TYPE_MOVE_xy_tour_av_arriere,          //Déplacement standard en marche arriere
	TYPE_MOVE_tourne_vers_cap_rad,         //Oriente le robot selon l'angle donné en absolu
	TYPE_MOVE_deplacement_libre,           //Permet de faire des tests en déplacement linéaires
	TYPE_MOVE_rotation_libre,              //Permet de faire des tests en rotaion
	TYPE_MOVE_consigne_vitesse_independantes,  //Permet de piloter les roues en vitesse sans passer par les couches supérieures d'asserv

	TYPE_MOVE_Spline_avant,            		//Déplacement standard en marche avant
	TYPE_MOVE_Spline_arriere,          		//Déplacement standard en marche arriere
};

/*************************************************/


/**************************************************
Declaration des enum contenant les parametres de déplacement
 **************************************************/

enum enum_TYPE_ARRET_
{
	depla_AVEC_freinage,	//Le robot freine et s'arret au point cible
	depla_SANS_freinage	//le robot ne freine pas et ne s'arrete pas au point cible
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant les parametres pour les déplacements
 **************************************************/

struct st_Parametre_Deplacement
{
	uint16_t Distance_Detection_Fin_Trajectoire;           //distance separant le robot de son objectif avant passage au point suivant * 100
	uint8_t Angle_Detection_Fin_Trajectoire;               //angle separant le robot de son objectif avant passage au point suivant pour les rotations * 100
	uint8_t Angle_Avant_Debut_Avance;                      //angle separant le robot de son objectif pour qu'il commence à avancer * 100
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant des coordonnees
 **************************************************/

struct st_COORDONNEES
{
	short X;                     //coordonnee X
	short Y;                     //coordonnee Y
	short Angle;                 //angle pour les rotations vers un cap absolu * 100
	short Distance;              //Indique la distance a parcourir pour les deplacements en distance libre, ou le rayon des cercles

	short Final_X;
	short Final_Y;

	short Vitesse_Roue_Gauche;
	short Vitesse_Roue_Droite;

	enum enum_TYPE_MOVE_ Type_Deplacement; 	//choix du type de deplacement

	enum enum_TYPE_ARRET_ Type_Arret;		//Type d'arret au point cible

	struct st_Parametre_Deplacement ptrParameters;
};

/*************************************************/


/*
 * Splines cubique
 */
struct CubicSpline_Point
{
	short X;
	short Y;
};

/**************************************************
Declaration de la definition de la Structure contenant des coordonnees de deplacement en Spline cubique
 **************************************************/
struct CubicSpline
{
	struct CubicSpline_Point P0;
	struct CubicSpline_Point M0;
	struct CubicSpline_Point P1;
	struct CubicSpline_Point M1;

	byte Direction;
	//0= Marche avant
	//1= Marche arriere

	byte Nombre_Points;
	struct CubicSpline_Point Taille_Terrain;

	byte Use_Current_speed;
	//0= Utilisation des valeurs de M0 fournies par l’IA
	//1= Utilise la vitesse courante comme vecteur M0

	struct st_Parametre_Deplacement ptrParameters;

	bool Replace;							// if == true, clear the current mouvement buffer, and add this new one
};


/**************************************************
Declaration de la definition de la Structure contenant des coordonnees de destination d'un robot
 **************************************************/
struct st_DESTINATION_ROBOT
{
	struct st_COORDONNEES coord;
	bool Replace;							// if == true, clear the current mouvement buffer, and add this new one
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure contenant les destination des servos (Servos et AX12 indifferement)
 **************************************************/
#define Communication_Nombre_Servos_Max_Message 10
struct servo_destination
{
	unsigned short ID;								//Identifiant du servos a deplacer
	unsigned short Destination;				//position a atteindre par le servo
	unsigned short Torque;					//Maximum torque, pour les AX12
};

struct st_Destination_Servos
{
	unsigned short Nombre_servos_to_move;				//Nombre de servos à mettre à jour
	unsigned short Time_to_move;			//Duree du deplacement demande
	struct servo_destination servo[Communication_Nombre_Servos_Max_Message];		//Tableau des destinations des servos
};
/*************************************************/


/**************************************************
Declaration de la definition de la Structure de passage en simulation ou non des moteurs
 **************************************************/
struct Simulation_Deplacement
{
	bool simulation;
};

/*************************************************/

/**************************************************
Declaration de la definition de la Structure d'alimentation des servos et aX12
 **************************************************/
struct SERVOS_AX12
{
	bool power;
};

/*************************************************/


/**************************************************
Declaration de la definition de la Structure d'activation de la puissance des moteurs
 **************************************************/
struct Motor_Power
{
	bool power_Gauche;
	bool power_Droite;
};

/*************************************************/



/**************************************************
Declaration de la definition de la Structure contenant les parametres de PID reçus
 **************************************************/
enum PID_Id
{
	PID_Id_vitesse_roues_independantes,
	PID_Id_vitesse_distance,
	PID_Id_vitesse_orientation,
	PID_Id_distance,
	PID_Id_orientation
};

struct reglage_pid
{
	enum PID_Id id;

	float P;
	float I;
	float D;

	byte Min_Max;

	short Cumul;

	byte Sommation;

	byte Enable;
};


/**************************************************
Declaration de la definition de la Structure contenant les parametres de vitesse reçus
 **************************************************/
struct reglage_speed
{
	unsigned short Vitesse_Avance;	//m/s*1000
	unsigned short Accel_Avance;	//m/s²*1000
	unsigned short Deccel_Avance;	//m/s²*1000

	unsigned short Vitesse_Rotation;//rad/s*1000
	unsigned short Accel_Rotation;	//rad/s²*1000
	unsigned short Deccel_Rotation;	//rad/s²*1000
};



/**************************************************
Declaration de la definition de la Structure contenant les valeurs à afficher dans le graphique
 **************************************************/
struct st_Graph_Data
{
	byte Channel;
	float Data;
};

struct st_Graph_Datas
{
	byte nb_datas_to_send;
	struct st_Graph_Data Datas[7];
};


/**************************************************
Declaration de la definition de la Structure contenant les valeurs du ASTAR
 **************************************************/
struct st_ASTAR_Data
{
	short line_id;
	byte Node_Data[120/2];
};


/**************************************************
Declaration de la definition de la Structure contenant les valeurs des vecteurs du ASTAR
 **************************************************/
#define NB_ASTAR_Vecteur_Par_Message 6

struct st_ASTAR_Vecteur
{
	byte Color:8;
	unsigned short Start_X;
	unsigned short Start_Y;
	unsigned short End_X;
	unsigned short End_Y;
};

struct st_ASTAR_VECTEURS
{
	byte Effacement;
	byte Nb_vecteurs;

	struct st_ASTAR_Vecteur Vecteurs[NB_ASTAR_Vecteur_Par_Message];
};




struct Point_multiFCT
{
	unsigned short x;       			       //Coordonnee X du point
	unsigned short y;       			       //Coordonnee Y du point
};

enum Astar_Vector_Color_multiFCT
{
	Astar_Vector_Color_Black_multiFCT,
	Astar_Vector_Color_Blue_multiFCT,
	Astar_Vector_Color_Green_multiFCT,
	Astar_Vector_Color_Red_multiFCT,
	Astar_Vector_Color_White_multiFCT,
};

struct Astar_Vector_multiFCT
{
	struct Point_multiFCT Start_Point;
	struct Point_multiFCT End_Point;
	enum Astar_Vector_Color_multiFCT Color;
};

struct Astar_smoothing_vector_multiFCT
{
	//Smoothing vectors array
	struct Astar_Vector_multiFCT Vectors[100];
	//Number of vector in the array
	unsigned char Nb_Vectors;
};




#endif /* COMMUNICATION_TYPEDEF_H_ */

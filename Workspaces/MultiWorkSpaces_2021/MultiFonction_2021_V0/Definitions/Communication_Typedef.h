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


#define NB_SERVO	6
#define NB_AX_12	4
#define NB_MES_ANA	4

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

	//Servo
	DESTINATION_SERVO = 20,
	DEMANDE_POSITION_SERVO,
	REPONSE_POSITION_SERVO,

	//AX_12
	DESTINATION_AX_12 = 24,
	DEMANDE_POSITION_AX_12,
	REPONSE_POSITION_AX_12,

	//Mesures analogiques
	DEMANDE_MESURES_ANALOGIQUES = 28,
	REPONSE_MESURES_ANALOGIQUES,

	//Mesures digitales
	DEMANDE_MESURES_DIGITALES = 31,
	REPONSE_MESURES_DIGITALES,
	SET_SORTIES_DIGITALES,

	//Info sur la carte
	DEMANDE_INFO = 35,
	REPONSE_INFO,                   //Toutes les infos d'une carte dans un seul message

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
	STRATEGIE_ENSEMBLE_ACTIONS,
	STRATEGIE_DIALOGUE_ENTREROBOTS,
	STRATEGIE_ETAT_INTERNE,

	//Logger
	LOGGER_DEBUG = 56,

	//ASTAR
	ASTAR_CONTENU,
	ASTAR_VECTEURS,

	//Graphiques
	GRAPHIQUES_ADD_DATA,
	GRAPHIQUES_CLEAR,

	//CLI
	CLI = 65,

	//RESET
	RESET_CMD,

	//Bootloader
	BOOTLOADER,
	BOOTLOADER_ACK,

	//Balises
	BALISE_START = 100,         //Start moteur et lasers
	BALISE_POSITION,            //Position du robot donnant la position centrale de la balise
	BALISE_POSITION_ADVERSAIRES,       //Retour sur la position des adversaires detectée par la balise

	//Lecture en Flash
	FLASH_CHANGE_FILE = 250,    //Demande un changement de fichier à l'IHM lors de l'enregistrement

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

	byte Length;                                             	//Nombre d'octets dans le tableau de data

	byte Data[COMMUNICATION_TRAME_MAX_DATA];

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
	byte length;												//Longueur du message à envoyer
	byte Data[COMMUNICATION_TRAME_MAX_DATA + 11];				//Data = en-tête + trame ("Communication_Trame") + pied
	enum enum_canal_communication canal_communication;			//Indique quel port de communication utiliser pour envoyer ce message
};


/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter la position du Robot vers la carte IA ou vers le PC
 **************************************************/
enum Communication_Ports
{
	RS485_1,
	RS485_2,
	I2C_1,
	I2C_2,
	USB,
	XBEE
};



/**************************************************
Declaration de la definition de la Structure de communication permettant de faire remonter la position du Robot vers la carte IA ou vers le PC
 **************************************************/
struct Com_Position_Robot
{
	//Instruction 10		//7 octets + alignement = 8
	//Position du Robot

	byte Numero_Robot;			//0 = PR, 1 = GR
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
	//instruction 101		//58 + alignement = 60 octets

	byte Numero_Carte;											//Numero de la carte emettant le message // 1 octet

	byte Etat_Alim;	//7=Simu 6=SimuAux 5=PowerMotor 4=PowerAux 3=PowerAX12 2=PowerServo					 //1 octet

	struct Com_Position_Robot PositionRobot;					//8 octets

	struct Com_Position_Servos Position_Servos;					//12 octets

	struct Com_Position_AX12 Position_AX12;						//16 octets

	struct Com_Mesures_Analogiques Mesures_Analogiques;			//8 octets


	unsigned short Tension_Batterie;							//Tension * 100		//2 octets

	/*struct Com_Position_Moteurs_Aux_Light PositionMoteursAux;	//4 octets

	struct Com_EntresSorties_Digitales_Light ValeursDigitales; 	//2 octets

	UInt16 Timer;												//Temps du match en 10*ms //2 octets
	 */
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


#define Max_Char_per_Log 60
struct Logger_Debug_Data
{
	byte Nombre_Carateres;								//Nombre d'octets de la chaine de commentaires à suivre						//1 octet

	enum Logger_Debug_Color Color;                            //Couleur du texte à afficher												//1 octet

	char Text[Max_Char_per_Log];			//Chaine de commentaire limitée au nombre ci-dessus, maximum 60 caratcers 	//60 octets
};


/**************************************************
Declaration des enum contenant les types de déplacements
 **************************************************/

enum enum_TYPE_MOVE_
{
	aucun_mouvement,             //Aucun mouvement
	xy_tour_av_avant,            //Déplacement standard en marche avant
	xy_tour_av_arriere,          //Déplacement standard en marche arriere
	tourne_vers_cap_rad,         //Oriente le robot selon l'angle donné en absolu
	deplacement_libre,           //Permet de faire des tests en déplacement linéaires
	rotation_libre,              //Permet de faire des tests en rotaion
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

	enum enum_TYPE_MOVE_ Type_Deplacement; 	//choix du type de deplacement

	enum enum_TYPE_ARRET_ Type_Arret;		//Type d'arret au point cible

	struct st_Parametre_Deplacement ptrParameters;
};

/*************************************************/


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
Declaration de la definition de la Structure contenant les parametres de PID reçus
 **************************************************/
enum PID_Id
{
	vitesse_roues_independantes,
	vitesse_distance,
	vitesse_orientation,
	distance,
	orientation
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



#endif /* COMMUNICATION_TYPEDEF_H_ */

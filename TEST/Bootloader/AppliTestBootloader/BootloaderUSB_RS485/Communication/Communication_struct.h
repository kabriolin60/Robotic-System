/*
 * Communication_struct.h
 *
 *  Created on: 1 févr. 2014
 *      Author: Fixe
 */

#ifndef COMMUNICATION_STRUCT_H_
#define COMMUNICATION_STRUCT_H_

#include "Communication_NG.h"

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short UInt16;
typedef short Int16;
typedef unsigned int UInt32;
typedef int Int32;

#ifndef NB_MOT_AUX
#define NB_MOT_AUX 2
#define NB_SERVO 6
#define NB_AX_12 4
#define NB_MES_ANA 4
#endif


enum Com_Instruction
{
	//Destination Robot
	DESTINATION_ROBOT = 0,
	DEPLACEMENT_SPLINE,
	DEPLACEMENT_CERCLE,
	FLUSH_DEPLACEMENT,

	//Position Robot
	SET_ROBOT_POSITION,
	DEMANDE_ROBOT_POSITION,
	REPONSE_ROBOT_POSITION,
	REPONSE_AUTRE_ROBOT_POSITION,

	//Robot Adverse
	DEMANDE_POSITION_ROBOT_ADVERSE = 8,
	REPONSE_POSITION_ROBOT_ADVERSE,

	//Moteurs Aux
	DESTINATION_MOTEURS_AUX,
	DEMANDE_POSITION_MOTEURS_AUX,
	REPONDE_POSITION_MOTEURS_AUX,
	RECALAGE_MOTEUR_AUX,

	//Servo
	DESTINATION_SERVO = 15,
	DEMANDE_POSITION_SERVO,
	REPONSE_POSITION_SERVO,

	//AX_12
	DESTINATION_AX_12 = 18,
	DEMANDE_POSITION_AX_12,
	REPONSE_POSITION_AX_12,

	//Mesures analogiques
	DEMANDE_MESURES_ANALOGIQUES = 21,
	REPONSE_MESURES_ANALOGIQUES,

	//Mesures digitales
	DEMANDE_MESURES_DIGITALES = 23,
	REPONSE_MESURES_DIGITALES,
	SET_SORTIES_DIGITALES,

	//Info sur la carte
	DEMANDE_INFO = 30,
	REPONSE_INFO,					//Toutes les infos d'une carte dans un seul message

	//Power
	DEMANDE_MOTEURS_POWER = 35,
	DEMANDE_MOTEURS_AUX_POWER,
	DEMANDE_SERVO_POWER,
	DEMANDE_AX_12_POWER,
	DEMANDE_SIMULATION_MOTEURS,
	DEMANDE_SIMULATION_MOTEURS_AUX,

	//Parametres
	PARAMETRES_ODOMETRIE = 41,
	PARAMETRES_PID,
	VITESSE_ROBOT,
	VITESSE_MOTEURS_AUX,
	TYPE_ASSERV,					//type d'asservissement: 0=Polaire; 1=Moteurs Aux

	//Bloc 50 a 54 reserve pour la strategie
	STRATEGIE_CHANGEMENT_ETAT = 50,
	STRATEGIE_ENSEMBLE_ACTIONS,
	STRATEGIE_DIALOGUE_ENTREROBOTS,
	STRATEGIE_ETAT_INTERNE,

	//Logger
	LOGGER_DEBUG = 55,

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

	//End Communication
	END_COMMUNICATION = 254,

	//Test
	TEST_COMMUNICATION = 255,
};


/*
 * Destination du robot
 */

struct Com_Destination_Robot
{
	//Instruction 0		//15 octets + alignement = 16
	//Deplacement du Robot vers un point

	byte Sens;						//1:Marche Avant; 2:Marche Arriere; 3:Rotations
	byte Remplacement;				//1:Remplace le point actuel, 2: Remplace le point actuel et efface les suivants
	byte Numero_Robot;
	byte Bloquage;
	Int16 Destination_X;
	Int16 Destination_Y;
	Int16 Angle;				//*1000 //Angle pour les rotations
	byte Distance_Avant_Fin;			//Distance pour les fins de translations
	Int16 Angle_Avant_Avance;	//*1000
	Int16 Angle_Fin_Rotations;	//*1000	//Angle pour les fins de rotation
};

struct Com_Destination_Robot_Cercle
{
	//Instruction 2		//14 octets + alignement = 20
	//Deplacement du Robot en un arc de cercle

	Int16 Centre_X;
	Int16 Centre_Y;
	Int16 Rayon;
	float Angle_Debut;
	float Angle_Fin;
	byte AVANT_ARRIERE;		//0= Marche avant, 1= Marche arriere
	byte Sens;				//0= Sens Anti-horaire, 1= Sens horaire
	Int16 Distance_points;
	byte Bloquage;
};

struct Com_Position_Robot
{
	//Instruction 6		//9 octets + alignement = 10
	//Position du Robot

	byte Numero_Robot;
	Int16 Position_X;			//*10
	Int16 Position_Y;			//*10
	Int16 Angle;				//*100 //Orientation du Robot (° *100)
	byte Bloquage;
	byte Fin_Deplacement;
};


/*
 * Robots Adverses
 */

struct Com_Position_Adversaires
{
	//Instruction 11		//16 octets
	//Position des robots adverses

	short Position_X_1;
	short Position_Y_1;
	short Position_X_2;
	short Position_Y_2;
};


/*
 * Moteurs Aux
 */

struct Com_Moteurs_Aux
{
	//10 octets

	UInt16 Numero_Moteur;				//Numero du moteur aux a mettre a jour (Carte*10 + moteur) //Plage: 10 a 61
	UInt16 Vitesse;
	UInt16 Acceleration;
	UInt16 Decceleration;
	Int16 Position;
};

struct Com_Moteurs_Aux_Light
{
	//2 octets

	Int16 Position;
};

struct Com_Destination_Moteurs_Aux
{
	//Instruction 20		//10 octets par moteur = 20 + 1 + alignement = 20
	//Destination Moteurs Aux

	byte Nb_Moteurs_Updated;			//Nombres de moteurs a venir maj
	struct Com_Moteurs_Aux Moteurs[NB_MOT_AUX];
};

struct Com_Position_Moteurs_Aux
{
	//Instruction 22		//10 octets par moteur = 20 + 1 + alignement = 20
	//Position Moteurs Aux

	byte Nb_Moteurs_Updated;			//Nombres de moteurs a venir maj
	struct Com_Moteurs_Aux Moteurs[NB_MOT_AUX];
};

struct Com_Position_Moteurs_Aux_Light
{
	//Instruction 22		//2 octets par moteur = 4 + alignement = 4
	//Position Moteurs Aux

	struct Com_Moteurs_Aux_Light Moteurs[NB_MOT_AUX];
};


/*
 * Servos
 */

struct Com_Servo
{
	//3 Octets par servo

	byte Numero_Servo;				//Numero du servo a mettre a jour (Carte*10 + servo) //Plage: 10 a 65
	UInt16 Position;
};

struct Com_Servo_Light
{
	//2 Octets par servo

	UInt16 Position;
};

struct Com_Destination_Servo
{
	//Instruction 30	//3 Octets par servo, soit 18 + 3 + alignement = 24 octets
	//Destination Servo

	byte Nb_Servo_Updated;		//Nombre de servos a venir
	UInt16 Temps_Mouvement;
	struct Com_Servo Servos[NB_SERVO];
};

struct Com_Position_Servo
{
	//Instruction 32	//3 Octets par servo, soit 18 + 1 = 20 octets
	//Position Servo

	byte Nb_Servo_Updated;		//Nombre de servos a venir
	struct Com_Servo Servos[NB_SERVO];
};

struct Com_Position_Servo_Light
{
	//Instruction 32	//2 Octets par servo, soit 12 = 12 octets
	//Position Servo

	struct Com_Servo_Light Servos[NB_SERVO];
};


/*
 * AX 12
 */

struct Com_AX_12
{
	//11 Octets par AX_12

	byte Numero_AX_12;				//Numero de l'AX_12 a mettre a jour (Carte*10 + AX_12) //Plage: 10 a 63
	byte LED;
	byte Torque_Enable;
	UInt16 Position;
	UInt16 Speed;
	UInt16 Torque;
	byte Slope;
	byte Margin;
};

struct Com_AX_12_Light
{
	//4 Octets par AX_12

	UInt16 Position;
	UInt16 Torque;
};

struct Com_Destination_AX_12
{
	//Instruction 40	//11 Octets par AX_12, soit 44 + 1 + alignement = 48 octets
	//Destination Servo AX-12

	byte Nb_AX_12_Updated;		//Nombre d'AX_12 a venir
	struct Com_AX_12 AX_12[NB_AX_12];
};

struct Com_Position_AX_12
{
	//Instruction 42	//11 Octets par AX_12, soit 44 + 1 + alignement = 48 octets
	//Position Servo AX-12

	byte Nb_AX_12_Updated;		//Nombre d'AX_12 a venir
	struct Com_AX_12 AX_12[NB_AX_12];
};

struct Com_Position_AX_12_Light
{
	//Instruction 42	//4 Octets par AX_12, soit 16 + alignement = 16 octets
	//Position Servo AX-12

	struct Com_AX_12_Light AX_12[NB_AX_12];
};

struct Com_CLI_ReccordAX12
{
	byte nombre_servos;
	byte numeros_servos[4];
};


/*
 * Mesures analogiques
 */

struct Com_Mesures
{
	//8 Octets par Mesure

	byte Numero;				//Numero du cannal a mettre a jour (Carte*10 + mesure) //Plage: 10 a 63
	unsigned short Valeur;		//Valeur brute
	float Tension_V;			//Valeur en V
};

struct Com_Mesures_Light
{
	//2 Octets par Mesure

	UInt16 Valeur;		//Valeur brute
};

struct Com_Mesures_Analogiques
{
	//Instruction 52	//8 Octets par mesure, soit 32 octets
	//Reponse aux mesures analogiques

	byte Nb_Mesures_Updated;		//Nombre d'AX_12 a venir
	struct Com_Mesures Mesure[NB_MES_ANA];
};

struct Com_Mesures_Analogiques_Light
{
	//Instruction 52	//2 Octets par mesure, soit 8 octets
	//Reponse aux mesures analogiques

	struct Com_Mesures_Light Mesure[NB_MES_ANA];
};


/*
 * Entres/Sorties Digitales
 */

struct Com_EntresSorties_Digitales
{
	//Instruction 62 et 63	//8 octets
	//Reponse ou ordre pour les entres/sorties digitales

	sbyte Numero_Carte;
	unsigned short Valeur;		//Valeur des entrees/sorties
	unsigned short Direction;	//Valeur des directions des entrees/sorties
	unsigned short Change;		//Indique les valeurs qui changent

	/*
	 * [0]	IO 0
	 * [1]	IO 1
	 * [2]	IO 2
	 * [3]	IO 3
	 * [4]  CTC 0
	 * [5]  CTC 1
	 * [6]  CTC 2
	 * [7]  CTC 3
	 * [8]  FDC 0
	 * [9]  FDC 1
	 * [10] RELAI 0
	 * [11] RELAI 1
	 */
};

struct Com_EntresSorties_Digitales_Light
{
	//Instruction 62 et 63	//2 octets
	//Reponse ou ordre pour les entres/sorties digitales

	UInt16 Valeur;		//Valeur des entrees/sorties

	/*
	 * [0]	IO 0
	 * [1]	IO 1
	 * [2]	IO 2
	 * [3]	IO 3
	 * [4]  CTC 0
	 * [5]  CTC 1
	 * [6]  CTC 2
	 * [7]  CTC 3
	 * [8]  FDC 0
	 * [9]  FDC 1
	 * [10] RELAI 0
	 * [11] RELAI 1
	 */
};


/*
 * Reponse infos, toutes les infos de la carte en un seul message
 */

struct Com_Reponse_Info
{
	//instruction 101		//57 + alignement = 60 octets

	byte Numero_Carte;											//Numero de la carte emettant le message // 1 octet

	struct Com_Position_Robot PositionRobot;					//10 octets

	struct Com_Position_Moteurs_Aux_Light PositionMoteursAux;	//4 octets

	struct Com_Position_Servo_Light PositionServos;				//12 octets

	struct Com_Position_AX_12_Light PositionAX12;				//16 octets

	struct Com_Mesures_Analogiques_Light MesuresAnalogiques; 	//8 octets

	struct Com_EntresSorties_Digitales_Light ValeursDigitales; 	//2 octets

	UInt16 Tension_Batterie;									//Tension * 100		//2 octets

	UInt16 Timer;												//Temps du match en 10*ms //2 octets
};


/*
 * POWER
 */

struct Com_Moteurs_Power
{
	//Instruction 150	//1 Octet
	//Power des moteurs

	byte State;
};

struct Com_Moteurs_Aux_Power
{
	//Instruction 151	//1 Octet
	//Power des moteurs aux

	byte State;
};

struct Com_Servo_Power
{
	//Instruction 152	//1 Octet
	//Power des servos

	byte State;
};

struct Com_AX_12_Power
{
	//Instruction 153	//1 Octet
	//Power des AX 12

	byte State;
};

struct Com_Simulation_Moteurs
{
	//Instruction 154	//1 Octet
	//Simulation des moteurs

	byte State;
};

struct Com_Simulation_Moteurs_Aux
{
	//Instruction 155	//1 Octet
	//Simulation des moteurs

	byte State;
};


/*
 * Parametres
 */
struct Com_Parametres_Odometrie
{
	//Instruction 160	//12 Octets
	//Parametres d'odometrie

	float Coef_Distance;
	float Coef_Rotation;
	float Coef_Correction_Diametres;
};

struct Com_Parametres_PID
{
	//Instruction 161	//20 Octets
	//Parametres des PID

	byte Numero_PID;	//Numero du PID a maj: 0:Distance, 1:Angle, 2:AUX_1, 3:AUX_2
	float Gain_P;
	float Gain_I;
	float Gain_D;
	float Max_Erreur_Cumul;
	sbyte MAX;
	sbyte MIN;
	byte Enable;
};

struct Com_Vitesse_Robot
{
	//Instruction 162	//32 Octets
	//Vitesse de deplcement du Robot

	float Vitesse_Positive;
	float Vitesse_Negative;
	float Vitesse_Rotation_Positive;
	float Vitesse_Rotation_Negative;
	float Acceleration_Positive;
	float Acceleration_Negative;
	float Acceleration_Rotation_Positive;
	float Acceleration_Rotation_Negative;
};

struct Com_Vitesse_Aux
{
	//Instruction 162	//24 Octets
	//Vitesses des moteurs Aux

	float Vitesse_Positive_Aux_1;
	float Vitesse_Positive_Aux_2;
	float Acceleration_Positive_Aux_1;
	float Acceleration_Negative_Aux_1;
	float Acceleration_Positive_Aux_2;
	float Acceleration_Negative_Aux_2;
};


/*
 * Debug
 */
struct Logger_Debug
{
	//Instruction 230					//Maximum COMMUNICATION_TRAME_MAX_DATA informations à rentrer
	//Logger

	byte Nombre_Carateres;	            //Nombre d'octets de la chaine de commentaires à suivre						//1 octet
	byte Color;                         //Couleur du texte à afficher

	char Text[60];						//Chaine de commentaire limitée au nombre ci-dessus, maximum 60 caratcers 	//60 octets
};


/*
 * ASTAR
 */
struct Com_ASTAR_CONTENU
{
	//Instruction 231
	//Contenu du ASTAR

	unsigned short Numero_Premier_Noeud;	//Numero du premier noeud a MaJ a venir
	byte data[60];							//60 byte representant 120 noeuds:  //No_Processed '*'; //Open: ' '; //Closed: 'X'; //Path: '#'; //Path_Lisse: '='
};

struct ASTAR_Vecteur
{
	short start_x;
	short start_y;

	short end_x;
	short end_y;
	byte color;
};


struct Com_ASTAR_VECTEURS
{
	//Instruction 232
	//Vecteurs ASTAR

	byte Nombre_vecteurs;										//Nombre de vecteurs a venir; 0= effacement des vecteurs

	struct ASTAR_Vecteur data[6];								//Maximum 6 vecteurs a venir
};


/*
 * Graphique
 */
struct Com_Graphique_Data
{
	sbyte serie;							//numero de la serie a laquelle ajouter les datas
	long value;								//valeur a ajouter
};

struct Com_Graphique_Add_Data
{
	//Instruction 232
	//Ajoute des data aux graphique de l'IHM
	sbyte nb_data;							//Nb de valeurs à suivre

	struct Com_Graphique_Data data[5];		//data a ajouter
};


/*
 * CLI
 */
struct st_CLI_Data
{
	//Instruction 235											//Maximum COMMUNICATION_TRAME_MAX_DATA informations à rentrer

	char data[64];
};


/*
 * Bootloader
 */
enum Type_Bootloader
{
	bootloader_data,
	bootloader_FindeFichier,
	bootloader_Adresse_Etendue,
	bootloader_StartSegment_Address,
	bootloader_ExtendedLinear_Address,
	bootloader_StartLinear_Address,
};

struct st_LineBootloader
{
	//Instruction 253
	byte longueur;
	unsigned short adresse;
	enum Type_Bootloader Type;
	byte data[16];
	byte CRC;
};

#endif /* COMMUNICATION_STRUCT_H_ */

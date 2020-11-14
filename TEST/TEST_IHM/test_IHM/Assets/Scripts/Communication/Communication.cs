using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

public class Communication
{
    #region Definition messages de communication
    public enum Com_Instruction : byte
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

		DESTINATION_SERVOS_AND_AX12 = 20,

		/*
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
		*/

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

		PING = 252,
		PONG,

		//End Communication
		END_COMMUNICATION = 254,

		//Test
		TEST_COMMUNICATION = 255,
	};

	public const byte COMMUNICATION_TRAME_MAX_DATA = 64;

	public enum Adress_Xbee : ushort
	{
		XBee_PC,
		ALL_XBEE = 0xFFFF,
	}

	public enum Slave_Adresses : sbyte
	{
		BALISE_BOARD = -3,
		PC = -2,
		ALL_CARDS = -1,
		IA_BOARD = 0,
		MultiFct_1 = 1,
		MultiFct_2 = 2,
		MultiFct_3 = 3,
		MultiFct_4 = 4
	};

	[StructLayout(LayoutKind.Sequential)]
	public class Communication_Trame
	{
		[MarshalAs(UnmanagedType.U1)]
		public Com_Instruction Instruction;                             //Instruction a echanger

		[MarshalAs(UnmanagedType.U1)]
		public Slave_Adresses Slave_Adresse;                            //Adresse de la carte ciblée //ALL_CARDS (-1) pour toutes les cartes

		[MarshalAs(UnmanagedType.U1)]
		public byte Length;                                             //Nombre d'octets dans le tableau de data

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = COMMUNICATION_TRAME_MAX_DATA)]
		public byte[] Data = new byte[COMMUNICATION_TRAME_MAX_DATA];

		[MarshalAs(UnmanagedType.U1)]
		public Adress_Xbee XBEE_DEST_ADDR;                              //Adresse de la cible Xbee en mode API
	};

	public class Communication_Message
    {
		public Communication_Trame Trame;
		public DateTime Heure;

		public Communication_Message()
        {
			Trame = new Communication_Trame();
			Heure = DateTime.Now;
		}
	}

	public byte[] COPYDATA(object input)
	{
		int size = Marshal.SizeOf(input);
		byte[] arr = new byte[size];

		IntPtr ptr = Marshal.AllocHGlobal(size);
		Marshal.StructureToPtr(input, ptr, true);
		Marshal.Copy(ptr, arr, 0, size);
		Marshal.FreeHGlobal(ptr);
		return arr;
	}


	Communication_Trame Byte_To_Trame(byte[] input)
	{
		Communication_Trame trame = new Communication_Trame();

		int size = Marshal.SizeOf(trame);
		IntPtr ptr = Marshal.AllocHGlobal(size);

		Marshal.Copy(input, 0, ptr, size);

		trame = (Communication_Trame)Marshal.PtrToStructure(ptr, trame.GetType());
		Marshal.FreeHGlobal(ptr);

		return trame;
	}

	/*
		* Conversion d'un tableau de byte en une structure
		* Exemple:
		* 
		* byte[] array = new byte[30]; //Tableau contenant les data
		* StructuresDeclaration.Com_Destination_Robot testdest2 = new StructuresDeclaration.Com_Destination_Robot(); //Structure devant recevoir les data
		* 
		* ByteArrayToStructure(array, ref testdest2); //Transformation 
		*/
	public static T GetStructFromArray<T>(byte[] data) where T : class
	{
		GCHandle gch = GCHandle.Alloc(data, GCHandleType.Pinned);
		try
		{
			return (T)Marshal.PtrToStructure(gch.AddrOfPinnedObject(), typeof(T));
		}
		finally
		{
			gch.Free();
		}
	}

    #endregion    
}

public class Infos_Carte
{
	//Instruction 36
	//Infos cartes

	public const byte NB_SERVO = 6;
	public const byte NB_AX_12 = 4;
	public const byte NB_MES_ANA = 4;

	[StructLayout(LayoutKind.Sequential)]
	public struct Com_Position_Robot_Data
	{
		[MarshalAs(UnmanagedType.U1)]
		public Com_Position_Robot.Com_Position_Robot_Identification Numero_Robot;

		[MarshalAs(UnmanagedType.I2)]
		public short Position_X;         //*10

		[MarshalAs(UnmanagedType.I2)]
		public short Position_Y;         //*10

		[MarshalAs(UnmanagedType.I2)]
		public short Angle;              //*100 //Orientation du Robot (° *100)
										 //byte Bloquage;
										 //byte Fin_Deplacement;
	}

	/**************************************************
	Declaration de la definition de la Structure de communication permettant de faire remonter la position des servos
	**************************************************/

	[StructLayout(LayoutKind.Sequential)]
	public struct Com_Position_Servos
	{
		[MarshalAs(UnmanagedType.ByValArray, SizeConst = NB_SERVO)]
		public ushort[] Position;
	}

	/**************************************************
	Declaration de la definition de la Structure de communication permettant de faire remonter la position des AX 12
	**************************************************/

	[StructLayout(LayoutKind.Sequential)]
	public struct Com_Position_AX12
	{
		[MarshalAs(UnmanagedType.ByValArray, SizeConst = NB_AX_12)]
		public ushort[] Position;

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = NB_AX_12)]
		public short[] Torque;
	}

	/**************************************************
	Declaration de la definition de la Structure de communication permettant de faire remonter les valeurs des mesures analogiques
	**************************************************/

	[StructLayout(LayoutKind.Sequential)]
	public struct Com_Mesures_Analogiques
	{
		[MarshalAs(UnmanagedType.ByValArray, SizeConst = NB_MES_ANA)]
		public ushort[] Mesure;
	}

	/**************************************************
	Declaration de la definition de la Structure de communication permettant de faire remonter l'ensemble des infos de la carte vers la carte IA ou vers le PC
	**************************************************/
	[StructLayout(LayoutKind.Sequential)]
	public class Com_Reponse_Info
	{
		//instruction 101		//59 + alignement = 60 octets

		[MarshalAs(UnmanagedType.U1)]
		public byte Numero_Carte;                                   //Numero de la carte emettant le message // 1 octet

		[MarshalAs(UnmanagedType.U1)]
		public byte Etat_Alim; //0= motor power; 1 = motor aux power; 2 = servos power; 3 = ax12 power; 4 = AUX 1 power; 5 = Aux 2 power					 //1 octet

		[MarshalAs(UnmanagedType.U1)]
		public byte Etat_Contacteurs; //0= FDC 0; 1 = FDC 1; 2 = CTC 0; 3 = CTC 1; 4 = CTC 2; 5 = CTC 3					 //1 octet

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Position_Robot_Data PositionRobot;               //8 octets

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Position_Servos Position_Servos;                 //12 octets

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Position_AX12 Position_AX12;                     //16 octets

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Mesures_Analogiques Mesures_Analogiques;         //8 octets

		[MarshalAs(UnmanagedType.U2)]
		public ushort Tension_Batterie;                             //Tension * 100		//2 octets
	}
}

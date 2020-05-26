using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class Communication
{
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
		IA_BOARD = 0
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
		public Adress_Xbee XBEE_DEST_ADDR;								//Adresse de la cible Xbee en mode API
	};

	public Communication_Trame Trame_Data;

	public Communication()
	{
		Trame_Data = new Communication_Trame();
	}

	public void Set_Instruction(Com_Instruction instruction)
	{
		Trame_Data.Instruction = instruction;
	}

	public void Set_SlaveAdress(Slave_Adresses slave)
	{
		Trame_Data.Slave_Adresse = slave;
		if(slave == Slave_Adresses.PC)
		{
			Trame_Data.XBEE_DEST_ADDR = Adress_Xbee.XBee_PC;
		}
	}

	public void Set_Data(byte[]data)
	{
		Trame_Data.Data = data;
		Trame_Data.Length = (byte)data.Length;
	}

	public void Set_Data(object input)
	{
		Trame_Data.Data = COPYDATA(input);
		Trame_Data.Length = (byte)Trame_Data.Data.Length;
	}

	public Communication_Trame Get_Trame()
	{
		return Trame_Data;
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
}





public class Structures_Echange
{
	public class Com_Demande_Position_Robot
	{
		//Instruction 9		//7 octets + alignement = 8
		//Demande Position du Robot

		public List<Communication> Request_Robot_Position()
		{
			//Création locale de le liste de messages à envoyer
			List<Communication> List_trame_echange = new List<Communication>();

			//Création de la trame de base d'un message
			Communication trame_echange = new Communication();
			trame_echange.Set_Instruction(Communication.Com_Instruction.DEMANDE_ROBOT_POSITION);

			trame_echange.Set_SlaveAdress(Communication.Slave_Adresses.ALL_CARDS);

			//Ajoute cette trame à la liste des trames à envoyer
			List_trame_echange.Add(trame_echange);

			return List_trame_echange;
		}
	}

	/*
	 * Remontee d'infos
	 */

	public class Com_Position_Robot
	{
		//Instruction 10		//7 octets + alignement = 8
		//Position du Robot

		public enum Com_Position_Robot_Identification : byte
		{
			Gros_Robot,
			Petit_Robot
		};

		/**************************************************
		Declaration de la definition de la Structure de communication permettant de faire remonter la position du Robot vers la carte IA ou vers le PC
		**************************************************/
		
		[StructLayout(LayoutKind.Sequential)]
		public class Com_Position_Robot_Data
		{
			[MarshalAs(UnmanagedType.U1)]
			public Com_Position_Robot_Identification Numero_Robot;

			[MarshalAs(UnmanagedType.I2)]
			public short Position_X;         //*10

			[MarshalAs(UnmanagedType.I2)]
			public short Position_Y;         //*10

			[MarshalAs(UnmanagedType.I2)]
			public short Angle;              //*100 //Orientation du Robot (° *100)
									  //byte Bloquage;
									  //byte Fin_Deplacement;
		}

		public Com_Position_Robot_Data Trame_To_Data(Communication.Communication_Trame input_trame)
		{
			Com_Position_Robot_Data output = new Com_Position_Robot_Data();
			output = (Com_Position_Robot_Data)Communication.GetStructFromArray<Com_Position_Robot_Data>(input_trame.Data);
			return output;
		}
	};


	/*
	 * Remontée de TOUTES les infos d'une carte
	 */

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
			//instruction 101		//58 + alignement = 60 octets

			[MarshalAs(UnmanagedType.U1)]
			public byte Numero_Carte;                                          //Numero de la carte emettant le message // 1 octet

			[MarshalAs(UnmanagedType.U1)]
			public byte Etat_Alim; //7=Simu 6=SimuAux 5=PowerMotor 4=PowerAux 3=PowerAX12 2=PowerServo					 //1 octet

			[MarshalAs(UnmanagedType.Struct)]
			public Com_Position_Robot_Data PositionRobot;                    //8 octets

			[MarshalAs(UnmanagedType.Struct)]
			public Com_Position_Servos Position_Servos;                 //12 octets

			[MarshalAs(UnmanagedType.Struct)]
			public Com_Position_AX12 Position_AX12;                     //16 octets

			[MarshalAs(UnmanagedType.Struct)]
			public Com_Mesures_Analogiques Mesures_Analogiques;         //8 octets

			[MarshalAs(UnmanagedType.U2)]
			public ushort Tension_Batterie;                            //Tension * 100		//2 octets
		}


		public Com_Reponse_Info Trame_To_Data(Communication.Communication_Trame input_trame)
		{
			Com_Reponse_Info output = new Com_Reponse_Info();

			output.Position_Servos.Position = new ushort[NB_SERVO];
			output.Position_AX12.Position = new ushort[NB_AX_12];
			output.Position_AX12.Torque = new short[NB_AX_12];
			output.Mesures_Analogiques.Mesure = new ushort[NB_MES_ANA];
			output.PositionRobot = new Com_Position_Robot_Data();

			output = (Com_Reponse_Info)Communication.GetStructFromArray<Com_Reponse_Info>(input_trame.Data);
			return output;
		}
	}



	/*
	 * Debug
	 */

	public class Logger_Debug
	{
		//Instruction 230					       //Maximum COMMUNICATION_TRAME_MAX_DATA informations à rentrer
		//Logger

		public enum Logger_Debug_Color : byte
		{
			Color_Black,
			Color_Blue,
			Color_Red
		};

		public const byte Max_Char_per_Log = 60;
		[StructLayout(LayoutKind.Sequential)]
		public class Logger_Debug_Data
		{
			[MarshalAs(UnmanagedType.U1)]
			public byte Nombre_Carateres;								//Nombre d'octets de la chaine de commentaires à suivre						//1 octet

			[MarshalAs(UnmanagedType.U1)]
			public Logger_Debug_Color Color;                            //Couleur du texte à afficher												//1 octet

			[MarshalAs(UnmanagedType.ByValArray, SizeConst = Max_Char_per_Log)]
			public char[] Text = new char[Max_Char_per_Log];			//Chaine de commentaire limitée au nombre ci-dessus, maximum 60 caratcers 	//60 octets
		}

		public List<Communication> Send_Logger(string str, Structures_Echange.Logger_Debug.Logger_Debug_Color color)
		{
			//Création locale de le liste de messages à envoyer
			List<Communication> List_trame_echange = new List<Communication>();

			//Création de la trame de base d'un message
			Communication trame_echange = new Communication();
			trame_echange.Set_Instruction(Communication.Com_Instruction.LOGGER_DEBUG);

			trame_echange.Set_SlaveAdress(Communication.Slave_Adresses.ALL_CARDS);

			//Création de la structure du message de Log à envoyer
			Logger_Debug_Data logger_Debug = new Logger_Debug_Data();
			logger_Debug.Color = color;

			if (str.Length < Max_Char_per_Log)
			{
				//Une seule trame suffit
				logger_Debug.Nombre_Carateres = (byte)str.Length;

				//Copie la string dans la structure du Logger
				str.CopyTo(0, logger_Debug.Text, 0, str.Length);

				//Ajoute un 0 de fin de chaine de caractere
				if(str.Length < Max_Char_per_Log-1)
				{
					logger_Debug.Text[str.Length] = (char)0;
				}

				//Copie la structure du Logger dans la trame à envoyer
				trame_echange.Set_Data(logger_Debug);

				//Ajoute cette trame à la liste des trames à envoyer
				List_trame_echange.Add(trame_echange);

				return List_trame_echange;
			}
			else
			{
				//Plusieurs trames serront necessaires


			}
			return List_trame_echange;
		}
	
		
		public Logger_Debug_Data Trame_To_Data(Communication.Communication_Trame input_trame)
		{
			Logger_Debug_Data output = new Logger_Debug_Data();
			output = (Logger_Debug_Data)Communication.GetStructFromArray<Logger_Debug_Data>(input_trame.Data);
			return output;
		}	
	};
}

public class Echange_Data
{
	//Envoi d'un message de Log (str), de la couleur (color)
	public List<Communication> Send_Logger(string str, Structures_Echange.Logger_Debug.Logger_Debug_Color color)
	{
		//Transforme le message de log, en une liste de messages de communication
		Structures_Echange.Logger_Debug logger = new Structures_Echange.Logger_Debug();
		return logger.Send_Logger(str, color);
	}


	public List<Communication> Request_Robot_Position()
	{
		Structures_Echange.Com_Demande_Position_Robot Com_Demande_Position_Robot = new Structures_Echange.Com_Demande_Position_Robot();
		return Com_Demande_Position_Robot.Request_Robot_Position();
	}
}

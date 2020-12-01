using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public partial class Infos_Carte
{
	//Instruction 36
	//Infos cartes

	public const byte NB_SERVO = 6;
	public const byte NB_AX_12 = 4;
	public const byte NB_MES_ANA = 4;

	public enum Com_Position_Robot_Identification : byte
	{
		Gros_Robot,
		Petit_Robot
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct Com_Position_Robot_Data
	{
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
		//instruction 		//59 + alignement = 60 octets

		[MarshalAs(UnmanagedType.U1)]
		public Com_Position_Robot_Identification Numero_Robot;

		[MarshalAs(UnmanagedType.U1)]
		public byte Numero_Carte;                                   //Numero de la carte emettant le message // 1 octet

		[MarshalAs(UnmanagedType.U1)]
		public byte Etat_Alim; //0= motor power; 1 = simulation; 2 = servos power; 3 = ax12 power; 4 = AUX 1 power; 5 = Aux 2 power					 //1 octet

		[MarshalAs(UnmanagedType.U1)]
		public byte Etat_Contacteurs; //0= FDC 0; 1 = FDC 1; 2 = CTC 0; 3 = CTC 1; 4 = CTC 2; 5 = CTC 3					 //1 octet

		[MarshalAs(UnmanagedType.U1)]
		public byte Etat_IO;          //0= IO 0; 1 = IO 1; 2 = IO 2; 3 = IO 3

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Position_Robot_Data PositionRobot;               //6 octets

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Position_Servos Position_Servos;                 //12 octets

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Position_AX12 Position_AX12;                     //16 octets

		[MarshalAs(UnmanagedType.Struct)]
		public Com_Mesures_Analogiques Mesures_Analogiques;         //8 octets

		[MarshalAs(UnmanagedType.U2)]
		public ushort Tension_Batterie;                             //Tension * 100		//2 octets
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

public partial class Infos_Carte
{	
	[StructLayout(LayoutKind.Sequential)]
	public class Com_Reponse_Info_IA
    {
		//instruction 	
		[MarshalAs(UnmanagedType.U1)]
		public Com_Position_Robot_Identification Numero_Robot;

		[MarshalAs(UnmanagedType.U1)]
		public byte Strategie;

		[MarshalAs(UnmanagedType.U2)]
		public ushort Temps_Match;                             //Temps /10		//2 octets

		[MarshalAs(UnmanagedType.U1)]
		public byte Etat_Inputs; //0= Jack; 1 = Color; 2 = Switchs; 3 = LED Red; 4 = LED Yellow; 5 = LED Green					 //1 octet

	}

	public Com_Reponse_Info_IA Trame_To_Data_IA(Communication.Communication_Trame input_trame)
	{
		Com_Reponse_Info_IA output = new Com_Reponse_Info_IA();		

		output = (Com_Reponse_Info_IA)Communication.GetStructFromArray<Com_Reponse_Info_IA>(input_trame.Data);
		return output;
	}
}
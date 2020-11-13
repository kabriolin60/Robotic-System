using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;

public class CarteMultiFct_maj : MonoBehaviour
{
	public GameObject Servos;
	public GameObject AX12;
	public GameObject ANA;
	public GameObject Mesures_Digitales;

	public GameObject Tension_Batterie;

	public void MaJ_Carte(Infos_Carte.Com_Reponse_Info infos)
	{
		Servos.GetComponent<Servos_maj>().Maj_Servos(infos.Position_Servos);
		AX12.GetComponent<AX12_maj>().Maj_AX12(infos.Position_AX12);
		ANA.GetComponent<ANA_maj>().Maj_Analogique(infos.Mesures_Analogiques);
		Mesures_Digitales.GetComponent<MesuresDigitales_maj>().Maj_Mesures_Digitales(infos.Etat_Alim);
		Mesures_Digitales.GetComponent<MesuresDigitales_maj>().Maj_Contacteurs(infos.Etat_Contacteurs);

		float tension = infos.Tension_Batterie;
		tension = tension / 100;
		Tension_Batterie.GetComponent<TextMeshProUGUI>().text = $"{tension}V";
	}
}


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
	};


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


using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

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

	public const byte Max_Char_per_Log = 59;
	[StructLayout(LayoutKind.Sequential)]
	public class Logger_Debug_Data
	{
		[MarshalAs(UnmanagedType.U1)]
		public byte Nombre_Carateres;                               //Nombre d'octets de la chaine de commentaires à suivre						//1 octet		

		[MarshalAs(UnmanagedType.U1)]
		public byte Channel;                                        //Cannal de débug à utiliser												//1 octet

		[MarshalAs(UnmanagedType.U1)]
		public Logger_Debug_Color Color;                            //Couleur du texte à afficher												//1 octet

		[MarshalAs(UnmanagedType.ByValArray, SizeConst = Max_Char_per_Log)]
		public char[] Text = new char[Max_Char_per_Log];            //Chaine de commentaire limitée au nombre ci-dessus, maximum 60 caratcers 	//60 octets
	}

	/*
	public List<Communication> Send_Logger(string str, Logger_Debug_Color color)
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
			if (str.Length < Max_Char_per_Log - 1)
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
	}*/

	public Logger_Debug_Data Trame_To_Data(Communication.Communication_Trame input_trame)
	{
		Logger_Debug_Data output = new Logger_Debug_Data();
		output = (Logger_Debug_Data)Communication.GetStructFromArray<Logger_Debug_Data>(input_trame.Data);
		return output;
	}
}

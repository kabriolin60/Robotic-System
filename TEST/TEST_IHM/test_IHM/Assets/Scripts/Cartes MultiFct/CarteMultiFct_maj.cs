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




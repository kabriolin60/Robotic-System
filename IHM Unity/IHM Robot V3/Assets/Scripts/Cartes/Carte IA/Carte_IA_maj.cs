﻿using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Carte_IA_maj : MonoBehaviour
{
	public GameObject Choix_Strategie;
	public GameObject Temps_Match;
	public GameObject Jack;
	public GameObject Color;
	public GameObject Switch;
	public GameObject LED_Red;
	public GameObject LED_Yellow;
	public GameObject LED_Green;
	public GameObject Tension_Batterie;
	public GameObject User_BP;
	public GameObject Communication_Board;

	public void MaJ_Carte(Infos_Carte.Com_Reponse_Info_IA infos)
	{
		Choix_Strategie.GetComponent<TextMeshProUGUI>().text = $"Stratégie: {infos.Strategie}";

		Temps_Match.GetComponent<TextMeshProUGUI>().text = $"{infos.Temps_Match * 10}ms";

		//0= Jack; 1 = Color; 2 = Switchs; 3 = LED Red; 4 = LED Yellow; 5 = LED Green
		Jack.GetComponent<Toggle>().isOn = ((infos.Etat_Inputs & 0b00000001) > 0 ? true : false);
		Color.GetComponent<Toggle>().isOn = ((infos.Etat_Inputs & 0b00000010) > 0 ? true : false);
		Switch.GetComponent<Toggle>().isOn = ((infos.Etat_Inputs & 0b00000100) > 0 ? true : false);
		LED_Red.GetComponent<Toggle>().isOn = ((infos.Etat_Inputs & 0b00001000) > 0 ? true : false);
		LED_Yellow.GetComponent<Toggle>().isOn = ((infos.Etat_Inputs & 0b00010000) > 0 ? true : false);
		LED_Green.GetComponent<Toggle>().isOn = ((infos.Etat_Inputs & 0b00100000) > 0 ? true : false);
		User_BP.GetComponent<Toggle>().isOn = ((infos.Etat_Inputs & 0b01000000) > 0 ? true : false);


		//float tension_batterie = Last_Infos.Robots_Last_Info[(int)(infos.Numero_Robot)].Get_Last_Infos(0).Tension_Batterie;

		float tension_batterie = infos.Tension_Batterie;
		tension_batterie /= 100;

		Tension_Batterie.GetComponent<TextMeshProUGUI>().text = $"{tension_batterie}V";

		//Maj des status des cartes du bus
		FindChildByRecursion(Communication_Board.transform, "Presence Carte IA").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_00000001) > 0;
		FindChildByRecursion(Communication_Board.transform, "Presence Carte MultiFCT1").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_00000010) > 0;
		FindChildByRecursion(Communication_Board.transform, "Presence Carte MultiFCT2").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_00000100) > 0;
		FindChildByRecursion(Communication_Board.transform, "Presence Carte MultiFCT3").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_00001000) > 0;
		FindChildByRecursion(Communication_Board.transform, "Presence Carte MultiFCT4").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_00010000) > 0;

		FindChildByRecursion(Communication_Board.transform, "Presence Carte Peripheriques1").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_00100000) > 0;
		FindChildByRecursion(Communication_Board.transform, "Presence Carte Peripheriques2").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_01000000) > 0;
		FindChildByRecursion(Communication_Board.transform, "Presence Carte Peripheriques3").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000000_10000000) > 0;
		FindChildByRecursion(Communication_Board.transform, "Presence Carte Peripheriques4").GetComponent<Toggle>().isOn = (infos.Boards_Comm_Status & 0b00000001_00000000) > 0;
	}


	internal static Transform FindChildByRecursion(Transform aParent, string aName)
	{
		if (aParent == null) return null;

		var result = aParent.transform.Find(aName);

		if (result != null)
			return result;

		foreach (Transform child in aParent)
		{
			result = FindChildByRecursion(child, aName);
			if (result != null)
				return result;
		}
		return null;
	}
}



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
		Mesures_Digitales.GetComponent<MesuresDigitales_maj>().Maj_IO(infos.Etat_IO);


		float tension = infos.Tension_Batterie;
		tension = tension / 100;
		Tension_Batterie.GetComponent<TextMeshProUGUI>().text = $"{tension}V";
	}
}



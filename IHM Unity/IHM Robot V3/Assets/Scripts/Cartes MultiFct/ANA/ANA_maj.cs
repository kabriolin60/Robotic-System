using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ANA_maj : MonoBehaviour
{
    public GameObject ANA0;
    public GameObject ANA1;
    public GameObject ANA2;
    public GameObject ANA3;

    public void Maj_Analogique(Infos_Carte.Com_Mesures_Analogiques values)
    {
        this.ANA0.GetComponent<Slider>().value = values.Mesure[0];
        this.ANA1.GetComponent<Slider>().value = values.Mesure[1];
        this.ANA2.GetComponent<Slider>().value = values.Mesure[2];
        this.ANA3.GetComponent<Slider>().value = values.Mesure[3];
    }
}

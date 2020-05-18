using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ADC : MonoBehaviour
{
    public GameObject ADC_Prefab;
    public Text Tension_Batterie;

    List<ADC_elementaire> ADC_values = new List<ADC_elementaire>();

    // Start is called before the first frame update
    void Start()
    {
        //Création des panels de servos pour la carte 

        for (int i = 0; i < Structures_Echange.Infos_Carte.NB_MES_ANA/2; i++)
        {
            ADC_values.Add(new ADC_elementaire());
            ADC_values[i].UI_Object = Instantiate(ADC_Prefab, new Vector3(-45 + 90 * i, 90, 0), Quaternion.identity);
            ADC_values[i].Init(i);
            ADC_values[i].UI_Object.transform.SetParent(this.transform, false);
        }

        for (int i = Structures_Echange.Infos_Carte.NB_MES_ANA/2; i < Structures_Echange.Infos_Carte.NB_MES_ANA; i++)
        {
            ADC_values.Add(new ADC_elementaire());
            ADC_values[i].UI_Object = Instantiate(ADC_Prefab, new Vector3(-45 + 90 * (i - 2), 90 - 210, 0), Quaternion.identity);
            ADC_values[i].Init(i);
            ADC_values[i].UI_Object.transform.SetParent(this.transform, false);
        }
    }


    public void Update_Mesures_Analogiques(Structures_Echange.Infos_Carte.Com_Mesures_Analogiques mesures)
    {
        for (int i = 0; i < Structures_Echange.Infos_Carte.NB_MES_ANA; i++)
        {
            ADC_values[i].Update_Value(mesures.Mesure[i]);
        }
    }

    public void Update_VBat(string tension)
    {
        Tension_Batterie.text = tension;
    }

}

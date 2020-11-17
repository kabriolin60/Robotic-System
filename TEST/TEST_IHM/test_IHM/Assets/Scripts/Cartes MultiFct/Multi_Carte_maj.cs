using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Multi_Carte_maj : MonoBehaviour
{
    public GameObject[] Carte_multiFct;

    public int Robot_id;


    public void Start()
    {
        Carte_multiFct = new GameObject[Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot];  
    }


    public void FixedUpdate()
    {
        for (int carte_id = 0; carte_id < Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot; carte_id++)
        {
            try
            {
                maj_Carte(Last_Infos.Robots_Last_Info[Robot_id].Get_Last_Infos(carte_id));
            }
            catch
            {

            }
        }
    }

    public void maj_Carte(Infos_Carte.Com_Reponse_Info infos)
    {
        Carte_multiFct[infos.Numero_Carte - 1].GetComponent<CarteMultiFct_maj>().MaJ_Carte(infos);
    }
}

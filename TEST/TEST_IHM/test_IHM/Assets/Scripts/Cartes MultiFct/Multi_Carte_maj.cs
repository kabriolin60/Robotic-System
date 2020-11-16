using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Multi_Carte_maj : MonoBehaviour
{
    public GameObject Carte_1;
    public GameObject Carte_2;
    public GameObject Carte_3;
    public GameObject Carte_4;

    public void maj_Carte(Infos_Carte.Com_Reponse_Info infos)
    {
        switch(infos.Numero_Carte)
        {
            case 1:
                Carte_1.GetComponent<CarteMultiFct_maj>().MaJ_Carte(infos);
                break;

            case 2:
                Carte_2.GetComponent<CarteMultiFct_maj>().MaJ_Carte(infos);
                break;

            case 3:
                Carte_3.GetComponent<CarteMultiFct_maj>().MaJ_Carte(infos);
                break;

            case 4:
                Carte_4.GetComponent<CarteMultiFct_maj>().MaJ_Carte(infos);
                break;

            default:
                break;
        }
    }
}

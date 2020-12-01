using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Last_Infos : MonoBehaviour
{
    private int Nombre_robots;
    private int Nombre_Cartes_MultiFct_Par_Robot;

    static public Robot_Last_Infos[] Robots_Last_Info;



    // Start is called before the first frame update
    void Start()
    {
        Nombre_robots = Common_settings.static_Nombre_Robots;
        Nombre_Cartes_MultiFct_Par_Robot = Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot;

        Robots_Last_Info = new Robot_Last_Infos[Nombre_robots];

        for(int i = 0; i < Robots_Last_Info.Length; i++)
        {
            Robots_Last_Info[i] = new Robot_Last_Infos(Nombre_Cartes_MultiFct_Par_Robot);
        }
    }


    public void Update_Last_Data_Received(Infos_Carte.Com_Reponse_Info data)
    {
        Robots_Last_Info[(int)data.Numero_Robot].Set_Last_Infos(data.Numero_Carte, data);
    }

    public void Update_Last_Data_Received_IA(Infos_Carte.Com_Reponse_Info_IA data)
    {
        Robots_Last_Info[(int)data.Numero_Robot].Set_Last_Infos_IA(data);
    }


    public class Robot_Last_Infos
    {
        private int Nombre_Cartes_MultiFct_Par_Robot;

        private Infos_Carte.Com_Reponse_Info[] Last_Cartes_MultiFct;

        private Infos_Carte.Com_Reponse_Info_IA Last_Carte_IA = new Infos_Carte.Com_Reponse_Info_IA();

        public Robot_Last_Infos(int nb_multifct)
        {
            Nombre_Cartes_MultiFct_Par_Robot = nb_multifct;
            Last_Cartes_MultiFct = new Infos_Carte.Com_Reponse_Info[Nombre_Cartes_MultiFct_Par_Robot];
            for (int i = 0; i < Last_Cartes_MultiFct.Length; i++)
            {
                Last_Cartes_MultiFct[i] = new Infos_Carte.Com_Reponse_Info();
            }
        }

        public void Set_Last_Infos(int card_number, Infos_Carte.Com_Reponse_Info data)
        {
            try
            {
                Last_Cartes_MultiFct[card_number - 1] = data;
            }
            catch
            {
                Debug.Log("Pas de carte définie");
            }
        }

        public void Set_Last_Infos_IA(Infos_Carte.Com_Reponse_Info_IA data)
        {
            try
            {
                Last_Carte_IA = data;
            }
            catch
            {
                Debug.Log("Pas de carte IA définie");
            }
        }

        public Infos_Carte.Com_Reponse_Info Get_Last_Infos(int card_number)
        {
            return Last_Cartes_MultiFct[card_number];
        }

        public Infos_Carte.Com_Reponse_Info_IA Get_Last_Infos_IA()
        {
            return Last_Carte_IA;
        }
    }
}

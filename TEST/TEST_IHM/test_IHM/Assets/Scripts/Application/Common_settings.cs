using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Common_settings : MonoBehaviour
{
    static public byte static_Nombre_Robots = 2;
    static public byte static_Nombre_Cartes_MultiFct_Par_Robot = 4;

    static public string[] static_Robots_Names = { "Gros Robot", "Petit Robot" };

    static public Infos_Carte.Com_Position_Robot_Identification[] static_Id_Robot;

    public void Start()
    {
        static_Id_Robot = (Infos_Carte.Com_Position_Robot_Identification[])typeof(Infos_Carte.Com_Position_Robot_Identification).GetEnumValues();
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Common_settings : MonoBehaviour
{
    public byte Nombre_Robots;
    public byte Nombre_Cartes_MultiFct_Par_Robot;

    static public byte static_Nombre_Robots;
    static public byte static_Nombre_Cartes_MultiFct_Par_Robot;

    static public string[] static_Robots_Names = { "Gros Robot", "Petit Robot", "test 1", "Test 2", "Test 3" };

    static public Infos_Carte.Com_Position_Robot_Identification[] static_Id_Robot;

    public void Start()
    {
        static_Nombre_Robots = Nombre_Robots;
        static_Nombre_Cartes_MultiFct_Par_Robot = Nombre_Cartes_MultiFct_Par_Robot;

        static_Id_Robot = (Infos_Carte.Com_Position_Robot_Identification[])typeof(Infos_Carte.Com_Position_Robot_Identification).GetEnumValues();
    }
}

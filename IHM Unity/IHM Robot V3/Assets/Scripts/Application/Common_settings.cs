using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Common_settings : MonoBehaviour
{
    public byte Nombre_Robots;
    public byte Nombre_Cartes_MultiFct_Par_Robot;
    public byte Nombre_Maximum_Actions;

    static public byte static_Nombre_Maximum_Actions;
    static public byte static_Nombre_Robots;
    static public byte static_Nombre_Cartes_MultiFct_Par_Robot;

    static public string[] static_Robots_Names = { "Gros Robot", "Petit Robot", "test 1", "Test 2", "Test 3" };

    static public string[] static_Cartes_Names = { "Carte MultiFct 1", "Carte MultiFct 2", "Carte MultiFct 3", "Carte MultiFct 4", "test 1", "Test 2", "Test 3" };

    static public Infos_Carte.Com_Position_Robot_Identification[] static_Id_Robot;

    public void Start()
    {
        static_Nombre_Robots = Nombre_Robots;
        static_Nombre_Cartes_MultiFct_Par_Robot = Nombre_Cartes_MultiFct_Par_Robot;
        static_Nombre_Maximum_Actions = Nombre_Maximum_Actions;

        static_Id_Robot = (Infos_Carte.Com_Position_Robot_Identification[])typeof(Infos_Carte.Com_Position_Robot_Identification).GetEnumValues();
    }
}

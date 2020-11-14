using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TEST : MonoBehaviour
{
    Infos_Carte.Com_Reponse_Info data_test;

    public GameObject Tab_Multi_Carte_Robot;

    public void Start()
    {
        byte[] test = new byte[1];


        data_test = new Infos_Carte.Com_Reponse_Info();

        data_test.Numero_Carte = 0;

        data_test.Position_Servos = new Infos_Carte.Com_Position_Servos();
        data_test.Position_Servos.Position = new ushort[Infos_Carte.NB_SERVO];
        data_test.Position_Servos.Position[0] = 500;
        data_test.Position_Servos.Position[1] = 600;
        data_test.Position_Servos.Position[2] = 300;
        data_test.Position_Servos.Position[3] = 800;
        data_test.Position_Servos.Position[4] = 100;
        data_test.Position_Servos.Position[5] = 50;

        data_test.Position_AX12 = new Infos_Carte.Com_Position_AX12();
        data_test.Position_AX12.Position = new ushort[Infos_Carte.NB_AX_12];
        data_test.Position_AX12.Position[0] = 500;
        data_test.Position_AX12.Position[1] = 250;
        data_test.Position_AX12.Position[2] = 26;
        data_test.Position_AX12.Position[3] = 128;

        data_test.Position_AX12.Torque = new short[Infos_Carte.NB_AX_12];
        data_test.Position_AX12.Torque[0] = 500;
        data_test.Position_AX12.Torque[1] = 250;
        data_test.Position_AX12.Torque[2] = 26;
        data_test.Position_AX12.Torque[3] = 128;

        data_test.Mesures_Analogiques = new Infos_Carte.Com_Mesures_Analogiques();
        data_test.Mesures_Analogiques.Mesure = new ushort[Infos_Carte.NB_MES_ANA];
        data_test.Mesures_Analogiques.Mesure[0] = 1023;
        data_test.Mesures_Analogiques.Mesure[1] = 2046;
        data_test.Mesures_Analogiques.Mesure[2] = 3069;
        data_test.Mesures_Analogiques.Mesure[3] = 4092;

        data_test.Tension_Batterie = 1524;

        data_test.Etat_Alim = 0b11111111;
        data_test.Etat_Contacteurs = 0b11111111;

        Tab_Multi_Carte_Robot.GetComponent<Multi_Carte_maj>().maj_Carte(data_test);
    }
}

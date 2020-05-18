using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cartes_MultiFct_Panel : MonoBehaviour
{
    public const byte Nb_Cartes_Max_Petit_Robot = 4;
    public const byte Nb_Cartes_Max_Gros_Robot = 4;

    //Conteneur de toutes les cartes
    public GameObject[] Cartes_MultiFct_Display_Panel = new GameObject[4];

    //Prefab d'un panneau d'une carte
    public GameObject Carte_MultiFct_Prefab;

    //Cartes du Robot
    private GameObject[] Cartes_MultiFCt = new GameObject[4];


    public int Get_AX12_Position(Structures_Echange.Com_Position_Robot.Com_Position_Robot_Identification idRobot, int idCarte, int idAx12)
    {
        //Debug.Log("Get_AX12_Position trappe= " + idRobot.ToString() + "; " + idCarte.ToString() + "; " + idAx12.ToString());
        int result = 0;

        result = Cartes_MultiFCt[idCarte - 1].GetComponentInChildren<AX12>().GetAX12Position(idAx12 % 10);

        return result;
    }


    public int Get_Servo_Position(Structures_Echange.Com_Position_Robot.Com_Position_Robot_Identification idRobot, int idCarte, int idservo)
    {
        //Debug.Log("Get_Servo_Position trappe= " + idRobot.ToString() + "; " + idCarte.ToString() + "; " + idservo.ToString());
        int result = 0;

        result = Cartes_MultiFCt[idCarte - 1].GetComponentInChildren<servos>().GetServoPosition(idservo % 10);

        return result;
    }


    // Start is called before the first frame update
    void Start()
    {
        //Création des objets de Toutes les cartes pour les deux Robots
        for(int i = 0; i < 4; i++ )
        {
            Cartes_MultiFCt[i] = Instantiate(Carte_MultiFct_Prefab, Cartes_MultiFct_Display_Panel[i].transform);
        }
    }

    public void Update_Infos(Structures_Echange.Infos_Carte.Com_Reponse_Info infos)
    {
        try
        {
            //Maj position des servos
            Cartes_MultiFCt[infos.Numero_Carte - 1].GetComponentInChildren<servos>().Update_Servos_Position(infos.Position_Servos);
            //Maj position des AX12
            Cartes_MultiFCt[infos.Numero_Carte - 1].GetComponentInChildren<AX12>().Update_AX12_Position(infos.Position_AX12);
            //Maj valeurs analogiques
            Cartes_MultiFCt[infos.Numero_Carte - 1].GetComponentInChildren<ADC>().Update_Mesures_Analogiques(infos.Mesures_Analogiques);
            //Tension Batterie
            Cartes_MultiFCt[infos.Numero_Carte - 1].GetComponentInChildren<ADC>().Update_VBat(((float)(infos.Tension_Batterie) / 100).ToString() + "V");
        }
        catch
        {

        }
    }
}

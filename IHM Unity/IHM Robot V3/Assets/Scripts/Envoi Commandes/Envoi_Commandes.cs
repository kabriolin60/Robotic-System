using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Envoi_Commandes : MonoBehaviour
{
    CultureInfo culture = CultureInfo.CreateSpecificCulture("en-GB");


    internal static Transform FindChildByRecursion(Transform aParent, string aName)
    {
        if (aParent == null) return null;

        var result = aParent.transform.Find(aName);

        if (result != null)
            return result;

        foreach (Transform child in aParent)
        {
            result = FindChildByRecursion(child, aName);
            if (result != null)
                return result;
        }
        return null;
    }

    public void Envoi_Commande_Simulation(bool simu)
    {
        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        byte[] arr = new byte[1];

        arr[0] = Convert.ToByte(simu);

        trame.Data = arr;
        trame.Length = 1;

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.DEMANDE_SIMULATION_MOTEURS;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }


    public void Envoi_Commande_MotorPower(bool power)
    {
        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        byte[] arr = new byte[2];

        arr[0] = Convert.ToByte(power);
        arr[1] = Convert.ToByte(power);

        trame.Data = arr;
        trame.Length = 2;

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.DEMANDE_MOTEURS_POWER;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }


    public void Envoi_Deplacement(GameObject deplacement_panel)
    {
        TMP_InputField Destination_X;
        TMP_InputField Destination_Y;
        TMP_InputField Destination_Distance;
        TMP_InputField Destination_Theta;

        Destination_X = FindChildByRecursion(deplacement_panel.transform, "X").GetComponent< TMP_InputField>();
        Destination_Y = FindChildByRecursion(deplacement_panel.transform, "Y").GetComponent<TMP_InputField>();
        Destination_Distance = FindChildByRecursion(deplacement_panel.transform, "Distance").GetComponent<TMP_InputField>();
        Destination_Theta = FindChildByRecursion(deplacement_panel.transform, "Theta").GetComponent<TMP_InputField>();


        Consignes_Deplacement.st_COORDONNEES coord = new Consignes_Deplacement.st_COORDONNEES();
        Consignes_Deplacement.st_DESTINATION_ROBOT dest = new Consignes_Deplacement.st_DESTINATION_ROBOT();


        if (Destination_X.text != "") coord.X = short.Parse(Destination_X.text, culture);
        if (Destination_Y.text != "") coord.Y = short.Parse(Destination_Y.text, culture);
        if (Destination_Distance.text != "") coord.Distance = short.Parse(Destination_Distance.text, culture);
        if (Destination_Theta.text != "") coord.Angle = short.Parse(Destination_Theta.text, culture); coord.Angle *= 100;

        coord.Type_Deplacement = (Consignes_Deplacement.enum_TYPE_MOVE_)FindChildByRecursion(deplacement_panel.transform, "type deplacement").GetComponent<Dropdown>().value;
        coord.Type_Arret = (Consignes_Deplacement.enum_TYPE_ARRET_)FindChildByRecursion(deplacement_panel.transform, "Freinage").GetComponent<Dropdown>().value;

        Consignes_Deplacement.st_Parametre_Deplacement parametres = new Consignes_Deplacement.st_Parametre_Deplacement();
        parametres.Distance_Detection_Fin_Trajectoire = 3 * 100;
        parametres.Angle_Avant_Debut_Avance = (byte)(0.1F * 100);
        parametres.Angle_Detection_Fin_Trajectoire = (byte)(0.05F * 100);

        coord.ptrParameters = parametres;

        dest.coord = coord;

        if (FindChildByRecursion(deplacement_panel.transform, "Effacement").GetComponent<Dropdown>().value == 0)
        {
            dest.Replace = false;
        }
        else
        {
            dest.Replace = true;
        }

        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame = Communication.GetArrayFromStruct<Consignes_Deplacement.st_DESTINATION_ROBOT>(dest);

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.DESTINATION_ROBOT;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }
}


public class Consignes_Deplacement
{
    /**************************************************
    Declaration des enum contenant les types de déplacements
     **************************************************/

    public enum enum_TYPE_MOVE_ : byte
    {
        aucun_mouvement,             //Aucun mouvement
        xy_tour_av_avant,            //Déplacement standard en marche avant
        xy_tour_av_arriere,          //Déplacement standard en marche arriere
        tourne_vers_cap_rad,         //Oriente le robot selon l'angle donné en absolu
        deplacement_libre,           //Permet de faire des tests en déplacement linéaires
        rotation_libre,              //Permet de faire des tests en rotaion
    };

    /*************************************************/


    /**************************************************
    Declaration des enum contenant les parametres de déplacement
     **************************************************/

    public enum enum_TYPE_ARRET_ : byte
    {
        depla_AVEC_freinage,    //Le robot freine et s'arret au point cible
        depla_SANS_freinage //le robot ne freine pas et ne s'arrete pas au point cible
    };

    /*************************************************/


    /**************************************************
    Declaration de la definition de la Structure contenant les parametres pour les déplacements
     **************************************************/
    [StructLayout(LayoutKind.Sequential)]
    public struct st_Parametre_Deplacement
    {
        [MarshalAs(UnmanagedType.U2)]
        public ushort Distance_Detection_Fin_Trajectoire;         //distance separant le robot de son objectif avant passage au point suivant * 100

        [MarshalAs(UnmanagedType.U1)]
        public byte Angle_Detection_Fin_Trajectoire;               //angle separant le robot de son objectif avant passage au point suivant pour les rotations * 100

        [MarshalAs(UnmanagedType.U1)]
        public byte Angle_Avant_Debut_Avance;                      //angle separant le robot de son objectif pour qu'il commence à avancer * 100
    };

    /*************************************************/


    /**************************************************
    Declaration de la definition de la Structure contenant des coordonnees
     **************************************************/
    [StructLayout(LayoutKind.Sequential)]
    public struct st_COORDONNEES
    {
        [MarshalAs(UnmanagedType.U2)]
        public short X;                     //coordonnee X

        [MarshalAs(UnmanagedType.U2)]
        public short Y;                     //coordonnee Y

        [MarshalAs(UnmanagedType.U2)]
        public short Angle;                 //angle pour les rotations vers un cap absolu * 100

        [MarshalAs(UnmanagedType.U2)]
        public short Distance;              //Indique la distance a parcourir pour les deplacements en distance libre, ou le rayon des cercles

        [MarshalAs(UnmanagedType.U1)]
        public enum_TYPE_MOVE_ Type_Deplacement;    //choix du type de deplacement

        [MarshalAs(UnmanagedType.U1)]
        public enum_TYPE_ARRET_ Type_Arret;     //Type d'arret au point cible

        [MarshalAs(UnmanagedType.Struct)]
        public st_Parametre_Deplacement ptrParameters;
    };

    /*************************************************/


    /**************************************************
    Declaration de la definition de la Structure contenant des coordonnees de destination d'un robot
     **************************************************/
    [StructLayout(LayoutKind.Sequential)]
    public class st_DESTINATION_ROBOT
    {
        [MarshalAs(UnmanagedType.Struct)]
        public st_COORDONNEES coord;

        [MarshalAs(UnmanagedType.U1)]
        public bool Replace;                           // if == true, clear the current mouvement buffer, and add this new one
    };

    /*************************************************/
}

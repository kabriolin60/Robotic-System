using System.Globalization;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Consignes_Deplacement
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


    public void Envoi_Deplacement(GameObject deplacement_panel)
    {
        TMP_InputField Destination_X;
        TMP_InputField Destination_Y;
        TMP_InputField Destination_Distance;
        TMP_InputField Destination_Theta;
        TMP_InputField Vitesse_Roue_Gauche;
        TMP_InputField Vitesse_Roue_Droite;

        Destination_X = FindChildByRecursion(deplacement_panel.transform, "X").GetComponent<TMP_InputField>();
        Destination_Y = FindChildByRecursion(deplacement_panel.transform, "Y").GetComponent<TMP_InputField>();
        Destination_Distance = FindChildByRecursion(deplacement_panel.transform, "Distance").GetComponent<TMP_InputField>();
        Destination_Theta = FindChildByRecursion(deplacement_panel.transform, "Theta").GetComponent<TMP_InputField>();
        Vitesse_Roue_Gauche = FindChildByRecursion(deplacement_panel.transform, "Vitesse Roue Gauche").GetComponent<TMP_InputField>();
        Vitesse_Roue_Droite = FindChildByRecursion(deplacement_panel.transform, "Vitesse Roue Droite").GetComponent<TMP_InputField>();


        st_COORDONNEES coord = new st_COORDONNEES();
        st_DESTINATION_ROBOT dest = new st_DESTINATION_ROBOT();


        if (Destination_X.text != "") coord.X = short.Parse(Destination_X.text, culture);
        if (Destination_Y.text != "") coord.Y = short.Parse(Destination_Y.text, culture);
        if (Destination_Distance.text != "") coord.Distance = short.Parse(Destination_Distance.text, culture);
        if (Destination_Theta.text != "") coord.Angle = short.Parse(Destination_Theta.text, culture); coord.Angle *= 100;

        if (Vitesse_Roue_Gauche.text != "") coord.Vitesse_Roue_Gauche = short.Parse(Vitesse_Roue_Gauche.text, culture);
        if (Vitesse_Roue_Droite.text != "") coord.Vitesse_Roue_Droite = short.Parse(Vitesse_Roue_Droite.text, culture);

        coord.Type_Deplacement = (enum_TYPE_MOVE_)FindChildByRecursion(deplacement_panel.transform, "type deplacement").GetComponent<TMP_Dropdown>().value;
        coord.Type_Arret = (enum_TYPE_ARRET_)FindChildByRecursion(deplacement_panel.transform, "Freinage").GetComponent<TMP_Dropdown>().value;

        st_Parametre_Deplacement parametres = new st_Parametre_Deplacement();
        parametres.Distance_Detection_Fin_Trajectoire = 3 * 100;
        parametres.Angle_Avant_Debut_Avance = (byte)(0.1F * 100);
        parametres.Angle_Detection_Fin_Trajectoire = (byte)(0.05F * 100);

        coord.ptrParameters = parametres;

        dest.coord = coord;

        if (FindChildByRecursion(deplacement_panel.transform, "Effacement").GetComponent<TMP_Dropdown>().value == 0)
        {
            dest.Replace = false;
        }
        else
        {
            dest.Replace = true;
        }

        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame = Communication.GetArrayFromStruct<st_DESTINATION_ROBOT>(dest);

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.DESTINATION_ROBOT;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }


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
        consigne_vitesse_independantes //Permet de piloter les roues en vitesse sans passer par les couches supérieures d'asserv
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

        [MarshalAs(UnmanagedType.U2)]
        public short Vitesse_Roue_Gauche;              //Indique la distance a parcourir pour les deplacements en distance libre, ou le rayon des cercles

        [MarshalAs(UnmanagedType.U2)]
        public short Vitesse_Roue_Droite;              //Indique la distance a parcourir pour les deplacements en distance libre, ou le rayon des cercles

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



    /**************************************************
    Declaration des types d'asservissements possibles
    **************************************************/
    public enum _enum_Type_Asserv : byte
    {
        Polaire_Tourne_Avance_point_unique,     //Asserv avec boucle de vitesse en distance, vitesse en angle, distance, angle
        Vitesse_Droite_Vitesse_Gauche_Indep,    //Uniquement un asserv en vitesse à droite et vitesse à Gauche
        Vitesse_D_G__Distance_Angle,            //Asserv en vitesse à D et à G, plus Asserv en distance et/ou angle (sans coordonnées)
        Moteurs_Aux,
    };

    /*************************************************/


    /**************************************************
    Declaration de la definition de la Structure contenant les caracteristiques du robot
    **************************************************/
    [StructLayout(LayoutKind.Sequential)]
    public class st_ROBOT_PARAMETRES
    {
        [MarshalAs(UnmanagedType.R4)]
        public float COEF_ROT;

        [MarshalAs(UnmanagedType.R4)]
        public float COEF_D;

        [MarshalAs(UnmanagedType.R4)]
        public float COEF_CORRECTION_DIAMETRES;

        [MarshalAs(UnmanagedType.U1)]
        public bool SIMULATION;        //Indique si les déplacements sont en simulation ou en réel

        [MarshalAs(UnmanagedType.U1)]
        public _enum_Type_Asserv _1_Odometrie_Type_Asserv;      //Type d'asservissement
    };

    /*************************************************/
}

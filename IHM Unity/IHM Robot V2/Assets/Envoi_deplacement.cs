using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.UI;

public class Envoi_deplacement : MonoBehaviour
{
    public Dropdown Type_deplacement;
    public InputField destination_x;
    public InputField destination_y;
    public InputField angle;
    public InputField distance;
    public Dropdown Effacement;
    public Dropdown Freinage;

    public GameObject Communication_port;

    #region définitions
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
    public struct st_DESTINATION_ROBOT
    {
        [MarshalAs(UnmanagedType.Struct)]
        public st_COORDONNEES coord;

        [MarshalAs(UnmanagedType.U1)]
        public bool Replace;                           // if == true, clear the current mouvement buffer, and add this new one
    };

    /*************************************************/

    #endregion



    public void Envoi_Ordre_Deplacement()
    {
        st_COORDONNEES coord = new st_COORDONNEES();
        st_DESTINATION_ROBOT dest = new st_DESTINATION_ROBOT();

        if (destination_x.text != "") coord.X = (short)Convert.ToSingle(destination_x.text.Replace('.', ','));
        if (destination_y.text != "") coord.Y = (short)Convert.ToSingle(destination_y.text.Replace('.', ','));
        if (angle.text != "") coord.Angle = (short)Convert.ToSingle(angle.text.Replace('.', ',')); coord.Angle *= 100;
        if (distance.text != "") coord.Distance = (short)Convert.ToSingle(distance.text.Replace('.', ','));

        coord.Type_Deplacement = (enum_TYPE_MOVE_)Type_deplacement.value;
        coord.Type_Arret = (enum_TYPE_ARRET_)Freinage.value;

        st_Parametre_Deplacement parametres = new st_Parametre_Deplacement();
        parametres.Distance_Detection_Fin_Trajectoire = 3 * 100;
        parametres.Angle_Avant_Debut_Avance = (byte)(0.1F * 100);
        parametres.Angle_Detection_Fin_Trajectoire = (byte)(0.05F * 100);

        coord.ptrParameters = parametres;

        dest.coord = coord;

        if (Effacement.value == 0)
        {
            dest.Replace = false;
        }
        else
        {
            dest.Replace = true;
        }

        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame.Slave_Adresse = Communication.Slave_Adresses.ALL_CARDS;
        trame.Instruction = Communication.Com_Instruction.DESTINATION_ROBOT;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        int size = Marshal.SizeOf(dest);
        byte[] arr = new byte[size];

        IntPtr ptr = Marshal.AllocHGlobal(size);
        Marshal.StructureToPtr(dest, ptr, true);
        Marshal.Copy(ptr, arr, 0, size);
        Marshal.FreeHGlobal(ptr);
        trame.Data = arr;
        trame.Length = (byte)size;

        Communication_port.GetComponent<Communication_Send_Instructions>().Send_Instruction(trame);
    }
}

using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Security.Policy;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Envoi_PID : MonoBehaviour
{
    public TMP_InputField coef_P;
    public TMP_InputField coef_I;
    public TMP_InputField coef_D;
    public TMP_InputField Max_Min;
    public TMP_InputField Cumul;

    public Toggle Sommation;
    public Toggle Enable;

    public PID_Id ID;


    [StructLayout(LayoutKind.Sequential)]
    public class reglage_pid
    {
        [MarshalAs(UnmanagedType.U1)]
        public PID_Id id;

        [MarshalAs(UnmanagedType.R4)]
        public float P; 

        [MarshalAs(UnmanagedType.R4)]
        public float I;

        [MarshalAs(UnmanagedType.R4)]
        public float D;

        [MarshalAs(UnmanagedType.U1)]
        public byte Min_Max;

        [MarshalAs(UnmanagedType.U2)]
        public short Cumul;

        [MarshalAs(UnmanagedType.U1)]
        public byte Sommation;

        [MarshalAs(UnmanagedType.U1)]
        public byte Enable;
    }

    public enum PID_Id : byte
    {
        vitesse_roues_independantes,
        vitesse_distance,
        vitesse_orientation,
        distance,
        orientation
    }

    



    public void Send_PID()
    {
        reglage_pid pid = new reglage_pid();

        pid.id = ID;

        if (coef_P.text != "") pid.P = float.Parse(coef_P.text, Common_settings.culture);
        if (coef_I.text != "") pid.I = float.Parse(coef_I.text, Common_settings.culture);
        if (coef_D.text != "") pid.D = float.Parse(coef_D.text, Common_settings.culture);

        if (Max_Min.text != "") pid.Min_Max = (byte)float.Parse(Max_Min.text, Common_settings.culture);
        if (Cumul.text != "") pid.Cumul = (short)float.Parse(Cumul.text, Common_settings.culture);

        if(Sommation.isOn)
        {
            pid.Sommation = 1;
        }
        else
        {
            pid.Sommation = 0;
        }

        if (Enable.isOn)
        {
            pid.Enable = 1;
        }
        else
        {
            pid.Enable = 0;
        }

        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame = Communication.GetArrayFromStruct<reglage_pid>(pid);

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.PARAMETRES_PID;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }
}

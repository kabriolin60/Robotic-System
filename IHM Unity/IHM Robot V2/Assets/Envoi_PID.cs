using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security.Policy;
using UnityEngine;
using UnityEngine.UI;

public class Envoi_PID : MonoBehaviour
{
    public InputField coef_P;
    public InputField coef_I;
    public InputField coef_D;
    public InputField Max_Min;
    public InputField Cumul;

    public Toggle Sommation;
    public Toggle Enable;

    public PID_Id ID;



    [StructLayout(LayoutKind.Sequential)]
    public struct reglage_pid
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

    public GameObject Communication_port;



    public void Send_PID()
    {
        reglage_pid pid = new reglage_pid();

        pid.id = ID;

        if (coef_P.text != "") pid.P = Convert.ToSingle(coef_P.text.Replace('.', ','));
        if (coef_I.text != "") pid.I = Convert.ToSingle(coef_I.text.Replace('.', ','));
        if (coef_D.text != "") pid.D = Convert.ToSingle(coef_D.text.Replace('.', ','));

        if (Max_Min.text != "") pid.Min_Max = (byte)Convert.ToSingle(Max_Min.text.Replace('.', ','));
        if (Cumul.text != "") pid.Cumul = (short)Convert.ToSingle(Cumul.text.Replace('.', ','));

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

        trame.Slave_Adresse = Communication.Slave_Adresses.ALL_CARDS;
        trame.Instruction = Communication.Com_Instruction.PARAMETRES_PID;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        int size = Marshal.SizeOf(pid);
        byte[] arr = new byte[size];

        IntPtr ptr = Marshal.AllocHGlobal(size);
        Marshal.StructureToPtr(pid, ptr, true);
        Marshal.Copy(ptr, arr, 0, size);
        Marshal.FreeHGlobal(ptr);
        trame.Data = arr;
        trame.Length = (byte)size;

        Communication_port.GetComponent<Communication_Send_Instructions>().Send_Instruction(trame);

    }
}

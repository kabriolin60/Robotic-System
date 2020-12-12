using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;

public class Commande_Servos : MonoBehaviour
{
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


    // Start is called before the first frame update
    void Start()
    {
        List<string> servos_associated_global_id = new List<string>();
        servos_associated_global_id.Add("");

        for (int index = 0; index < Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot; index++)
        {
            for (int index_servo = 0; index_servo < 10; index_servo++)
            {
                servos_associated_global_id.Add($"{(index + 1) * 10 + index_servo}");
            }
        }
        GetComponentInChildren<TMP_Dropdown>().AddOptions(servos_associated_global_id);
    }

    public Commande_Servos_message.servo_destination_temp Get_Servos_Values()
    {
        Commande_Servos_message.servo_destination_temp values = new Commande_Servos_message.servo_destination_temp();

        values.numero = (byte)FindChildByRecursion(this.transform, "Servo ID").GetComponent<TMP_Dropdown>().value;

        if (values.numero == 0)
            return values;

        if (FindChildByRecursion(this.transform, "Destination").GetComponent<TMP_InputField>().text != "")
        {
            values.destination = ushort.Parse(FindChildByRecursion(this.transform, "Destination").GetComponent<TMP_InputField>().text);
        }
        else
        {
            values.destination = 0;
        }


        if (FindChildByRecursion(this.transform, "Torque").GetComponent<TMP_InputField>().text != "")
        {
            values.torque = ushort.Parse(FindChildByRecursion(this.transform, "Torque").GetComponent<TMP_InputField>().text);
        }
        else
        {
            values.torque = 0;
        }

        int numero_carte = (values.numero - 1) / 10 + 1;
        int numero_servo = (values.numero % 10) - 1;
        values.numero = (byte)(numero_carte * 10 + numero_servo);

        return values;
    }
}

public class Commande_Servos_message
{
    public class servo_destination_temp
    {
        public byte numero;
        public ushort destination;
        public ushort torque;
    }

    public const byte Communication_Nombre_Servos_Max_Message = 10;

    [StructLayout(LayoutKind.Sequential)]
    public struct servo_destination
    {
        [MarshalAs(UnmanagedType.U2)]
        public byte ID;                                //Identifiant du servos a deplacer

        [MarshalAs(UnmanagedType.U2)]
        public ushort Destination;             //position a atteindre par le servo

        [MarshalAs(UnmanagedType.U2)]
        public ushort Torque;                  //Maximum torque, pour les AX12
    };


    [StructLayout(LayoutKind.Sequential)]
    public class st_Destination_Servos
    {
        [MarshalAs(UnmanagedType.U2)]
        public ushort Nombre_servos_to_move;             //Nombre de servos à mettre à jour

        [MarshalAs(UnmanagedType.U2)]
        public ushort Time_to_move;            //Duree du deplacement demande

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = Communication_Nombre_Servos_Max_Message)]
        public servo_destination[] servo;       //Tableau des destinations des servos
    };


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

    public void Envoi_Servos(GameObject servo_panel)
    {
        st_Destination_Servos destination = new st_Destination_Servos();
        destination.servo = new servo_destination[Communication_Nombre_Servos_Max_Message];

        int index_message_output = 0;

        //Pour chaque servo selectionné
        for(int index_input = 0; index_input < Communication_Nombre_Servos_Max_Message; index_input++)
        {
            destination.servo[index_input] = new servo_destination();

            servo_destination_temp temp = FindChildByRecursion(servo_panel.transform, $"1 Servo destination ({index_input})").GetComponentInChildren<Commande_Servos>().Get_Servos_Values();

            if (temp.numero != 0)
            {
                destination.servo[index_message_output].ID = (byte)temp.numero;
                destination.servo[index_message_output].Destination = (ushort)temp.destination;
                destination.servo[index_message_output].Torque = (ushort)temp.torque;
                index_message_output++;
            }
            else
            {

            }
        }

        destination.Nombre_servos_to_move = (byte)index_message_output;

        //Lecture du temps de déplacement des servos
        if (FindChildByRecursion(servo_panel.transform, "Temps deplacement").GetComponentInChildren<TMP_InputField>().text != "")
        {
            destination.Time_to_move = ushort.Parse(FindChildByRecursion(servo_panel.transform, "Temps deplacement").GetComponentInChildren<TMP_InputField>().text);
        }
        else
        {
            destination.Time_to_move = 0;
        }


        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame = Communication.GetTrameFromClass<st_Destination_Servos>(destination);

        trame.Slave_Adresse = Communication.Slave_Adresses.ALL_CARDS; //Toutes les cartes
        trame.Instruction = Communication.Com_Instruction.DESTINATION_SERVOS_AND_AX12;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }
}

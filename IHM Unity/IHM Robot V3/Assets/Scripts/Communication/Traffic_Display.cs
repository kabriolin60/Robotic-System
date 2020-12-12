using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Traffic_Display : MonoBehaviour
{
    public GameObject PortCom_Status;
    public GameObject Nombre_Messages_Display;
    public GameObject Nombre_Erreur_Display;

    public GameObject Demande_Infos_Toggle;

    public GameObject PortCom_Status_big;
    public GameObject Nombre_Messages_Display_big;
    public GameObject Nombre_Erreur_Display_big;


    public bool port_opened = false;

    private Virtual_SerialPort serialport;

    private void Start()
    {
        serialport = this.gameObject.GetComponent<Virtual_SerialPort>();       
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (serialport.port_opened)
        {
            port_opened = true;

            //Le port existe et il est ouvert
            PortCom_Status.GetComponent<TextMeshProUGUI>().text = $"{serialport.portName} opened";
            PortCom_Status.GetComponent<TextMeshProUGUI>().color = Color.green;

            PortCom_Status_big.GetComponent<TextMeshProUGUI>().text = $"{serialport.portName} opened";
            PortCom_Status_big.GetComponent<TextMeshProUGUI>().color = Color.green;

            Nombre_Messages_Display.GetComponent<TextMeshProUGUI>().text = $"Mess = {this.GetComponent<Trame_Decoder>().Messages_Number}";
            Nombre_Erreur_Display.GetComponent<TextMeshProUGUI>().text = $"Errors = {this.GetComponent<Trame_Decoder>().Error_Number}";

            Nombre_Messages_Display_big.GetComponent<TextMeshProUGUI>().text = $"Mess = {this.GetComponent<Trame_Decoder>().Messages_Number}";
            Nombre_Erreur_Display_big.GetComponent<TextMeshProUGUI>().text = $"Errors = {this.GetComponent<Trame_Decoder>().Error_Number}";
        }
        else
        {
            port_opened = false;
            //Le port existe mais il est fermé
            PortCom_Status.GetComponent<TextMeshProUGUI>().text = $"{serialport.portName} closed";
            PortCom_Status.GetComponent<TextMeshProUGUI>().color = Color.red;

            PortCom_Status_big.GetComponent<TextMeshProUGUI>().text = $"{serialport.portName} closed";
            PortCom_Status_big.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
    }
}

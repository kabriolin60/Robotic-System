using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Traffic_Display : MonoBehaviour
{
    public GameObject PortCom_Status;
    public GameObject Nombre_Messages_Display;
    public GameObject Nombre_Erreur_Display;    

    System.IO.Ports.SerialPort _serialPort;

    // Update is called once per frame
    void Update()
    {
        if (_serialPort != null)
        {
            if (_serialPort.IsOpen)
            {
                //Le port existe et il est ouvert
                PortCom_Status.GetComponent<TextMeshProUGUI>().text = $"{_serialPort.PortName} opened";
                PortCom_Status.GetComponent<TextMeshProUGUI>().color = Color.green;

                Nombre_Messages_Display.GetComponent<TextMeshProUGUI>().text = $"Mess = {this.GetComponent<Trame_Decoder>().Messages_Number}";
                Nombre_Erreur_Display.GetComponent<TextMeshProUGUI>().text = $"Errors = {this.GetComponent<Trame_Decoder>().Error_Number}";
            }
            else
            {
                //Le port existe mais il est fermé
                PortCom_Status.GetComponent<TextMeshProUGUI>().text = $"{_serialPort.PortName} closed";
                PortCom_Status.GetComponent<TextMeshProUGUI>().color = Color.red;
            }
        }
        else
        {
            Virtual_SerialPort serialport = this.gameObject.GetComponent<Virtual_SerialPort>();
            if (serialport != null)
                _serialPort = serialport.getserialPort();
        }
    }
}

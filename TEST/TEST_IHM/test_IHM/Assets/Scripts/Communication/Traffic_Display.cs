using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Traffic_Display : MonoBehaviour
{
    public GameObject PortCom_Status;
    public GameObject Nombre_Messages_Display;
    public GameObject Nombre_Erreur_Display;

    
    public bool port_opened = false;

    private System.IO.Ports.SerialPort _serialPort;

    // Update is called once per frame
    void FixedUpdate()
    {
        Virtual_SerialPort serialport = this.gameObject.GetComponent<Virtual_SerialPort>();
        if (serialport != null)
        {
            _serialPort = serialport.getserialPort();

            if (_serialPort != null)
            {
                if (_serialPort.IsOpen)
                {
                    port_opened = true;

                    //Le port existe et il est ouvert
                    PortCom_Status.GetComponent<TextMeshProUGUI>().text = $"{serialport.portName} opened";
                    PortCom_Status.GetComponent<TextMeshProUGUI>().color = Color.green;

                    Nombre_Messages_Display.GetComponent<TextMeshProUGUI>().text = $"Mess = {this.GetComponent<Trame_Decoder>().Messages_Number}";
                    Nombre_Erreur_Display.GetComponent<TextMeshProUGUI>().text = $"Errors = {this.GetComponent<Trame_Decoder>().Error_Number}";
                }
                else
                {
                    port_opened = false;
                    //Le port existe mais il est fermé
                    PortCom_Status.GetComponent<TextMeshProUGUI>().text = $"{serialport.portName} closed";
                    PortCom_Status.GetComponent<TextMeshProUGUI>().color = Color.red;
                }

                _serialPort.Dispose();
            }
        }
    }
}

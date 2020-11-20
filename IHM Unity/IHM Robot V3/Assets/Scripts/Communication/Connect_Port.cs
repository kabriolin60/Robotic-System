using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Connect_Port : MonoBehaviour
{
    public GameObject VirtualPort;
    public GameObject PortList_dropdown;

    private bool actual_state = false;

    public void OnClick()
    {
        if(actual_state)
        {
            //disconnect request
            actual_state = false;
            OnDisconnect_Request();
        }
        else
        {
            //connect request
            actual_state = true;
            Onconnect_Request();
        }
    }

    private void Onconnect_Request()
    {
        int index = PortList_dropdown.GetComponent<TMPro.TMP_Dropdown>().value;

        var list = PortList_dropdown.GetComponent<TMPro.TMP_Dropdown>().options;

        VirtualPort.GetComponent<Virtual_SerialPort>().Connect(list[index].text);

        this.GetComponentInChildren<TextMeshProUGUI>().text = "Disconnect";
    }

    private void OnDisconnect_Request()
    {
        VirtualPort.GetComponent<Virtual_SerialPort>().DisConnect();

        this.GetComponentInChildren<TextMeshProUGUI>().text = "Connect";
    }
}

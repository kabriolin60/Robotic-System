using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;

public class COMPORT_List : MonoBehaviour
{
    string[] ports;
    List<string> ports_list;

    // Update is called once per frame
    void Start()
    {
        if (this.GetComponent<TMPro.TMP_Dropdown>() != null)
        {
            ports = SerialPort.GetPortNames();

            this.GetComponent<TMPro.TMP_Dropdown>().ClearOptions();

            ports_list = new List<string>();

            foreach (string str in ports)
            {
                ports_list.Add(str);
            }

            this.GetComponent<TMPro.TMP_Dropdown>().AddOptions(ports_list);
        }
    }
}

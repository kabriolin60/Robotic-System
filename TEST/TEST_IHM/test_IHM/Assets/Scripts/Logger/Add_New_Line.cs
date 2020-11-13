using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using UnityEngine;
using UnityEngine.UI;

public class Add_New_Line : MonoBehaviour
{
    public GameObject Line_Contener;
    public GameObject Line_Prefab;

    public GameObject Channel_Toggle;

    public void Start()
    {
        //string time = $"{System.DateTime.Now.Hour}:{System.DateTime.Now.Minute}:{System.DateTime.Now.Second}:{System.DateTime.Now.Millisecond}";
        //Add_New_Logger_Line(time, 3, Color.green, "Toto est passé par ici");
    }

    public void Add_New_Logger_Line(string time, int Channel, Color color, string text)
    {
        Logger_Channel[] channels = Channel_Toggle.GetComponentsInChildren<Logger_Channel>();        

        GameObject go = Instantiate(Line_Prefab, Line_Contener.transform) as GameObject;
        go.GetComponent<Logger_Line>().SetNewLine(time, Channel, color, text);

        foreach (Logger_Channel _channel in channels)
        {
            if (_channel.Channel_id == Channel)
            {
                if (_channel.GetComponentInParent<Toggle>().isOn == false)
                {
                    //Debug.Log("Channel desabled");
                    go.SetActive(false);
                }
            }
        }
    }
}

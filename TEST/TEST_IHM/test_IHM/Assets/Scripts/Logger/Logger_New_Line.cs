using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using UnityEngine;
using UnityEngine.UI;

public class Logger_New_Line : MonoBehaviour
{
    public GameObject Line_Contener;
    public GameObject Line_Prefab;

    public GameObject Channel_Toggle;


    public class Logger_Message
    {
        public string time;
        public int channel;

        public Color color;

        public string text;


        public Logger_Message(string _time, int _Channel, Color _color, string _text)
        {
            time = _time;
            channel = _Channel;
            color = _color;
            text = _text;
        }
    }


    public void Start()
    {
        //string time = $"{System.DateTime.Now.Hour}:{System.DateTime.Now.Minute}:{System.DateTime.Now.Second}:{System.DateTime.Now.Millisecond}";
        //Add_New_Logger_Line(time, 3, Color.green, "Toto est passé par ici");
    }


    public void Add_New_Logger_Line(Logger_Message message)
    {
        Add_New_Logger_Line(message.time, message.channel, message.color, message.text);
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
                try
                {
                    if (_channel.GetComponentInParent<Toggle>().isOn == false)
                    {
                        //Debug.Log("Channel disabled");
                        go.SetActive(false);
                    }
                }
                catch
                {

                }
            }
        }
    }
}

using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using UnityEngine;
using UnityEngine.UI;

public class Logger_New_Line : MonoBehaviour
{
    static public Logger_New_Line Logger_Viewer;


    public GameObject Line_Contener;
    public GameObject Line_Prefab;

    public GameObject Channel_Toggle;

    public int Max_Logger_Lines = 100;

    private Dictionary<int, Toggle> Logger_Channel_Dictionnary = new Dictionary<int, Toggle>();

    private List<GameObject> List_logger_lines = new List<GameObject>();


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
        //utilisé pour le logger, accessible en static
        Logger_Viewer = this.gameObject.GetComponent<Logger_New_Line>();


        Logger_Channel[] channels = Channel_Toggle.GetComponentsInChildren<Logger_Channel>();
        foreach(Logger_Channel chan in channels)
        {
            Logger_Channel_Dictionnary.Add(chan.Channel_id, chan.GetComponentInParent<Toggle>());
        }
    }


    public void Add_New_Logger_Line(Logger_Message message)
    {
        Add_New_Logger_Line(message.time, message.channel, message.color, message.text);
    }


    public void Add_New_Logger_Line(string time, int Channel, Color color, string text)
    {
        GameObject go = Instantiate(Line_Prefab, Line_Contener.transform) as GameObject;
        go.GetComponent<Logger_Line>().SetNewLine(time, Channel, color, text);

        List_logger_lines.Add(go);


        if (Logger_Channel_Dictionnary.TryGetValue(Channel, out var _channel))
        {
            if (_channel.isOn == false)
            {
                go.SetActive(false);
            }
        }

        if(List_logger_lines.Count > this.Max_Logger_Lines)
        {
            DestroyImmediate(List_logger_lines[0]);
            List_logger_lines.RemoveAt(0);
        }
    }




    #region INTERNAL_LOGGER
    public static void Log(string text, int channel, Color color, [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0, [System.Runtime.CompilerServices.CallerFilePath] string caller = null)
    {
        Debug.Log(text);

        string time = $"{System.DateTime.Now.Hour}:{System.DateTime.Now.Minute}:{System.DateTime.Now.Second}:{System.DateTime.Now.Millisecond}";

        Internal_Logger(time, channel, color, text, lineNumber, caller);
    }


    public static void Internal_Logger(string time, int Channel, Color color, string text, int lineNumber, string caller)
    {
        string path_string = "File: " + System.IO.Path.GetFileName(caller) + ", " + lineNumber;

        try
        {
            //Logger_New_Line.Logger_Viewer.Add_New_Logger_Line(new Logger_New_Line.Logger_Message(time, Channel, color, path_string + ":: " + text));
        }
        catch
        {
            Debug.Log("Error in Internal Logger");
        }
    }
    #endregion
}

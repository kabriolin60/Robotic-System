using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Logger_Line : MonoBehaviour
{
    public int Channel_id;  //Id du channel pour savoir s'il doit être affiché ou non   

    public GameObject Time_Text;
    public GameObject Channel_Text;
    public GameObject Text;    

    public void SetNewLine(string time, int channel, Color color, string text)
    {
        this.Channel_id = channel;

        Time_Text.GetComponent<TextMeshProUGUI>().text = time;
        Channel_Text.GetComponent<TextMeshProUGUI>().text = $"Ch {channel}";
        Text.GetComponent<TextMeshProUGUI>().text = text;

        Text.GetComponent<TextMeshProUGUI>().color = color;
    }
}

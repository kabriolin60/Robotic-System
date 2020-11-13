using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Logger_Channel : MonoBehaviour
{
    public int Channel_id;
    public GameObject ChannelText;
    public string Channel_Name = "Channel";
    public GameObject Message_Holder;

    private void Start()
    {
        ChannelText.GetComponent<TextMeshProUGUI>().text= $"{Channel_Name} #{Channel_id}";
    }

    public void ShowHide_Channel()
    {
        Component[] Lines;
        Lines = Message_Holder.GetComponentsInChildren(typeof(Logger_Line), true) ;

        foreach (Logger_Line _line in Lines)
        {
            if (_line.Channel_id == this.Channel_id)
            {
                if (this.GetComponent<Toggle>().isOn)
                {
                    _line.gameObject.SetActive(true);
                }
                else
                {
                    _line.gameObject.SetActive(false);
                }
            }
        }
    }
}

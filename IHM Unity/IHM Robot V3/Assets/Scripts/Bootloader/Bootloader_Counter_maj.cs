using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Bootloader_Counter_maj : MonoBehaviour
{
    public GameObject text;
    public void Change_Value()
    {
        this.text.GetComponent<TextMeshProUGUI>().text = $"{this.GetComponent<Slider>().value}/{this.GetComponent<Slider>().maxValue} lines";
    }
}

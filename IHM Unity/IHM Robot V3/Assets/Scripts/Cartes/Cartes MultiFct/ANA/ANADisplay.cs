using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class ANADisplay : MonoBehaviour
{
    public GameObject Slider;

    public void Set_ANA_Value()
    {
        float value = Slider.GetComponent<Slider>().value;
        this.GetComponent<TextMeshProUGUI>().text = value.ToString();
    }
}

﻿using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class AX12Display : MonoBehaviour
{
    public GameObject Slider;

    public void Set_AX12_Value()
    {
        float value = Slider.GetComponent<Slider>().value;
        this.GetComponent<TextMeshProUGUI>().text = value.ToString();
    }
}

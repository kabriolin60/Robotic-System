using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Autoscroll : MonoBehaviour
{
    public GameObject Scrollview;
    // Update is called once per frame

    float previous_scoll_position;

    public void FixedUpdate()
    {
        if (GetComponent<Toggle>().isOn)
        {
            Scrollview.GetComponent<Scrollbar>().value = 0;
        }
    }

    public void Onchange()
    {
        Debug.Log("Autoscroll change");
        Scrollview.GetComponent<Scrollbar>().value = 0;
    }
}

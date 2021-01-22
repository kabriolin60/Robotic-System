using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Element_Avion : MonoBehaviour
{
    public TMP_InputField Appareil;

    public void Set_Name(string name)
    {
        Appareil.text = name;
    }
}

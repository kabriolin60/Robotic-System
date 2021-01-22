using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Element_Batterie : MonoBehaviour
{
    public TMP_InputField Batterie_Name;

    public void Set_Name(string name)
    {
        Batterie_Name.text = name;
    }
}

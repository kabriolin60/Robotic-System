using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;


public class Element_Programme : MonoBehaviour
{
    public TMP_InputField Programme;

    public void Set_Name(string name)
    {
        Programme.text = name;
    }
}

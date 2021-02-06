using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class test : MonoBehaviour
{
    public TextMeshProUGUI Debugger;

    // Start is called before the first frame update
    void Start()
    {
        if(this.GetComponent<File_Manager>().Load_XML_Local_File<XML_Datas>("/test3.xml") != null)
        {
            //Chargement réussi du fichier
            Debugger.text = ("File loaded");
        }
        else
        {
            //Echec de chargement du fichier
            Debugger.text = ("Failed to load the File");
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

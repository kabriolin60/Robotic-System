using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Main_Data : MonoBehaviour
{
    const string Xml_File_Name = "/test5.xml";

    private XML_Datas Lipo_Datas = new XML_Datas();


    void Start()
    {
        Load_Existing_XML();
    }

    public void Load_Existing_XML()
    {
        Lipo_Datas = this.GetComponent<File_Manager>().Load_XML<XML_Datas>(Xml_File_Name);
        if (Lipo_Datas != null)
        {
            //Chargement réussi du fichier
            Debug.Log("File loaded");


            Debug.Log("Appareils Charges:" + Lipo_Datas.Known_Elements.Known_Appareils.Count.ToString());
            Debug.Log("Batteries Charges:" + Lipo_Datas.Known_Elements.Known_Batteries.Count.ToString());
            Debug.Log("Programmes Charges:" + Lipo_Datas.Known_Elements.Known_Programmes.Count.ToString());
            Debug.Log("Operations Charges:" + Lipo_Datas.Operations.Count.ToString());
        }
        else
        {
            //Echec de chargement du fichier
            Debug.Log("Failed to load the File");
        }
    }

    public void Save_XML()
    {
        if (this.GetComponent<File_Manager>().Save_XML<XML_Datas>(Xml_File_Name, Lipo_Datas))
        {
            //Chargement réussi du fichier
            Debug.Log("File saved");
        }
        else
        {
            //Echec de chargement du fichier
            Debug.Log("Failed to save the File");
        }
    }





    public int Get_Number_Known_Aircraft()
    {
        return Lipo_Datas.Known_Elements.Known_Appareils.Count;
    }

    public void Add_New_Aircraft(Appareil app_to_add)
    {
        Lipo_Datas.Known_Elements.Known_Appareils.Add(app_to_add);
        Save_XML();
    }



    public int Get_Number_Known_Batteries()
    {
        return Lipo_Datas.Known_Elements.Known_Batteries.Count;
    }

    public void Add_New_Batterie(Batterie app_to_add)
    {
        Lipo_Datas.Known_Elements.Known_Batteries.Add(app_to_add);
        Save_XML();
    }



    public int Get_Number_Known_Programmes()
    {
        return Lipo_Datas.Known_Elements.Known_Programmes.Count;
    }

    public void Add_New_Programme(Programme app_to_add)
    {
        Lipo_Datas.Known_Elements.Known_Programmes.Add(app_to_add);
        Save_XML();
    }
}

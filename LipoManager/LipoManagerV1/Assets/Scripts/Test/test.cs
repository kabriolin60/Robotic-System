using System;
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
        XML_Datas test_data = new XML_Datas();

        Appareil new_appareil = new Appareil();
        new_appareil.Id = 0;
        new_appareil.Marque = "Marque_0";
        new_appareil.Modele = "Modele_0";
        new_appareil.Name = "Name_0";
        new_appareil.Price = 125;
        new_appareil.State = XML_Datas.State.In_Use;
        new_appareil.Buy_Date = DateTime.Now;

        test_data.Known_Elements.Known_Appareils.Add(new_appareil);

        Programme new_programme = new Programme();
        new_programme.Id = 0;
        new_programme.Name = "Programme_0";

        test_data.Known_Elements.Known_Programmes.Add(new_programme);

        Batterie new_batterie = new Batterie();
        new_batterie.Id = 0;
        new_batterie.Marque = "Marque_0";
        new_batterie.Modele = "Modele_0";
        new_batterie.Name = "Name_0";
        new_batterie.Price = 12;
        new_batterie.Reference = "Ref_0";
        new_batterie.State = XML_Datas.State.In_Use;
        new_batterie.S_Config = 5;
        new_batterie.Fournisseur = "Fournisseur_0";
        new_batterie.C_Rate = 45;
        new_batterie.Capacity = 5100;
        new_batterie.Buy_Date = DateTime.Now;

        test_data.Known_Elements.Known_Batteries.Add(new_batterie);

        Operation new_Operation = new Operation();
        new_Operation.Id = 0;
        new_Operation.date = DateTime.Now;
        new_Operation.Ope_Type = Operation.Operation_Type.Charge;
        new_Operation.Elements_List = new List<Element>();
        Element new_element = new Element();
        new_element.Name = "Name_0";
        new_element.Type = Element.Element_Type.Batterie;
        new_element.Courant = 0.2F;
        new_element.Capacite = 300;

        new_Operation.Elements_List.Add(new_element);

        test_data.Operations.Add(new_Operation);

        /*
        if (this.GetComponent<File_Manager>().Save_XML<XML_Datas>("/test4 - modele.xml", test_data))
        {
            //Chargement réussi du fichier
            Debugger.text += ("File saved\n");
        }
        else
        {
            //Echec de chargement du fichier
            Debugger.text += ("Failed to save the File\n");
        }*/


        
        /*XML_Datas test_read_data = new XML_Datas();
        test_read_data = this.GetComponent<File_Manager>().Load_XML<XML_Datas>("/test3 - Copie.xml");
        if (test_read_data != null)
        {
            //Chargement réussi du fichier
            Debugger.text += ("File loaded\n");


            Debugger.text += ("Appareils Charges:" + test_read_data.Known_Elements.Known_Appareils.Count.ToString() + "\n");
            Debugger.text += ("Batteries Charges:" + test_read_data.Known_Elements.Known_Batteries.Count.ToString() + "\n");
            Debugger.text += ("Programmes Charges:" + test_read_data.Known_Elements.Known_Programmes.Count.ToString() + "\n");
            Debugger.text += ("Operations Charges:" + test_read_data.Operations.Count.ToString() + "\n");
        }
        else
        {
            //Echec de chargement du fichier
            Debugger.text += ("Failed to load the File\n");
        }*/
    }
}

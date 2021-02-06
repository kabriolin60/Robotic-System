using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using TMPro;
using UnityEngine;

public class Add_New_Element : MonoBehaviour
{
    public GameObject Script_Holder;


    public GameObject Panel_Add_Aircraft;
    public GameObject Panel_Add_Battery;
    public GameObject Panel_Add_Schedule;

    private int Type_Panel = 0;


    public void Display_Panel(int Panel_Type)
    {
        Type_Panel = Panel_Type;

        switch (Panel_Type)
        {
            case 1:
                Panel_Add_Aircraft.SetActive(true);
                Panel_Add_Battery.SetActive(false);
                Panel_Add_Schedule.SetActive(false);
                break;

            case 2:
                Panel_Add_Aircraft.SetActive(false);
                Panel_Add_Battery.SetActive(true);
                Panel_Add_Schedule.SetActive(false);
                break;

            case 3:
                Panel_Add_Aircraft.SetActive(false);
                Panel_Add_Battery.SetActive(false);
                Panel_Add_Schedule.SetActive(true);
                break;

            default:
                Panel_Add_Aircraft.SetActive(false);
                Panel_Add_Battery.SetActive(false);
                Panel_Add_Schedule.SetActive(false);
                break;
        }
    }


    public void Validate_Save()
    {
        switch (Type_Panel)
        {
            default:
                break;

            case 1:
                //Sauvegarde nouveau Appareil;
                Save_New_Aircraft(Panel_Add_Aircraft.transform);
                break;

            case 2:
                //Sauvegarde nouvelle Batterie;
                break;

            case 3:
                //Souvegarde nouveau programme;
                break;
        }
    }


    


    public void Save_New_Aircraft(Transform parent_Panel)
    {
        Appareil new_Appareil = new Appareil();

        new_Appareil.Id = Script_Holder.GetComponent<Main_Data>().Get_Number_Known_Aircraft();
        new_Appareil.Marque = FindChildByRecursion(parent_Panel, "InputField_Name").GetComponent<TMP_InputField>().text;
        new_Appareil.Modele = FindChildByRecursion(parent_Panel, "InputField_Marque").GetComponent<TMP_InputField>().text;
        new_Appareil.Name = FindChildByRecursion(parent_Panel, "InputField_Modele").GetComponent<TMP_InputField>().text;
        new_Appareil.Price = int.Parse(FindChildByRecursion(parent_Panel, "InputField_Prix").GetComponent<TMP_InputField>().text, CultureInfo.CreateSpecificCulture("fr-FR"));
        new_Appareil.State = (XML_Datas.State)FindChildByRecursion(parent_Panel, "Dropdown_State").GetComponent<TMP_Dropdown>().value;
        new_Appareil.Buy_Date = DateTime.Now;

        Script_Holder.GetComponent<Main_Data>().Add_New_Aircraft(new_Appareil);

        Debug.Log("Save New Aircraft");
    }


    internal static Transform FindChildByRecursion(Transform aParent, string aName)
    {
        if (aParent == null) return null;

        var result = aParent.transform.Find(aName);

        if (result != null)
            return result;

        foreach (Transform child in aParent)
        {
            result = FindChildByRecursion(child, aName);
            if (result != null)
                return result;
        }
        return null;
    }

}

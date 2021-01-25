using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Main_Display : MonoBehaviour
{
    public GameObject Main_Ope_Container;

    public GameObject Prefab_Main_Ope_Display;

    public void Display_All_Ope(List<QR_Scanner.Operation> Liste_Operations)
    {
        foreach(QR_Scanner.Operation ope in Liste_Operations)
        {
            Display_New_Ope(ope);
        }
    }

    public void Display_New_Ope(QR_Scanner.Operation new_ope)
    {
        GameObject new_obj = Instantiate(Prefab_Main_Ope_Display, Main_Ope_Container.transform);

        new_obj.GetComponent<Main_Display_Element>().Set_Ope_Values(new_ope);
    }
}

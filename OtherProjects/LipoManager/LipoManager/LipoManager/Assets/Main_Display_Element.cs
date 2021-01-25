using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Main_Display_Element : MonoBehaviour
{
    public TextMeshProUGUI Type_Ope;
    public TextMeshProUGUI Date;

    public GameObject Panel_Appareil_Programme;
    public TextMeshProUGUI Appareil;
    public TextMeshProUGUI Programme;
    public TextMeshProUGUI Temps_Vol;

    public TextMeshProUGUI Batterie_1;
    public TextMeshProUGUI Batterie_2;



    public void Set_Ope_Values(QR_Scanner.Operation ope)
    {
        Type_Ope.text = ope.Ope_Type.ToString();
        Date.text = ope.date.ToString("dd/MM/yyyy");

        foreach (QR_Scanner.QRCode_Data element in ope.Elements_Presents_List)
        {
            switch(element.Type)
            {
                case QR_Scanner.QRCode_Data.Element_Type.Avion:
                    Appareil.text = element.Name;
                    break;

                case QR_Scanner.QRCode_Data.Element_Type.Programme:
                    Programme.text = element.Name;
                    Temps_Vol.text = element.Temps_Vol.ToString() + " min";
                    break;

                case QR_Scanner.QRCode_Data.Element_Type.Batterie:

                    break;

                default:
                    break;
            }
        }
        

        switch (ope.Ope_Type)
        {
            case QR_Scanner.Operation.Operation_Type.Vol:
                Appareil.gameObject.SetActive(true);
                Panel_Appareil_Programme.SetActive(true);
                break;
        }
    }
}

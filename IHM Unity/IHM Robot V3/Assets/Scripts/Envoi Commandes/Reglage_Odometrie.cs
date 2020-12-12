using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Reglage_Odometrie: MonoBehaviour
{
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

    public void Envoi_Reglages_Odometrie(GameObject input)
    {
        TMP_InputField Coef_Distance;
        TMP_InputField Coef_Rotation;
        TMP_InputField Correction_Diametre_roues;

        //When pre-setting

        Coef_Distance = FindChildByRecursion(input.transform, "Coef D").GetComponent<TMP_InputField>();
        Coef_Rotation = FindChildByRecursion(input.transform, "Coef Rot").GetComponent<TMP_InputField>();
        Correction_Diametre_roues = FindChildByRecursion(input.transform, "Correction Diametre").GetComponent<TMP_InputField>();

        TMP_Dropdown Asserv;
        Asserv = FindChildByRecursion(input.transform, "Type Asserv").GetComponent<TMP_Dropdown>();

        bool Simulation = FindChildByRecursion(input.transform, "Simulation").GetComponent<Toggle>().enabled;


        Odometrie.Reglages_Odometrie reglages = new Odometrie.Reglages_Odometrie();

        reglages.Coef_D = float.Parse(Coef_Distance.text, Common_settings.culture);
        reglages.Coef_Rot = float.Parse(Coef_Rotation.text, Common_settings.culture);
        reglages.Correction_Diametre = float.Parse(Correction_Diametre_roues.text, Common_settings.culture);

        reglages.simulation = (byte)(Simulation ? 1 : 0);

        reglages.asserv = (Odometrie.Type_Asservissements)Asserv.value;



        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame = Communication.GetTrameFromClass<Odometrie.Reglages_Odometrie>(reglages);

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.PARAMETRES_ODOMETRIE;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }


    public void OnChange_Pre_Reglage_Odometrie(GameObject input)
    {
        TMP_InputField Coef_Distance;
        TMP_InputField Coef_Rotation;
        TMP_InputField Correction_Diametre_roues;

        //When pre-setting

        Coef_Distance = FindChildByRecursion(input.transform, "Coef D").GetComponent<TMP_InputField>();
        Coef_Rotation = FindChildByRecursion(input.transform, "Coef Rot").GetComponent<TMP_InputField>();
        Correction_Diametre_roues = FindChildByRecursion(input.transform, "Correction Diametre").GetComponent<TMP_InputField>();

        TMP_Dropdown QuelRobot;
        QuelRobot = FindChildByRecursion(input.transform, "Robot").GetComponent<TMP_Dropdown>();


        Odometrie.Reglages_Odometrie parametres = Common_settings.static_Pre_Reglages_Odometrie[QuelRobot.value];

        Coef_Distance.text = parametres.Coef_D.ToString();
        Coef_Rotation.text = parametres.Coef_Rot.ToString();
        Correction_Diametre_roues.text = parametres.Correction_Diametre.ToString();
    }
}


public class Odometrie
{

    public enum Type_Asservissements : byte
    {
        Polaire,
        Vitesse_Droite_Vitesse_Gauche_Indep,
        Vitesse_D_G__Distance_Angle,
        Moteurs_Aux
    }

    [StructLayout(LayoutKind.Sequential)]
    public class Reglages_Odometrie
    {
        [MarshalAs(UnmanagedType.R4)]
        public float Coef_D;

        [MarshalAs(UnmanagedType.R4)]
        public float Coef_Rot;

        [MarshalAs(UnmanagedType.R4)]
        public float Correction_Diametre;

        [MarshalAs(UnmanagedType.U1)]
        public byte simulation;

        [MarshalAs(UnmanagedType.U1)]
        public Type_Asservissements asserv;
    }
}

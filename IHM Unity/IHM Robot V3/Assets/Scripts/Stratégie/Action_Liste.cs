using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Action_Liste : MonoBehaviour
{
    public GameObject Action_Viewer;
    public GameObject Action_Line_Prefab;

    private byte Nombre_Maximum_Actions;

    private static Dictionary<byte, Action_Line> Dictionnaire_Action_Gros_Robot = new Dictionary<byte, Action_Line>();
    private static Dictionary<byte, Action_Line> Dictionnaire_Action_Petit_Robot = new Dictionary<byte, Action_Line>();
    private static Dictionary<byte, GameObject> Dictionnaire_Action_2_Robots = new Dictionary<byte, GameObject>();


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




    // Start is called before the first frame update
    void Start()
    {
        Nombre_Maximum_Actions = Common_settings.static_Nombre_Maximum_Actions;

        //Crée toutes les actions possible sans Data
        for(int i=0; i < Nombre_Maximum_Actions; i++)
        {
            GameObject new_action = Instantiate(Action_Line_Prefab, Action_Viewer.transform) as GameObject;
            new_action.SetActive(false);

            Dictionnaire_Action_2_Robots.Add((byte)i, new_action);

            Dictionnaire_Action_Gros_Robot.Add((byte)i, FindChildByRecursion(new_action.transform, "Action cote Gros Robot").GetComponent<Action_Line>());
            Dictionnaire_Action_Petit_Robot.Add((byte)i, FindChildByRecursion(new_action.transform, "Action cote Petit Robot").GetComponent<Action_Line>());
        }
    }

    public static Action_Line Get_Existing_Line(byte key, Infos_Carte.Com_Position_Robot_Identification Robot_ID)
    {
        Dictionary<byte, Action_Line> Dictionnaire = new Dictionary<byte, Action_Line>();

        switch (Robot_ID)
        {
            case Infos_Carte.Com_Position_Robot_Identification.Gros_Robot:
                Dictionnaire = Dictionnaire_Action_Gros_Robot;
                break;

            case Infos_Carte.Com_Position_Robot_Identification.Petit_Robot:
                Dictionnaire = Dictionnaire_Action_Petit_Robot;
                break;
        }


        Action_Line updated_line;
        if (!Dictionnaire.TryGetValue(key, out updated_line))
        {
            // the key isn't in the dictionary.
            return null; // or whatever you want to do
        }

        return updated_line;
    }


    public static void Update_Action(Action_Strategie.Action_Datas data, Infos_Carte.Com_Position_Robot_Identification Robot_ID)
    {
        Dictionary<byte, Action_Line> Dictionnaire = new Dictionary<byte, Action_Line>();

        switch (Robot_ID)
        {
            case Infos_Carte.Com_Position_Robot_Identification.Gros_Robot:
                Dictionnaire = Dictionnaire_Action_Gros_Robot;
                break;

            case Infos_Carte.Com_Position_Robot_Identification.Petit_Robot:
                Dictionnaire = Dictionnaire_Action_Petit_Robot;
                break;
        }

        Action_Line updated_line;
        if (!Dictionnaire.TryGetValue(data.ID, out updated_line))
        {
            // the key isn't in the dictionary.
            return; // or whatever you want to do
        }

        //Cette action existe dans le dictionnaire

        //Mise à jour
        updated_line.Update_Action(data);

        //Activation
        GameObject updated_line_Object;
        if (!Dictionnaire_Action_2_Robots.TryGetValue(data.ID, out updated_line_Object))
        {
            // the key isn't in the dictionary.
            return; // or whatever you want to do
        }
        updated_line_Object.SetActive(true);
    }
}

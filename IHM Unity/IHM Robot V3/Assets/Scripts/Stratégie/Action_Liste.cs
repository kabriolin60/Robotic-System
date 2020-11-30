using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Action_Liste : MonoBehaviour
{
    public GameObject Action_Viewer;
    public GameObject Action_Line_Prefab;

    private byte Nombre_Maximum_Actions;

    private static Dictionary<byte, Action_Line> Dictionnaire_Action = new Dictionary<byte, Action_Line>();

    // Start is called before the first frame update
    void Start()
    {
        Nombre_Maximum_Actions = Common_settings.static_Nombre_Maximum_Actions;

        //Crée toutes les actions possible sans Data
        for(int i=0; i < Nombre_Maximum_Actions; i++)
        {
            GameObject new_action = Instantiate(Action_Line_Prefab, Action_Viewer.transform) as GameObject;
            new_action.SetActive(false);
            Dictionnaire_Action.Add((byte)i, new_action.GetComponent<Action_Line>());
        }
    }

    public static Action_Line Get_Existing_Line(byte key)
    {
        Action_Line updated_line;
        if (!Dictionnaire_Action.TryGetValue(key, out updated_line))
        {
            // the key isn't in the dictionary.
            return null; // or whatever you want to do
        }

        return updated_line;
    }


    public static void Update_Action(Action.Action_Datas data)
    {
        Action_Line updated_line;
        if (!Dictionnaire_Action.TryGetValue(data.ID, out updated_line))
        {
            // the key isn't in the dictionary.
            return; // or whatever you want to do
        }

        //Cette action existe dans le dictionnaire

        //Mise à jour
        updated_line.Update_Action(data);

        //Activation
        updated_line.gameObject.SetActive(true);
    }
}

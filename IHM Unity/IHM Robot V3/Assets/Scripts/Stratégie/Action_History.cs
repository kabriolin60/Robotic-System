using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Action_History : MonoBehaviour
{
    public GameObject Action_Viewer;
    public GameObject Action_Line_Prefab;

    public void Update_Action(Action.Action_Datas data)
    {
        GameObject new_action = Instantiate(Action_Line_Prefab, Action_Viewer.transform) as GameObject;

        Action_Line updated_line = new_action.GetComponent<Action_Line>();
        updated_line.Update_Action(data);
    }
}

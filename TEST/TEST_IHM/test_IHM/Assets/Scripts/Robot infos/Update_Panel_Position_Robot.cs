using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Update_Panel_Position_Robot : MonoBehaviour
{
    public GameObject Robot_Position_X;
    public GameObject Robot_Position_Y;
    public GameObject Robot_Angle;

    public int Robot_id;


    // Update is called once per frame
    void FixedUpdate()
    {
        try
        {
            Infos_Carte.Com_Reponse_Info last_position = Last_Infos.Robots_Last_Info[Robot_id].Get_Last_Infos(0);
            float pos_x = (float)(last_position.PositionRobot.Position_X) / 10;
            float pos_y = (float)(last_position.PositionRobot.Position_Y) / 10;
            float angle = (float)(last_position.PositionRobot.Angle) / 100;

            Robot_Position_X.GetComponent<TextMeshProUGUI>().text = $"X = {pos_x.ToString()} mm";
            Robot_Position_Y.GetComponent<TextMeshProUGUI>().text = $"Y = {pos_y.ToString()} mm";
            Robot_Angle.GetComponent<TextMeshProUGUI>().text = $"Ang = {angle.ToString()} °";
        }
        catch
        {

        }
    }
}

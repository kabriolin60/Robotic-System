using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Position_Robots : MonoBehaviour
{
    private Vector3 Position = new Vector3();
    private float Orientation = 0;
    private Vector3 Rotation = new Vector3();
    public float Lefp_Rate;

    public Infos_Carte.Com_Position_Robot_Identification Robot_id;

    void FixedUpdate()
    {
        try
        {
            Infos_Carte.Com_Reponse_Info last_position = Last_Infos.Robots_Last_Info[(int)Robot_id].Get_Last_Infos(0);
            Position.x = (float)(last_position.PositionRobot.Position_X) / 10 / 1000;
            Position.y = (float)(last_position.PositionRobot.Position_Y) / 10 / 1000;
            Orientation = (float)(last_position.PositionRobot.Angle) / 100;
            transform.localPosition = Vector3.Lerp(transform.localPosition, Position, Lefp_Rate);

            Rotation.z = Orientation;

            Rotation.z = Mathf.Lerp(transform.rotation.eulerAngles.z, Orientation, Lefp_Rate);

            this.transform.localEulerAngles = Rotation;
        }
        catch
        {

        }
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Position_Robots : MonoBehaviour
{
    public bool Manual_Position = false;

    public Vector3 Position = new Vector3();
    public float Orientation = 0;
    public Vector3 Rotation = new Vector3();
    public float Lerp_Rate;

    public Infos_Carte.Com_Position_Robot_Identification Robot_id;

    void FixedUpdate()
    {
        Infos_Carte.Com_Reponse_Info last_position;
        try
        {
            if (!Manual_Position)
            {
                last_position = Last_Infos.Robots_Last_Info[(int)Robot_id].Get_Last_Infos(0);
                Position.x = (float)(last_position.PositionRobot.Position_X) / 10 / 1000;
                Position.y = (float)(last_position.PositionRobot.Position_Y) / 10 / 1000;
                Orientation = (float)(last_position.PositionRobot.Angle) / 100;
            }

            if (Position.x != 0 && Position.y != 0)
            {
                transform.localPosition = Vector3.Lerp(transform.localPosition, Position, Lerp_Rate);

                Rotation.z = Orientation;

                this.transform.localEulerAngles = Rotation;
            }

        }
        catch
        {

        }
    }
}

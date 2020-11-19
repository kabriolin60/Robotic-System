using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trappe_PR_TEST : MonoBehaviour
{
    public Infos_Carte.Com_Position_Robot_Identification Robot_id;
    public int Carte_Id;
    public int Numero_Servo;

    public float Position_Trappe_Bas = 20;
    public float Position_Trappe_Haut = 150;

    public short Servo_Trappe_Bas = 100;
    public short Servo_Trappe_Haut = 500;

    // Update is called once per frame
    void Update()
    {
        try
        {
            Infos_Carte.Com_Reponse_Info last_position = Last_Infos.Robots_Last_Info[(int)Robot_id].Get_Last_Infos(Carte_Id - 1);

            float position_trappe = Position_Trappe_Avant(last_position.Position_Servos.Position[Numero_Servo]);

            this.transform.localPosition = new Vector3(this.transform.localPosition.x, this.transform.localPosition.y, Mathf.Lerp(this.transform.localPosition.z, position_trappe, 0.2f));
        }
        catch
        { 
        }
    }

    public float Position_Trappe_Avant(int position)
    {
        float new_position = 0;

        float a = ((float)(Position_Trappe_Haut - Position_Trappe_Bas)) / ((float)(Servo_Trappe_Haut - Servo_Trappe_Bas));

        new_position = (position - Servo_Trappe_Bas) * a + Position_Trappe_Bas;

        if (new_position < Position_Trappe_Bas)
            new_position = Position_Trappe_Bas;

        if (new_position > Position_Trappe_Haut)
            new_position = Position_Trappe_Haut;

        return new_position;
        //Debug.Log("Position trappe= " + new_position.ToString());
    }
}

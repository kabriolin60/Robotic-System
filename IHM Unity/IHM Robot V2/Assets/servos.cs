using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class servos : MonoBehaviour
{
    // Start is called before the first frame update
    public GameObject Servo_Prefab;

    List<servo_elementaire> Servos = new List<servo_elementaire>();

    void Start()
    {
        //Création des panels de servos pour la carte 

        for (int i = 0; i < Structures_Echange.Infos_Carte.NB_SERVO /2; i++)
        {
            Servos.Add(new servo_elementaire());            
            Servos[i].UI_Object = Instantiate(Servo_Prefab, new Vector3(-80 + 80 * i, 90, 0), Quaternion.identity);
            Servos[i].Init(i);
            Servos[i].UI_Object.transform.SetParent(this.transform, false);
        }

        for (int i = Structures_Echange.Infos_Carte.NB_SERVO/2; i < Structures_Echange.Infos_Carte.NB_SERVO; i++)
        {
            Servos.Add(new servo_elementaire());            
            Servos[i].UI_Object = Instantiate(Servo_Prefab, new Vector3(-80 + 80 * (i - 3), 90 - 210, 0), Quaternion.identity);
            Servos[i].Init(i);
            Servos[i].UI_Object.transform.SetParent(this.transform, false);
        }
    }


    public void Update_Servos_Position(Structures_Echange.Infos_Carte.Com_Position_Servos positions)
    {
        for (int i = 0; i < Structures_Echange.Infos_Carte.NB_SERVO; i++)
        {
            Servos[i].update_servo_value(positions.Position[i]);
        }
    }


    public int GetServoPosition(int idServo)
    {
        return Servos[idServo].Get_Servo_Position();
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Servos_maj : MonoBehaviour
{
    public GameObject Servo0;
    public GameObject Servo1;
    public GameObject Servo2;
    public GameObject Servo3;
    public GameObject Servo4;
    public GameObject Servo5;

    public void Maj_Servos(Infos_Carte.Com_Position_Servos values)
    {
        this.Servo0.GetComponent<Slider>().value = values.Position[0];
        this.Servo1.GetComponent<Slider>().value = values.Position[1];
        this.Servo2.GetComponent<Slider>().value = values.Position[2];
        this.Servo3.GetComponent<Slider>().value = values.Position[3];
        this.Servo4.GetComponent<Slider>().value = values.Position[4];
        this.Servo5.GetComponent<Slider>().value = values.Position[5];
    } 
}

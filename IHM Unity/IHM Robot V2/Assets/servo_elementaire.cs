using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class servo_elementaire : MonoBehaviour
{
    public GameObject UI_Object;
    public int number;

    public Slider slider;
    public Text Name;
    public Text Value_Texte;

    public void Init(int numero)
    {
        number = numero;
        this.slider = UI_Object.GetComponentInChildren<Slider>();

        getChildGameObject(UI_Object, "Name Text").GetComponent<Text>().text = "Servo #" + numero.ToString();
        this.Value_Texte = getChildGameObject(UI_Object, "Value texte").GetComponent<Text>();
    }


    public void update_servo_value(int value)
    {
        this.slider.value = value;
        Value_Texte.text = value.ToString();
    }


    public int Get_Servo_Position()
    {
        return (int)this.slider.value;
    }


    public GameObject getChildGameObject(GameObject fromGameObject, string withName)
    {
        Transform[] ts = fromGameObject.transform.GetComponentsInChildren<Transform>(true);
        foreach (Transform t in ts) if (t.gameObject.name == withName) return t.gameObject;
        return null;
    }
}

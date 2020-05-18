using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AX12_elementaire : MonoBehaviour
{
    public GameObject UI_Object;
    public int number;

    public Slider slider_position;
    public Slider slider_torque;
    public Text Name;
    public Text Position_Texte;
    public Text Torque_Texte;

    // Start is called before the first frame update
    public void Init(int numero)
    {
        number = numero;
        this.slider_position = getChildGameObject(UI_Object, "Slider_Position").GetComponent<Slider>();
        this.slider_torque = getChildGameObject(UI_Object, "Slider_Torque").GetComponent<Slider>();


        getChildGameObject(UI_Object, "AX 12").GetComponent<Text>().text = "AX 12 #" + numero.ToString();
        this.Position_Texte = getChildGameObject(UI_Object, "Position Text").GetComponent<Text>();
        this.Torque_Texte = getChildGameObject(UI_Object, "Torque Text").GetComponent<Text>();
    }

    public void Update_Position_Torque(int position, int torque)
    {
        this.slider_position.value = position;
        Position_Texte.text = position.ToString();

        this.slider_torque.value = torque;
        Torque_Texte.text = torque.ToString();
    }


    public int Get_AX12_Position()
    {
        return (int)this.slider_position.value;
    }



    public GameObject getChildGameObject(GameObject fromGameObject, string withName)
    {
        Transform[] ts = fromGameObject.transform.GetComponentsInChildren<Transform>(true);
        foreach (Transform t in ts) if (t.gameObject.name == withName) return t.gameObject;
        return null;
    }
}

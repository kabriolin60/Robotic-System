using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ADC_elementaire : MonoBehaviour
{
    public GameObject UI_Object;
    public int number;

    public Slider slider;
    public Text Name;
    public Text Texte;

    // Start is called before the first frame update
    public void Init(int numero)
    {
        number = numero;
        this.slider = getChildGameObject(UI_Object, "Slider_Position").GetComponent<Slider>();


        getChildGameObject(UI_Object, "ADC").GetComponent<Text>().text = "ADC #" + numero.ToString();
        this.Texte = getChildGameObject(UI_Object, "Position Text").GetComponent<Text>();

    }

    public void Update_Value(int valeur)
    {
        this.slider.value = valeur;
        Texte.text = valeur.ToString();
    }

    public GameObject getChildGameObject(GameObject fromGameObject, string withName)
    {
        Transform[] ts = fromGameObject.transform.GetComponentsInChildren<Transform>(true);
        foreach (Transform t in ts) if (t.gameObject.name == withName) return t.gameObject;
        return null;
    }
}

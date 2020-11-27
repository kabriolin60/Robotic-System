using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Create_Actions_Container : MonoBehaviour
{
    public GameObject Switch_Holder;
    public GameObject Contenu_Panel;

    public GameObject Onglet_Prefab;
    public GameObject Strategie_Panel_Prefab;

    private List<GameObject> list_Panels_Robots = new List<GameObject>(); 

    // Start is called before the first frame update
    void Start()
    {
        
        //Premier Panel qui est actif
        GameObject go_Panel_Strategie = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel_Strategie.GetComponent<Toggle>().isOn = true;
        go_Panel_Strategie.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        //Le premier contenu
        GameObject go_Strategie_contenu = Instantiate(Strategie_Panel_Prefab, Contenu_Panel.transform) as GameObject;
        list_Panels_Robots.Add(go_Strategie_contenu);
        go_Strategie_contenu.SetActive(true);

        //Associe le panel à cet onglet
        go_Panel_Strategie.GetComponent<Toggle>().onValueChanged.AddListener(go_Strategie_contenu.SetActive);

        //Set le nom de l'onglet
        go_Panel_Strategie.name = $"Onglet Strategie";
        //Set onglet button name
        go_Panel_Strategie.GetComponentInChildren<TextMeshProUGUI>().text = "Strategie";
        //Set le nom du panel
        go_Strategie_contenu.name = $"Tab Strategie";        
    }
}

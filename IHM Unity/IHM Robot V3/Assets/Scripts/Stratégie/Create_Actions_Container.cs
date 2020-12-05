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
    public GameObject PID_Panel_Prefab;

    private List<GameObject> list_Panels = new List<GameObject>(); 

    // Start is called before the first frame update
    void Start()
    {
        //Premier Panel qui est actif
        Create_Strategie_Panel(true).SetActive(true);

        //Panel de PID
        Create_PID_Panel(false).SetActive(false);
    }

    private GameObject Create_Strategie_Panel(bool is_tabOn)
    {
        GameObject go_Panel = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel.GetComponent<Toggle>().isOn = is_tabOn;
        go_Panel.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        //Le premier contenu
        GameObject go_contenu = Instantiate(Strategie_Panel_Prefab, Contenu_Panel.transform) as GameObject;
        list_Panels.Add(go_contenu);

        //Associe le panel à cet onglet
        go_Panel.GetComponent<Toggle>().onValueChanged.AddListener(go_contenu.SetActive);

        //Set le nom de l'onglet
        go_Panel.name = $"Onglet Strategie";
        //Set onglet button name
        go_Panel.GetComponentInChildren<TextMeshProUGUI>().text = "Strategie";
        //Set le nom du panel
        go_contenu.name = $"Tab Strategie";

        return go_contenu;
    }

    private GameObject Create_PID_Panel(bool is_tabOn)
    {
        GameObject go_Panel = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel.GetComponent<Toggle>().isOn = is_tabOn;
        go_Panel.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        GameObject go_contenu = Instantiate(PID_Panel_Prefab, Contenu_Panel.transform) as GameObject;
        list_Panels.Add(go_contenu);

        //Associe le panel à cet onglet
        go_Panel.GetComponent<Toggle>().onValueChanged.AddListener(go_contenu.SetActive);

        //Set le nom de l'onglet
        go_Panel.name = $"Onglet PID";
        //Set onglet button name
        go_Panel.GetComponentInChildren<TextMeshProUGUI>().text = "PIDs";
        //Set le nom du panel
        go_contenu.name = $"Tab PID";

        return go_contenu;
    }
}

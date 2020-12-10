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
    public GameObject Deplacement_Panel_Prefab;

    public GameObject Servos_Panel_Prefab;

    private List<GameObject> list_Panels = new List<GameObject>(); 

    // Start is called before the first frame update
    void Start()
    {
        //Premier Panel qui est actif
        Create_Panel(true, Strategie_Panel_Prefab, "Stratégie").SetActive(true);

        //Panel de PID
        Create_Panel(false, PID_Panel_Prefab, "PID").SetActive(false);

        //Panel de Déplacements
        Create_Panel(false, Deplacement_Panel_Prefab, "Déplacements").SetActive(false);

        //Panel de déplacement des servos
        Create_Panel(false, Servos_Panel_Prefab, "Servos").SetActive(false);
    }


    private GameObject Create_Panel(bool is_tabOn, GameObject panel, string name)
    {
        GameObject go_Panel = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel.GetComponent<Toggle>().isOn = is_tabOn;
        go_Panel.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        //Le premier contenu
        GameObject go_contenu = Instantiate(panel, Contenu_Panel.transform) as GameObject;
        list_Panels.Add(go_contenu);

        //Associe le panel à cet onglet
        go_Panel.GetComponent<Toggle>().onValueChanged.AddListener(go_contenu.SetActive);

        //Set le nom de l'onglet
        go_Panel.name = $"Onglet {name}";
        //Set onglet button name
        go_Panel.GetComponentInChildren<TextMeshProUGUI>().text = name;
        //Set le nom du panel
        go_contenu.name = $"Tab {name}";

        return go_contenu;
    }



    private GameObject Create_Strategie_Panel(bool is_tabOn, GameObject panel)
    {
        GameObject go_Panel = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel.GetComponent<Toggle>().isOn = is_tabOn;
        go_Panel.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        //Le premier contenu
        GameObject go_contenu = Instantiate(panel, Contenu_Panel.transform) as GameObject;
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

    private GameObject Create_PID_Panel(bool is_tabOn, GameObject panel)
    {
        GameObject go_Panel = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel.GetComponent<Toggle>().isOn = is_tabOn;
        go_Panel.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        GameObject go_contenu = Instantiate(panel, Contenu_Panel.transform) as GameObject;
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

    private GameObject Create_Deplacement_Panel(bool is_tabOn, GameObject panel)
    {
        GameObject go_Panel = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel.GetComponent<Toggle>().isOn = is_tabOn;
        go_Panel.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        GameObject go_contenu = Instantiate(panel, Contenu_Panel.transform) as GameObject;
        list_Panels.Add(go_contenu);

        //Associe le panel à cet onglet
        go_Panel.GetComponent<Toggle>().onValueChanged.AddListener(go_contenu.SetActive);

        //Set le nom de l'onglet
        go_Panel.name = $"Onglet Deplacements";
        //Set onglet button name
        go_Panel.GetComponentInChildren<TextMeshProUGUI>().text = "Deplacements";
        //Set le nom du panel
        go_contenu.name = $"Tab Deplacements";

        return go_contenu;
    }

    private GameObject Create_Servos_Panel(bool is_tabOn, GameObject panel)
    {
        GameObject go_Panel = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel.GetComponent<Toggle>().isOn = is_tabOn;
        go_Panel.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        GameObject go_contenu = Instantiate(panel, Contenu_Panel.transform) as GameObject;
        list_Panels.Add(go_contenu);

        //Associe le panel à cet onglet
        go_Panel.GetComponent<Toggle>().onValueChanged.AddListener(go_contenu.SetActive);

        //Set le nom de l'onglet
        go_Panel.name = $"Onglet Actionneurs";
        //Set onglet button name
        go_Panel.GetComponentInChildren<TextMeshProUGUI>().text = "Actionneurs";
        //Set le nom du panel
        go_contenu.name = $"Tab Actionneurs";

        return go_contenu;
    }
}

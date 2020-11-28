using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Create_Strategie_Container : MonoBehaviour
{
    public GameObject Switch_Holder;
    public GameObject Contenu_Panel;

    public GameObject Onglet_Prefab;
    public GameObject Action_Liste_Panel_Prefab;
    public GameObject Actions_History_Prefab;

    private List<GameObject> list_Panels_Robots = new List<GameObject>(); 

    // Start is called before the first frame update
    void Start()
    {        
        //Premier Panel qui est actif
        GameObject go_Panel_Action_Liste = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel_Action_Liste.GetComponent<Toggle>().isOn = true;
        go_Panel_Action_Liste.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        //Le premier contenu
        GameObject go_Strategie_contenu = Instantiate(Action_Liste_Panel_Prefab, Contenu_Panel.transform) as GameObject;
        list_Panels_Robots.Add(go_Strategie_contenu);
        go_Strategie_contenu.SetActive(true);

        //Associe le panel à cet onglet
        go_Panel_Action_Liste.GetComponent<Toggle>().onValueChanged.AddListener(go_Strategie_contenu.SetActive);

        //Set le nom de l'onglet
        go_Panel_Action_Liste.name = $"Liste Actions";
        //Set onglet button name
        go_Panel_Action_Liste.GetComponentInChildren<TextMeshProUGUI>().text = "Actions";
        //Set le nom du panel
        go_Strategie_contenu.name = $"Tab Action_Liste";



        //Second Panel qui est non-actif
        GameObject go_Panel_Action_History = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_Panel_Action_History.GetComponent<Toggle>().isOn = false;
        go_Panel_Action_History.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        //Le premier contenu
        GameObject go_Strategie_History = Instantiate(Actions_History_Prefab, Contenu_Panel.transform) as GameObject;
        list_Panels_Robots.Add(go_Strategie_History);

        //Initialement Active ce panel afin de le trouver dans l'interpreter de message qui ensuite le desactivera dans son Start
        go_Strategie_History.SetActive(true);

        //Associe le panel à cet onglet
        go_Panel_Action_History.GetComponent<Toggle>().onValueChanged.AddListener(go_Strategie_History.SetActive);

        //Set le nom de l'onglet
        go_Panel_Action_History.name = $"Actions Historique";
        //Set onglet button name
        go_Panel_Action_History.GetComponentInChildren<TextMeshProUGUI>().text = "Historique";
        //Set le nom du panel
        go_Strategie_History.name = $"Tab Action Historique";
    }
}

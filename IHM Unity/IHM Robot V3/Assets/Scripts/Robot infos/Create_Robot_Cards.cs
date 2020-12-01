using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Create_Robot_Cards : MonoBehaviour
{
    public GameObject Switch_Holder;
    public GameObject Contenu_Panel;

    public GameObject Onglet_Prefab;
    public GameObject Carte_MultiFct_Prefab;
    public GameObject Carte_IA_Prefab;

    private List<GameObject> list_Robot_Cards= new List<GameObject>();


    void Start()
    {
        //Creation des cartes multiFct
        Create_Cartes_MultiFct();
    }


    private void Create_Cartes_MultiFct()
    {
        if (Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot == 0)
            return;

        if (this.GetComponent<Multi_Carte_maj>().Carte_multiFct.Length != Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot)
        {
            this.GetComponent<Multi_Carte_maj>().Carte_multiFct = new GameObject[Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot + 1]; //+1 pour la carte IA
        }


        //Création de la carte de type IA
        {
            //La première carte est active
            GameObject go_carte_onglet = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
            go_carte_onglet.GetComponent<Toggle>().isOn = true;
            go_carte_onglet.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

            //Le premier contenu
            GameObject go_carte_contenu = Instantiate(Carte_IA_Prefab, Contenu_Panel.transform) as GameObject;
            list_Robot_Cards.Add(go_carte_contenu);
            go_carte_contenu.SetActive(true);

            //Associe le panel à cet onglet
            go_carte_onglet.GetComponent<Toggle>().onValueChanged.AddListener(go_carte_contenu.SetActive);

            //Set le nom de l'onglet
            go_carte_onglet.name = $"Onglet IA";
            //Set onglet button name
            go_carte_onglet.GetComponentInChildren<TextMeshProUGUI>().text = "Carte IA";
            //Set le nom du panel
            go_carte_contenu.name = $"Tab IA";

            //Associe la carte à cet object pour la mise à jour
            this.GetComponent<Multi_Carte_maj>().Carte_IA = go_carte_contenu;
        }

        //Creation des tab des cartes multi_fonctions qui sont desactives
        for (int robot_count = 0; robot_count < Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot; robot_count++)
        {
            //L'onglet
            GameObject go_carte_onglet = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;

            //Desactive le toogle des autres onglets
            go_carte_onglet.GetComponent<Toggle>().isOn = false;
            go_carte_onglet.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

            //Le contenu de l'onglet
            GameObject go_carte_contenu = Instantiate(Carte_MultiFct_Prefab, Contenu_Panel.transform) as GameObject;
            list_Robot_Cards.Add(go_carte_contenu);

            //desactive les cartes
            //go_carte_contenu.SetActive(false);

            //Associe le panel à cet onglet
            go_carte_onglet.GetComponent<Toggle>().onValueChanged.AddListener(go_carte_contenu.SetActive);

            //Set le nom de l'onglet
            go_carte_onglet.name = $"Onglet {Common_settings.static_Cartes_Names[robot_count]}";
            //Set le nom du panel
            go_carte_contenu.name = $"Tab {Common_settings.static_Cartes_Names[robot_count]}";
            //Set onglet button name
            go_carte_onglet.GetComponentInChildren<TextMeshProUGUI>().text = Common_settings.static_Cartes_Names[robot_count];

            //Associe la carte à cet object pour la mise à jour
            this.GetComponent<Multi_Carte_maj>().Carte_multiFct[robot_count] = go_carte_contenu;
        }

        for (int robot_count = 1; robot_count < Common_settings.static_Nombre_Cartes_MultiFct_Par_Robot + 1; robot_count++)
        {
            list_Robot_Cards[robot_count].SetActive(false);
        }
    }
}

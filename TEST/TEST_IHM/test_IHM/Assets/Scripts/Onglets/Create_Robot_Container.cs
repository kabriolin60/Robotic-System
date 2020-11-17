using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Create_Robot_Container : MonoBehaviour
{
    public GameObject Switch_Holder;
    public GameObject Contenu_Panel;

    public GameObject Onglet_Prefab;
    public GameObject Robot_Prefab;

    // Start is called before the first frame update
    void Start()
    {
        //Premier Robot qui est actif
        GameObject go_robot = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;
        go_robot.GetComponent<Toggle>().isOn = true;
        go_robot.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

        //Le premier contenu
        GameObject go_robot_contenu = Instantiate(Robot_Prefab, Contenu_Panel.transform) as GameObject;
        go_robot_contenu.SetActive(true);

        //Associe le panel à cet onglet
        go_robot.GetComponent<Toggle>().onValueChanged.AddListener(go_robot_contenu.SetActive);

        //Set le nom de l'onglet
        go_robot.name = $"Onglet {Common_settings.static_Robots_Names[0]}";
        //Set onglet button name
        go_robot.GetComponentInChildren<TextMeshProUGUI>().text = Common_settings.static_Robots_Names[0];
        //Set le nom du panel
        go_robot_contenu.name = $"Tab {Common_settings.static_Robots_Names[0]}";

        //Set le Robot ID
        go_robot_contenu.GetComponent<Multi_Carte_maj>().Robot_id = (int)Common_settings.static_Id_Robot[0];


        //Creation des tab des autres Robots qui sont desactives
        for (int robot_count = 1; robot_count < Common_settings.static_Nombre_Robots; robot_count++)
        {
            //L'onglet
            go_robot = Instantiate(Onglet_Prefab, Switch_Holder.transform) as GameObject;

            //Desactive le toogle des autres onglets
            go_robot.GetComponent<Toggle>().isOn = false;
            go_robot.GetComponent<Toggle>().group = Switch_Holder.GetComponent<ToggleGroup>();

            //Le contenu de l'onglet
            go_robot_contenu = Instantiate(Robot_Prefab, Contenu_Panel.transform) as GameObject;

            //desactive les cartes
            go_robot_contenu.SetActive(false);

            //Associe le panel à cet onglet
            go_robot.GetComponent<Toggle>().onValueChanged.AddListener(go_robot_contenu.SetActive);

            //Set le nom de l'onglet
            go_robot.name = $"Onglet {Common_settings.static_Robots_Names[robot_count]}";
            //Set le nom du panel
            go_robot_contenu.name = $"Tab {Common_settings.static_Robots_Names[robot_count]}";
            //Set onglet button name
            go_robot.GetComponentInChildren<TextMeshProUGUI>().text = Common_settings.static_Robots_Names[robot_count];

            //Set le Robot ID
            go_robot_contenu.GetComponent<Multi_Carte_maj>().Robot_id = (int)Common_settings.static_Id_Robot[robot_count];
        }
    }
}

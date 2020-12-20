using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;

public class Action_Line : MonoBehaviour
{
    public GameObject Text_Action_ID;
    public GameObject Text_Action_Name;
    public GameObject Text_Action_State;
    public GameObject Text_Action_Step;
    public GameObject Text_Action_Priority;
    public GameObject Text_Action_Points;
    public GameObject Text_Action_Qui_Fait;
    public GameObject Text_Action_Qui_Peut;
    public GameObject Text_Action_Temps_mini;
    public GameObject Text_Action_Temps_maxi;

    private Action_Strategie Action_Data;

    // Start is called before the first frame update
    void Awake()
    {
        Action_Data = new Action_Strategie();
        Action_Data.Datas = new Action_Strategie.Action_Datas();
    }


    public void Update_Action(Action_Strategie.Action_Datas data)
    {
        if (Action_Data == null)
        {
            Action_Data = new Action_Strategie();
            Action_Data.Datas = new Action_Strategie.Action_Datas();
        }

        Action_Data.Datas = data;

        Text_Action_ID.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.ID.ToString();

        if (Action_Data.Name == null)
        {
            //N'affiche le nom que pour la création
            string text = new string(data.Name, 0, data.Name_Length);
            Action_Data.Name = text;

            Text_Action_Name.GetComponent<TextMeshProUGUI>().text = Action_Data.Name;
        }

        Action_Line previous_values = Action_Liste.Get_Existing_Line(data.ID);

        if (previous_values != null && data.State != previous_values.Action_Data.Datas.State)
        {
            Text_Action_State.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_State.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_State.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.State.ToString();


        if (previous_values != null && data.Step != previous_values.Action_Data.Datas.Step)
        {
            Text_Action_Step.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_Step.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_Step.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Step.ToString();


        if (previous_values != null && data.Priority != previous_values.Action_Data.Datas.Priority)
        {
            Text_Action_Priority.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_Priority.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_Priority.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Priority.ToString();


        if (previous_values != null && data.Points != previous_values.Action_Data.Datas.Points)
        {
            Text_Action_Points.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_Points.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_Points.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Points.ToString();


        if (previous_values != null && data.Qui_Fait != previous_values.Action_Data.Datas.Qui_Fait)
        {
            Text_Action_Qui_Fait.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_Qui_Fait.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_Qui_Fait.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Qui_Fait.ToString();


        if (previous_values != null && data.Qui_Peut != previous_values.Action_Data.Datas.Qui_Peut)
        {
            Text_Action_Qui_Peut.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_Qui_Peut.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_Qui_Peut.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Qui_Peut.ToString();


        if (previous_values != null && data.Temps_mini != previous_values.Action_Data.Datas.Temps_mini)
        {
            Text_Action_Temps_mini.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_Temps_mini.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_Temps_mini.GetComponent<TextMeshProUGUI>().text = ((int)(Action_Data.Datas.Temps_mini) * 10).ToString();


        if (previous_values != null && data.Temps_maxi != previous_values.Action_Data.Datas.Temps_maxi)
        {
            Text_Action_Temps_maxi.GetComponent<TextMeshProUGUI>().fontStyle = FontStyles.Bold;
            Text_Action_Temps_maxi.GetComponent<TextMeshProUGUI>().color = Color.red;
        }
        Text_Action_Temps_maxi.GetComponent<TextMeshProUGUI>().text = ((int)(Action_Data.Datas.Temps_maxi) * 10).ToString();
    }
}


public class Action_Strategie
{
    public enum Action_State: byte
    {
        En_Attente,
        En_cours,
        Terminee,
        Annulee,
        Impossible
    }

    public enum Qui: byte
    {
        Personne,
        Gros_Robot,
        Petit_Robot,
        Deux_Robots
    }

    [StructLayout(LayoutKind.Sequential)]
    public class Action_Datas
    {
        [MarshalAs(UnmanagedType.U1)]
        public byte ID;

        [MarshalAs(UnmanagedType.U1)]
        public Infos_Carte.Com_Position_Robot_Identification Robot_ID;

        [MarshalAs(UnmanagedType.U1)]
        public byte Name_Length;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
        public char[] Name = new char[40];

        [MarshalAs(UnmanagedType.U1)]
        public Action_State State;

        [MarshalAs(UnmanagedType.U1)]
        public byte Step;

        [MarshalAs(UnmanagedType.U1)]
        public byte Priority;

        [MarshalAs(UnmanagedType.U1)]
        public byte Points;

        [MarshalAs(UnmanagedType.U1)]
        public Qui Qui_Fait;

        [MarshalAs(UnmanagedType.U1)]
        public Qui Qui_Peut;

        [MarshalAs(UnmanagedType.U2)]
        public ushort Temps_mini;

        [MarshalAs(UnmanagedType.U2)]
        public ushort Temps_maxi;
    }

    public Action_Datas Datas;

    public string Name;

    public Action_Datas Trame_To_Data(Communication.Communication_Trame input_trame)
    {
        Action_Datas output = new Action_Datas();

        output = (Action_Datas)Communication.GetClassFromArray<Action_Datas>(input_trame.Data);
        return output;
    }
}

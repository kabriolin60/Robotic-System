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

    private Action Action_Data;

    // Start is called before the first frame update
    void Awake()
    {
        Action_Data = new Action();
        Action_Data.Datas = new Action.Action_Datas();
    }


    public void Update_Action(Action.Action_Datas data)
    {
        if(Action_Data == null)
        {
            Action_Data = new Action();
            Action_Data.Datas = new Action.Action_Datas();
        }


        Action_Data.Datas = data;

        Text_Action_ID.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.ID.ToString();

        if (Action_Data.Name == null)
        {
            string text = new string(data.Name, 0, data.Name_Length);
            Action_Data.Name = text;
        }

        Text_Action_Name.GetComponent<TextMeshProUGUI>().text = Action_Data.Name;

        Text_Action_State.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.State.ToString();

        Text_Action_Step.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Step.ToString();

        Text_Action_Priority.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Priority.ToString();

        Text_Action_Points.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Points.ToString();

        Text_Action_Qui_Fait.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Qui_Fait.ToString();

        Text_Action_Qui_Peut.GetComponent<TextMeshProUGUI>().text = Action_Data.Datas.Qui_Peut.ToString();

        Text_Action_Temps_mini.GetComponent<TextMeshProUGUI>().text = ((int)(Action_Data.Datas.Temps_mini) * 10).ToString();

        Text_Action_Temps_maxi.GetComponent<TextMeshProUGUI>().text = ((int)(Action_Data.Datas.Temps_maxi) * 10).ToString();
    }
}


public class Action
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

        output = (Action_Datas)Communication.GetStructFromArray<Action_Datas>(input_trame.Data);
        return output;
    }
}

using SimpleGraph;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;
using UnityEngine.UI;
using static Communication;
using static Structures_Echange.Com_Position_Robot;
using static Structures_Echange.Logger_Debug;

public class Interpretation_Rx_Com : MonoBehaviour
{
    public long Nb_Messages_recus = 0;

    private Communication_TX_RX_Trames Lecteurs_Messages;

    public GameObject Fenetre_Logger;
    public GameObject Log_Message_Prefab;

    [SerializeField]
    private List<Log_Message> LogMessages = new List<Log_Message>();


    //public bool test_check;

    //Objets 3D des Robots
    public GameObject Gros_Robot;
    public GameObject Petit_Robot;

    //Conteneur des panneaux des cartes multi FCT
    public GameObject Carte_MultiFct_Tabs_Conteneur_PR;
    public GameObject Carte_MultiFct_Tabs_Conteneur_GR;


    //Graphique
    public Grapher Graph1;



    // Start is called before the first frame update
    void Start()
    {
        Lecteurs_Messages = GetComponent<Communication_TX_RX_Trames>();

        /*if(test_check)
            test();*/
    }


    void test()
    {
        List<Communication> list_com = new List<Communication>();

        Communication_Trame test_trame = new Communication_Trame();

        Structures_Echange.Logger_Debug Logger = new Structures_Echange.Logger_Debug();

        for (int i = 0; i < 30; i++)
        {
            list_com = Logger.Send_Logger("test " + i.ToString(), Logger_Debug_Color.Color_Black);

            Lecteurs_Messages.Received_Messages.AddRange(list_com);
        }

        for (int i = 0; i < 30; i++)
        {
            list_com = Logger.Send_Logger("test " + i.ToString(), Logger_Debug_Color.Color_Blue);

            Lecteurs_Messages.Received_Messages.AddRange(list_com);
        }

        for (int i = 0; i < 30; i++)
        {
            list_com = Logger.Send_Logger("test " + i.ToString(), Logger_Debug_Color.Color_Red);

            Lecteurs_Messages.Received_Messages.AddRange(list_com);
        }
    }


    private void Interprete_Trame(Communication_Trame input_trame)
    {
        switch (input_trame.Instruction)
        {
            case Com_Instruction.REPONSE_ROBOT_POSITION:
                Decodage_Position_Robot(input_trame);
                break;

            case Com_Instruction.REPONSE_INFO:
                Decodage_Infos_Cartes(input_trame);
                break;

            case Com_Instruction.LOGGER_DEBUG:
                Decodage_Logger(input_trame);
                break;

            case Com_Instruction.GRAPHIQUES_ADD_DATA:
                Decodage_Graphique(input_trame);
                break;

            case Com_Instruction.GRAPHIQUES_CLEAR:
                Graph1.Clear();
                break;

            default:
                break;
        }
    }    

    // Update is called once per frame
    void Update()
    {
        while(Lecteurs_Messages.Received_Messages.Count >0)
        {
            //S'il y a des messages reçus à interpreter
            Communication new_Message = Lecteurs_Messages.Received_Messages[0]; Lecteurs_Messages.Received_Messages.RemoveAt(0);

            Interprete_Trame(new_Message.Trame_Data);
            Nb_Messages_recus++;
        }
    }

    


    #region Position du Robot
    private void Decodage_Position_Robot(Communication_Trame input_trame)
    {
        Structures_Echange.Com_Position_Robot position = new Structures_Echange.Com_Position_Robot();
        //Transforme la trame en Message utile
        Com_Position_Robot_Data received_message = position.Trame_To_Data(input_trame);

        switch(received_message.Numero_Robot)
        {
            case Com_Position_Robot_Identification.Gros_Robot:
                if (Gros_Robot != null)
                {
                    Gros_Robot.GetComponent<Robot_Position>().Real_Position.x = (float)(received_message.Position_X) / 10 / 1000;
                    Gros_Robot.GetComponent<Robot_Position>().Real_Position.y = (float)(received_message.Position_Y) / 10 / 1000;
                    Gros_Robot.GetComponent<Robot_Position>().Orientation = (float)(received_message.Angle) / 100;
                }
                break;

            case Com_Position_Robot_Identification.Petit_Robot:
                if (Petit_Robot != null)
                {
                    Petit_Robot.GetComponent<Robot_Position>().Real_Position.x = (float)(received_message.Position_X) / 10 / 1000;
                    Petit_Robot.GetComponent<Robot_Position>().Real_Position.y = (float)(received_message.Position_Y) / 10 / 1000;
                    Petit_Robot.GetComponent<Robot_Position>().Orientation = (float)(received_message.Angle) / 100;
                }
                break;

            default:
                break;
        }
    }
    #endregion

    #region Infos Cartes
    private void Decodage_Infos_Cartes(Communication_Trame input_trame)
    {
        Structures_Echange.Infos_Carte info_carte = new Structures_Echange.Infos_Carte();
        //Transforme la trame en Message utile
        Structures_Echange.Infos_Carte.Com_Reponse_Info received_message = info_carte.Trame_To_Data(input_trame);

        //La mise à jour de la position du Robot, est faite, uniquement par la carte numéro 1
        if (received_message.Numero_Carte == 1)
        {
            //Mise à jour de la position du Robot
            switch (received_message.PositionRobot.Numero_Robot)
            {
                case Com_Position_Robot_Identification.Gros_Robot:
                    if (Gros_Robot != null)
                    {
                        Gros_Robot.GetComponent<Robot_Position>().Real_Position.x = (float)(received_message.PositionRobot.Position_X) / 10 / 1000;
                        Gros_Robot.GetComponent<Robot_Position>().Real_Position.y = (float)(received_message.PositionRobot.Position_Y) / 10 / 1000;
                        Gros_Robot.GetComponent<Robot_Position>().Orientation = (float)(received_message.PositionRobot.Angle) / 100;
                    }
                    //Mise à jour des infos de la carte
                    Carte_MultiFct_Tabs_Conteneur_GR.GetComponent<Cartes_MultiFct_Panel>().Update_Infos(received_message);
                    break;

                case Com_Position_Robot_Identification.Petit_Robot:
                    if (Petit_Robot != null)
                    {
                        Petit_Robot.GetComponent<Robot_Position>().Real_Position.x = (float)(received_message.PositionRobot.Position_X) / 10 / 1000;
                        Petit_Robot.GetComponent<Robot_Position>().Real_Position.y = (float)(received_message.PositionRobot.Position_Y) / 10 / 1000;
                        Petit_Robot.GetComponent<Robot_Position>().Orientation = (float)(received_message.PositionRobot.Angle) / 100;
                    }
                    //Mise à jour des infos de la carte
                    Carte_MultiFct_Tabs_Conteneur_PR.GetComponent<Cartes_MultiFct_Panel>().Update_Infos(received_message);
                    break;

                default:
                    break;
            }
        }        
    }
    #endregion

    #region Decodage Logger

    public const int Max_Log_Message_Number = 300;


    [System.Serializable]
    public class Log_Message
    {
        public String text;
        public Text textobject;
        public Logger_Debug_Color messageType;
    }


    private void Decodage_Logger(Communication_Trame input_trame)
    {
        if(LogMessages.Count >= Max_Log_Message_Number)
        {
            Destroy(LogMessages[0].textobject.gameObject);
            LogMessages.RemoveAt(0);
        }


        Structures_Echange.Logger_Debug Logger = new Structures_Echange.Logger_Debug();
        //Transforme la trame en Message utile
        Logger_Debug_Data received_message = Logger.Trame_To_Data(input_trame);

        string recieved_log = "["+DateTime.Now.ToLongTimeString() + ":" + DateTime.Now.Millisecond.ToString() + "] ";


        recieved_log += new string(received_message.Text, 0, received_message.Nombre_Carateres);
        Logger_Debug_Color color = received_message.Color;

        //Crée un nouveau message de Log
        Log_Message newLogMessage = new Log_Message();
        newLogMessage.text = recieved_log;
        newLogMessage.messageType = received_message.Color;

        //Crée le gameObject associé
        GameObject newgameobject = Instantiate(Log_Message_Prefab, Fenetre_Logger.transform);
        newLogMessage.textobject = newgameobject.GetComponent<Text>();
        newLogMessage.textobject.text = newLogMessage.text;

        switch (received_message.Color)
        {
            default:
            case Logger_Debug_Color.Color_Black:
                newLogMessage.textobject.color = Color.black;
                break;

            case Logger_Debug_Color.Color_Blue:
                newLogMessage.textobject.color = Color.blue;
                break;

            case Logger_Debug_Color.Color_Red:
                newLogMessage.textobject.color = Color.red;
                break;
        }

        //Ajoute ce log à la liste
        LogMessages.Add(newLogMessage);
    }
    #endregion

    #region Graphique
    private void Decodage_Graphique(Communication_Trame input_trame)
    {
        Graphiques Datas = new Graphiques();
        //Transforme la trame en Message utile
        Graphiques.Com_Graphique_Datas received_message = Datas.Trame_To_Data(input_trame);

        string csv_output = "";

        for (int data_in = 0; data_in < received_message.nb_datas_to_send; data_in++)
        {
            //Graph1.Add_Data(received_message.Datas[data_in].Data, received_message.Datas[data_in].Channel);

            csv_output += received_message.Datas[data_in].Data.ToString() + ",";
        }

        try
        {
            /*using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"graph_data.txt", true))
            {
                file.WriteLine(csv_output);
            }*/
        }
        catch (Exception)
        {

            throw;
        }
        
    }
    #endregion

}

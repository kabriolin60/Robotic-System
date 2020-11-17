using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TEST : MonoBehaviour
{
    Infos_Carte.Com_Reponse_Info data_test;

    public GameObject Communication_GO;

    public void Start()
    {
        data_test = new Infos_Carte.Com_Reponse_Info();

        data_test.Numero_Robot = Infos_Carte.Com_Position_Robot_Identification.Gros_Robot;
        data_test.Numero_Carte = 1;

        data_test.PositionRobot.Position_X = 2500;
        data_test.PositionRobot.Position_Y = 10000;
        data_test.PositionRobot.Angle = 12300;

        data_test.Position_Servos = new Infos_Carte.Com_Position_Servos();
        data_test.Position_Servos.Position = new ushort[Infos_Carte.NB_SERVO];
        data_test.Position_Servos.Position[0] = 500;
        data_test.Position_Servos.Position[1] = 600;
        data_test.Position_Servos.Position[2] = 300;
        data_test.Position_Servos.Position[3] = 800;
        data_test.Position_Servos.Position[4] = 100;
        data_test.Position_Servos.Position[5] = 50;

        data_test.Position_AX12 = new Infos_Carte.Com_Position_AX12();
        data_test.Position_AX12.Position = new ushort[Infos_Carte.NB_AX_12];
        data_test.Position_AX12.Position[0] = 500;
        data_test.Position_AX12.Position[1] = 250;
        data_test.Position_AX12.Position[2] = 26;
        data_test.Position_AX12.Position[3] = 128;

        data_test.Position_AX12.Torque = new short[Infos_Carte.NB_AX_12];
        data_test.Position_AX12.Torque[0] = 500;
        data_test.Position_AX12.Torque[1] = 250;
        data_test.Position_AX12.Torque[2] = 26;
        data_test.Position_AX12.Torque[3] = 128;

        data_test.Mesures_Analogiques = new Infos_Carte.Com_Mesures_Analogiques();
        data_test.Mesures_Analogiques.Mesure = new ushort[Infos_Carte.NB_MES_ANA];
        data_test.Mesures_Analogiques.Mesure[0] = 1023;
        data_test.Mesures_Analogiques.Mesure[1] = 2046;
        data_test.Mesures_Analogiques.Mesure[2] = 3069;
        data_test.Mesures_Analogiques.Mesure[3] = 4092;

        data_test.Tension_Batterie = 1524;

        data_test.Etat_Alim = 0b11111111;
        data_test.Etat_Contacteurs = 0b11111111;

        Communication com = new Communication();
        Communication.Communication_Message message = new Communication.Communication_Message();
        message.Heure = DateTime.Now;
        message.Trame = new Communication.Communication_Trame();
        message.Trame.Instruction = Communication.Com_Instruction.REPONSE_INFO;
        message.Trame.Data = com.COPYDATA(data_test);

        Communication_GO.GetComponent<Interprete_Message>().Decodage_Message(message);



        data_test.Numero_Robot = Infos_Carte.Com_Position_Robot_Identification.Petit_Robot;
        data_test.Numero_Carte = 1;

        data_test.PositionRobot.Position_X = 7555;
        data_test.PositionRobot.Position_Y = 12234;
        data_test.PositionRobot.Angle = -12300;
        message.Trame.Data = com.COPYDATA(data_test);

        Communication_GO.GetComponent<Interprete_Message>().Decodage_Message(message);

        this.StartCoroutine("Routine_Internal_Logger");

        //this.StartCoroutine("test_logger_quantity");
    }

    IEnumerator test_logger_quantity()
    {
        for (int i = 0; i < 1000; i++)
        {
            Log("Test Internal Logger", 6, Color.black);
            yield return new WaitForSeconds(0.1F);
        }

        yield return null;
    }


        #region INTERNAL_LOGGER
    public GameObject Logger_View;
    private void Log(string text, int channel, Color color, [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0, [System.Runtime.CompilerServices.CallerFilePath] string caller = null)
    {
        Debug.Log(text);

        string time = $"{System.DateTime.Now.Hour}:{System.DateTime.Now.Minute}:{System.DateTime.Now.Second}:{System.DateTime.Now.Millisecond}";

        Internal_Logger(time, channel, color, text, lineNumber, caller);
    }

    List<Logger_New_Line.Logger_Message> messages_for_internal_Logger = new List<Logger_New_Line.Logger_Message>();


    private void Internal_Logger(string time, int Channel, Color color, string text, int lineNumber, string caller)
    {
        string path_string = "File: " + System.IO.Path.GetFileName(caller) + ", " + lineNumber;

        messages_for_internal_Logger.Add(new Logger_New_Line.Logger_Message(time, Channel, color, path_string + ":: " + text));
    }

    IEnumerator Routine_Internal_Logger()
    {
        yield return new WaitForSeconds(0.5F);
        Logger_New_Line new_Liner;

        new_Liner = Logger_View.GetComponent<Logger_New_Line>();

        int message_used = 0;

        while (true)
        {
            yield return new WaitForSeconds(0.2F);

            message_used = 0;
            foreach (Logger_New_Line.Logger_Message message in messages_for_internal_Logger)
            {
                new_Liner.Add_New_Logger_Line(message);
                message_used++;
            }


            messages_for_internal_Logger.RemoveRange(0, message_used);
            yield return null;
        }
    }
    #endregion



}

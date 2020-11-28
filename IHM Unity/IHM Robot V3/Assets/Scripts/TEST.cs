using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine;

public class TEST : MonoBehaviour
{
    Infos_Carte.Com_Reponse_Info data_test;

    public GameObject Communication_GO;

    List<Task> tasks = new List<Task>();


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
        message.Trame.Instruction = Communication.Com_Instruction.REPONSE_INFO;
        message.Trame.Data = com.COPYDATA(data_test);

        //Communication_GO.GetComponent<Interprete_Message>().Decodage_and_Save_Message(message);



        data_test.Numero_Robot = Infos_Carte.Com_Position_Robot_Identification.Petit_Robot;
        data_test.Numero_Carte = 1;

        data_test.PositionRobot.Position_X = 10000;
        data_test.PositionRobot.Position_Y = 15000;
        data_test.PositionRobot.Angle = 0;
        message.Trame.Data = com.COPYDATA(data_test);

        //this.StartCoroutine("test_logger_quantity");
        //this.StartCoroutine(test_logger_saver());

        Trame_Decoder[] Decodeurs = Communication_GO.GetComponentsInChildren<Trame_Decoder>();

        tasks.Add(Task.Factory.StartNew(async () =>
        {
            await Task.Delay(5000);            

            for (int i = 0; i < 4000; i++)
            {
                data_test.PositionRobot.Position_X++;
                //data_test.PositionRobot.Position_Y--;
                data_test.PositionRobot.Angle += 3;

                message = new Communication.Communication_Message();
                message.Heure = DateTime.Now;
                message.Trame.Instruction = Communication.Com_Instruction.REPONSE_INFO;
                message.Trame.Data = com.COPYDATA(data_test);

                Decodeurs[0].Received_Messages.Enqueue(message);
                //Decodeurs[1].Received_Messages.Enqueue(message);
                Decodeurs[0].Received_Messages.Enqueue(message);
                //Decodeurs[1].Received_Messages.Enqueue(message);

                await Task.Delay(10);
            }
        }));
    }

    IEnumerator test_logger_quantity()
    {
        for (int i = 0; i < 1000; i++)
        {
            Logger_New_Line.Log("Test Internal Logger", 6, Color.black);
            yield return new WaitForSeconds(0.1F);
        }

        yield return null;
    }
}

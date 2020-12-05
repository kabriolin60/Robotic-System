﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using UnityEngine;

public class Envoi_Commandes : MonoBehaviour
{
    public void Envoi_Commande_Simulation(bool simu)
    {
        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        byte[] arr = new byte[1];

        arr[0] = Convert.ToByte(simu);

        trame.Data = arr;
        trame.Length = 1;

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.DEMANDE_SIMULATION_MOTEURS;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
    }


    public void Envoi_Commande_MotorPower(bool power)
    {
        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        byte[] arr = new byte[2];

        arr[0] = Convert.ToByte(power);
        arr[1] = Convert.ToByte(power);

        trame.Data = arr;
        trame.Length = 2;

        trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1; //uniquement la carte 1 qui gère les déplacement
        trame.Instruction = Communication.Com_Instruction.DEMANDE_MOTEURS_POWER;
        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);

        if (!power)
        {
            for (int i = 0; i < 10; i++)
            {
                GameObject.FindWithTag("Communication port").GetComponent<Message_Sender>().Send_Trame(trame);
            }
        }
    }


    public void Envoi_Deplacement(GameObject deplacement_panel)
    {
        Consignes_Deplacement deplacement = new Consignes_Deplacement();
        deplacement.Envoi_Deplacement(deplacement_panel);
    }

    public void Envoi_Reglages_Odometrie(GameObject odometrie_panel)
    {
        Reglage_Odometrie odometrie = new Reglage_Odometrie();
        odometrie.Envoi_Reglages_Odometrie(odometrie_panel);
    }
}

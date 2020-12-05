using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Message_Sender : MonoBehaviour
{
    Trame_Decoder[] Decodeurs;

    public Int32 Nb_Messages_Envoies = 0;


    // Start is called before the first frame update
    void Start()
    {
        Decodeurs = this.GetComponentsInChildren<Trame_Decoder>();
    }


    public void Send_Trame(Communication.Communication_Trame trame)
    {
        Communication.Communication_Message message = new Communication.Communication_Message();
        message.Heure = DateTime.Now;
        message.Trame = trame;
        Push_Message_Out(message);
    }

    public void Push_Message_Out(Communication.Communication_Message message)
    {
        Nb_Messages_Envoies++;

        //Log le message envoyé
        this.GetComponent<Interprete_Message>().Save_Message(message);

        //Envoi le message sur chaque port connecté
        foreach (Trame_Decoder encoder in Decodeurs)
        {
            encoder.Push_Message_Out(message);
        }
    }
}

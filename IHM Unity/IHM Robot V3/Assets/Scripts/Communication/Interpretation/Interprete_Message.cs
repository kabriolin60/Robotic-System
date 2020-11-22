﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine;

public class Interprete_Message : MonoBehaviour
{
	Trame_Decoder[] Decodeurs;

	public Int32 Nb_Messages_Interpretes = 0;

	//Class qui contiendra les toutes dernières infos reçues et mise à jour
	Last_Infos Last_Data_Received;

	File_Logger file_Logger;


	// Start is called before the first frame update
	void Start()
	{
		Decodeurs = this.GetComponentsInChildren<Trame_Decoder>();
		file_Logger = this.GetComponent<File_Logger>();

		Last_Data_Received = this.GetComponent<Last_Infos>();

		//Creation d'une tâche asynchrone chargée de lire les messages dans les décodeurs et de les interpreter

		this.StartCoroutine(Interpreteur_Message());


		/*Virtual_SerialPort[] serialports = this.GetComponentsInChildren<Virtual_SerialPort>();
		Communication.Communication_Message message = new Communication.Communication_Message();
		Task.Factory.StartNew(() =>
		{
			int nb_mess = 0;
			while (true)
			{
				Task.Factory.StartNew(() =>
				{
					nb_mess = 0;
					for (int i = 0; i < serialports.Length; i++)
					{
						while (serialports[i].Number_Byte_To_Read() > 100)
						{
							message = Reception_Data.ReadTrame(serialports[i]);
							if (message != null)
							{
								nb_mess++;
								//Pour chaque décodeur (chacun son tour)
								//Enregistre pour le log
								//compte le nombre de messages recus
								Decodage_and_Save_Message(message);
							}
						}
					}
					if (nb_mess > 0)
					{
						Debug.Log($"Interpreteur salve de {nb_mess} messages");
					}
				}).ContinueWith(t => Decodage_and_Save_Message(message), TaskScheduler.FromCurrentSynchronizationContext);
			}
		});*/
	}


	private void Update2()
	{
		int nb_mess = 0;
		Communication.Communication_Message message;

		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < Decodeurs.Length; i++)
			{
				Decodeurs[i].Pick_Message(out message);
				if (message != null)
				{
					nb_mess++;
					//Pour chaque décodeur (chacun son tour)
					//Enregistre pour le log
					//compte le nombre de messages recus
					Decodage_and_Save_Message(message);
				}
			}
		}

		Debug.Log($"nb_mess: {nb_mess}");
	}



    IEnumerator Interpreteur_Message()
	{
		int nb_mess = 0;
		Communication.Communication_Message message;
		while (true)
		{
			nb_mess = 0;
			for (int j = 0; j < 1000; j++)
			{
				for (int i = 0; i < Decodeurs.Length; i++)
				{
					Decodeurs[i].Pick_Message(out message);
					if (message != null)
					{
						nb_mess++;
						//Pour chaque décodeur (chacun son tour)
						//Enregistre pour le log
						//compte le nombre de messages recus
						Decodage_and_Save_Message(message);
					}
				}
			}
			/*if (nb_mess > 0)
			{
				Debug.Log($"Interpreteur salve de {nb_mess} messages");
			}*/
			yield return null;// new WaitForSeconds(0.005F);
		}
	}



	public void Decodage_and_Save_Message(Communication.Communication_Message message)
	{
		if (message == null)
			return;
		//Pour chaque décodeur (chacun son tour)
		Decodage_Message(message);

		//Enregistre pour le log
		Save_Message(message);
	}



	public void Decodage_Message(Communication.Communication_Message message)
	{
		if (message == null)
			return;
		switch (message.Trame.Instruction)
		{
			default:
				break;

			case Communication.Com_Instruction.LOGGER_DEBUG:
				Decode_Logger_Debug(message);
				break;

			case Communication.Com_Instruction.REPONSE_INFO:
				Decode_Reponse_Info(message);
				break;
		}

		//compte le nombre de messages recus
		Nb_Messages_Interpretes++;
	}



	private void Save_Message(Communication.Communication_Message message)
    {
		file_Logger.Write_Logging_Data(message);
	}






	private void Decode_Logger_Debug(Communication.Communication_Message message)
    {
		Logger_Debug debugger = new Logger_Debug();
		//décompose les datas du message recu
		Logger_Debug.Logger_Debug_Data data = debugger.Trame_To_Data(message.Trame);
		//tranforme-les en un Log
		string time = $"{message.Heure.Hour}:{message.Heure.Minute}:{message.Heure.Second}:{message.Heure.Millisecond}";

		Color color;
		switch (data.Color)
		{
			default:
			case Logger_Debug.Logger_Debug_Color.Color_Black:
				color = Color.black;
				break;

			case Logger_Debug.Logger_Debug_Color.Color_Blue:
				color = Color.blue;
				break;

			case Logger_Debug.Logger_Debug_Color.Color_Red:
				color = Color.red;
				break;
		}

		string text = new string(data.Text, 0, data.Nombre_Carateres);
		Logger_New_Line.Logger_Message log = new Logger_New_Line.Logger_Message(time, data.Channel, color, text);

		Logger_New_Line.Logger_Viewer.Add_New_Logger_Line(log);
	}


	private void Decode_Reponse_Info(Communication.Communication_Message message)
    {
		Infos_Carte temp = new Infos_Carte();
		//Interprete un message de type "Infos des cartes
		//Transforme la trame en un message interpretable
		Infos_Carte.Com_Reponse_Info data = temp.Trame_To_Data(message.Trame);

		//Maj de la classe contenant les dernieres infos pour toutes les cartes
		Last_Data_Received.Update_Last_Data_Received(data);
	}
}
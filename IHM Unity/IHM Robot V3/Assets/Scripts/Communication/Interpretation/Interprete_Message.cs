using System;
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

	private GameObject Action_History_go;


	// Start is called before the first frame update
	void Start()
	{
		Decodeurs = this.GetComponentsInChildren<Trame_Decoder>();
		file_Logger = this.GetComponent<File_Logger>();

		Last_Data_Received = this.GetComponent<Last_Infos>();

		

		//Creation d'une tâche asynchrone chargée de lire les messages dans les décodeurs et de les interpreter

		this.StartCoroutine(Interpreteur_Message());
	}


	public void Add_Message_To_Queue(Communication.Communication_Message message)
    {
		if(Decodeurs[0] != null)
        {
			Decodeurs[0].Pop_Message(message);
		}
    }

	

	IEnumerator Interpreteur_Message()
	{
		yield return new WaitForSeconds(.01f);

		//Identifie l'affichage de l'historique des actions, puis masque-le
		Action_History_go = GameObject.FindWithTag("Action_History");
		Action_History_go.SetActive(false);

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
			yield return null;
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

			case Communication.Com_Instruction.ASTAR_CONTENU:
				Decode_ASTAR_Contenu(message);
				break;

			case Communication.Com_Instruction.ASTAR_VECTEURS:
				Decode_ASTAR_Vector(message);
				break;

			case Communication.Com_Instruction.STRATEGIE_CHANGEMENT_ETAT:
				Decode_Strategie_Changement_Etat(message);
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


	private void Decode_ASTAR_Contenu(Communication.Communication_Message message)
	{
		ASTAR.Reception_Message_Astar(message.Trame);
	}


	private void Decode_ASTAR_Vector(Communication.Communication_Message message)
	{
		ASTAR_VECT.Reception_Message_Astar_Vector(message.Trame);
	}

	private void Decode_Strategie_Changement_Etat(Communication.Communication_Message message)
	{
		Action decoder = new Action();
		Action.Action_Datas data = new Action.Action_Datas();
		//décompose les datas du message recu
		data = decoder.Trame_To_Data(message.Trame);		

		//Ajoute une ligne à l'historique des changements d'états de la Strategie
		if (Action_History_go != null)
			Action_History_go.GetComponent<Action_History>().Update_Action(data);

		//Met à jour le tableau avec l'ensemble des actions
		Action_Liste.Update_Action(data);
	}
}

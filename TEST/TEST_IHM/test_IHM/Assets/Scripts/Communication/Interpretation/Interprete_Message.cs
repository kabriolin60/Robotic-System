using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine;

public class Interprete_Message : MonoBehaviour
{
	List<Task> tasks = new List<Task>();

	Trame_Decoder[] Decodeurs;

	public Int32 Nb_Messages_Decodes = 0;

	public GameObject Tab_Multi_Carte_Petit_Robot;
	public GameObject Tab_Multi_Carte_Gros_Robot;

	//Class qui contiendra les toutes dernières infos reçues et mise à jour
	Last_Infos Last_Data_Received;


	// Start is called before the first frame update
	void Start()
	{
		Decodeurs = this.GetComponentsInChildren<Trame_Decoder>();

		Last_Data_Received = this.GetComponent<Last_Infos>();

		//Creation d'une tâche asynchrone chargée de lire les messages dans les décodeurs et de les interpreter

		this.StartCoroutine("Interpreteur_Message");
	}


	IEnumerator Interpreteur_Message()
	{
		Communication.Communication_Message message;
		while (true)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int i = 0; i < Decodeurs.Length; i++)
				{
					message = Decodeurs[i].Pick_Message();
					if (message != null)
					{
						//Pour chaque décodeur (chacun son tour)
						Decodage_Message(message);
						Nb_Messages_Decodes++;
					}
				}
			}
			yield return new WaitForSeconds(0.005F);
		}
	}


	private void Decodage_Message(Communication.Communication_Message message)
	{
		switch (message.Trame.Instruction)
		{
			default:
			case Communication.Com_Instruction.LOGGER_DEBUG:

				break;

			case Communication.Com_Instruction.REPONSE_INFO:
				Infos_Carte temp = new Infos_Carte();
				//Interprete un message de type "Infos des cartes
				//Transforme la trame en un message interpretable
				Infos_Carte.Com_Reponse_Info data = temp.Trame_To_Data(message.Trame);

				//Maj de la classe contenant les dernieres infos pour toutes les cartes
				Last_Data_Received.Update_Last_Data_Received(data);
				break;				
		}
	}
}

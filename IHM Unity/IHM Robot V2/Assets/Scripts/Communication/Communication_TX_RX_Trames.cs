using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;

public class Communication_TX_RX_Trames : MonoBehaviour
{
	UDP_Communication portserie; //Port de commuication
	List<Communication> Messages_to_send = new List<Communication>();
	public List<Communication> Received_Messages = new List<Communication>();

	public long Nb_Erreur_Recues = 0;
	public long Nb_Erreur_Data_non_Rx = 0;

	public bool Lunch_Test_Messages;

	// Start is called before the first frame update
	void Start()
	{
		/*TEST*/
		if (Lunch_Test_Messages)
			StartCoroutine("TEST");

		StartCoroutine("Coroutine_Send");
	}

	IEnumerator TEST()
	{
		yield return new WaitForSeconds(5);

		Echange_Data Demande_Posi_Robot = new Echange_Data();
		while (true)
		{
			Messages_to_send.AddRange(Demande_Posi_Robot.Request_Robot_Position());
			yield return new WaitForSeconds(0.05F);
		}


		string test_string_Debug_noir = "Toto est ici en noir change de ligne\r\n";
		string test_string_Debug_bleu = "Toto est ici en bleu change de ligne\r\n";
		string test_string_Debug_rouge = "Toto est ici en rouge change de ligne\r\n";

		Echange_Data Log = new Echange_Data();

		yield return new WaitForSeconds(5);

		for (int i = 0; i < 4500*3; i++)
		{
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));

			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));

			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));

			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));
			Messages_to_send.AddRange(Log.Send_Logger(test_string_Debug_noir, Structures_Echange.Logger_Debug.Logger_Debug_Color.Color_Black));

			yield return new WaitForSeconds(0.005F);
		}

		yield return null;
	}

	IEnumerator Coroutine_Send()
	{
		while (true)
		{
			if (Messages_to_send.Count > 0)
			{
				Send_Trame(Messages_to_send[0]);
				Messages_to_send.RemoveAt(0);			
			}
			else
			{
				yield return new WaitForSeconds(0.01F);
			}
		}
	}


	public void Send_trame(Communication.Communication_Trame trame)
	{
		Communication comm = new Communication();
		comm.Trame_Data = trame;
		Messages_to_send.Add(comm);
	}

	public void FixedUpdate()
	{
		if (portserie != null)
		{
			while (portserie.InputBuffer.Count > 0)
			{
				ReadTrame();
			}
		}
		else
		{
			portserie = GetComponent<UDP_Communication>();
		}
	}



	/* Prend un message, et transforme-le en trame pret à partir, avec en-tête X-bee et CRC */
	private void Send_Trame(Communication Message_to_send)
	{
		Message_to_send.Trame_Data.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE; //Envoie du PC vers tous les XBEE par defaut

		byte[] data = new byte[Message_to_send.Trame_Data.Length + 11];
		data[0] = 0x7E;                         //Xbee API start byte

		int length = Message_to_send.Trame_Data.Length + 7;

		data[1] = (byte)(length >> 8);                  //length high
		data[2] = (byte)(length & 0xFF);                //length low

		data[3] = 0x01;                         //Frame type: Tx, 16 bits addr
		data[4] = 0x01;                         //Frame ID

		data[5] = (byte)((int)Message_to_send.Trame_Data.XBEE_DEST_ADDR >> 8);        //add high
		data[6] = (byte)((int)Message_to_send.Trame_Data.XBEE_DEST_ADDR & 0xFF);      //add low

		data[7] = 0x00;                         //Option

		byte index = 0;
		//Datas
		{
			data[8] = (byte)(Message_to_send.Trame_Data.Instruction);

			data[9] = (byte)(Message_to_send.Trame_Data.Slave_Adresse);

			for (index = 0; index < Message_to_send.Trame_Data.Length; index++)
			{
				data[10 + index] = Message_to_send.Trame_Data.Data[index];
			}
		}

		short API_CRC = 0;
		for (index = 3; index < 10 + Message_to_send.Trame_Data.Length; index++)
		{
			API_CRC += data[index];
		}

		API_CRC &= 0xFF;
		API_CRC = (byte)(0xFF - API_CRC);

		data[index] = (byte)(API_CRC);

		//Envoi de la trame
		portserie.Add_Data_To_Send(data);
	}

	/* Reception des octets reçus et transformation en une trame brute */
	private void ReadTrame()
	{
		Communication received_trame = new Communication();

		byte API_start = 0, API_LENGTH_HIGH = 0, API_LENGTH_LOW = 0, API_DUMMY, API_FRAME_TYPE = 0;
		short API_LENGTH = 0;
		short crc = 0;
		byte index = 0, rx_crc = 0;

		//reception de l'heure de la trame
		//received_trame.Heure = DateTime.Now;

		//Thread.Sleep(5);

		//Reception En-tête API
		byte boucle = 0;
		while (API_start != 0x7E)
		{
			API_start = (byte)portserie.ReadRemoveInputByte();
			boucle++;
			if (boucle > 5)
				return;
		}
		boucle = 0;

		//Le start byte est recu, on demarre la lecture de la trame
		API_LENGTH_HIGH = (byte)portserie.ReadRemoveInputByte();
		API_LENGTH_LOW = (byte)portserie.ReadRemoveInputByte();

		API_LENGTH = API_LENGTH_HIGH;
		API_LENGTH <<= 8;
		API_LENGTH += API_LENGTH_LOW;

		if (API_LENGTH > Communication.COMMUNICATION_TRAME_MAX_DATA + 11)
		{
			Nb_Erreur_Recues++;
			return;
		}

		//Packet Type
		API_FRAME_TYPE = (byte)portserie.ReadRemoveInputByte();
		crc += API_FRAME_TYPE;

		if (API_FRAME_TYPE == 0x89)
		{
			//Tx status
			API_DUMMY = portserie.ReadRemoveInputByte();
			crc += API_DUMMY;

			API_DUMMY = portserie.ReadRemoveInputByte();
			crc += API_DUMMY;

			if (API_DUMMY == 00)
			{
				//Mess tx succes
				//RxXbeeAck = OK;
			}

			//API CRC
			rx_crc = portserie.ReadRemoveInputByte();
			return;
		}

		boucle = 0;
		while (portserie.Wainting_Data_in_ReceptionList() < API_LENGTH - 1)
		{
			boucle++;

			if (boucle > 5)
			{
				Nb_Erreur_Data_non_Rx++;
				return;
			}
			Thread.Sleep(1);
		}

		//Tx Address
		API_DUMMY = (byte)portserie.ReadRemoveInputByte();
		crc += API_DUMMY;

		API_DUMMY = (byte)portserie.ReadRemoveInputByte();
		crc += API_DUMMY;

		//RSSI
		API_DUMMY = (byte)portserie.ReadRemoveInputByte();
		crc += API_DUMMY;

		//Option
		API_DUMMY = (byte)portserie.ReadRemoveInputByte();
		crc += API_DUMMY;

		//Reception datas
		received_trame.Trame_Data.Instruction = (Communication.Com_Instruction)(byte)portserie.ReadRemoveInputByte();
		crc += (byte)received_trame.Trame_Data.Instruction;

		received_trame.Trame_Data.Slave_Adresse = (Communication.Slave_Adresses)(byte)portserie.ReadRemoveInputByte();
		crc += (byte)received_trame.Trame_Data.Slave_Adresse;

		received_trame.Trame_Data.Length = (byte)(API_LENGTH - 7);

		//Thread.Sleep(5);

		if (received_trame.Trame_Data.Length <= Communication.COMMUNICATION_TRAME_MAX_DATA)
		{
			boucle = 0;
			while (portserie.Wainting_Data_in_ReceptionList() < received_trame.Trame_Data.Length + 1)
			{
				boucle++;

				if (boucle > 5)
				{
					Nb_Erreur_Data_non_Rx++;
					return;
				}
				Thread.Sleep(1);
			}

			//Reception des data
			for (index = 0; index < received_trame.Trame_Data.Length; index++)
			{
				received_trame.Trame_Data.Data[index] = (byte)portserie.ReadRemoveInputByte();
				crc += (byte)(received_trame.Trame_Data.Data[index]);
			}

			//API CRC
			rx_crc = (byte)portserie.ReadRemoveInputByte();

			//Contrôle CRC
			crc &= 0xFF;
			crc = (byte)(0xFF - crc);

			//Vérifie le CRC
			if (crc == rx_crc)
			{
				//Debug.Log("New Message Received");
				Received_Messages.Add(received_trame);
			}
			else
			{
				Nb_Erreur_Recues++;
			}
		}
	}

}

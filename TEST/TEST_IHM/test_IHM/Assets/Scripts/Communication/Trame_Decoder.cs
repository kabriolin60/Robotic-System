using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine;

public class Trame_Decoder : MonoBehaviour
{
	public List<Communication.Communication_Message> Received_Messages = new List<Communication.Communication_Message>();
	public uint Error_Number = 0;
	public uint Messages_Number = 0;

	List<Task> tasks = new List<Task>();

	// Start is called before the first frame update
	void Start()
    {
		Virtual_SerialPort serialport = this.gameObject.GetComponent<Virtual_SerialPort>();
		if (serialport != null)
        {
			//Creation d'une tâche asynchrone chargée de lire les datas recues par le port virtuel et d'en faire des trames de communication
			tasks.Add(Task.Factory.StartNew(async () =>
			{
				await Task.Delay(50);

				while (true)
				{
					if (serialport.InputBuffer.Count > 10)
					{
						var dataReceived = await Reception_Data.ReadTrame(serialport);
						if (dataReceived != null)
						{
							//Un message a ete reçu correctement
							Received_Messages.Add(dataReceived);
							Messages_Number++;
						}
						else
						{
							//Une erreur a ete recue
							Error_Number++;
						}
                    }
                    else
                    {
						await Task.Delay(50);
					}
				}
			}));
		}
	}    
}

public class Sending_Data
{
	/* Prend un message, et transforme-le en trame pret à partir, avec en-tête X-bee et CRC */
	public byte[] Send_Trame(Communication.Communication_Message Message_to_send)
	{
		Message_to_send.Trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE; //Envoie du PC vers tous les XBEE par defaut

		byte[] data = new byte[Message_to_send.Trame.Length + 11];
		data[0] = 0x7E;                         //Xbee API start byte

		int length = Message_to_send.Trame.Length + 7;

		data[1] = (byte)(length >> 8);                  //length high
		data[2] = (byte)(length & 0xFF);                //length low

		data[3] = 0x01;                         //Frame type: Tx, 16 bits addr
		data[4] = 0x01;                         //Frame ID

		data[5] = (byte)((int)Message_to_send.Trame.XBEE_DEST_ADDR >> 8);        //add high
		data[6] = (byte)((int)Message_to_send.Trame.XBEE_DEST_ADDR & 0xFF);      //add low

		data[7] = 0x00;                         //Option

		byte index = 0;
		//Datas
		{
			data[8] = (byte)(Message_to_send.Trame.Instruction);

			data[9] = (byte)(Message_to_send.Trame.Slave_Adresse);

			for (index = 0; index < Message_to_send.Trame.Length; index++)
			{
				data[10 + index] = Message_to_send.Trame.Data[index];
			}
		}

		short API_CRC = 0;
		for (index = 3; index < 10 + Message_to_send.Trame.Length; index++)
		{
			API_CRC += data[index];
		}

		API_CRC &= 0xFF;
		API_CRC = (byte)(0xFF - API_CRC);

		data[index] = (byte)(API_CRC);

		return data;
	}

}


public class Reception_Data
{
	/* Reception des octets reçus et transformation en une trame brute */
	public static async Task<Communication.Communication_Message> ReadTrame(Virtual_SerialPort _SerialPort)
	{
		Communication.Communication_Message received_message = new Communication.Communication_Message();

		byte API_start = 0, API_LENGTH_HIGH = 0, API_LENGTH_LOW = 0, API_DUMMY, API_FRAME_TYPE = 0;
		short API_LENGTH = 0;
		short crc = 0;
		byte index = 0, rx_crc = 0;

		//reception de l'heure de la trame
		received_message.Heure = DateTime.Now;

		//Reception En-tête API
		byte boucle = 0;
		while (API_start != 0x7E)
		{
			API_start = (byte)_SerialPort.ReadRemoveInputByte();
			boucle++;
			if (boucle > 5)
				return null;
		}
		boucle = 0;

		//Le start byte est recu, on demarre la lecture de la trame
		API_LENGTH_HIGH = (byte)_SerialPort.ReadRemoveInputByte();
		API_LENGTH_LOW = (byte)_SerialPort.ReadRemoveInputByte();

		API_LENGTH = API_LENGTH_HIGH;
		API_LENGTH <<= 8;
		API_LENGTH += API_LENGTH_LOW;

		if (API_LENGTH > Communication.COMMUNICATION_TRAME_MAX_DATA + 11)
		{
			return null;
		}

		//Packet Type
		API_FRAME_TYPE = (byte)_SerialPort.ReadRemoveInputByte();
		crc += API_FRAME_TYPE;

		if (API_FRAME_TYPE == 0x89)
		{
			//Tx status
			API_DUMMY = _SerialPort.ReadRemoveInputByte();
			crc += API_DUMMY;

			API_DUMMY = _SerialPort.ReadRemoveInputByte();
			crc += API_DUMMY;

			if (API_DUMMY == 00)
			{
				//Mess tx succes
				//RxXbeeAck = OK;
			}

			//API CRC
			rx_crc = _SerialPort.ReadRemoveInputByte();
			return null;
		}

		boucle = 0;
		while (_SerialPort.InputBuffer.Count < API_LENGTH - 1)
		{
			boucle++;

			if (boucle > 5)
			{
				return null;
			}
			await Task.Delay(1);
		}

		//Tx Address
		API_DUMMY = (byte)_SerialPort.ReadRemoveInputByte();
		crc += API_DUMMY;

		API_DUMMY = (byte)_SerialPort.ReadRemoveInputByte();
		crc += API_DUMMY;

		//RSSI
		API_DUMMY = (byte)_SerialPort.ReadRemoveInputByte();
		crc += API_DUMMY;

		//Option
		API_DUMMY = (byte)_SerialPort.ReadRemoveInputByte();
		crc += API_DUMMY;

		//Reception datas
		received_message.Trame.Instruction = (Communication.Com_Instruction)(byte)_SerialPort.ReadRemoveInputByte();
		crc += (byte)received_message.Trame.Instruction;

		received_message.Trame.Slave_Adresse = (Communication.Slave_Adresses)(byte)_SerialPort.ReadRemoveInputByte();
		crc += (byte)received_message.Trame.Slave_Adresse;

		received_message.Trame.Length = (byte)(API_LENGTH - 7);

		//Thread.Sleep(5);

		if (received_message.Trame.Length <= Communication.COMMUNICATION_TRAME_MAX_DATA)
		{
			boucle = 0;
			while (_SerialPort.InputBuffer.Count < received_message.Trame.Length + 1)
			{
				boucle++;

				if (boucle > 5)
				{
					return null;
				}
				await Task.Delay(1);
			}

			//Reception des data
			for (index = 0; index < received_message.Trame.Length; index++)
			{
				received_message.Trame.Data[index] = (byte)_SerialPort.ReadRemoveInputByte();
				crc += (byte)(received_message.Trame.Data[index]);
			}

			//API CRC
			rx_crc = (byte)_SerialPort.ReadRemoveInputByte();

			//Contrôle CRC
			crc &= 0xFF;
			crc = (byte)(0xFF - crc);

			//Vérifie le CRC
			if (crc == rx_crc)
			{
				return received_message;
			}
			else
			{
				return null;
			}
		}

		return null;
	}
}


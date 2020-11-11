using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using UnityEngine;

public class UDP_Communication : MonoBehaviour
{ 

	public List<byte> InputBuffer = new List<byte>();
	public List<byte[]> OutBuffer = new List<byte[]>();
	public List<char[]> OutBuffer_char = new List<char[]>();

	UdpUser client;

	IEnumerator Ask_Question()
	{
		while (true)
		{
			client.Send("t");
			client.Send("t");
			client.Send("t");
			client.Send("t");
			client.Send("t");

			yield return new WaitForSeconds(0.005F);
		}
	}

	// Use this for initialization
	void Start()
	{		
		try
		{
			//create a new server
			var server = new UdpListener();

			//start listening for messages and copy the messages back to the client
			Task.Factory.StartNew(async () => {
				while (true)
				{
					var received = await server.Receive();
					/*server.Reply("copy " + received.Message, received.Sender);
					if (received.Message == "quit")
						break;*/
				}
			});


			//create a new client
			client = UdpUser.ConnectTo("192.168.1.35", 26000);

			//wait for reply messages from server and send them to console 
			Task.Factory.StartNew(async () => {
				while (true)
				{
					try
					{
						var received = await client.Receive();

						//Reception d'un message
						//byte[] byteBuffer = new byte[received.Message.Length];
						InputBuffer.AddRange(received.Data);

						/*Console.WriteLine(received.Message);
						if (received.Message.Contains("quit"))
							break;*/
					}
					catch (Exception ex)
					{
						Console.WriteLine(ex);
					}
				}
			});

			StartCoroutine("Ask_Question");
		}
		catch
		{

		}
	}

	public void Add_Data_To_Send(byte[] data)
	{
		OutBuffer.Add(data);
	}

	public void Add_Data_To_Send(char[] data)
	{
		OutBuffer_char.Add(data);
	}

	private void Send(byte[] data)
	{
		
	}

	private void Send(char[] data)
	{
		
	}

	public byte ReadRemoveInputByte()
	{
		if (InputBuffer.Count > 0)
		{
			byte readed = InputBuffer[0];
			InputBuffer.RemoveAt(0);
			return readed;
		}
		else
		{
			return 0;
		}
	}

	public void OnDestroy()
	{
		try
		{

		}
		catch (System.Exception e)
		{
			//Debug.LogError(e);
		}
	}

	public int Wainting_Data_in_ReceptionList()
	{
		return InputBuffer.Count;
	}
			

	/* Créeation et/ou Ouverture du port de communication */
	void OpenPort()
	{
		
	}


	public struct Received
	{
		public IPEndPoint Sender;
		public string Message;
		public byte[] Data;
	}

	abstract class UdpBase
	{
		protected UdpClient Client;

		protected UdpBase()
		{
			Client = new UdpClient();
		}

		public async Task<Received> Receive()
		{
			var result = await Client.ReceiveAsync();

			return new Received()
			{
				Message = Encoding.ASCII.GetString(result.Buffer, 0, result.Buffer.Length),
				Sender = result.RemoteEndPoint,
				Data = result.Buffer
			};
		}
	}

	//Server
	class UdpListener : UdpBase
	{
		private IPEndPoint _listenOn;

		public UdpListener() : this(new IPEndPoint(IPAddress.Any, 32123))
		{
		}

		public UdpListener(IPEndPoint endpoint)
		{
			_listenOn = endpoint;
			Client = new UdpClient(_listenOn);
		}

		public void Reply(string message, IPEndPoint endpoint)
		{
			var datagram = Encoding.ASCII.GetBytes(message);
			Client.Send(datagram, datagram.Length, endpoint);
		}

	}

	//Client
	class UdpUser : UdpBase
	{
		private UdpUser() { }

		public static UdpUser ConnectTo(string hostname, int port)
		{
			var connection = new UdpUser();
			connection.Client.Connect(hostname, port);
			return connection;
		}

		public void Send(string message)
		{
			var datagram = Encoding.ASCII.GetBytes(message);
			Client.Send(datagram, datagram.Length);
		}

		public void Send(byte[] data)
		{
			Client.Send(data, data.Length);
		}
	}

}

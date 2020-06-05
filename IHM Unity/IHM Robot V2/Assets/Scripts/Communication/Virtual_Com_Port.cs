using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using UnityEngine;

public class Virtual_Com_Port : MonoBehaviour
{ 
	public string portName;
	public int portSpeed;
	public SerialPort serial;

	public List<byte> InputBuffer = new List<byte>();
	public List<byte[]> OutBuffer = new List<byte[]>();
	public List<char[]> OutBuffer_char = new List<char[]>();

	// Use this for initialization
	void Start()
	{
		
		try
		{
			OpenPort();
		}
		catch
		{
		}


		StartCoroutine("Coroutine_Receive");
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
		if (serial == null || !serial.IsOpen)
		{
			OpenPort();
		}

		if (!serial.IsOpen)
			return;

		if (serial != null && serial.IsOpen)
		{
			if (data.Length <= 64)
			{
				try
				{
					this.serial.Write(data, 0, data.Length);
					while (this.serial.BytesToWrite > 0) ;
				}
				catch
				{

				}
			}
			else
			{
				byte offset = 0;
				int len = data.Length;
				while (len > 0)
				{
					try
					{
						this.serial.Write(data, offset, len > 64 ? 64 : len);
						while (this.serial.BytesToWrite > 0) ;
					}
					catch
					{

					}
					len -= 64;
					offset += 64;
				}
			}
		}

		while (this.serial.BytesToWrite > 0) ;
	}

	private void Send(char[] data)
	{
		if (serial == null || !serial.IsOpen)
		{
			OpenPort();
		}

		if (!serial.IsOpen)
			return;

		if (serial != null && serial.IsOpen)
		{
			if (data.Length <= 64)
			{
				try
				{
					this.serial.Write(data, 0, data.Length);
				}
				catch
				{

				}
			}
			else
			{
				byte offset = 0;
				int len = data.Length;
				while (len > 0)
				{
					try
					{
						this.serial.Write(data, offset, len > 64 ? 64 : len);
					}
					catch
					{

					}
					len -= 64;
					offset += 64;
				}
			}
		}
		while (this.serial.BytesToWrite > 0) ;
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
			serial.Close();
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



	IEnumerator Coroutine_Receive()
	{
		while (true)
		{
			if (serial != null)
			{
				if (serial.IsOpen)
				{
					//Receptions des datas	
					int intBuffer;
					intBuffer = serial.BytesToRead;
					byte[] byteBuffer = new byte[intBuffer];
					serial.Read(byteBuffer, 0, intBuffer);
					InputBuffer.AddRange(byteBuffer);
				}
			}
			yield return new WaitForSeconds(0.05F);
		}
	}



	void Update()
	{
		if (serial != null)
		{
			if (!serial.IsOpen)
			{
				// try to open
				try
				{
					serial.Open();
				}
				catch {
					
				}
			}

			if (serial.IsOpen)
			{
				if (serial.BytesToRead == -1)
				{
					// Happens when leonardo is reset, device disapears in system
					Debug.Log("is open: " + serial.IsOpen);

					try
					{
						serial.Close();
					}
					catch 
					{
						
					}
				}

				//S'il y a des infos à envoyer
				if (OutBuffer.Count > 0)
				{
					Send(OutBuffer[0]);
					OutBuffer.RemoveAt(0);
				}

				if (OutBuffer_char.Count > 0)
				{
					Send(OutBuffer_char[0]);
					OutBuffer_char.RemoveAt(0);
				}
			}
		}
	}

	/* Créeation et/ou Ouverture du port de communication */
	void OpenPort()
	{
		if (serial == null)
		{
			serial = new SerialPort();
			serial.WriteBufferSize = 30000;
			serial.ReadBufferSize = 30000;
			serial.ReadTimeout = 2000;			
		}

		if (serial.IsOpen)
		{
			serial.Close();
		}


		// Get a list of available ports
		List<string> portNames = new List<string>();
		portNames.AddRange(System.IO.Ports.SerialPort.GetPortNames());

		if (portName == "")
		{
			// try with last port of the list
			portName = portNames[portNames.Count - 1];
		}

		serial.PortName = portName;
		serial.BaudRate = portSpeed;

		try
		{
			serial.Open();
			//Enable Event Handler
		}
		catch 
		{
			
		}

		Debug.Log("Port " + portName + " is open: " + serial.IsOpen + " @" + portSpeed + "baud");
		Thread.Sleep(3);
	}
}

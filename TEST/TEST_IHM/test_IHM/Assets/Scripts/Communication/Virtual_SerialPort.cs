using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEditor;
using System.Text;
using System.Runtime.InteropServices;
using System;
using UnityEngine.EventSystems;
using SFB;
using UnityEngine.UI;
using System.Threading;
using System.Threading.Tasks;

public class Virtual_SerialPort : MonoBehaviour
{
    static System.IO.Ports.SerialPort serialPort;

    public string portName;
    public int portSpeed;

    public List<byte> InputBuffer = new List<byte>();
    public List<byte> OutputBuffer = new List<byte>();


    List<Task> tasks = new List<Task>();

    public System.IO.Ports.SerialPort getserialPort()
    {
        return serialPort;
    }

    // Start is called before the first frame update
    void Start()
    {
        //start listening for messages asynchronously
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            Debug.Log("Waiting for receiving datas!");

            //Attente des 10 secondes de lancement du bootloader
            await Task.Delay(9000);

            if (Check_Serial_Opened())
            {
                Debug.Log("Serial Port:" + serialPort.PortName + " Opened! Starting listening!");
                while (true)
                {
                    var dataReceived = await Read_Rx_Bytes();
                    if (dataReceived.Length > 0)
                    {
                        InputBuffer.AddRange(dataReceived);
                    }
                }
            }
            Debug.Log("Error while opening Serial Port: " + serialPort.PortName);
        }));


        //start asynchronous data sending
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            Debug.Log("Waiting for sending datas!");

            await Task.Delay(9100);

            if (serialPort != null && serialPort.IsOpen) 
            {
                Debug.Log("Serial Port:" + serialPort.PortName + " Opened! Ready for Sending datas!");
                while (true)
                {
                    await SendBytes(OutputBuffer);
                }
            }
        }));
    }


    private static async Task<byte[]> Read_Rx_Bytes()
    {         
        //Receptions des datas	
        int intBuffer;
        intBuffer = serialPort.BytesToRead;
        byte[] byteBuffer = new byte[intBuffer];
        serialPort.Read(byteBuffer, 0, intBuffer);        

        //await Task.Delay(1);

        return byteBuffer;
    }


    private static async Task SendBytes(List<byte> datas)
    {
        //S'il y a des infos à envoyer
        byte[] datas_to_send = datas.ToArray();
        serialPort.Write(datas_to_send, 0, datas_to_send.Length);
    }


    private bool Check_Serial_Opened()
    {
        if (serialPort == null)
        {
            serialPort = new System.IO.Ports.SerialPort();
            // 
            // serialPort1
            // 
            serialPort.BaudRate = portSpeed;
            serialPort.PortName = portName;
        }

        if (serialPort.IsOpen == false)
        {
            try
            {
                serialPort.Open();
                return true;
            }
            catch
            {
                return false;
            }
        }

        return true;
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

    public byte[] ReadRemoveInputByte(int count)
    {
        if (InputBuffer.Count < count)
        {
            count = InputBuffer.Count;
        }

        byte[] readed = new byte[count];
        for (int i = 0; i < count; i++)
        {
            readed[i] = InputBuffer[i];
            InputBuffer.RemoveRange(0, count);
        }
        return readed;
    }
}

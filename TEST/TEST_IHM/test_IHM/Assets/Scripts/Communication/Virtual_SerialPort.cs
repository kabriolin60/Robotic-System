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
using System.IO;

public class Virtual_SerialPort : MonoBehaviour
{
    System.IO.Ports.SerialPort serialPort;

    public bool port_opened = false;

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
        Log("Waiting for serialport!", 6, Color.black);

        //start listening for messages asynchronously
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            await Task.Delay(1000);
            //Attente des 10 secondes de lancement du bootloader
            Log("Serial Port:" + serialPort.PortName + " Opened! Starting listening!", 6, Color.black);
            while (true)
            {
                var dataReceived = await Read_Rx_Bytes(serialPort);
                if (dataReceived != null && dataReceived.Length > 0)
                {
                    InputBuffer.AddRange(dataReceived);
                }

                await Task.Delay(2);
            }
        }));


        //start asynchronous data sending
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            await Task.Delay(1100);
            Log("Serial Port:" + serialPort.PortName + " Opened! Starting sending!", 6, Color.black);
            while (true)
            {
                if (serialPort != null && serialPort.IsOpen)
                {
                    port_opened = true;
                    await SendBytes(serialPort, OutputBuffer);
                }
                else
                {
                    port_opened = false;
                    await Task.Delay(20);
                }
            }
        }));


        this.StartCoroutine("Routine_Internal_Logger");
    }


    public void Connect(string name)
    {
        if (serialPort == null)
        {
            serialPort = new System.IO.Ports.SerialPort();
        }

        serialPort.BaudRate = portSpeed;
        serialPort.PortName = name;
        this.portName = name;

        try
        {
            serialPort.Open();
            Log("Serial Port:" + serialPort.PortName + " Opened!", 6, Color.black);
        }
        catch
        {
            Log("Error while oppening Serial Port:" + serialPort.PortName + "!", 6, Color.red);
        }       
    }

    
    public void DisConnect()
    {
        try
        {
            serialPort.Close();
            Log("Serial Port:" + serialPort.PortName + " Closed!", 6, Color.black);
        }
        catch
        {

        }
    }


    public void OnDestroy()
    {
        try
        {
            serialPort.Close();
        }
        catch
        {

        }
    }


    private static async Task<byte[]> Read_Rx_Bytes(System.IO.Ports.SerialPort port)
    {       
        if(port == null || port.IsOpen == false)
        {
            return null;
        }

        //Receptions des datas	
        int intBuffer;
        intBuffer = port.BytesToRead;
        byte[] byteBuffer = new byte[intBuffer];
        port.Read(byteBuffer, 0, intBuffer);    

        return byteBuffer;
    }


    private static async Task SendBytes(System.IO.Ports.SerialPort port, List<byte> datas)
    {
        if (port != null && port.IsOpen == false)
        {           
            //S'il y a des infos à envoyer
            byte[] datas_to_send = datas.ToArray();
            port.Write(datas_to_send, 0, datas_to_send.Length);
        }
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



    #region INTERNAL_LOGGER

    public GameObject Logger;

    private void Log(string text, int channel, Color color)
    {
        Debug.Log(text);

        string time = $"{System.DateTime.Now.Hour}:{System.DateTime.Now.Minute}:{System.DateTime.Now.Second}:{System.DateTime.Now.Millisecond}";

        Internal_Logger(time, channel, color, text);
    }

    List<Logger_New_Line.Logger_Message> messages_for_internal_Logger = new List<Logger_New_Line.Logger_Message>();
    

    private void Internal_Logger(string time, int Channel, Color color, string text, [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0, [System.Runtime.CompilerServices.CallerFilePath] string caller = null)
    {
        string path_string = "File: " + Path.GetFileName(caller) + ", " + lineNumber;

        messages_for_internal_Logger.Add(new Logger_New_Line.Logger_Message(time, Channel, color, path_string + ":: " + text));
    }

    IEnumerator Routine_Internal_Logger()
    {
        yield return new WaitForSeconds(0.5F);
        Logger_New_Line new_Liner;

        new_Liner = Logger.GetComponent<Logger_New_Line>();

        int message_used = 0;

        while (true)
        {
            yield return new WaitForSeconds(0.2F);

            message_used = 0;
            foreach (Logger_New_Line.Logger_Message message in messages_for_internal_Logger)
            {
                new_Liner.Add_New_Logger_Line(message);
                message_used++;
            }


            messages_for_internal_Logger.RemoveRange(0, message_used);
            yield return null;
        }
    }
    #endregion
}


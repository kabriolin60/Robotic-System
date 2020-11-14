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
    public GameObject Logger;

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
        this.StartCoroutine("Routine_Internal_Logger");

        //start listening for messages asynchronously
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            Log("Waiting for receiving datas!", 6, Color.black);            

            //Attente des 10 secondes de lancement du bootloader
            await Task.Delay(9000);

            if (Check_Serial_Opened())
            {
                Log("Serial Port:" + serialPort.PortName + " Opened! Starting listening!", 6, Color.black);
                while (true)
                {
                    var dataReceived = await Read_Rx_Bytes();
                    if (dataReceived.Length > 0)
                    {
                        InputBuffer.AddRange(dataReceived);
                    }
                }
            }
            Log("Error while opening Serial Port: " + serialPort.PortName, 6, Color.red);
        }));


        //start asynchronous data sending
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            await Task.Delay(20);

            Log("Waiting for sending datas!" + serialPort.PortName, 6, Color.black);

            await Task.Delay(9080);

            if (serialPort != null && serialPort.IsOpen) 
            {
                Log("Serial Port:" + serialPort.PortName + " Opened! Ready for Sending datas!", 6, Color.black);
                while (true)
                {
                    await SendBytes(OutputBuffer);
                }
            }
        }));
    }


    public void OnDestroy()
    {
        try
        {
            serialPort.Close();
        }
        catch (System.Exception e)
        {

        }
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

    #region INTERNAL_LOGGER

    private void Log(string text, int channel, Color color)
    {
        Debug.Log(text);

        string time = $"{System.DateTime.Now.Hour}:{System.DateTime.Now.Minute}:{System.DateTime.Now.Second}:{System.DateTime.Now.Millisecond}";

        Internal_Logger(time, channel, color, text);
    }

    List<Logger_New_Line.Logger_Message> messages_for_internal_Logger = new List<Logger_New_Line.Logger_Message>();
    static Logger_New_Line new_Liner;

    private void Internal_Logger(string time, int Channel, Color color, string text, [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0, [System.Runtime.CompilerServices.CallerFilePath] string caller = null)
    {
        string path_string = "File: " + Path.GetFileName(caller) + ", " + lineNumber;

        messages_for_internal_Logger.Add(new Logger_New_Line.Logger_Message(time, Channel, color, path_string + ":: " + text));
    }

    IEnumerator Routine_Internal_Logger()
    {
        yield return new WaitForSeconds(0.5F);

        new_Liner = Logger.GetComponent<Logger_New_Line>();

        int message_used = 0;

        while (true)
        {
            yield return new WaitForSeconds(1);

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

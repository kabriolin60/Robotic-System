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
    public int data_in_port_read_buffer = 0;
    public int data_in_input_buffer = 0;

    public List<byte> InputBuffer = new List<byte>();
    public List<byte> OutputBuffer = new List<byte>();  

    private bool Comport_cancellationToken = false;

    List<Task> tasks = new List<Task>();

    public System.IO.Ports.SerialPort getserialPort()
    {
        return serialPort;
    }


    // Start is called before the first frame update
    void Start()
    {
        if (serialPort == null)
        {
            serialPort = new System.IO.Ports.SerialPort();
        }

        //start listening for messages asynchronously
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            Log("Waiting for serialport!", 6, Color.black);

            while (this.serialPort.IsOpen == false)
            {
                await Task.Delay(100);
            }

            Log("Serial Port:" + serialPort.PortName + " Opened! Starting listening!", 6, Color.black);
            while (true)
            {
                if (this.serialPort.IsOpen)
                {
                    try
                    {
                        var dataReceived = Read_Rx_Bytes(serialPort);
                        if (dataReceived != null && dataReceived.Length > 0)
                        {
                            try
                            {
                                InputBuffer.AddRange(dataReceived);
                            }
                            catch
                            {
                                Log($"Error in buffer insertion {dataReceived.Length}", 6, Color.black);
                            }
                        }

                        data_in_port_read_buffer = serialPort.BytesToRead;
                        data_in_input_buffer = InputBuffer.Count;

                        await Task.Delay(2);

                    }
                    catch (Exception _ex)
                    {
                        Log($"Receiving task Exception!: {_ex}", 6, Color.red);
                    }
                }else
                {
                    await Task.Delay(100);
                }

                if(Comport_cancellationToken)
                    throw new TaskCanceledException();
            }
        }));


        //start asynchronous data sending
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            while (this.serialPort.IsOpen == false)
            {
                await Task.Delay(100);
            }

            Log("Serial Port:" + serialPort.PortName + " Opened! Starting sending!", 6, Color.black);
            while (true)
            {
                if (serialPort != null && serialPort.IsOpen)
                {
                    port_opened = true;
                    SendBytes(serialPort, OutputBuffer);
                }
                else
                {
                    port_opened = false;
                    await Task.Delay(20);
                }

                if (Comport_cancellationToken)
                    throw new TaskCanceledException();
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

        if (name == "")
            return;

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
            Comport_cancellationToken = true;
            serialPort.Close();
        }
        catch
        {

        }
    }


    private byte[] Read_Rx_Bytes(System.IO.Ports.SerialPort port)
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


    private void SendBytes(System.IO.Ports.SerialPort port, List<byte> datas)
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
            byte read = InputBuffer[0];
            InputBuffer.RemoveAt(0);
            return read;
        }
        else
        {
            Log("Not enought data in RX buffer", 6, Color.black);
            return 0;
        }
    }

    public byte[] ReadRemoveInputByte(int count)
    {
        if (InputBuffer.Count < count)
        {
            count = InputBuffer.Count;
            Log($"Not enought data {count} in RX buffer {InputBuffer.Count}", 6, Color.black);
        }

        byte[] read = new byte[count];
        read = InputBuffer.GetRange(0, count).ToArray();
        InputBuffer.RemoveRange(0, count);
        return read;
    }



    #region INTERNAL_LOGGER

    public GameObject Logger;

    private void Log(string text, int channel, Color color, [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0, [System.Runtime.CompilerServices.CallerFilePath] string caller = null)
    {
        Debug.Log(text);

        string hour= String.Format("{0,-2}", System.DateTime.Now.Hour);
        string minutes=String.Format("{0,-2}", System.DateTime.Now.Minute);
        string secondes=String.Format("{0,-2}", System.DateTime.Now.Second);
        string miliseconds= String.Format("{0,-23}", System.DateTime.Now.Millisecond);

        string time = hour+":"+minutes+":"+secondes+":"+miliseconds;

        //string time = $"{System.DateTime.Now.Hour}:{System.DateTime.Now.Minute}:{System.DateTime.Now.Second}:{System.DateTime.Now.Millisecond}";

        Internal_Logger(time, channel, color, text, lineNumber, caller);
    }

    List<Logger_New_Line.Logger_Message> messages_for_internal_Logger = new List<Logger_New_Line.Logger_Message>();
    

    private void Internal_Logger(string time, int Channel, Color color, string text, int lineNumber, string caller)
    {
        string path_string = "File: " + System.IO.Path.GetFileName(caller) + ", " + lineNumber;

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
            try
            {
                foreach (Logger_New_Line.Logger_Message message in messages_for_internal_Logger)
                {
                    new_Liner.Add_New_Logger_Line(message);
                    message_used++;
                }
            }catch
            {

            }


            messages_for_internal_Logger.RemoveRange(0, message_used);
            yield return null;
        }
    }
    #endregion
}


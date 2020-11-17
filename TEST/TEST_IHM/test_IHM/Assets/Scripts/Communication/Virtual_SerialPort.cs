using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Threading.Tasks;

public class Virtual_SerialPort : MonoBehaviour
{
    System.IO.Ports.SerialPort serialPort;

    public bool port_opened = false;

    public string portName;
    public int portSpeed;
    public int data_in_port_read_buffer = 0;
    public int data_in_input_buffer = 0;

    //private List<byte> InputBuffer = new List<byte>();
    private List<byte> OutputBuffer = new List<byte>();  

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

        //Start_Reading_Task();

        Start_Sending_Task();
    }



    private void Start_Reading_Task()
    {
        //start listening for messages asynchronously
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            Logger_New_Line.Log("Waiting for serialport!", 6, Color.black);

            while (this.serialPort.IsOpen == false)
            {
                await Task.Delay(100);
            }

            Logger_New_Line.Log("Serial Port:" + serialPort.PortName + " Opened! Starting listening!", 6, Color.black);
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
                                //InputBuffer.AddRange(dataReceived);
                            }
                            catch
                            {
                                Logger_New_Line.Log($"Error in buffer insertion {dataReceived.Length}", 6, Color.black);
                            }
                        }

                        data_in_port_read_buffer = serialPort.BytesToRead;
                        //data_in_input_buffer = InputBuffer.Count;

                        await Task.Delay(2);

                    }
                    catch (Exception _ex)
                    {
                        Logger_New_Line.Log($"Receiving task Exception!: {_ex}", 6, Color.red);
                    }
                }
                else
                {
                    await Task.Delay(100);
                }

                if (Comport_cancellationToken)
                    throw new TaskCanceledException();
            }
        }));
    }

    private void Start_Sending_Task()
    {
        //start asynchronous data sending
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            while (this.serialPort.IsOpen == false)
            {
                await Task.Delay(100);
            }

            Logger_New_Line.Log("Serial Port:" + serialPort.PortName + " Opened! Starting sending!", 6, Color.black);
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
        serialPort.ReadBufferSize = 65000;
        this.portName = name;

        try
        {
            serialPort.Open();
            Logger_New_Line.Log("Serial Port:" + serialPort.PortName + " Opened!", 6, Color.black);
        }
        catch
        {
            Logger_New_Line.Log("Error while oppening Serial Port:" + serialPort.PortName + "!", 6, Color.red);
        }       
    }

    
    public void DisConnect()
    {
        try
        {
            serialPort.Close();
            Logger_New_Line.Log("Serial Port:" + serialPort.PortName + " Closed!", 6, Color.black);
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


    public int Number_Byte_To_Read()
    {
        if (serialPort == null)
            return 0;

        if (serialPort.IsOpen)
        {
            data_in_port_read_buffer = serialPort.BytesToRead;
            return serialPort.BytesToRead;
        }

        return 0;
    }


    public bool ReadRemoveInputByte(out byte data)
    {
        if (serialPort == null || serialPort.IsOpen == false)
        {
            data = 0;
            return false;
        }

        if (serialPort.BytesToRead > 0)
        {
            data = (byte)serialPort.ReadByte();
            return true;
        }

        data = 0;
        return false;
    }
        
  

    public bool ReadRemoveInputByte(int count, out byte[] datas)
    {
        if (serialPort == null || serialPort.IsOpen == false)
        {
            datas = new byte[0];
            return false;
        }

        if (serialPort.BytesToRead < count)
        {
            Logger_New_Line.Log($"Not enought data {count} in RX buffer {serialPort.BytesToRead}", 6, Color.black);
            count = serialPort.BytesToRead;
        }

        //Receptions des datas
        datas = new byte[count];
        serialPort.Read(datas, 0, count);

        return true;
    }
}


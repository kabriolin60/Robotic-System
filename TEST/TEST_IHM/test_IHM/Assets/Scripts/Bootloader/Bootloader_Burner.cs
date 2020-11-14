using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEditor;
using UnityEngine;
using System.Text;
using System.Runtime.InteropServices;
using System;
using UnityEngine.EventSystems;
using SFB;
using UnityEngine.UI;
using System.Threading;

public class Bootloader_Burner : MonoBehaviour
{
    string Application_HEX_File_Path;

    public GameObject Main_IHM_To_Hide;
    public GameObject File_path_Display;
    public GameObject Status;
    public GameObject ProgressBar;
    public GameObject Destination;

    System.IO.Ports.SerialPort serialPort1;
    string[] _lines;

    public void Bootloader_Form_Visible()
    {
        this.gameObject.SetActive(true);
        Main_IHM_To_Hide.SetActive(false);
    }

    public void Bootloader_Form_Hide()
    {
        this.gameObject.SetActive(false);
        Main_IHM_To_Hide.SetActive(true);
    }

    public void Bootloader_Select_File_Button_OnClick()
    {
        /*
         * 
         * https://github.com/gkngkc/UnityStandaloneFileBrowser         
         * 
         */



        var multipath = StandaloneFileBrowser.OpenFilePanel("Select HEX file", "", "hex", false);
        if (multipath != null)
        {
            if (multipath.Length > 0)
            {
                Application_HEX_File_Path = multipath[0];
            }
        }

        if (Application_HEX_File_Path != null)
        {
            Debug.Log($"HEX file path = {Application_HEX_File_Path}");
            File_path_Display.GetComponent<TextMeshProUGUI>().text = Application_HEX_File_Path;
        }
    }


    public void Bootloader_Start_Burning()
    {
        if(Application_HEX_File_Path == null)
        {
            Status.GetComponent<TextMeshProUGUI>().text = "No file path found";
            return;
        }

        //Read all file lines:
        _lines = System.IO.File.ReadAllLines(Application_HEX_File_Path);


        //Transform this string lines in byte[] lines and send over Serial Port

        if (this.serialPort1 == null)
        {
            this.serialPort1 = new System.IO.Ports.SerialPort();
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 2000000;
            this.serialPort1.PortName = "COM5";
        }

            if (this.serialPort1.IsOpen == false)
        {
            try
            {
                this.serialPort1.Open();
            }catch
            {
                Status.GetComponent<TextMeshProUGUI>().text = "No port opened";
                return;
            }
        }

        try
        {
            get_HEX_lines(_lines);

            this.serialPort1.Close();
        }
        catch
        {

        }
    }
    

    private void get_HEX_lines(string[] _lines)
    {
        byte[] data_to_send;
        
        int line_number = 0;

        this.ProgressBar.GetComponent<Slider>().maxValue = _lines.Length;

        Status.GetComponent<TextMeshProUGUI>().text = "Burning";

        //start listening for messages and copy the messages back to the client
        foreach (string _line in _lines)
        {
            data_to_send = ReadHexLine_to_ByteArray(_line);

            //Cree une trame de communication
            var dummy = Create_Trame(data_to_send);
            line_number++;

            this.ProgressBar.GetComponent<Slider>().value = line_number;

            if ((line_number - 1) % 256 == 0 && line_number != 1)
            {
                //Ecritude d'un bloc
                Thread.Sleep(200);
            }
        }

        Status.GetComponent<TextMeshProUGUI>().text = "Target burned";
    }


    public byte Create_Trame(byte[] _data)
    {
        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame.Instruction = Communication.Com_Instruction.BOOTLOADER;

        switch (this.Destination.GetComponent<TMPro.TMP_Dropdown>().value)
        {
            case 0:
                trame.Slave_Adresse = Communication.Slave_Adresses.IA_BOARD;
                break;

            case 1:
                trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1;
                break;

            case 2:
                trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_2;
                break;

            case 3:
                trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_3;
                break;

            case 4:
                trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_4;
                break;

            default:
                return 0;
        }

        trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        trame.Length = (byte)_data.Length;

        for (int i = 0; i < _data.Length; i++)
        {
            trame.Data[i] = _data[i];
        }


        //Envoie le message
        Communication comm = new Communication();
        comm.Trame_Data = trame;

        //Envoi du message
        Send(comm.Send_Trame(comm));

        Thread.Sleep(10);

        return 1;
    }


    


    private void Send(byte[] data)
    {
        if (this.serialPort1 == null || !this.serialPort1.IsOpen)
        {
            return;
        }

        if (!this.serialPort1.IsOpen)
            return;


        if (data.Length <= 64)
        {
            try
            {
                this.serialPort1.Write(data, 0, data.Length);
                while (this.serialPort1.BytesToWrite > 0) ;
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
                    this.serialPort1.Write(data, offset, len > 64 ? 64 : len);
                    while (this.serialPort1.BytesToWrite > 0) ;
                }
                catch
                {

                }
                len -= 64;
                offset += 64;
            }
        }


        while (this.serialPort1.BytesToWrite > 0) ;
    }


    private byte[] ReadHexLine_to_ByteArray(string _line)
    {
        //transforme cette string en un tableau de chars
        char[] _line_char = _line.ToCharArray();



        //char[0] = ':'
        //Get the line length (number of datas)
        byte _line_length = TwoChar_To_Byte(_line_char[1], _line_char[2]);

        byte[] _real_line = new byte[_line_length + 6];

        _real_line[0] = 58; //':'
        _real_line[1] = _line_length; //number of datas
        _real_line[2] = TwoChar_To_Byte(_line_char[3], _line_char[4]); //@ absolue high
        _real_line[3] = TwoChar_To_Byte(_line_char[5], _line_char[6]); //@ absolue low
        _real_line[4] = TwoChar_To_Byte(_line_char[7], _line_char[8]); //Type

        //Datas
        byte index = 0;
        for (index = 0; index < _line_length; index++)
        {
            _real_line[5 + index] = TwoChar_To_Byte(_line_char[9 + index * 2], _line_char[10 + index * 2]); //Datas
        }

        //CheckSum
        _real_line[5 + index] = TwoChar_To_Byte(_line_char[9 + index * 2], _line_char[10 + index * 2]);


        return _real_line;
    }


    private byte TwoChar_To_Byte(char _charA, char _charB)
    {
        byte _result = 0;

        _result = (byte)(char_to_byte(_charA) * 16 + char_to_byte(_charB));

        return _result;
    }

    private byte char_to_byte(char _input)
    {
        switch (_input)
        {
            case '0':
                return 0;

            case '1':
                return 1;

            case '2':
                return 2;

            case '3':
                return 3;

            case '4':
                return 4;

            case '5':
                return 5;

            case '6':
                return 6;

            case '7':
                return 7;

            case '8':
                return 8;

            case '9':
                return 9;

            case 'A':
                return 10;

            case 'B':
                return 11;

            case 'C':
                return 12;

            case 'D':
                return 13;

            case 'E':
                return 14;

            case 'F':
                return 15;

            default:
                return 0;
        }
    }



}

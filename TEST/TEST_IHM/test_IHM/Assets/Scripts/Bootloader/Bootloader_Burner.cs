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
using System.Threading.Tasks;

public class Bootloader_Burner : MonoBehaviour
{
    string Application_HEX_File_Path;

    public GameObject Main_IHM_To_Hide;
    public GameObject File_path_Display;
    public GameObject Status;
    public GameObject ProgressBar;
    public GameObject Destination;

    public string portName;
    public int portSpeed;


    static System.IO.Ports.SerialPort serialPort1;
    static string[] _lines;
    static int destination_board;
    static bool burn_ended = false;

    static int line_number_position = 0;
    int max_line_number = 0;


    public void FixedUpdate()
    {
        ProgressBar.GetComponent<Slider>().value = line_number_position;

        if (burn_ended)
        {
            Status.GetComponent<TextMeshProUGUI>().text = "Target burned";
            serialPort1.Close();

            burn_ended = false;
        }
    }


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


    public void Bootloader_Start_BurningAsync()
    {
        if (Application_HEX_File_Path == null)
        {
            Status.GetComponent<TextMeshProUGUI>().text = "No file path found";
            return;
        }

        //Read all file lines:
        _lines = System.IO.File.ReadAllLines(Application_HEX_File_Path);


        //Transform this string lines in byte[] lines and send over Serial Port

        if (serialPort1 == null)
        {
            serialPort1 = new System.IO.Ports.SerialPort();
            serialPort1.BaudRate = portSpeed;
            serialPort1.PortName = portName;
        }

        if (serialPort1.IsOpen == false)
        {
            try
            {
                serialPort1.Open();
            }
            catch
            {
                Status.GetComponent<TextMeshProUGUI>().text = "No port opened";
                return;
            }
        }

        try
        {
            max_line_number = _lines.Length;

            destination_board = Destination.GetComponent<TMPro.TMP_Dropdown>().value;

            ProgressBar.GetComponent<Slider>().maxValue = _lines.Length;

            Status.GetComponent<TextMeshProUGUI>().text = "Burning";

            //start listening for messages and copy the messages back to the client
            Task.Factory.StartNew(async () =>
            {
                bool received = await EnvoiAsync();

                if (received)
                    burn_ended = true;
            });

        }
        catch
        {

        }
    }   

    private static async Task<bool> EnvoiAsync()
    {
        byte[] data_to_send;

        //Envoie le message
        Sending_Data comm = new Sending_Data();

        Communication.Communication_Message trame = new Communication.Communication_Message();

        trame.Trame.Instruction = Communication.Com_Instruction.BOOTLOADER;
        switch (destination_board)
        {
            case 0:
                trame.Trame.Slave_Adresse = Communication.Slave_Adresses.IA_BOARD;
                break;

            case 1:
                trame.Trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_1;
                break;

            case 2:
                trame.Trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_2;
                break;

            case 3:
                trame.Trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_3;
                break;

            case 4:
                trame.Trame.Slave_Adresse = Communication.Slave_Adresses.MultiFct_4;
                break;

            default:
                trame.Trame.Slave_Adresse = Communication.Slave_Adresses.IA_BOARD;
                break;
        }
        trame.Trame.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE;

        //start listening for messages and copy the messages back to the client
        foreach (string _line in _lines)
        {
            data_to_send = ReadHexLine_to_ByteArray(_line);

            trame.Trame.Length = (byte)data_to_send.Length;
            for (int i = 0; i < data_to_send.Length; i++)
            {
                trame.Trame.Data[i] = data_to_send[i];
            }

            //Envoi du message
            Send(comm.Send_Trame(trame));

            //Cree une trame de communication
            line_number_position++;            

            await Task.Delay(5);

            if ((line_number_position - 1) % 256 == 0 && line_number_position != 1)
            {
                //Ecritude d'un bloc
                await Task.Delay(200);
            }
        }                

        return true;
    }


    static private void Send(byte[] data)
    {
        if (serialPort1 == null || !serialPort1.IsOpen)
        {
            return;
        }

        if (!serialPort1.IsOpen)
            return;


        if (data.Length <= 64)
        {
            try
            {
                serialPort1.Write(data, 0, data.Length);
                while (serialPort1.BytesToWrite > 0) ;
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
                    serialPort1.Write(data, offset, len > 64 ? 64 : len);
                    while (serialPort1.BytesToWrite > 0) ;
                }
                catch
                {

                }
                len -= 64;
                offset += 64;
            }
        }


        while (serialPort1.BytesToWrite > 0) ;
    }


    static private byte[] ReadHexLine_to_ByteArray(string _line)
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


    static private byte TwoChar_To_Byte(char _charA, char _charB)
    {
        byte _result = 0;

        _result = (byte)(char_to_byte(_charA) * 16 + char_to_byte(_charB));

        return _result;
    }


    static private byte char_to_byte(char _input)
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

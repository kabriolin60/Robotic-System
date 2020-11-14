using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Windows.Forms;
using System;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;



public class Bootloader : MonoBehaviour
{
    System.Windows.Forms.Form Bootloader_Form;

    System.IO.Ports.SerialPort serialPort1;

    string Application_HEX_File_Path;

    string[] _lines;

    public void Open_Bootloader_Form()
    {
        if(Bootloader_Form == null)
        {
            Bootloader_Form = new Form();
            InitializeComponent();

            Bootloader_Form.ShowDialog();
        }


    }


    private void Select_HEX_File_Click(object sender, EventArgs e)
    {
        OpenFileDialog openFileDialog1 = new OpenFileDialog();
        openFileDialog1.DefaultExt = "hex";
        openFileDialog1.Filter = "hex files(*.hex) | *.hex";
        openFileDialog1.FilterIndex = 2;

        //Get application file path:
        if (openFileDialog1.ShowDialog() == DialogResult.OK)
        {
            Application_HEX_File_Path = openFileDialog1.FileName;

            this.File_Name.Text = Application_HEX_File_Path;

            //Read file content
            _lines = System.IO.File.ReadAllLines(Application_HEX_File_Path);
        }
    }


    private void Envoi_Click(object sender, EventArgs e)
    {
        //Transform this string lines in byte[] lines and send over Serial Port

        try
        {
            this.serialPort1.Open();
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

        this.Avancement.Text = "0/" + _lines.Length.ToString() + " Lines";
        int line_number = 0;

        this.progressBar1.Maximum = _lines.Length;

        //start listening for messages and copy the messages back to the client
        foreach (string _line in _lines)
        {
            data_to_send = ReadHexLine_to_ByteArray(_line);

            //Cree une trame de communication
            var dummy = Create_Trame(data_to_send);
            line_number++;

            this.Avancement.Text = line_number.ToString() + "/ " + _lines.Length.ToString() + " Lines";
            this.progressBar1.Value = line_number;

            this.Bootloader_Form.Update();


            if ((line_number - 1) % 256 == 0 && line_number != 1)
            {
                //Ecritude d'un bloc
                Thread.Sleep(200);
            }
        }
    }


    public byte Create_Trame(byte[] _data)
    {
        Communication.Communication_Trame trame = new Communication.Communication_Trame();

        trame.Instruction = Communication.Com_Instruction.BOOTLOADER;

        switch (this.Destination.SelectedIndex)
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

        Send_Trame(comm);

        Thread.Sleep(10);

        return 1;
    }


    /* Prend un message, et transforme-le en trame pret à partir, avec en-tête X-bee et CRC */
    private void Send_Trame(Communication Message_to_send)
    {
        Message_to_send.Trame_Data.XBEE_DEST_ADDR = Communication.Adress_Xbee.ALL_XBEE; //Envoie du PC vers tous les XBEE par defaut

        byte[] data = new byte[Message_to_send.Trame_Data.Length + 11];
        data[0] = 0x7E;                         //Xbee API start byte

        int length = Message_to_send.Trame_Data.Length + 7;

        data[1] = (byte)(length >> 8);                  //length high
        data[2] = (byte)(length & 0xFF);                //length low

        data[3] = 0x01;                         //Frame type: Tx, 16 bits addr
        data[4] = 0x01;                         //Frame ID

        data[5] = (byte)((int)Message_to_send.Trame_Data.XBEE_DEST_ADDR >> 8);        //add high
        data[6] = (byte)((int)Message_to_send.Trame_Data.XBEE_DEST_ADDR & 0xFF);      //add low

        data[7] = 0x00;                         //Option

        byte index = 0;
        //Datas
        {
            data[8] = (byte)(Message_to_send.Trame_Data.Instruction);

            data[9] = (byte)(Message_to_send.Trame_Data.Slave_Adresse);

            for (index = 0; index < Message_to_send.Trame_Data.Length; index++)
            {
                data[10 + index] = Message_to_send.Trame_Data.Data[index];
            }
        }

        short API_CRC = 0;
        for (index = 3; index < 10 + Message_to_send.Trame_Data.Length; index++)
        {
            API_CRC += data[index];
        }

        API_CRC &= 0xFF;
        API_CRC = (byte)(0xFF - API_CRC);

        data[index] = (byte)(API_CRC);

        //Envoi de la trame
        Send(data);
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




    /// <summary>
    /// Variable nécessaire au concepteur.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    #region Code généré par le Concepteur Windows Form

    /// <summary>
    /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
    /// le contenu de cette méthode avec l'éditeur de code.
    /// </summary>
    private void InitializeComponent()
    {
        this.components = new System.ComponentModel.Container();
        this.Select_HEX_File = new System.Windows.Forms.Button();
        this.Envoi = new System.Windows.Forms.Button();
        if(this.serialPort1 == null)
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
        this.progressBar1 = new System.Windows.Forms.ProgressBar();
        this.File_Name = new System.Windows.Forms.Label();
        this.Avancement = new System.Windows.Forms.Label();
        this.Destination = new System.Windows.Forms.ComboBox();
        this.Bootloader_Form.SuspendLayout();
        // 
        // Select_HEX_File
        // 
        this.Select_HEX_File.Location = new System.Drawing.Point(13, 13);
        this.Select_HEX_File.Name = "Select_HEX_File";
        this.Select_HEX_File.Size = new System.Drawing.Size(75, 23);
        this.Select_HEX_File.TabIndex = 0;
        this.Select_HEX_File.Text = "Select .HEX";
        this.Select_HEX_File.UseVisualStyleBackColor = true;
        this.Select_HEX_File.Click += new System.EventHandler(this.Select_HEX_File_Click);
        // 
        // Envoi
        // 
        this.Envoi.Location = new System.Drawing.Point(12, 70);
        this.Envoi.Name = "Envoi";
        this.Envoi.Size = new System.Drawing.Size(75, 23);
        this.Envoi.TabIndex = 1;
        this.Envoi.Text = "Burn";
        this.Envoi.UseVisualStyleBackColor = true;
        this.Envoi.Click += new System.EventHandler(this.Envoi_Click);
        // 
        // serialPort1
        // 
        this.serialPort1.BaudRate = 2000000;
        this.serialPort1.PortName = "COM7";
        // 
        // progressBar1
        // 
        this.progressBar1.Location = new System.Drawing.Point(12, 100);
        this.progressBar1.Name = "progressBar1";
        this.progressBar1.Size = new System.Drawing.Size(221, 23);
        this.progressBar1.TabIndex = 2;
        // 
        // File_Name
        // 
        this.File_Name.AutoSize = true;
        this.File_Name.Location = new System.Drawing.Point(94, 18);
        this.File_Name.Name = "File_Name";
        this.File_Name.Size = new System.Drawing.Size(16, 13);
        this.File_Name.TabIndex = 3;
        this.File_Name.Text = "...";
        // 
        // Avancement
        // 
        this.Avancement.AutoSize = true;
        this.Avancement.Location = new System.Drawing.Point(239, 105);
        this.Avancement.Name = "Avancement";
        this.Avancement.Size = new System.Drawing.Size(0, 13);
        this.Avancement.TabIndex = 4;
        // 
        // Destination
        // 
        this.Destination.FormattingEnabled = true;
        this.Destination.Items.AddRange(new object[] {
            "IA",
            "MultiFct_1",
            "MultiFct_2",
            "MultiFct_3",
            "MultiFct_4"});
        this.Destination.Location = new System.Drawing.Point(13, 43);
        this.Destination.Name = "Destination";
        this.Destination.Size = new System.Drawing.Size(121, 21);
        this.Destination.TabIndex = 5;
        // 
        // Bootloader
        // 
        this.Bootloader_Form.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        this.Bootloader_Form.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.Bootloader_Form.ClientSize = new System.Drawing.Size(800, 450);
        this.Bootloader_Form.Controls.Add(this.Destination);
        this.Bootloader_Form.Controls.Add(this.Avancement);
        this.Bootloader_Form.Controls.Add(this.File_Name);
        this.Bootloader_Form.Controls.Add(this.progressBar1);
        this.Bootloader_Form.Controls.Add(this.Envoi);
        this.Bootloader_Form.Controls.Add(this.Select_HEX_File);
        this.Bootloader_Form.Name = "Bootloader";
        this.Bootloader_Form.Text = "Robotic System: Bootloader";
        this.Bootloader_Form.ResumeLayout(false);
        this.Bootloader_Form.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Button Select_HEX_File;
    private System.Windows.Forms.Button Envoi;
    private System.Windows.Forms.ProgressBar progressBar1;
    private System.Windows.Forms.Label File_Name;
    private System.Windows.Forms.Label Avancement;
    private System.Windows.Forms.ComboBox Destination;

}

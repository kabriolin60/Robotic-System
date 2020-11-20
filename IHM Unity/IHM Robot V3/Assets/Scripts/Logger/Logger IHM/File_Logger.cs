using SFB;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml.Serialization;
using System.Threading.Tasks;
using TMPro;
using UnityEngine;

public class File_Logger : MonoBehaviour
{
    string Logger_File_Path;

    private bool Use_XML = true;

    FileStream fileStream;

    Serialized_Datas serialized_data = new Serialized_Datas();
    Serialized_Datas deserialized_data;
    List<Task> tasks = new List<Task>();

    private bool Pause_Reading_Token = true;
    private bool Stop_Reading_Token = false;
    private bool Is_Reading = false;

    public GameObject Button_Start_Pause;

    public float Reading_Speed = 1.0F;

    public void Save_Data_Select_File_Button_OnClick()
    {
        /*
         * 
         * https://github.com/gkngkc/UnityStandaloneFileBrowser         
         * 
         */

        if (Logger_File_Path == null)
        {
            //Dans quel fichier écrire
            if (Use_XML)
            {
                Logger_File_Path = StandaloneFileBrowser.SaveFilePanel("Save File", "", "", "xml");
            }
            else
            {
                Logger_File_Path = StandaloneFileBrowser.SaveFilePanel("Save File", "", "", "txt");
            }
        }
        else
        {
            //On a deja commence à écrire, il s'agit de la demande d'arret d'écriture
            Serialize_And_Close();
        }
    }

    private void Serialize_And_Close()
    {
        if(Use_XML)
        {
            if(!Is_Reading)
            XMLSerializer(typeof(Serialized_Datas), serialized_data);
        }
        else
        {
            BinarySerializer(serialized_data);
        }        
    }

    private void BinarySerializer(object data)
    {
        if (Logger_File_Path != null)
        {
            Debug.Log($"Logger file path = {Logger_File_Path}");

            if (File.Exists(Logger_File_Path) == false)
            {
                //Le fichier n'existe pas encore
                //Il faut donc commencer par le créer
                fileStream = File.Create(Logger_File_Path);
            }
            else
            {
                fileStream = File.OpenWrite(Logger_File_Path);
            }
        }

        //On peut désormais écrire
        BinaryFormatter formater = new BinaryFormatter();
        //Serialisation de nos datas et écritures
        formater.Serialize(fileStream, data);

        //Ferme le fichier
        fileStream.Close();
    }

    private void XMLSerializer(Type dataType, object data)
    {
        if (Logger_File_Path != null)
        {
            Debug.Log($"Logger file path = {Logger_File_Path}");

            if (File.Exists(Logger_File_Path) == false)
            {
                //Le fichier n'existe pas encore
                //Il faut donc commencer par le créer
                File.Delete(Logger_File_Path);
            }
        }

        XmlSerializer serialiser = new XmlSerializer(dataType);
        TextWriter writer = new StreamWriter(Logger_File_Path);

        serialiser.Serialize(writer, data);
        writer.Close();
    }

    private void BinaryDeSerializer()
    {
        //Ouvre le fichier
        fileStream = File.OpenRead(Logger_File_Path);

        //Dezerialize les datas dans le fichier
        BinaryFormatter formater = new BinaryFormatter();
        object _deserialized_data = formater.Deserialize(fileStream);

        deserialized_data = new Serialized_Datas();
        deserialized_data = (Serialized_Datas)_deserialized_data;
    }

    private void XMLDeSerializer(Type dataType)
    {
        object obj = null;

        XmlSerializer serialiser = new XmlSerializer(dataType);
        TextReader reader = new StreamReader(Logger_File_Path);

        obj = serialiser.Deserialize(reader);
        deserialized_data = new Serialized_Datas();
        deserialized_data = (Serialized_Datas)obj;

        reader.Close();
    }


    public void Open_Data_Select_File_Button_OnClick()
    {
        /*
         * 
         * https://github.com/gkngkc/UnityStandaloneFileBrowser         
         * 
         */

        string extension = "txt";
        if (Use_XML)
        {
            extension = "xml";
        }

        var multipath = StandaloneFileBrowser.OpenFilePanel("Save File", "", extension, false);


        if (multipath != null)
        {
            if (multipath.Length > 0)
            {
                Logger_File_Path = multipath[0];
            }
        }

        if (Logger_File_Path != null)
        {
            Debug.Log($"Logger file path = {Logger_File_Path}");
        }

        Read_Logger_Data();
    }

    public void Write_Logging_Data(object DataToWrite)
    {
        if (Logger_File_Path == null)
        {
            //Aucun enregistrement demandé
            return;
        }        

        //ajoute les infos à la liste des données reçues
        serialized_data.Add_Data((Communication.Communication_Message)DataToWrite);
    }

    
    public bool Read_Logger_Data()
    {
        if (File.Exists(Logger_File_Path) == false)
        {
            //Le fichier n'existe pas
            return false;
        }

        if (Use_XML)
        {            
            XMLDeSerializer(typeof(Serialized_Datas));
        }
        else
        {
            BinaryDeSerializer();
        }
        
        
        //crée une tache asynchrone qui va lire les infos en temps réel
        Reading_Task();

        return true;
    }


    public void Request_Stop_Reading()
    {
        this.Stop_Reading_Token = true;
        Is_Reading = false;
    }

    public void Request_Pause_Reading()
    {
        if (!this.Pause_Reading_Token)
        {
            this.Pause_Reading_Token = true;
            Button_Start_Pause.GetComponent<TextMeshProUGUI>().text = "Run";
        }
        else
        {
            Is_Reading = true;
            this.Pause_Reading_Token = false;
            Button_Start_Pause.GetComponent<TextMeshProUGUI>().text = "Pause";
        }
    }


    private void Reading_Task()
    {
        //vérifie qu'il y a bien des données à lire
        if (deserialized_data.messages.Count == 0)
            return;

        Interprete_Message Interpreter = this.GetComponent<Interprete_Message>();

        //start asynchronous data sending
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            Logger_New_Line.Log($"Start Reading logged data: {System.IO.Path.GetFileName(Logger_File_Path)}", 6, Color.black);
            Communication.Communication_Message message_output = new Communication.Communication_Message();

            DateTime heure_premier_enregistrement = deserialized_data.messages[0].Heure;

            TimeSpan delay = new TimeSpan();
            DateTime last_hour = heure_premier_enregistrement;

            //Si une pause dans la lecture est demandee
            while (Pause_Reading_Token)
            {
                await Task.Delay(100);
            }

            //Pour chaque message dans la Liste
            foreach (Communication.Communication_Message message in deserialized_data.messages)
            {
                //Attend que l'heure de traiter ce message soit venue
                delay = message.Heure.Subtract(last_hour);
                await Task.Delay((int)(delay.TotalMilliseconds / Reading_Speed));

                //interprete ce message
                Interpreter.Decodage_Message(message);

                last_hour = message.Heure;

                //si on demande l'arret de la lecture
                if (Stop_Reading_Token)
                {
                    Is_Reading = false;
                    throw new TaskCanceledException();
                }

                //Si une pause dans la lecture est demandee
                while (Pause_Reading_Token)
                {
                    await Task.Delay(100);
                }
            }

            Is_Reading = false;
        }));
    }


    public void Update()
    {
        if(Is_Reading)
        {
            if(Input.GetKeyDown(KeyCode.Plus) || Input.GetKeyDown(KeyCode.KeypadPlus))
            {
                this.Reading_Speed *= 2;
            }

            if (Input.GetKeyUp(KeyCode.Minus) || Input.GetKeyUp(KeyCode.KeypadMinus))
            {
                this.Reading_Speed /= 2;
            }
        }
    }

    public void OnDestroy()
    {
        try
        {
            Serialize_And_Close();
        }
        catch
        {

        }
    }
}


[Serializable]
public class Serialized_Datas
{
    public int Nombre_Messages = 0;
    public List<Communication.Communication_Message> messages;

    public Serialized_Datas()
    {
        messages = new List<Communication.Communication_Message>();
    }

    public void Add_Data(Communication.Communication_Message message_to_add)
    {
        messages.Add(message_to_add);
        Nombre_Messages = messages.Count;
    }
}
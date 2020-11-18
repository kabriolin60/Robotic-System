using SFB;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Threading.Tasks;
using TMPro;
using UnityEngine;

public class File_Logger : MonoBehaviour
{
    string Logger_File_Path;

    FileStream fileStream;

    Serialized_Datas serialized_data = new Serialized_Datas();
    Serialized_Datas deserialized_data;
    List<Task> tasks = new List<Task>();

    private bool Pause_Reading_Token = true;
    private bool Stop_Reading_Token = false;

    public GameObject Button_Start_Pause;

    public void Save_Data_Select_File_Button_OnClick()
    {
        /*
         * 
         * https://github.com/gkngkc/UnityStandaloneFileBrowser         
         * 
         */

        Logger_File_Path = StandaloneFileBrowser.SaveFilePanel("Save File", "", "", "txt");

        if (Logger_File_Path != null)
        {
            Debug.Log($"Logger file path = {Logger_File_Path}");
        }
    }

    public void Open_Data_Select_File_Button_OnClick()
    {
        /*
         * 
         * https://github.com/gkngkc/UnityStandaloneFileBrowser         
         * 
         */

        var multipath = StandaloneFileBrowser.OpenFilePanel("Save File", "", "txt", false);
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

        //ajoute les infos à la liste des données reçues
        serialized_data.Add_Data((Communication.Communication_Message)DataToWrite);

        //Le fichier existe
        //On peut désormais écrire
        BinaryFormatter formater = new BinaryFormatter();
        //Serialisation de nos datas et écritures
        formater.Serialize(fileStream, serialized_data);

        //Ferme le fichier
        fileStream.Close();
    }

    
    public bool Read_Logger_Data()
    {
        if (File.Exists(Logger_File_Path) == false)
        {
            //Le fichier n'existe pas
            return false;
        }

        //Ouvre le fichier
        fileStream = File.OpenRead(Logger_File_Path);

        //Dezerialize les datas dans le fichier
        BinaryFormatter formater = new BinaryFormatter();
        object _deserialized_data = formater.Deserialize(fileStream);

        deserialized_data = new Serialized_Datas();
        deserialized_data = (Serialized_Datas)_deserialized_data;

        //crée une tache asynchrone qui va lire les infos en temps réel
        Start_Reading_Task();

        return true;
    }


    public void Request_Stop_Reading()
    {
        this.Stop_Reading_Token = true;
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
            this.Pause_Reading_Token = false;
            Button_Start_Pause.GetComponent<TextMeshProUGUI>().text = "Pause";
        }
    }


    private void Start_Reading_Task()
    {
        //vérifie qu'il y a bien des données à lire
        if (deserialized_data.messages.Count == 0)
            return;

        Interprete_Message Interpreter = this.GetComponent<Interprete_Message>();

        //start asynchronous data sending
        tasks.Add(Task.Factory.StartNew(async () =>
        {
            await Task.Delay(2000);

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
                await Task.Delay((int)delay.TotalMilliseconds);

                //interprete ce message
                Interpreter.Decodage_Message(message);

                last_hour = message.Heure;

                //si on demande l'arret de la lecture
                if(Stop_Reading_Token)
                    throw new TaskCanceledException();

                //Si une pause dans la lecture est demandee
                while (Pause_Reading_Token)
                {
                    await Task.Delay(100);
                }
            }
        }));
    }


    public void OnDestroy()
    {
        try
        {
            fileStream.Close();
        }
        catch
        {

        }
    }
}


[Serializable]
public class Serialized_Datas
{
    public List<Communication.Communication_Message> messages;

    public Serialized_Datas()
    {
        messages = new List<Communication.Communication_Message>();
    }

    public void Add_Data(Communication.Communication_Message message_to_add)
    {
        messages.Add(message_to_add);
    }
}
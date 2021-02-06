using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;
using TMPro;
using UnityEngine;

public class File_Manager : MonoBehaviour
{
    public TextMeshProUGUI Debugger;

    public T Load_XML<T>(string File_name) where T : class
    {
        object obj = null;

        string path;

#if UNITY_ANDROID
        //path = Application.persistentDataPath + File_name;
        path = "/storage/emulated/0/Download/du_gdt_download" + File_name;
#endif

#if UNITY_EDITOR
        path = "C:/Users/kabri/OneDrive/Documents/Avion/gestion Lipo" + File_name;
#endif

        Debugger.text += ("Trying to load:\n");
        Debugger.text += (path + "\n");
        Debug.Log("Trying to load: " + path);

        if (File.Exists(path) == true)
        {
            Debugger.text += ("Deserializing the File...\n");
            Debug.Log("Deserializing the File...");

            XmlSerializer serialiser = new XmlSerializer(typeof(T));
            TextReader reader = new StreamReader(path);

            obj = serialiser.Deserialize(reader);

            reader.Close();

            Debugger.text += ("Load XML File succeed\n");
            Debug.Log("Load XML File succeed");

            return (T)obj;
        }

        return null;
    }


    public bool Save_XML<T>(string File_name, object data) where T : class
    {
        string path;

#if UNITY_ANDROID
        //path = Application.persistentDataPath + File_name;
        path = "/storage/emulated/0/Download/du_gdt_download" + File_name;
#endif

#if UNITY_EDITOR
        path = "C:/Users/kabri/OneDrive/Documents/Avion/gestion Lipo" + File_name;
#endif

        Debugger.text += ("Trying to save:\n");
        Debugger.text += (path + "\n");
        Debug.Log("Trying to save: " + path);


        if (File.Exists(path) == false)
        {
            //Le fichier n'existe pas encore
            //Il faut donc commencer par le créer
            File.Delete(path);
        }

        XmlSerializer serialiser = new XmlSerializer(typeof(T));
        TextWriter writer = new StreamWriter(path);

        serialiser.Serialize(writer, data);
        writer.Close();

        return true;
    }
}

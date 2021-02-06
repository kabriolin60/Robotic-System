using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;
using UnityEngine;

public class File_Manager : MonoBehaviour
{
    public T Load_XML_Local_File<T>(string File_name) where T : class
    {
        object obj = null;

        string path;

#if UNITY_ANDROID
            //path = Application.persistentDataPath + File_name;
            path = "/storage/emulated/0/Download/du_gdt_download" + File_name;
#endif

        Debug.Log("Trying to load" + path);

        if (File.Exists(path) == true)
        {
            XmlSerializer serialiser = new XmlSerializer(typeof(T));
            TextReader reader = new StreamReader(path);

            obj = serialiser.Deserialize(reader);

            reader.Close();

            Debug.Log("Load XML file succed");

            return (T)obj;
        }

        return null;
    }
}

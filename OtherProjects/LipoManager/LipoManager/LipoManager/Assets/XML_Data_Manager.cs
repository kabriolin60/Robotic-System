﻿using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;
using UnityEngine;

public class XML_Data_Manager : MonoBehaviour
{
    string Logger_File_Path = "/Test_LipoManager.xml";


    public List<QR_Scanner.Operation> Liste_Operations = new List<QR_Scanner.Operation>();


    void Start()
    {
        Load_XML_Local_File();
    }


    public void Add_New_Operation(QR_Scanner.Operation new_ope)
    {
        Liste_Operations.Add(new_ope);
    }

    public void Load_XML_Local_File()
    {
        object obj = null;

        string path = Logger_File_Path;

        #if UNITY_ANDROID || UNITY_IOS
            path = Application.persistentDataPath + Logger_File_Path;
#endif

        if (File.Exists(path) == true)
        {
            XmlSerializer serialiser = new XmlSerializer(typeof(List<QR_Scanner.Operation>));
            TextReader reader = new StreamReader(path);

            obj = serialiser.Deserialize(reader);
            Liste_Operations = (List<QR_Scanner.Operation>)obj;

            reader.Close();

            foreach(QR_Scanner.Operation ope in Liste_Operations)
            {
                foreach(QR_Scanner.QRCode_Data element in ope.Elements_Presents_List)
                {
                    if (!ope.Elements_Presents.ContainsKey(element.Name))
                    {
                        ope.Elements_Presents.Add(element.Name, element);
                    }
                }                
            }

            this.GetComponent<Main_Display>().Display_All_Ope(Liste_Operations);
        }
    }




    public void Save_XML_File()
    {
        string path = Logger_File_Path;

        #if UNITY_ANDROID || UNITY_IOS
            path = Application.persistentDataPath + Logger_File_Path;
        #endif



        Debug.Log($"Logger file path = {path}");

        if (File.Exists(path) == false)
        {
            //Le fichier n'existe pas encore
            //Il faut donc commencer par le créer
            File.Delete(path);
        }

        XmlSerializer serialiser = new XmlSerializer(typeof(List<QR_Scanner.Operation>));
        TextWriter writer = new StreamWriter(path);

        serialiser.Serialize(writer, Liste_Operations);
        writer.Close();
    }
}


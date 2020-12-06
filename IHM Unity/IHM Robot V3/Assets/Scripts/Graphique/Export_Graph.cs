using SFB;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class Export_Graph : MonoBehaviour
{
    string Logger_File_Path = null;

    public void Save_Data_Select_File_Button_OnClick()
    {
        /*
         * 
         * https://github.com/gkngkc/UnityStandaloneFileBrowser         
         * 
         */

        //Dans quel fichier écrire           
        Logger_File_Path = StandaloneFileBrowser.SaveFilePanel("Save File", "", "", "txt");

    }

    public void Export_Graphique()
    {
        Save_Data_Select_File_Button_OnClick();        

        StreamWriter writer = new StreamWriter(Logger_File_Path, false);

        int nombre_data = 0;


        if(this.GetComponent<Graphique>().Data_Historique == null)
            return;

        if (this.GetComponent<Graphique>().Data_Historique[0] == null)
            return;

        nombre_data = this.GetComponent<Graphique>().Data_Historique[0].Count;        

        for(int position = 0; position < nombre_data; position++)
        {
            string line = position.ToString();
            line += ",";

            foreach (var serie in this.GetComponent<Graphique>().Data_Historique)
            {
                line += serie[position].y.ToString();
                line += ",";
            }

            writer.WriteLine(line);
        }

        writer.Close();

        this.GetComponent<Graphique>().Data_Historique.Clear();
        this.GetComponent<Graphique>().CategoryDictionnary.Clear();
    }
}

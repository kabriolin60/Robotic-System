using ChartAndGraph;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public partial class Graphique : MonoBehaviour
{
    public GraphChart chart;
    public Material linematerial_Prefab;
    public MaterialTiling lineMaterialTiling_Prefab;
    public Material infillMaterial_Prefab;
    public Material pointMaterial_Prefab;

    private Dictionary<byte, string> CategoryDictionnary = new Dictionary<byte, string>();


    private void Create_New_Channel(string name, byte id)
    {
        string found_name;
        //Si ce channel n'existe pas
        if (!CategoryDictionnary.TryGetValue(id, out found_name))
        {
            //crée-le
            chart.DataSource.AddCategory(name, linematerial_Prefab, 4f, lineMaterialTiling_Prefab, infillMaterial_Prefab, false, pointMaterial_Prefab, 0f);

            //Puis ajoute-le au dictionnaire
            CategoryDictionnary.Add(id, name);
        }
    }

    public void Ajoute_Data(byte channel_id, int position_X, float value)
    {
        //Check if this channel already exist or create it
        Create_New_Channel($"Channel_{channel_id}", channel_id);

        //chart.DataSource.StartBatch();
        //Then add the datas to this channel
        chart.DataSource.AddPointToCategoryRealtime($"Channel_{channel_id}", position_X, value);

        if (position_X > 200)
        {
            chart.DataSource.HorizontalViewOrigin++;
            //chart.DataSource.HorizontalViewSize++;
        }

        //chart.DataSource.EndBatch();
    }

    public void Ajoute_Data(st_Graph_Datas datas, int position_X)
    {
        chart.DataSource.StartBatch();
        byte channel_id;

        for (byte i = 0; i < datas.nb_datas_to_send; i++)
        {
            channel_id = datas.Datas[i].Channel;

            //Check if this channel already exist or create it
            Create_New_Channel($"Channel_{channel_id}", channel_id);

            //Then add the datas to this channel
            chart.DataSource.AddPointToCategory($"Channel_{channel_id}", position_X, datas.Datas[i].Data);            
        }

        if (position_X > 200)
        {
            chart.DataSource.HorizontalViewOrigin++;
            //chart.DataSource.HorizontalViewSize++;
        }

        chart.DataSource.EndBatch();
    }
}

public partial class Graphique
{
    /**************************************************
Declaration de la definition de la Structure contenant les valeurs à afficher dans le graphique
 **************************************************/
    [StructLayout(LayoutKind.Sequential)]
    public class st_Graph_Data
    {
        [MarshalAs(UnmanagedType.U1)]
        public byte Channel;

        [MarshalAs(UnmanagedType.R4)]
        public float Data;
    };

    [StructLayout(LayoutKind.Sequential)]
    public class st_Graph_Datas
    {
        [MarshalAs(UnmanagedType.U1)]
        public byte nb_datas_to_send;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]
        public st_Graph_Data[] Datas = new st_Graph_Data[12];
    };


    public st_Graph_Datas Trame_To_Data(Communication.Communication_Trame input_trame)
    {
        st_Graph_Datas output = new st_Graph_Datas();

        output = (st_Graph_Datas)Communication.GetStructFromArray<st_Graph_Datas>(input_trame.Data);
        return output;
    }
}




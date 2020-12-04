using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test_Graphqiue : MonoBehaviour
{
    public Graphique Graph;

    Graphique.st_Graph_Datas data = new Graphique.st_Graph_Datas();

    int X = 0;

    // Start is called before the first frame update
    void Start()
    {
        data.Datas[0] = new Graphique.st_Graph_Data();
        data.Datas[1] = new Graphique.st_Graph_Data();

        data.nb_datas_to_send = 2;
        data.Datas[0].Channel = 0;
        data.Datas[1].Channel = 1;        
    }

    // Update is called once per frame
    void Update()
    {
        data.Datas[0].Data = UnityEngine.Random.value * 1000;
        data.Datas[1].Data = UnityEngine.Random.value * 1000;

        Graph.Ajoute_Data(data.Datas[0].Channel, X, data.Datas[0].Data);
        Graph.Ajoute_Data(data.Datas[1].Channel, X, data.Datas[1].Data);

        X++;
    }
}

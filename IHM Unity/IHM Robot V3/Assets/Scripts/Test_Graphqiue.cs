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
        data.datas.Datas[0] = new Graphique.st_Graph_Data();
        data.datas.Datas[1] = new Graphique.st_Graph_Data();
        data.datas.Datas[2] = new Graphique.st_Graph_Data();
        data.datas.Datas[3] = new Graphique.st_Graph_Data();

        data.datas.nb_datas_to_send = 4;
        data.datas.Datas[0].Channel = 0;
        data.datas.Datas[1].Channel = 1;
        data.datas.Datas[2].Channel = 2;
        data.datas.Datas[3].Channel = 3;

        this.StartCoroutine(Add_Data());
    }

    // Update is called once per frame
    /*void Update()
    {
        data.Datas[0].Data = UnityEngine.Random.value * 1000;
        data.Datas[1].Data = UnityEngine.Random.value * 1000;

        Graph.Ajoute_Data(data.Datas[0].Channel, X, data.Datas[0].Data);
        Graph.Ajoute_Data(data.Datas[1].Channel, X, data.Datas[1].Data);

        X++;
    }*/

    IEnumerator Add_Data()
    {
        while(true)
        {
            yield return new WaitForSeconds(.01f);

            data.datas.Datas[0].Data = (int)(UnityEngine.Random.value * 1000);
            data.datas.Datas[1].Data = (int)(UnityEngine.Random.value * 1000);
            data.datas.Datas[2].Data = (int)(UnityEngine.Random.value * 1000);
            data.datas.Datas[3].Data = (int)(UnityEngine.Random.value * 1000);

            Graph.Ajoute_Data(data, X);

            X++;
        }
    }
}

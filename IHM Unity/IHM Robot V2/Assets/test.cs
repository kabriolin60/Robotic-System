﻿using SimpleGraph;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class test : MonoBehaviour
{

    public Grapher Graph1;


    // Start is called before the first frame update
    void Start()
    {
        /*Graph1.FillData(new Vector2(10, 20), 0);
        Graph1.FillData(new Vector2(20, 20), 0);
        Graph1.FillData(new Vector2(40, 40), 0);

        Graph1.FillData(new Vector2(10, 90), 2);
        Graph1.FillData(new Vector2(20, 40), 2);
        Graph1.FillData(new Vector2(40, 20), 2);*/

        //StartCoroutine("add_datas");

        //Graph1.RemoveData(1, 2);
        //Graph1.RemoveData(0, 2);
    }

    int i = 0;

    // Update is called once per frame
    void Update()
    {
        if (i < 5000)
        {
            Graph1.Add_Data(i + 10, 0);
            Graph1.Add_Data(i + 11, 1);
            Graph1.Add_Data(i + 12, 2);
            Graph1.Add_Data(i + 13, 3);
            Graph1.Add_Data(i + 14, 4);
            Graph1.Add_Data(i + 15, 5);
            i++;
        }
    }

    IEnumerator add_datas()
    {
        yield return new WaitForSeconds(5);

        for (int i = 0; i < 200; i++)
        {
            Graph1.Add_Data(i + 10, 0);
            Graph1.Add_Data(i + 20, 5);
            yield return new WaitForSeconds(0.5F);
        }


        yield return null;
    }
}

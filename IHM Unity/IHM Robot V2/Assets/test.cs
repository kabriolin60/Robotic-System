using SimpleGraph;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class test : MonoBehaviour
{

    public Grapher Graph1;


    // Start is called before the first frame update
    void Start()
    {
        Graph1.FillData(new Vector2(10, 20), 0);
        Graph1.FillData(new Vector2(20, 20), 0);
        Graph1.FillData(new Vector2(40, 40), 0);

        Graph1.FillData(new Vector2(10, 90), 1);
        Graph1.FillData(new Vector2(20, 40), 1);
        Graph1.FillData(new Vector2(40, 20), 1);

        Graph1.RemoveData(1, 2);
        Graph1.RemoveData(0, 2);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

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
        StartCoroutine("add_datas");
    }

    

    // Update is called once per frame
    private void FixedUpdate()
    {
        
    }

    IEnumerator add_datas()
    {
        yield return new WaitForSeconds(5);

        int i = 0;

        while (i < 250)
        {
            Graph1.Add_Data(i + 10, 0);
            Graph1.Add_Data(i + 11, 1);
            Graph1.Add_Data(i + 12, 2);
            Graph1.Add_Data(i + 13, 3);
            Graph1.Add_Data(i + 14, 4);
            Graph1.Add_Data(i + 15, 5);
            i++;
            yield return new WaitForSeconds(0.1F);
        }

        yield return new WaitForSeconds(2);

        Graph1.Clear();

        yield return new WaitForSeconds(2);

        i = 0;
        while (i < 250)
        {
            Graph1.Add_Data(i + 10, 0);
            Graph1.Add_Data(i + 11, 1);
            Graph1.Add_Data(i + 12, 2);
            Graph1.Add_Data(i + 13, 3);
            Graph1.Add_Data(i + 14, 4);
            Graph1.Add_Data(i + 15, 5);
            i++;
            yield return new WaitForSeconds(0.1F);

        }
        
        yield return null;
    }
}

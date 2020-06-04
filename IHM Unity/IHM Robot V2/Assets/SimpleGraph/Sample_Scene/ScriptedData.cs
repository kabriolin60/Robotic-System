using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScriptedData : MonoBehaviour {

    public SimpleGraph.Grapher graph; //yo

	void Start () {
        graph.FillData(getData(), 0);
	}

    private Vector2[] getData()
    {
        var ret = new Vector2[200];
        float step = 0.03f;

        var vel_time = new Vector2();

        ret[0] = vel_time;
        for (int i = 1; i < ret.Length; i++)
        {
            vel_time.y *= (1 - step);
            vel_time.x += step;
            vel_time.y += (step * 9.8f);

            ret[i] = vel_time;
        }

        return ret;
    }
}

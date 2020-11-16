using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Interprete_Message : MonoBehaviour
{
    public Trame_Decoder[] Decodeurs;


    // Start is called before the first frame update
    void Start()
    {
        Decodeurs = this.GetComponentsInChildren<Trame_Decoder>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Communication_Send_Instructions : MonoBehaviour
{
    public void Send_Instruction(Communication.Communication_Trame trame)
    {
        this.GetComponent<Communication_TX_RX_Trames>().Send_trame(trame);
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AX12_maj : MonoBehaviour
{
    public GameObject AX12_0_pos;
    public GameObject AX12_1_pos;
    public GameObject AX12_2_pos;
    public GameObject AX12_3_pos;

    public GameObject AX12_0_tor;
    public GameObject AX12_1_tor;
    public GameObject AX12_2_tor;
    public GameObject AX12_3_tor;

    public void Maj_AX12(Infos_Carte.Com_Position_AX12 values)
    {
        this.AX12_0_pos.GetComponent<Slider>().value = values.Position[0];
        this.AX12_1_pos.GetComponent<Slider>().value = values.Position[1];
        this.AX12_2_pos.GetComponent<Slider>().value = values.Position[2];
        this.AX12_3_pos.GetComponent<Slider>().value = values.Position[3];

        this.AX12_0_tor.GetComponent<Slider>().value = values.Torque[0];
        this.AX12_1_tor.GetComponent<Slider>().value = values.Torque[1];
        this.AX12_2_tor.GetComponent<Slider>().value = values.Torque[2];
        this.AX12_3_tor.GetComponent<Slider>().value = values.Torque[3];
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MesuresDigitales_maj : MonoBehaviour
{
    public GameObject Motor_Power;
    public GameObject Aux_Motor_Power;
    public GameObject Servo_Power;
    public GameObject AX12_Power;
    public GameObject AUX_1_Power;
    public GameObject AUX_2_Power;

    public GameObject FDC_0;
    public GameObject FDC_1;
    public GameObject CTC_0;
    public GameObject CTC_1;
    public GameObject CTC_2;
    public GameObject CTC_3;

    public void Maj_Mesures_Digitales(byte values)
    {
        //0= motor power; 1 = motor aux power; 2 = servos power; 3 = ax12 power; 4 = AUX 1 power; 5 = Aux 2 power
        this.Motor_Power.GetComponent<Toggle>().isOn = ((values & 0b00000001) > 0 ? true : false);
        this.Aux_Motor_Power.GetComponent<Toggle>().isOn = ((values & 0b00000010) > 0 ? true : false);
        this.Servo_Power.GetComponent<Toggle>().isOn = ((values & 0b00000100) > 0 ? true : false);
        this.AX12_Power.GetComponent<Toggle>().isOn = ((values & 0b00001000) > 0 ? true : false);
        this.AUX_1_Power.GetComponent<Toggle>().isOn = ((values & 0b00010000) > 0 ? true : false);
        this.AUX_2_Power.GetComponent<Toggle>().isOn = ((values & 0b00100000) > 0 ? true : false);
    }

    public void Maj_Contacteurs(byte values)
    {
        //0= FDC 0; 1 = FDC 1; 2 = CTC 0; 3 = CTC 1; 4 = CTC 2; 5 = CTC 3
        this.FDC_0.GetComponent<Toggle>().isOn = ((values & 0b00000001) > 0 ? true : false);
        this.FDC_1.GetComponent<Toggle>().isOn = ((values & 0b00000010) > 0 ? true : false);
        this.CTC_0.GetComponent<Toggle>().isOn = ((values & 0b00000100) > 0 ? true : false);
        this.CTC_1.GetComponent<Toggle>().isOn = ((values & 0b00001000) > 0 ? true : false);
        this.CTC_2.GetComponent<Toggle>().isOn = ((values & 0b00010000) > 0 ? true : false);
        this.CTC_3.GetComponent<Toggle>().isOn = ((values & 0b00100000) > 0 ? true : false);
    }
}

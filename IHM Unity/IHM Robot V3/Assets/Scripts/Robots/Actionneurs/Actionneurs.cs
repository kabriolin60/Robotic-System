using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Actionneurs : MonoBehaviour
{
    public Infos_Carte.Com_Position_Robot_Identification Robot;
    public int Actionneur_ID;

    public enum Activate_Action
    {
        None,
        On_Mini_value,
        On_Maxi_Value
    }; 
    
    public Activate_Action Active_Action;
    
    public float valeur_mini;
    public float position_mini;

    public float valeur_maxi;
    public float position_maxi;

    public float Current_Value;
    public float Current_Position;

    private Articulation articulation;

    private void Awake()
    {
        articulation = this.GetComponent<Articulation>();
    }


    // Update is called once per frame
    void FixedUpdate()
    {
        Infos_Carte.Com_Reponse_Info last_position = Last_Infos.Robots_Last_Info[(int)Robot].Get_Last_Infos(Actionneur_ID/10 - 1);

        try
        {
            Current_Value = last_position.Position_Servos.Position[Actionneur_ID % 10];

            if (Active_Action == Activate_Action.None)
            {
                articulation.Valeur = Position(Current_Value);
                Current_Position = articulation.Valeur;
            }
        }
        catch
        {

        }

        try
        {
            if(Active_Action == Activate_Action.On_Mini_value)
            {
                if (Current_Value == valeur_mini)
                {
                    this.GetComponent<Pince>().Activate_Actionneur(true);
                }else
                {
                    this.GetComponent<Pince>().Activate_Actionneur(false);
                }
            }

            if (Active_Action == Activate_Action.On_Maxi_Value)
            {
                if (Current_Value == valeur_maxi)
                {
                    this.GetComponent<Pince>().Activate_Actionneur(true);
                }
                else
                {
                    this.GetComponent<Pince>().Activate_Actionneur(false);
                }
            }
        }
        catch
        {

        }
    }

    public float Position(float position)
    {
        float new_position = 0;

        float a = ((float)(position_maxi - position_mini)) / ((float)(valeur_maxi - valeur_mini));

        new_position = (position - valeur_mini) * a + position_mini;

        /*if (new_position < position_mini)
            new_position = position_mini;

        if (new_position > position_maxi)
            new_position = position_maxi;*/

        return new_position;
    }
}

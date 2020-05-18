using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AX12 : MonoBehaviour
{
    public GameObject AX12_Prefab;

    List<AX12_elementaire> Ax12 = new List<AX12_elementaire>();


    // Start is called before the first frame update
    void Start()
    {
        for (int i = 0; i < Structures_Echange.Infos_Carte.NB_AX_12 / 2; i++)
        {
            Ax12.Add(new AX12_elementaire());
            Ax12[i].UI_Object = Instantiate(AX12_Prefab, new Vector3(-80+160*i, 90, 0), Quaternion.identity);
            Ax12[i].Init(i);
            Ax12[i].UI_Object.transform.SetParent(this.transform, false);

        }

        for (int i = Structures_Echange.Infos_Carte.NB_AX_12 / 2; i < Structures_Echange.Infos_Carte.NB_AX_12; i++)
        {
            Ax12.Add(new AX12_elementaire());
            Ax12[i].UI_Object = Instantiate(AX12_Prefab, new Vector3(-80+160*(i-2), 90-210, 0), Quaternion.identity);
            Ax12[i].Init(i);
            Ax12[i].UI_Object.transform.SetParent(this.transform, false);

        }
    }


    public void Update_AX12_Position(Structures_Echange.Infos_Carte.Com_Position_AX12 positions)
    {
        for (int i = 0; i < Structures_Echange.Infos_Carte.NB_AX_12; i++)
        {
            Ax12[i].Update_Position_Torque(positions.Position[i], positions.Torque[i]);
        }
    }


    public int GetAX12Position(int idAX12)
    {
        try
        {
            return Ax12[idAX12].Get_AX12_Position();
        }catch
        {
            return 0;
        }
    }

}

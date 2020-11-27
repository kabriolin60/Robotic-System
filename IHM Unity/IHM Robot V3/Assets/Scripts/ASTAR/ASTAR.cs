using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using TMPro;
using UnityEngine;

public class ASTAR : MonoBehaviour
{
    public enum Astar_Node_Status : byte
    {
        Walkable,
        Limited,
        Blocked,
        Trajectory,
        Start_Node,
        End_Node
    }


    public GameObject Astar_Panel_Holder;
    public GameObject Node_Prefab;

    static public int Field_Size_X = 2000;
    static public int Field_Size_Y = 3000;
    static public int Node_Size = 50;

    static private Astar_Node[,] Nodes;

    // Start is called before the first frame update
    void Start()
    {
        //Create All Nodes
        Nodes = new Astar_Node[Field_Size_X / Node_Size, Field_Size_Y / Node_Size];

        //Instantiate
        for(int x = 0; x < Field_Size_X / Node_Size; x++)
        {
            for(int y = 0; y< Field_Size_Y / Node_Size; y++)
            {
                Nodes[x, y] = (Astar_Node)ScriptableObject.CreateInstance(typeof(Astar_Node));
                Nodes[x, y].go = Instantiate(Node_Prefab, Astar_Panel_Holder.transform);
                Nodes[x, y].Set_Status(Astar_Node_Status.Walkable);
            }
        }
    }


    static public void Reception_Message_Astar(Communication.Communication_Trame trame)
    {
        ASTAR_COMMUNICATION comm = new ASTAR_COMMUNICATION();
        //Commence par transformer la trame en un message utilisable
        ASTAR_COMMUNICATION.Nodes_Values data = comm.Trame_To_Data(trame);

        //Puis met à jour l'affichage
        try
        {
            //Un message contient 60 valeurs, donc le status de 120 noeuds, soit 2 lignes
            //1ère ligne
            for (int noeud = 0; noeud < Field_Size_Y / Node_Size; noeud++)
            {
                Nodes[data.line_id, noeud].Set_Status(data.nodes_values[noeud]);
            }

            //2ème ligne
            for (int noeud = 0; noeud < Field_Size_Y / Node_Size; noeud++)
            {
                Nodes[data.line_id + 1, noeud].Set_Status(data.nodes_values[noeud + Field_Size_Y / Node_Size]);
            }
        }
        catch
        {

        }
    }



    public class Astar_Node : ScriptableObject
    {
        Astar_Node_Status Status = Astar_Node_Status.Walkable;

        public GameObject go;
        private TextMeshProUGUI Text;

        public void Set_Status(Astar_Node_Status new_status)
        {
            if(this.Text == null)
            {
                Text = go.GetComponent<TextMeshProUGUI>();
            }
            this.Status = new_status;

            switch(this.Status)
            {
                case Astar_Node_Status.Walkable:
                    Text.text = " ";
                    break;

                case Astar_Node_Status.Limited:
                    Text.text = "x";
                    break;

                case Astar_Node_Status.Blocked:
                    Text.text = "X";
                    break;

                case Astar_Node_Status.Trajectory:
                    Text.text = "#";
                    break;

                case Astar_Node_Status.Start_Node:
                    Text.text = "S";
                    break;

                case Astar_Node_Status.End_Node:
                    Text.text = "E";
                    break;

                default:
                    Text.text = " ";
                    break;
            }
        }
    }
}



public class ASTAR_COMMUNICATION
{
    private const byte NB_Noeuds_par_mess = 120;  //a raison de 2 noeuds par messages => 120 donne 60 octets par message

    //Un octet contient les valeurs de 2 noeuds

    //Ensemble d'octets de noeuds reçus dans un message
    //Max 30 octets = 60 noeuds
    [StructLayout(LayoutKind.Sequential)]
    private class Nodes_Values_array
    {
        [MarshalAs(UnmanagedType.I2)]
        public short Line_ID;                   //Identifiant de la ligne sur laquelle les données doivent être affichées

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = NB_Noeuds_par_mess/2)]
        public byte[] nodes_values;
    }


    //Classe contenant les données APRES decodées
    public class Nodes_Values
    {
        public short line_id;
        public ASTAR.Astar_Node_Status[] nodes_values;
    }

    public Nodes_Values Trame_To_Data(Communication.Communication_Trame input_trame)
    {
        Nodes_Values_array output_temp = new Nodes_Values_array();

        output_temp.nodes_values = new byte[NB_Noeuds_par_mess / 2];

        output_temp = (Nodes_Values_array)Communication.GetStructFromArray<Nodes_Values_array>(input_trame.Data);

        Nodes_Values output = new Nodes_Values();
        output.line_id = output_temp.Line_ID;
        output.nodes_values = new ASTAR.Astar_Node_Status[NB_Noeuds_par_mess];

        for(int index = 0; index < NB_Noeuds_par_mess / 2; index++)
        {
            output.nodes_values[index * 2] = (ASTAR.Astar_Node_Status)((output_temp.nodes_values[index] & 0b00001111));
            output.nodes_values[index * 2 + 1] = (ASTAR.Astar_Node_Status)((output_temp.nodes_values[index] & 0b11110000) >> 4);
        }

        return output;
    }
}

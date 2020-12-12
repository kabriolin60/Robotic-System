using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using TMPro;

public class ASTAR_VECT : MonoBehaviour
{
    static private GameObject this_go;
    public GameObject Astar_Vector_Prefab;
    static private GameObject static_Astar_Vector_Prefab;

    static private List<GameObject> Vector_List = new List<GameObject>();

    private void Start()
    {
        this_go = this.gameObject;
        static_Astar_Vector_Prefab = Astar_Vector_Prefab;
    }


    static public void Reception_Message_Astar_Vector(Communication.Communication_Trame trame)
    {
        ASTAR_VECTOR_COMMUNICATION comm = new ASTAR_VECTOR_COMMUNICATION();
        //Commence par transformer la trame en un message utilisable
        ASTAR_VECTOR_COMMUNICATION.Vectors_Values data = comm.Trame_To_Data(trame);

        //Puis met à jour l'affichage
        try
        {
            //Si un effacement est demandé
            if(data.Effacement == 1)
            {
                Clear_Vectors();
            }


            //Un message contient 8 vecteurs maximum
            foreach(ASTAR_VECTOR_COMMUNICATION.ASTAR_Vector vect in data.vectors)
            {
                Create_New_Vector(vect.Color, vect.Start, vect.End);
            }
        }
        catch
        {

        }
    }

    static private void Clear_Vectors()
    {
        foreach(GameObject vect in Vector_List)
        {
            DestroyImmediate(vect);
        }
        Vector_List.Clear();
    }

    static private void Create_New_Vector(ASTAR_VECTOR_COMMUNICATION.Astar_Vector_Color Color, Vector2 Start, Vector2 End)
    {
        UnityEngine.Color color = new Color();
        
        switch (Color)
        {
            default:
                color = UnityEngine.Color.black;
                break;

            case ASTAR_VECTOR_COMMUNICATION.Astar_Vector_Color.Black:
                color = UnityEngine.Color.black;
                break;

            case ASTAR_VECTOR_COMMUNICATION.Astar_Vector_Color.Blue:
                color = UnityEngine.Color.blue;
                break;

            case ASTAR_VECTOR_COMMUNICATION.Astar_Vector_Color.Green:
                color = UnityEngine.Color.green;
                break;

            case ASTAR_VECTOR_COMMUNICATION.Astar_Vector_Color.Red:
                color = UnityEngine.Color.red;
                break;

            case ASTAR_VECTOR_COMMUNICATION.Astar_Vector_Color.White:
                color = UnityEngine.Color.white;
                break;
        }


        Vector_List.Add(Instantiate(static_Astar_Vector_Prefab, this_go.transform));
        Vector_List[Vector_List.Count - 1].GetComponent<LineRenderer>().positionCount = 2;

        Vector3 start = new Vector3();
        start.x = Start.y / 1000;
        start.y = -Start.x / 1000;
        start.z = -0.05f;
        Vector_List[Vector_List.Count - 1].GetComponent<LineRenderer>().SetPosition(0, start);
        Vector_List[Vector_List.Count - 1].GetComponent<LineRenderer>().startColor = color;

        Vector3 end = new Vector3();
        end.x = End.y / 1000;
        end.y = -End.x / 1000;
        end.z = -0.05f;        
        Vector_List[Vector_List.Count - 1].GetComponent<LineRenderer>().SetPosition(1, end);
        Vector_List[Vector_List.Count - 1].GetComponent<LineRenderer>().endColor = color;
    }
}


public class ASTAR_VECTOR_COMMUNICATION
{
    private const byte NB_Vector_par_mess = 6;

    public enum Astar_Vector_Color : byte
    {
        Black,
        Blue,
        Green,
        Red,
        White
    }


    //Ensemble des vecteurs contenus dans un message
    [StructLayout(LayoutKind.Sequential)]
    private class Vectors_Values_array
    {
        [MarshalAs(UnmanagedType.I1)]
        public byte Effacement;                 //1 octet pour l'effacement 

        [MarshalAs(UnmanagedType.I1)]
        public byte Nb_vecteurs;                //Nombre de vecteurs dans le message

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = NB_Vector_par_mess * 10)]
        public byte[] vectors_values;           //8 octets par vecteurs
    }


    //Classe contenant les données d'un vecteur décodées
    public class ASTAR_Vector
    {
        public Astar_Vector_Color Color;
        public Vector3 Start;
        public Vector3 End;
    }


    //Classe contenant les données APRES décodées
    public class Vectors_Values
    {
        public byte Effacement;
        public byte Nb_vecteurs;
        public ASTAR_Vector[] vectors;
    }


    public Vectors_Values Trame_To_Data(Communication.Communication_Trame input_trame)
    {
        Vectors_Values_array output_temp = new Vectors_Values_array();

        output_temp.vectors_values = new byte[NB_Vector_par_mess * 8];

        output_temp = (Vectors_Values_array)Communication.GetClassFromArray<Vectors_Values_array>(input_trame.Data);

        Vectors_Values output = new Vectors_Values();
        output.Effacement = (byte)(output_temp.Effacement);
        output.Nb_vecteurs = (byte)(output_temp.Nb_vecteurs);

        output.vectors = new ASTAR_Vector[output.Nb_vecteurs];


        for (byte vecteur_index = 0; vecteur_index < output.Nb_vecteurs; vecteur_index++)
        {
            byte[] data = new byte[10];
            for (int i = 0; i < 10; i++)
            {
                data[i] = output_temp.vectors_values[i + 10 * vecteur_index];
            }

            output.vectors[vecteur_index] = new ASTAR_Vector();
            //Pour chaque vecteur reçu            

            //1 octet = Couleur
            output.vectors[vecteur_index].Color = (Astar_Vector_Color)data[0];

            //1 octet perdu d'alignement

            //4 octets pour Start X et Start Y
            //Start X
            int value = data[3];
            value = value << 8;
            value += data[2];

            output.vectors[vecteur_index].Start.x = (float)(value);

            //start Y
            value = data[5];
            value = value << 8;
            value += data[4];
            output.vectors[vecteur_index].Start.y = (float)(value);


            //4 octets pour End X et End Y
            //End X
            value = data[7];
            value = value << 8;
            value += data[6];

            output.vectors[vecteur_index].End.x = (float)(value);

            //End Y
            value = data[9];
            value = value << 8;
            value += data[8];
            output.vectors[vecteur_index].End.y = (float)(value);
        }

        return output;
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Robot_Position : MonoBehaviour
{
    public Vector3 Real_Position;
    public float Orientation = 10;

    Trajectory_Traceur traceur;

    public Text Robot_Position_X;
    public Text Robot_Position_Y;
    public Text Robot_Angle;

    public Text Robot_Big_Position_X;
    public Text Robot_Big_Position_Y;
    public Text Robot_Big_Angle;


    // Start is called before the first frame update
    void Start()
    {
        traceur = GetComponent<Trajectory_Traceur>();
    }

    // Update is called once per frame
    void Update()
    {
        //Mise à jour de la position du Robot pour l'affichage de l'object 3D
        Vector3 newposition = new Vector3();
        newposition.x = Real_Position.x;
        newposition.y = 0;
        newposition.z = Real_Position.y; //inversion des axes Y et Z

        transform.position = Vector3.Lerp(transform.position, newposition, 0.2f);
        //transform.position = newposition;

        Robot_Big_Position_X.text = Robot_Position_X.text = "X = " + (Real_Position.x * 1000).ToString() + " mm";
        Robot_Big_Position_Y.text = Robot_Position_Y.text = "Y = " + (Real_Position.y * 1000).ToString() + " mm";
        Robot_Big_Angle.text = Robot_Angle.text = "Ang = " + (Orientation).ToString() + " °";


        //Mise à jour de l'orientation du Robot pour affichage de l'object 3D 
        transform.Rotate(0.0f, 0.0f, Mathf.LerpAngle(transform.rotation.eulerAngles.z, transform.rotation.eulerAngles.y + Orientation, 0.2f), Space.Self);
        //transform.Rotate(0.0f, 0.0f, transform.rotation.eulerAngles.y + Orientation, Space.Self);

        traceur.Add_Trajectory_Position(newposition, newposition, Color.red);

    }
}

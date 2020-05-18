using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Actionneurs_2020_PR : MonoBehaviour
{
    public GameObject Conteneur_des_Cartes;

    public GameObject Object_Trappe_Avant;

    public GameObject Ventouse_Droite, Ventouse_Gauche;

    Trappe_Avant trappe_Avant = new Trappe_Avant();

    private void Start()
    {
        
    }

    private int Get_AX_12_Position(Structures_Echange.Com_Position_Robot.Com_Position_Robot_Identification idRobot, int idCarte, int idAx12)
    {
        return Conteneur_des_Cartes.GetComponent<Cartes_MultiFct_Panel>().Get_AX12_Position(idRobot, idCarte, idAx12);
    }


    private void Update()
    {
        float position_trappe = trappe_Avant.Position_Trappe_Avant(Get_AX_12_Position(trappe_Avant.idRobot, trappe_Avant.idCarte, trappe_Avant.idAx12));

        if(position_trappe < 40)
        {
            Ventouse_Droite.GetComponent<Ventouse>().Start_Ventousage();
            Ventouse_Gauche.GetComponent<Ventouse>().Start_Ventousage();
        }
        else
        {
            Ventouse_Droite.GetComponent<Ventouse>().Stop_Ventousage();
            Ventouse_Gauche.GetComponent<Ventouse>().Stop_Ventousage();
        }
        
        this.Object_Trappe_Avant.transform.localPosition = new Vector3(this.Object_Trappe_Avant.transform.localPosition.x, Mathf.Lerp(this.Object_Trappe_Avant.transform.localPosition.y, position_trappe, 0.2f), this.Object_Trappe_Avant.transform.localPosition.z);
    }

    public class Trappe_Avant
    {
        public Structures_Echange.Com_Position_Robot.Com_Position_Robot_Identification idRobot = Structures_Echange.Com_Position_Robot.Com_Position_Robot_Identification.Petit_Robot;
        public int idCarte = 1;
        public int idAx12 = 1;

        private int Position_Trappe_Bas = 20;
        private int Position_Trappe_Haut = 150;

        private short Servo_Trappe_Bas = 100;
        private short Servo_Trappe_Haut = 500;


        public float Position_Trappe_Avant(int position)
        {
            float new_position = 0;

            float a = ((float)(Position_Trappe_Haut - Position_Trappe_Bas)) / ((float)(Servo_Trappe_Haut - Servo_Trappe_Bas));

            new_position = (position - Servo_Trappe_Bas) * a + Position_Trappe_Bas;

            if (new_position < Position_Trappe_Bas)
                new_position = Position_Trappe_Bas;

            if (new_position > Position_Trappe_Haut)
                new_position = Position_Trappe_Haut;

            return new_position;
            //Debug.Log("Position trappe= " + new_position.ToString());
        }
    }

    
}

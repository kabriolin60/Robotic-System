using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ventouse : MonoBehaviour
{
    public LayerMask layermask;

    public float Longueur_Action = 0.01f;
    private bool Deja_Ventouse = false;
    private bool Ventousage_Actif = true;

    FixedJoint Joint;


    public void Start_Ventousage()
    {
        Ventousage_Actif = true;
    }

    public void Stop_Ventousage()
    {
        if(Ventousage_Actif)
            Action_Deventousage();        
    }

    private void Action_Ventousage(GameObject hittedObj)
    {
        Deja_Ventouse = true;

        if (Joint == null)
        {
            Joint = this.gameObject.AddComponent<FixedJoint>();
            Joint.connectedBody = hittedObj.GetComponent<Rigidbody>();
        }
    }

    private void Action_Deventousage()
    {
        Ventousage_Actif = false;
        Deja_Ventouse = false;
        
        if(Joint != null)
            Destroy(Joint);
    }

    // Update is called once per frame
    void Update()
    {
        if (!Deja_Ventouse && Ventousage_Actif)
        {
            Debug.DrawRay(transform.position, this.transform.localToWorldMatrix * Vector3.down * Longueur_Action / this.transform.localScale.y * 1000);

            RaycastHit hit;
            Ray ray = new Ray(transform.position, this.transform.localToWorldMatrix * Vector3.down * Longueur_Action / this.transform.localScale.y*1000);

            if (Physics.Raycast(ray, out hit, Longueur_Action, layermask))
            {
                print(hit.transform.name);

                Action_Ventousage(hit.transform.gameObject);
            }
        }
    }
}

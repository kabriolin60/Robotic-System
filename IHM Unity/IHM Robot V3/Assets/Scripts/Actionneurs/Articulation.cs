using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Articulation : MonoBehaviour
{
    public enum Type_Joint
    {
        Free,
        Lineaire,
        Rotation
    }

    public enum Repere_Joint
    {
        Local,
        Parent,
        World
    }

    public Type_Joint type_Joint;
    public Repere_Joint repere_Joint;
    public Transform Connected_Body;

    public Vector3 Anchor;
    public Vector3 Axis;

    public bool use_automatic_parent_anchor = true;
    public Vector3 Connected_Anchor;


    private HingeJoint hinge_joint;

    void OnDrawGizmosSelected()
    {
        Vector3 direction = new Vector3();
        Ray joint_ray;
        switch (type_Joint)
        {
            case Type_Joint.Free:
                break;

            case Type_Joint.Lineaire:            
                Gizmos.color = Color.green;               
                direction = Connected_Body.TransformVector(Axis);

                joint_ray = new Ray(this.transform.TransformPoint(Anchor), direction*3.0f);
                Gizmos.DrawRay(joint_ray);
                break;

            case Type_Joint.Rotation:
                Gizmos.color = Color.green;
                direction = this.transform.TransformVector(Axis);

                joint_ray = new Ray(this.transform.TransformPoint(Anchor), direction * 3.0f);
                Gizmos.DrawRay(joint_ray);
                break;
        }
    }


    // Start is called before the first frame update
    void Awake()
    {
        switch (type_Joint)
        {
            default:
                break;

            case Type_Joint.Rotation:
                this.hinge_joint = this.gameObject.AddComponent<HingeJoint>();
                hinge_joint.connectedBody = Connected_Body.GetComponent<Rigidbody>();
                hinge_joint.anchor = this.Anchor;
                hinge_joint.axis = this.Axis;

                if(!use_automatic_parent_anchor)
                {
                    hinge_joint.autoConfigureConnectedAnchor = false;
                    hinge_joint.connectedAnchor = this.Connected_Anchor;
                }

                this.GetComponent<Rigidbody>().useGravity = false;
                break;
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

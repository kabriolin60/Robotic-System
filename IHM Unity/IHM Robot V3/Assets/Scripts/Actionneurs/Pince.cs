using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pince : MonoBehaviour
{
    public LayerMask target_layer;
    public Vector3 Anchor;
    public Vector3 Axis;
    public float Raycast_Length;

    public bool Activate = false;

    private FixedJoint fixed_Joint;
    private Transform grabed_Object;

    private void Awake()
    {
        if (this.GetComponent<Rigidbody>() == null)
        {
            this.gameObject.AddComponent<Rigidbody>();
            this.GetComponent<Rigidbody>().isKinematic = true;
            this.GetComponent<Rigidbody>().useGravity = false;
            this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeAll;            
        }

        if(this.transform.parent.GetComponent<Rigidbody>() == null)
        {
            this.transform.parent.gameObject.AddComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
        }
    }

    void OnDrawGizmosSelected()
    {
        Vector3 direction = new Vector3();

        Gizmos.color = Color.yellow;
        direction = this.transform.TransformVector(Axis);

        Gizmos.DrawLine(this.transform.TransformPoint(Anchor), this.transform.TransformPoint(Anchor) + direction.normalized * Raycast_Length);
    }

    public void Activate_Actionneur(bool state)
    {
        Activate = state;

        if(!state)
        {
            grabed_Object = null;
            this.gameObject.GetComponent<FixedJoint>().connectedBody = null;
        }
    }
    

    RaycastHit hit;
    void FixedUpdate()
    {
        if (this.Activate && grabed_Object == null)
        {
            //Does the ray intersect any object in the range, according to layer mask
            if (Physics.Raycast(new Ray(this.transform.TransformPoint(Anchor), this.transform.TransformVector(this.Axis)), out hit, this.Raycast_Length, this.target_layer))
            {
                Vector3 hit_length = new Vector3();
                hit_length = TransformExtensions.TransformPointUnscaled(this.transform, hit.point) - this.transform.TransformPoint(Anchor);
                Debug.DrawLine(this.transform.TransformPoint(Anchor), hit.point, Color.red);

                if (grabed_Object == null)
                {
                    grabed_Object = hit.transform;
                    this.fixed_Joint = this.gameObject.AddComponent<FixedJoint>();
                    this.fixed_Joint.connectedBody = grabed_Object.GetComponent<Rigidbody>();
                }
            }
        }
    }
}


public static class TransformExtensions
{
    public static Vector3 TransformPointUnscaled(this Transform transform, Vector3 position)
    {
        var localToWorldMatrix = Matrix4x4.TRS(transform.position, transform.rotation, Vector3.one);
        return localToWorldMatrix.MultiplyPoint3x4(position);
    }

    public static Vector3 InverseTransformPointUnscaled(this Transform transform, Vector3 position)
    {
        var WorldToLocalMatrix = Matrix4x4.TRS(transform.position, transform.rotation, Vector3.one).inverse;
        return WorldToLocalMatrix.MultiplyPoint3x4(position);
    }
}

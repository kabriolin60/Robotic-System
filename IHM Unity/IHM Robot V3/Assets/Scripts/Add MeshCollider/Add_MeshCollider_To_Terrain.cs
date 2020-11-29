using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Add_MeshCollider_To_Terrain : MonoBehaviour
{
    // Start is called before the first frame update
    [System.Obsolete]
    IEnumerator Start()
    {
        yield return new WaitForSeconds(.01f);

        Debug.Log("Start Creating meshes");
        ForEachChildren(this.transform, this.transform);
        Debug.Log("End Creating meshes");
    }


    [System.Obsolete]
    private int ChildrenCount(Transform go)
    {
        int children;
        children = go.GetChildCount();

        return children;
    }



    [System.Obsolete]
    private void ForEachChildren(Transform go, Transform parent)
    {
        //does this go have children?
        int children;
        children = ChildrenCount(go);

        if (children == 0)
        {
            //This go doesn't have any children
            //This is the last element of hierarchy
            //Add a mesh Collider to him
            Add_MeshCollider(go);

            //This add a rigidbody to it's parrent if it doesn't already have one
            Add_RigiBody_To_Parent(parent);
        }
        else
        {
            //There are children in this tranform
            for (int index = 0; index < children; index++)
            {
                //Go down to the next level
                ForEachChildren(go.GetChild(index), go);
            }
        }
    }

    private void Add_MeshCollider(Transform go)
    {
        try
        {
            go.gameObject.AddComponent<MeshCollider>().convex = true;
        }
        catch (Exception ex)
        {

        }
    }

    private void Add_RigiBody_To_Parent(Transform go)
    {
        if (go.GetComponent<Rigidbody>() == null)
        {
            try
            {
                go.gameObject.AddComponent<Rigidbody>().isKinematic = true;
            }
            catch (Exception ex)
            {

            }
        }
    }
}

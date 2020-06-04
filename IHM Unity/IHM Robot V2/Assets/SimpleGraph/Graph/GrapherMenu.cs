#if UNITY_EDITOR
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class GrapherMenu  {

    [MenuItem("GameObject/UI/SimpleGraph", false)]
    public static void CreateTextArea(MenuCommand menuCommand)
    {
        GameObject go = (AssetDatabase.LoadAssetAtPath<GameObject>("Assets/SimpleGraph/Graph/Graph_Resizable.prefab"));
        if (go == null)
        {
            go = (AssetDatabase.LoadAssetAtPath<GameObject>("Assets/SimpleGraph/SimpleGraph/Graph/Graph_Resizable.prefab"));
            if (go == null)
            {
                var find = AssetDatabase.FindAssets("Graph_Resizable");
                if (find.Length == 1)
                {
                    go = AssetDatabase.LoadAssetAtPath<GameObject>(find[0]);
                    Debug.Log(      "The Graph_Resizable directory has changed. Please move it back to Assets/SimpleGraph/Graph/Graph_Resizable.prefab");
                }
                else
                {
                    Debug.LogError( "The Graph_Resizable directory has changed. Please move it back to Assets/SimpleGraph/Graph/Graph_Resizable.prefab");
                }
            }
            else
                Debug.Log(          "The Graph_Resizable directory has changed. Please move it back to Assets/SimpleGraph/Graph/Graph_Resizable.prefab");
        }


        if (go != null)
        {
            go = GameObject.Instantiate(go);
            go.name = "New Graph";

            // Ensure it gets reparented if this was a context click (otherwise does nothing)
            GameObjectUtility.SetParentAndAlign(go, menuCommand.context as GameObject);
            // Register the creation in the undo system
            Undo.RegisterCreatedObjectUndo(go, "Create " + go.name);
            Selection.activeObject = go;
        }
    }
}


#endif

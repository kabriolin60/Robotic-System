using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EXIT : MonoBehaviour
{
    // Start is called before the first frame update
    public void Exit_Application()
    {
        // save any game data here
#if UNITY_EDITOR
        // Application.Quit() does not work in the editor so
        // UnityEditor.EditorApplication.isPlaying need to be set to false to end the game
        UnityEditor.EditorApplication.isPlaying = false;
#else
         Application.Quit();
#endif
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class New_Small_Table_Click : MonoBehaviour
{
    public GameObject Small_Table_View;
    public GameObject Big_Table_View;


    private float firstClickTime, timeBetweenClicks;
    private bool coroutineAllowed;
    private int clickCounter;

    private void Start()
    {
        firstClickTime = 0;
        timeBetweenClicks = 0.2F;
        clickCounter = 0;
        coroutineAllowed = true;
    }


    public void Click()
    {
        if (Input.GetMouseButtonUp(0))
            clickCounter += 1;

        if (clickCounter == 1 && coroutineAllowed)
        {
            firstClickTime = Time.time;
            StartCoroutine(DoubleClickDetection());
        }
    }



    void Switch_To_Big_Table_View()
    {
        Big_Table_View.SetActive(!Big_Table_View.activeSelf);        
    }


    private IEnumerator DoubleClickDetection()
    {
        coroutineAllowed = false;
        while (Time.time < firstClickTime + timeBetweenClicks)
        {
            if (clickCounter == 2)
            {
                Switch_To_Big_Table_View();
                break;
            }
            yield return new WaitForEndOfFrame();
        }
        clickCounter = 0;
        firstClickTime = 0f;
        coroutineAllowed = true;
    }
}

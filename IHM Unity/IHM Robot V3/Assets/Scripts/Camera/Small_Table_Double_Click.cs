using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Small_Table_Double_Click : MonoBehaviour
{
    public GameObject Small_Table_View;
    public GameObject Big_Table_View;

    public Camera Table_View_Camera;

    public bool isSmall;

    public UnityEngine.UI.RawImage Small_Rendering_Image;
    public UnityEngine.UI.RawImage Big_Rendering_Image;

    private float firstClickTime, timeBetweenClicks;
    private bool coroutineAllowed;
    private int clickCounter;

    Vector3 position_camera_origine = new Vector3();
    Quaternion rotation_camera_origine = new Quaternion();


    public float SpeedH = 10.0F;
    public float SpeedV = 10.0F;
    public float Yaw = 0;
    public float Pitch = 0;

    private Vector3 Center_Rotation = new Vector3();
    public LayerMask layermask;


    // Start is called before the first frame update
    void Start()
    {
        firstClickTime = 0;
        timeBetweenClicks = 0.2F;
        clickCounter = 0;
        coroutineAllowed = true;

        position_camera_origine = Table_View_Camera.transform.position;
        rotation_camera_origine = Table_View_Camera.transform.rotation;

        Pitch = rotation_camera_origine.eulerAngles.x;
        Yaw = rotation_camera_origine.eulerAngles.y;
    }

    public void OnActivate()
    {

    }

    public void OnDesactivate()
    {
        if(isSmall)
        {
            //On passe sur la grande Image
            Table_View_Camera.GetComponent<camera_render>().Change_Rendering_Panel(Big_Rendering_Image);

            Big_Table_View.SetActive(true);
            Small_Table_View.SetActive(false);
        }
        else
        {
            //On passe sur la grande Image
            Table_View_Camera.GetComponent<camera_render>().Change_Rendering_Panel(Small_Rendering_Image);

            Big_Table_View.SetActive(false);
            Small_Table_View.SetActive(true);

            //remet la camera en ortho
            //Met la camera en mode "orthographic"
            Table_View_Camera.orthographic = true;

            Table_View_Camera.transform.position = position_camera_origine;
            Table_View_Camera.transform.rotation = rotation_camera_origine;
        }
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


    Vector3 clicObj_Coord()
    {
        Vector3 result = new Vector3();

        RaycastHit hit;
        Ray ray = Table_View_Camera.ScreenPointToRay(Input.mousePosition);

        if(Physics.Raycast(ray, out hit, 10, layermask))
        {
            result = hit.point;
        }

        return result;
    }


    public void RotateCamera()
    {
        //Debug.Log("Click Central, bouge la camera");
        //Met la camera en mode "perspective"
        Table_View_Camera.orthographic = false;

        //Rotation de la camera
        Yaw = SpeedH * Input.GetAxis("Mouse X");
        Pitch = SpeedV * Input.GetAxis("Mouse Y");

        //Table_View_Camera.transform.eulerAngles = new Vector3(Pitch, Yaw, 0.0F); 

        Table_View_Camera.transform.RotateAround(Center_Rotation, transform.TransformDirection(Vector3.up), Yaw);
        Table_View_Camera.transform.RotateAround(Center_Rotation, transform.TransformDirection(Vector3.left), Pitch);

    }


    public void MoveCamera()
    {
        //Translation de la camera
        Vector3 deplacementVector = new Vector3();

        deplacementVector.x = -Input.GetAxis("Mouse X") * 0.15F;
        deplacementVector.y = -Input.GetAxis("Mouse Y") * 0.15F;

        Table_View_Camera.transform.position += Table_View_Camera.transform.TransformVector(deplacementVector);

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonUp(1))
        {
            //Debug.Log("Click Droit, remise en position de la camera");

            Table_View_Camera.transform.position = position_camera_origine;
            Table_View_Camera.transform.rotation = rotation_camera_origine;

            Table_View_Camera.orthographic = true;
        }

        //Deplacement de la camera uniquement en grande vue
        if (!isSmall)
        {
            if(Input.GetMouseButtonDown(2))
            {
                //Get le point de pivot = endroit du clik avec la souris
                Center_Rotation = clicObj_Coord();
                //Debug.Log("Rotation Center= " + Center_Rotation.ToString());
            }

            if (Input.GetMouseButton(2))
            {
                RotateCamera();
            }

            if (Input.GetMouseButton(0))
            {
                MoveCamera();
            }

            if (Input.GetAxis("Mouse ScrollWheel") != 0)
            {
                Table_View_Camera.transform.Translate(new Vector3(0, 0, Input.GetAxis("Mouse ScrollWheel") * 3), Space.Self);
            }
        }
    }

    private IEnumerator DoubleClickDetection()
    {
        coroutineAllowed = false;
        while(Time.time < firstClickTime + timeBetweenClicks)
        {
            if(clickCounter == 2)
            {
                //Debug.Log("Double Click");
                OnDesactivate();
                break;
            }
            yield return new WaitForEndOfFrame();
        }
        clickCounter = 0;
        firstClickTime = 0f;
        coroutineAllowed = true;
    }
}

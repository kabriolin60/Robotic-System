using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class New_Big_Image_View : MonoBehaviour
{
    public GameObject Top_View_Camera;
    public GameObject Orbital_Camera;

    protected Transform _XFORM_Camera_orbitale;
    protected Transform _XFORM_Parent;
    protected Vector3 _localRotation;
    protected Vector3 _localPosition;
    protected float CameraDistance;

    public float MouseSensitivity = 4f;
    public float MouveMove_Sensibility = 2f;
    public float ScrollSensitivity = 2f;
    public float OrbitDampening = 10f;
    public float ScrollDampening = 6f;


    private void Start()
    {
        this._XFORM_Camera_orbitale = Orbital_Camera.transform;
        this._XFORM_Parent = Orbital_Camera.transform.parent;

        CameraDistance = this._XFORM_Camera_orbitale.localPosition.y;

        _localPosition = this._XFORM_Parent.localPosition;
    }


    // Update is called once per frame
    void LateUpdate()
    {
        //Change de camera entre la vue du dessus et la vue 3d
        if(Input.GetMouseButtonDown(1))
        {
            Switch_Camera();
        }

        Update_Rotation();

        Update_Scroll();

        Update_Position();
    }


    void Update_Rotation()
    {
        //Rotate the orbital camera based on mouse movement
        if (Input.GetMouseButton(2)) //Press mouse center button to rotate the camera
        {
            if (Input.GetAxis("Mouse X") != 0 || Input.GetAxis("Mouse Y") != 0)
            {
                //Mouse is moving
                _localRotation.x += Input.GetAxis("Mouse X") * MouseSensitivity;
                _localRotation.y -= Input.GetAxis("Mouse Y") * MouseSensitivity;

                //Clamp the rotation
                _localRotation.y = Mathf.Clamp(_localRotation.y, -90f, 0f);
            }
        }       

        //Actualise Camera pivot Orientation
        Quaternion QT = Quaternion.Euler(_localRotation.y, _localRotation.x, this._XFORM_Parent.rotation.eulerAngles.z);

        this._XFORM_Parent.rotation = Quaternion.Lerp(this._XFORM_Parent.rotation, QT, Time.deltaTime * OrbitDampening);
    }


    void Update_Scroll()
    {
        //Scrolling 
        if (Input.GetAxis("Mouse ScrollWheel") != 0f)
        {
            float scrollamount = Input.GetAxis("Mouse ScrollWheel") * ScrollSensitivity;

            //Adjust scroll speed according to distance to the target
            scrollamount *= (CameraDistance * 0.3f);

            CameraDistance -= scrollamount;

            //Limit the camera distance
            CameraDistance = Mathf.Clamp(CameraDistance, 0.5f, 5f);
        }

        if(this._XFORM_Camera_orbitale.localPosition.y != this.CameraDistance)
        {
            this._XFORM_Camera_orbitale.localPosition = new Vector3(0f, Mathf.Lerp(this._XFORM_Camera_orbitale.localPosition.y, this.CameraDistance, Time.deltaTime * this.ScrollDampening), 0.15f);
        }
    }

    void Update_Position()
    {
        if (Input.GetMouseButton(0)) //Press mouse left button to move the camera
        {
            if (Input.GetAxis("Mouse X") != 0 || Input.GetAxis("Mouse Y") != 0)
            {
                //Move camera
                _localPosition.x -= Input.GetAxis("Mouse X") * MouveMove_Sensibility * Mathf.Cos(Mathf.Deg2Rad*_localRotation.x) + Input.GetAxis("Mouse Y") * MouveMove_Sensibility * Mathf.Sin(Mathf.Deg2Rad * _localRotation.x);
                _localPosition.z -= Input.GetAxis("Mouse Y") * MouveMove_Sensibility * Mathf.Cos(Mathf.Deg2Rad * _localRotation.x) - Input.GetAxis("Mouse X") * MouveMove_Sensibility * Mathf.Sin(Mathf.Deg2Rad * _localRotation.x);

                //Clamp position
                _localPosition.x = Mathf.Clamp(_localPosition.x, -3f, 3f);
                _localPosition.z = Mathf.Clamp(_localPosition.z, -3f, 3f);

                this._XFORM_Parent.localPosition =  Vector3.Lerp(this._XFORM_Parent.localPosition, _localPosition, Time.deltaTime * OrbitDampening);
            }
        }
    }


    void Switch_Camera()
    {
        Top_View_Camera.SetActive(!Top_View_Camera.activeSelf);
        Orbital_Camera.SetActive(!Orbital_Camera.activeSelf);
    }
}

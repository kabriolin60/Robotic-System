using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class TableViewClick : MonoBehaviour, IPointerClickHandler
{
    public Vector3 worldPosition;

    public Camera playerCamera;
    private RectTransform _screenRectTransform;

    private void Awake()
    {
        _screenRectTransform = GetComponent<RectTransform>();
    }


    public void OnPointerClick(PointerEventData eventData)
    {
        RectTransformUtility.ScreenPointToLocalPointInRectangle(_screenRectTransform, eventData.position, null, out Vector2 localClick);
        
        Vector2 image_position = new Vector2();
        image_position.x = localClick.x + _screenRectTransform.rect.width / 2;
        image_position.y = localClick.y + _screenRectTransform.rect.height / 2;

        Vector3 Worldposition = new Vector3();
        Worldposition.x = 2000 + image_position.y / _screenRectTransform.rect.height * 2000 * -1;
        Worldposition.y = image_position.x / _screenRectTransform.rect.width * 3000;
        Worldposition.z = 0;

        Debug.Log($"localClick: {Worldposition.x}, {Worldposition.y}");


        if(GetComponent<Envoi_Commandes>() != null)
            GetComponent<Envoi_Commandes>().Envoi_Position_Adversaire(Worldposition);


        /*RectTransformUtility.ScreenPointToLocalPointInRectangle(_screenRectTransform, eventData.position, null, out Vector2 localClick);

        localClick.y = (_screenRectTransform.rect.y *-1) -(localClick.y * -1);

        Vector2 viewportClick = new Vector2(localClick.x / _screenRectTransform.rect.x, localClick.y / (_screenRectTransform.rect.y *-1));

        Ray ray = playerCamera.ViewportPointToRay(new Vector3(viewportClick.x, viewportClick.y, 0));
        if (Physics.Raycast(ray, out RaycastHit hit))
        {
            Vector3 hitpoint = new Vector3();
            hitpoint = hit.point;
            //Debug.DrawRay(playerCamera.transform.position, hitpoint, Color.green, 3.0f);

            Debug.Log($"localClick: {(hitpoint.z+100)*1000}, {hitpoint.x*1000}");
        }*/
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trajectory_Traceur : MonoBehaviour
{
    public GameObject RenderingImage;
    private LineRenderer lineRenderer;

    public class Trajectory_Vector
    {
        public Vector3 start;
        public Vector3 end;
        public Color couleur;

        public Trajectory_Vector(Vector3 start, Vector3 end, Color couleur)
        {
            this.start = start;
            this.end = end;
            this.couleur = couleur;
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        lineRenderer = RenderingImage.GetComponent<LineRenderer>();

        /*Vector3 start = new Vector3(0.25F, 0, 1.0F);
        Vector3 end = new Vector3(0.35F, 0, 1.0F);
        Color couleur = new Color();
        couleur = Color.red;

        /*Add_Trajectory_Position(start, end, couleur);

        start = new Vector3(0.4F, 0, 1.0F);
        end = new Vector3(1.4F, 0, 1.2F);
        Add_Trajectory_Position(start, end, couleur);

        start = new Vector3(1.5F, 0, 1.2F);
        end = new Vector3(1.0F, 0, 1.5F);
        couleur = Color.blue;
        Add_Trajectory_Position(start, end, couleur);*/
    }

    public void Add_Trajectory_Position(Vector3 start, Vector3 end, Color color)
    {
        start.y = RenderingImage.transform.localPosition.y;
        end.y = RenderingImage.transform.localPosition.y;

        lineRenderer.positionCount++;
        //Si c'est le premier point, ajoute le  départ
        if (lineRenderer.positionCount == 1)
        {
            lineRenderer.SetPosition(lineRenderer.positionCount-1, start);
            lineRenderer.positionCount++;
        }

        //Si la distance est trop faible, j'ajoute pas le point
        if (Vector3.Distance(lineRenderer.GetPosition(lineRenderer.positionCount - 2), end) > 0.01f)
        {
            //Dans tous les cas ajoute la fin
            lineRenderer.SetPosition(lineRenderer.positionCount - 1, end);
        }
        else
        {
            lineRenderer.positionCount -= 1;
        }
    }  

    private void Update()
    {
       
    }
}

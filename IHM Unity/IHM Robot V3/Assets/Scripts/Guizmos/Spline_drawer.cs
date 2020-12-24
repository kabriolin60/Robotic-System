using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spline_drawer : MonoBehaviour
{

    public Vector2 P0;
    public Vector2 M0;

    public Vector2 M1;
    public Vector2 P1;

	public Color color;


	public Vector2 Map_Size = new Vector2(2000, 3000);

	public float Nombre_Points = 20;

	private class Vector_double
    {
		public Vector2 Start = new Vector2();
		public Vector2 End = new Vector2();

		public Vector_double(Vector2 start, Vector2 end)
        {
			Start = start;
			End = end;
		}
	}


	List<Vector_double> Spline_Points = new List<Vector_double>();

	void OnDrawGizmosSelected()
	{
		Spline_Points.Clear();

		float P0_x, P0_y, M0_x, M0_y;
		float P1_x, P1_y, M1_x, M1_y;

		//Conversion des points de passage entre 0 et 1
		P0_x = P0.x;
		P0_x /= Map_Size.x;
		P0_y = P0.y;
		P0_y /= Map_Size.y;

		//Tangente initiale
		//Utilisation des valeurs de M0 fournies par l’IA
		M0_x = M0.x;
		M0_x /= Map_Size.x;
		M0_y = M0.y;
		M0_y /= Map_Size.y;


		//Point d’arrivée
		P1_x = P1.x;
		P1_x /= Map_Size.x;
		P1_y = P1.y;
		P1_y /= Map_Size.y;

		//Tangente d’arrivée
		M1_x = M1.x;
		M1_x /= Map_Size.x;
		M1_y = M1.y;
		M1_y /= Map_Size.y;


		float pas = Nombre_Points;
		pas = 1 / pas;


		Vector2 temp_vect_Start = new Vector2();
		Vector2 temp_vect_End = new Vector2();


		temp_vect_Start.x = P0_x * Map_Size.x;
		temp_vect_Start.y = P0_y * Map_Size.y;

		float Result_X = 0, Result_Y = 0;

		for (float t = pas; t < 1; t += pas)
		{
			Result_X = CubicSpline_Point_Processing(P0_x, M0_x, P1_x, M1_x, t);
			Result_X *= Map_Size.x;

			Result_Y = CubicSpline_Point_Processing(P0_y, M0_y, P1_y, M1_y, t);
			Result_Y *= Map_Size.y;

			temp_vect_End.x = Result_X;
			temp_vect_End.y = Result_Y;


			Spline_Points.Add(new Vector_double(temp_vect_Start, temp_vect_End));

			temp_vect_Start = new Vector2();

			temp_vect_Start = temp_vect_End;

			temp_vect_End = new Vector2();
		}


		{
			Result_X = CubicSpline_Point_Processing(P0_x, M0_x, P1_x, M1_x, 1);
			Result_X *= Map_Size.x;

			Result_Y = CubicSpline_Point_Processing(P0_y, M0_y, P1_y, M1_y, 1);
			Result_Y *= Map_Size.y;

			temp_vect_End.x = Result_X;
			temp_vect_End.y = Result_Y;


			Spline_Points.Add(new Vector_double(temp_vect_Start, temp_vect_End));
		}

		foreach(Vector_double vec in this.Spline_Points)
        {
			Vector3 start = new Vector3();
			Vector3 end = new Vector3();

			start.x = vec.Start.y;
			start.y = 0.05f;
			start.z = -vec.Start.x-100;

			end.x = vec.End.y;
			end.y = 0.05f;
			end.z = -vec.End.x-100;

			Debug.DrawLine(start, end, color);
		}

		{
			//Draw directors vectors
			Vector3 start = new Vector3();
			Vector3 end = new Vector3();

			start.x = P0.y;
			start.y = 0.05f;
			start.z = -P0.x- 100;

			end.x = M0.y;
			end.y = 0.05f;
			end.z = -M0.x - 100;

			Debug.DrawLine(start, end, Color.white);

			Gizmos.DrawIcon(start, "Light Gizmo.tiff", true);
			Gizmos.DrawIcon(end, "Light Gizmo.tiff", true);

			start = new Vector3();
			end = new Vector3();

			start.x = P1.y;
			start.y = 0.05f;
			start.z = -P1.x - 100;

			end.x = M1.y;
			end.y = 0.05f;
			end.z = -M1.x - 100;

			Debug.DrawLine(start, end, Color.white);

			Gizmos.DrawIcon(start, "Light Gizmo.tiff", true);
			Gizmos.DrawIcon(end, "Light Gizmo.tiff", true);
		}
	}

	float CubicSpline_Point_Processing(float p0, float m0, float p1, float m1, float t)
	{
		float result = 0;

		result = p0 * (1 - t) * (1 - t) * (1 - t);
		result += 3 * m0 * t * (1 - t) * (1 - t);
		result += 3 * m1 * t * t * (1 - t);
		result += p1 * t * t * t;

		return result;
	}
}

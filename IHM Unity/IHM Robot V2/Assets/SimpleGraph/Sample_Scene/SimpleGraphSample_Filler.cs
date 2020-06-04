using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SimpleGraph
{
    public class SimpleGraphSample_Filler : MonoBehaviour
    {
        public Grapher Sine;
        public Grapher Cosine;
        public Grapher Tanh;
        public Grapher Noise;

        #region gui
        void OnGUI()
        {
            GUILayout.Label("X OFFSET");
            Test_XOffset = GUILayout.HorizontalSlider(Test_XOffset, -10, 10);

            GUILayout.Label("Y AMPLITUDE");
            Test_YAmplitude = GUILayout.HorizontalSlider(Test_YAmplitude, -10, 10);

            if (GUILayout.Button("UPDATE GUI"))
                Fill();
        }
        #endregion


        #region FillData
        public List<Vector2> Sines = new List<Vector2>();
        public List<Vector2> Cosines = new List<Vector2>();
        public List<Vector2> Tanhs = new List<Vector2>();
        public List<Vector2> Noises = new List<Vector2>();



        public float XMin = -Mathf.PI * 2;
        public float XMax = Mathf.PI * 2;
        public float Stepping = 0.2f;
        [Range(-10, 10)]
        float Test_XOffset = 0;
        [Range(-10, 10)]
        float Test_YAmplitude = 1;

        public void Fill()
        {
            Sines.Clear();
            Cosines.Clear();
            Tanhs.Clear();
            Noises.Clear();


            for (float i = XMin; i < XMax; i += Stepping)
            {
                {
                    Sines.Add(new Vector2(i + Test_XOffset, Test_YAmplitude * Mathf.Sin(i)));
                    Cosines.Add(new Vector2(i + Test_XOffset, Test_YAmplitude * Mathf.Cos(i)));
                    Tanhs.Add(new Vector2(i + Test_XOffset, Test_YAmplitude * (float)System.Math.Tanh(i)));
                    Noises.Add(new Vector2(i + Test_XOffset, Test_YAmplitude * UnityEngine.Random.value));
                }
            }

            if (Sine) Sine.FillData(Sines.ToArray(), 0);
            if (Cosine) Cosine.FillData(Cosines.ToArray(), 0);
            if (Tanh) Tanh.FillData(Tanhs.ToArray(), 0);
            if (Noise) Noise.FillData(Noises.ToArray(), 0);
        }

        #endregion
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace SimpleGraph
{
    public class Grapher : MonoBehaviour
    {
        [Header("Modify this data")]
        public string TitleText = "FILL THIS";
        public string YUnitDataText = "YUNITS";
        public string XUnitDataText = "XUNITS";

        public int Resolution;
        public bool ThickLine = true;
        public Color LineColor = new Color(1, 0, 0, 0);

        public int FontSize = 9;

        [Header("Initialization Values.")]
        public Vector2[] StartingPoints = new Vector2[] { Vector2.zero, Vector2.one };
        public bool UseStartingPoints = false;

        private void Start()
        {
            if (UseStartingPoints)
            { 
                FillData(StartingPoints);
                //Init();
            }
        }

        [Header("Inner Data")]
        [SerializeField]
        private Text Title;
        [SerializeField]
        private Text SlideValueX;
        [SerializeField]
        private Text SlideValueY;
        [SerializeField]
        private Scrollbar Scroll;
        [SerializeField]
        private Text X_Data_Text;
        [SerializeField]
        private Text Y_Data_Text;
        [SerializeField]
        private Vector2[] PointsGraph;
        [SerializeField]
        private Texture2D Texture;
        [SerializeField]
        private RawImage Texture_Image;
        [SerializeField]
        private Text[] XTags;
        [SerializeField]
        private Text[] YTags;

        void Update()
        {
            {
                try
                {
                    //Slider.gameObject.SetActive(true);

                    //var xpoint = Scroll.value * 10;
                    var xpoint = (x_size * Scroll.value + x_zero);
                    var ypoint = FindYFromX(xpoint);

                    SlideValueX.text = "x: " + xpoint.ToString("0.0");
                    SlideValueY.text = "y: " + ypoint.ToString("0.00");
                }
                catch
                {
                    SlideValueX.text = "x: ";
                    SlideValueY.text = "y: ";
                }
            }
        }

        private float FindYFromX(float xpoint)
        {
            int i;
            for (i = 0; i < StartingPoints.Length - 1; i++)
            {
                if ((StartingPoints[i].x < xpoint || Mathf.Approximately(StartingPoints[i].x, xpoint)) &&
                    StartingPoints[i + 1].x > xpoint)
                {
                    break;
                }
            }

            var p = (1 / (StartingPoints[i + 1].x - StartingPoints[i].x)) * (xpoint - StartingPoints[i].x);

            return Mathf.Lerp(StartingPoints[i].y, StartingPoints[i + 1].y, p);
        }



        /// <summary>
        /// This fills and updates the Graph.
        /// </summary>
        /// <param name="points">
        /// X is the horizontal axis.
        /// Y is the vertical axis.
        /// </param>
        public void FillData(Vector2[] points)
        {
            StartingPoints = (Vector2[])points.Clone();
            Init();


            X_Data_Text.fontSize = FontSize;
            Y_Data_Text.fontSize = FontSize;
            for (int t = 0; t < XTags.Length; t++)
            {
                XTags[t].fontSize = FontSize; //styling
            }
            for (int t = 0; t < YTags.Length; t++)
            {
                YTags[t].fontSize = FontSize; //styling
            }
        }


        private float[] PossibleSteps = new float[] { //possible improvement -> could be done programatically
        0.01f, 0.02f, 0.03f, 0.05f,
        0.1f, 0.2f, 0.3f, 0.5f,
        1f, 2f, 3f, 5f,
        10f, 20f, 30f, 50f,
        100f, 200f, 300f, 500f,
        1000f, 2000f, 3000f, 5000f,
        10000f, 20000f, 30000f, 50000f,
        100000f, 200000f, 300000f, 500000f, 1000000f,
        1000000f, 2000000f, 3000000f, 5000000f, 10000000f,
        10000000f, 20000000f, 30000000f, 50000000f, 100000000f,
        100000000f, 200000000f, 300000000f, 500000000f, 1000000000f,
        1000000000f, 2000000000f, 3000000000f, 5000000000f, 10000000000f,
        10000000000f, 20000000000f, 30000000000f, 50000000000f, 100000000000f }; //upto^11 * 6


        string InterleaveWithSpace(string str)
        {
            string ret = "";
            for (int i = 0; i < str.Length - 1; i++)
            {
                ret = string.Concat(string.Concat(ret, str[i]), " ");
            }
            ret = string.Concat(ret, str[str.Length - 1]);

            return ret;
        }

        float x_zero = 0;
        float x_size = 1;
        void Init()
        {
            Y_Data_Text.text = InterleaveWithSpace(YUnitDataText);
            X_Data_Text.text = InterleaveWithSpace(XUnitDataText);

            Title.text = TitleText;

            var ymin = (StartingPoints[0].y);
            var ymax = (StartingPoints[0].y);

            var xmin = (StartingPoints[0].x);
            var xmax = (StartingPoints[0].x);
            for (int i = 1; i < StartingPoints.Length; i++)
            {
                ymin = Mathf.Min(ymin, (StartingPoints[i].y));
                ymax = Mathf.Max(ymax, (StartingPoints[i].y));

                xmin = Mathf.Min(xmin, (StartingPoints[i].x));
                xmax = Mathf.Max(xmax, (StartingPoints[i].x));
            }

            float y_zero = 0;
            float y_size = 1;

            /*float */
            x_zero = 0;
            /*float */
            x_size = 1;

            #region copyy
            for (int i = 0; i < PossibleSteps.Length; i++)
            {
                var floor = Mathf.FloorToInt(ymin / PossibleSteps[i]);
                var ceil = Mathf.CeilToInt(ymax / PossibleSteps[i]);
                int count = ceil - floor;

                y_zero = floor * PossibleSteps[i];
                y_size = PossibleSteps[i] * 6;

                if (count < YTags.Length)
                {
                    float cur = floor * PossibleSteps[i];
                    for (int t = 0; t < YTags.Length; t++)
                    {
                        if (PossibleSteps[i] < 0.09)
                            YTags[t].text = cur.ToString("0.00");
                        else if (PossibleSteps[i] < 0.9)
                            YTags[t].text = cur.ToString("0.0");
                        else
                            YTags[t].text = cur.ToString("0");

                        cur += PossibleSteps[i];
                    }

                    break;
                }
            }
            var YGraphSize = y_size;
            #endregion

            #region copyx
            for (int i = 0; i < PossibleSteps.Length; i++)
            {
                var floor = Mathf.FloorToInt(xmin / PossibleSteps[i]);
                var ceil = Mathf.CeilToInt(xmax / PossibleSteps[i]);
                int count = ceil - floor;

                x_zero = floor * PossibleSteps[i];
                x_size = PossibleSteps[i] * 6;

                if (count < XTags.Length)
                {
                    float cur = floor * PossibleSteps[i];
                    for (int t = 0; t < XTags.Length; t++)
                    {
                        if (PossibleSteps[i] < 0.09)
                            XTags[t].text = cur.ToString("0.00");
                        else if (PossibleSteps[i] < 0.9)
                            XTags[t].text = cur.ToString("0.0");
                        else
                            XTags[t].text = cur.ToString("0");

                        cur += PossibleSteps[i];
                    }

                    break;
                }
            }
            var XGraphSize = x_size;
            #endregion

            PointsGraph = new Vector2[StartingPoints.Length];
            for (int i = 0; i < StartingPoints.Length; i++)
            {
                //PointsGraph[i].x = ((Points[i].x * (Resolution / XGraphSize)) /** Resolution*/); //fixed X starts from 0, not working after trnasformation to dynamic
                PointsGraph[i].x = ((StartingPoints[i].x - x_zero) * (Resolution / XGraphSize));
                PointsGraph[i].y = ((StartingPoints[i].y - y_zero) * (Resolution / YGraphSize));
            }

            ResetBitmap();
            for (int i = 0; i < StartingPoints.Length - 1; i++)
            {
                DrawLine(PointsGraph[i], PointsGraph[i + 1]);

                if (ThickLine)
                {
                    DrawLine(PointsGraph[i] + new Vector2(1, 0), PointsGraph[i + 1] + new Vector2(1, 0));
                    DrawLine(PointsGraph[i] + new Vector2(-1, 0), PointsGraph[i + 1] + new Vector2(-1, 0));
                    DrawLine(PointsGraph[i] + new Vector2(0, 1), PointsGraph[i + 1] + new Vector2(0, 1));
                    DrawLine(PointsGraph[i] + new Vector2(0, -1), PointsGraph[i + 1] + new Vector2(0, -1));

                    DrawLine(PointsGraph[i] + new Vector2(-1, -1), PointsGraph[i + 1] + new Vector2(-1, -1));
                    DrawLine(PointsGraph[i] + new Vector2(-1, 1), PointsGraph[i + 1] + new Vector2(-1, 1));
                    DrawLine(PointsGraph[i] + new Vector2(1, 1), PointsGraph[i + 1] + new Vector2(1, 1));
                    DrawLine(PointsGraph[i] + new Vector2(1, -1), PointsGraph[i + 1] + new Vector2(1, -1));
                }
            }
            FinishBitmap();
        }

        void ResetBitmap()
        {
            if (Texture)
                Destroy(Texture);

            Texture = new Texture2D(Resolution, Resolution, TextureFormat.ARGB32, true, true);

            var col = LineColor;
            LineColor.a = 0;
            for (int x = 0; x < Resolution; x++)
            {
                for (int y = 0; y < Resolution; y++)
                {
                    Texture.SetPixel(x, y, col);
                }
            }
        }
        #region helper

        void DrawLine(Vector2 from, Vector2 to)
        {
            float x0 = from.x, y0 = from.y,
                        x1 = to.x, y1 = to.y;

            bool steep = Mathf.Abs(y1 - y0) > Mathf.Abs(x1 - x0);
            float temp;
            if (steep)
            {
                temp = x0; x0 = y0; y0 = temp;
                temp = x1; x1 = y1; y1 = temp;
            }
            if (x0 > x1)
            {
                temp = x0; x0 = x1; x1 = temp;
                temp = y0; y0 = y1; y1 = temp;
            }

            float dx = x1 - x0;
            float dy = y1 - y0;
            float gradient = dy / dx;

            float xEnd = _round(x0);
            float yEnd = y0 + gradient * (xEnd - x0);
            float xGap = _rfpart(x0 + 0.5f);
            float xPixel1 = xEnd;
            float yPixel1 = _ipart(yEnd);

            if (steep)
            {
                _plot(yPixel1, xPixel1, _rfpart(yEnd) * xGap);
                _plot(yPixel1 + 1, xPixel1, _fpart(yEnd) * xGap);
            }
            else
            {
                _plot(xPixel1, yPixel1, _rfpart(yEnd) * xGap);
                _plot(xPixel1, yPixel1 + 1, _fpart(yEnd) * xGap);
            }
            float intery = yEnd + gradient;

            xEnd = _round(x1);
            yEnd = y1 + gradient * (xEnd - x1);
            xGap = _fpart(x1 + 0.5f);
            float xPixel2 = xEnd;
            float yPixel2 = _ipart(yEnd);
            if (steep)
            {
                _plot(yPixel2, xPixel2, _rfpart(yEnd) * xGap);
                _plot(yPixel2 + 1, xPixel2, _fpart(yEnd) * xGap);
            }
            else
            {
                _plot(xPixel2, yPixel2, _rfpart(yEnd) * xGap);
                _plot(xPixel2, yPixel2 + 1, _fpart(yEnd) * xGap);
            }

            if (steep)
            {
                for (int x = (int)(xPixel1 + 1); x <= xPixel2 - 1; x++)
                {
                    _plot(_ipart(intery), x, _rfpart(intery));
                    _plot(_ipart(intery) + 1, x, _fpart(intery));
                    intery += gradient;
                }
            }
            else
            {
                for (int x = (int)(xPixel1 + 1); x <= xPixel2 - 1; x++)
                {
                    _plot(x, _ipart(intery), _rfpart(intery));
                    _plot(x, _ipart(intery) + 1, _fpart(intery));
                    intery += gradient;
                }
            }
        }




        int _ipart(float x) { return (int)x; }
        int _round(float x) { return _ipart(x + 0.5f); }
        float _fpart(float x)
        {
            if (x < 0) return (1 - (x - Mathf.Floor(x)));
            return (x - Mathf.Floor(x));
        }
        float _rfpart(float x)
        { return 1 - _fpart(x); }
        private void _plot(float x, float y, float c)
        {
            int alpha = (int)(c * 255);
            if (alpha > 255) alpha = 255;
            if (alpha < 0) alpha = 0;



            if (_checkIfInside((int)x, (int)y))
            {
                Color color = LineColor;
                color.a = Mathf.Max(alpha / 255f, Texture.GetPixel((int)x, (int)y).a);
                Texture.SetPixel((int)x, (int)y, color);
            }
        }

        bool _checkIfInside(float x, float y)
        {
            return x >= 0 && y >= 0 && x < Resolution && y < Resolution;
        }
        #endregion
        void FinishBitmap()
        {
            Texture.Apply();
            Texture_Image.texture = Texture;
        }


    }
}
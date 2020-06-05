using System;
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
        public int Maxi_Number_Datas;
        public bool ThickLine = true;
        public Color[] LineColor = { new Color(1, 0, 0, 0), new Color(1, 0, 0, 0) };

        public int FontSize = 9;

        [Header("Initialization Values.")]
        [SerializeField]
        public List<Vector2[]> StartingPoints = new List<Vector2[]>();

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

        public GameObject toggleholder;
        public GameObject toggle_prefab;
        private List<GameObject> Enabled_Channels = new List<GameObject>();

        void Update()
        {
            {
                try
                {
                    //Slider.gameObject.SetActive(true);

                    //var xpoint = Scroll.value * 10;
                    var xpoint = (x_size * Scroll.value + x_zero);
                    //var ypoint = FindYFromX(xpoint);

                    SlideValueX.text = "x: " + xpoint.ToString("0.0");
                    //SlideValueY.text = "y: " + ypoint.ToString("0.00");
                }
                catch
                {
                    SlideValueX.text = "x: ";
                    //SlideValueY.text = "y: ";
                }
            }

            //Redraw_Graph();
        }

        private void Start()
        {
            StartCoroutine("Redraw");
        }


        IEnumerator Redraw()
        {
            for(; ;)
            {
                Redraw_Graph();
                yield return new WaitForSeconds(0.2F);
            }
        }



        private float FindYFromX(float xpoint)
        {
            /*int i;
            for (i = 0; i < StartingPoints.Length - 1; i++)
            {
                if ((StartingPoints[i].x < xpoint || Mathf.Approximately(StartingPoints[i].x, xpoint)) &&
                    StartingPoints[i + 1].x > xpoint)
                {
                    break;
                }
            }

            var p = (1 / (StartingPoints[i + 1].x - StartingPoints[i].x)) * (xpoint - StartingPoints[i].x);

            return Mathf.Lerp(StartingPoints[i].y, StartingPoints[i + 1].y, p);*/
            return 0;
        }



        /// <summary>
        /// This fills and updates the Graph.
        /// </summary>
        /// <param name="points">
        /// X is the horizontal axis.
        /// Y is the vertical axis.
        /// </param>
        public void FillData(Vector2[] points, int channel)
        {
            StartingPoints[channel] = (Vector2[])points.Clone();
            //Redraw_Graph();

            /*
            X_Data_Text.fontSize = FontSize;
            Y_Data_Text.fontSize = FontSize;
            for (int t = 0; t < XTags.Length; t++)
            {
                XTags[t].fontSize = FontSize; //styling
            }
            for (int t = 0; t < YTags.Length; t++)
            {
                YTags[t].fontSize = FontSize; //styling
            }*/
        }


        /// <summary>
        /// Add one data at X, Y to one channel.
        /// </summary>
        /// <param name="points">
        /// X is the horizontal axis.
        /// Y is the vertical axis.
        /// /// <param name="channel">
        /// Channel to add this data
        /// </param>
        public void FillData(Vector2 points, int channel)
        {
            Check_Channel_Number(channel);

            Vector2[] initial_data = StartingPoints[channel];
            Array.Resize(ref initial_data, initial_data.Length + 1);
            initial_data[initial_data.Length - 1] = points;
            StartingPoints[channel] = initial_data;

            Check_Channel_DATAS_Number(channel);
           
            /*X_Data_Text.fontSize = FontSize;
            Y_Data_Text.fontSize = FontSize;
            for (int t = 0; t < XTags.Length; t++)
            {
                XTags[t].fontSize = FontSize; //styling
            }
            for (int t = 0; t < YTags.Length; t++)
            {
                YTags[t].fontSize = FontSize; //styling
            }*/
        }

        /// <summary>
        /// Add one data to the channel.
        /// </summary>
        /// <param name="value">
        /// Y value to add
        /// X will be incremented automatically
        /// <param name="channel">
        /// channel to be incremented
        /// </param>
        public void Add_Data(float value, int channel)
        {
            Check_Channel_Number(channel);

            if (StartingPoints[channel].Length == 0)
            {
                Vector2 newdata = new Vector2(0, value);
                FillData(newdata, channel);
            }
            else
            {
                Vector2 newdata = new Vector2(StartingPoints[channel][StartingPoints[channel].Length - 1].x + 1, value);
                FillData(newdata, channel);
            }
        }


        /// <summary>
        /// check is this channel does already exist, or add it.
        /// </summary>
        /// <param name="channel">
        /// channel number to check
        /// </param>
        private void Check_Channel_Number(int channel)
        {
            while (channel + 1 > StartingPoints.Count)
            {
                //Add a new channel
                StartingPoints.Add(new Vector2[] { Vector2.zero, Vector2.one });
                RemoveData(StartingPoints.Count - 1, 2); //Remove the two dummies datas at the beginning

                //Add a new toogle checkbox for this channel

                Vector3 toggle_pos = new Vector3(15, 120 - this.Enabled_Channels.Count * 10, 0);

                GameObject newtoggle = Instantiate(toggle_prefab, toggle_pos, new Quaternion(0, 0, 0, 0), this.toggleholder.transform);
                this.Enabled_Channels.Add(newtoggle/*.GetComponent<Toggle>()*/);

                newtoggle.GetComponentInChildren<Text>().text = "Channel " + (this.Enabled_Channels.Count - 1).ToString();
            }
        }


        /// <summary>
        /// check is this channel does not exeed the maximum number of datas
        /// </summary>
        /// <param name="channel">
        /// channel number to check
        /// </param>
        private void Check_Channel_DATAS_Number(int channel)
        {    
            if(StartingPoints[channel].Length > Maxi_Number_Datas)
                RemoveData(channel, StartingPoints[channel].Length - Maxi_Number_Datas); //Remove            
        }


        /// <summary>
        /// Remove datas at the begining of a channel.
        /// </summary>
        /// <param name="channel">
        /// channel number to reduce
        /// /// <param name="count">
        /// number of value to delete
        /// </param>
        public void RemoveData(int channel, int count)
        {
            if (StartingPoints[channel].Length == 0)
                return; 

            Vector2[] numbers = StartingPoints[channel];

            for (int i = 0; i < count; i++)
            {
                if (numbers.Length == 0)
                {
                    StartingPoints[channel] = numbers;
                    //Redraw_Graph();
                    return;
                }

                RemoveAt<Vector2>(ref numbers, 0);
            }

            StartingPoints[channel] = numbers;
            //Redraw_Graph();
        }

        private static void RemoveAt<T>(ref T[] arr, int index)
        {
            if (arr.Length < 1)
                return;

            for (int a = index; a < arr.Length - 1; a++)
            {
                // moving elements downwards, to fill the gap at [index]
                arr[a] = arr[a + 1];
            }
            // finally, let's decrement Array's size by one
            Array.Resize(ref arr, arr.Length - 1);
        }


        public void Clear()
        {
            StartingPoints.Clear();
            foreach (GameObject go in Enabled_Channels)
                Destroy(go);
            Enabled_Channels.Clear();            
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
        void Redraw_Graph()
        {
            if (this.Enabled_Channels.Count == 0)
                return;

            Y_Data_Text.text = InterleaveWithSpace(YUnitDataText);
            X_Data_Text.text = InterleaveWithSpace(XUnitDataText);

            Title.text = TitleText;


            float ymin = 100000;
            float ymax = 0;

            float xmin = 100000;
            float xmax = 0;
            int i;
            int channel_number = 0;
            foreach (Vector2[] channel in StartingPoints)
            {
                if (channel.Length != 0)
                {
                    for (i = 0; i < channel.Length; i++)
                    {
                        //check if this channel is enabled
                        if (this.Enabled_Channels[channel_number].GetComponent<Toggle>().isOn)
                        {
                            ymin = Mathf.Min(ymin, (channel[i].y));
                            ymax = Mathf.Max(ymax, (channel[i].y));

                            xmin = Mathf.Min(xmin, (channel[i].x));
                            xmax = Mathf.Max(xmax, (channel[i].x));
                        }
                    }
                }
                channel_number++;
            }


            ResetBitmap();

            //display of all channels

            channel_number = 0;
            foreach (Vector2[] channel in StartingPoints)
            {
                //check if this channel is enabled
                if (this.Enabled_Channels.Count > channel_number)
                {
                    if (this.Enabled_Channels[channel_number].GetComponent<Toggle>().isOn)
                    {
                        Display_channel(channel, LineColor[channel_number], xmin, ymin, xmax, ymax);
                    }
                }
                channel_number++;
            }

            FinishBitmap();
        }


        private void Display_channel(Vector2[] datas, Color color, float xmin, float ymin, float xmax, float ymax)
        {
            
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

            PointsGraph = new Vector2[datas.Length];
            for (int i = 0; i < datas.Length; i++)
            {
                //PointsGraph[i].x = ((Points[i].x * (Resolution / XGraphSize)) /** Resolution*/); //fixed X starts from 0, not working after trnasformation to dynamic
                PointsGraph[i].x = ((datas[i].x - x_zero) * (Resolution / XGraphSize));
                PointsGraph[i].y = ((datas[i].y - y_zero) * (Resolution / YGraphSize));
            }

           
            for (int i = 0; i < datas.Length - 1; i++)
            {
                DrawLine(PointsGraph[i], PointsGraph[i + 1], color);

                if (ThickLine)
                {
                    DrawLine(PointsGraph[i] + new Vector2(1, 0), PointsGraph[i + 1] + new Vector2(1, 0), color);
                    DrawLine(PointsGraph[i] + new Vector2(-1, 0), PointsGraph[i + 1] + new Vector2(-1, 0), color);
                    DrawLine(PointsGraph[i] + new Vector2(0, 1), PointsGraph[i + 1] + new Vector2(0, 1), color);
                    DrawLine(PointsGraph[i] + new Vector2(0, -1), PointsGraph[i + 1] + new Vector2(0, -1), color);

                    DrawLine(PointsGraph[i] + new Vector2(-1, -1), PointsGraph[i + 1] + new Vector2(-1, -1), color);
                    DrawLine(PointsGraph[i] + new Vector2(-1, 1), PointsGraph[i + 1] + new Vector2(-1, 1), color);
                    DrawLine(PointsGraph[i] + new Vector2(1, 1), PointsGraph[i + 1] + new Vector2(1, 1), color);
                    DrawLine(PointsGraph[i] + new Vector2(1, -1), PointsGraph[i + 1] + new Vector2(1, -1), color);
                }
            }
        }






        void ResetBitmap()
        {
            if (Texture)
                Destroy(Texture);

            Texture = new Texture2D(Resolution, Resolution, TextureFormat.ARGB32, true, true);

            var col = LineColor;
            LineColor[0].a = 0;
            for (int x = 0; x < Resolution; x++)
            {
                for (int y = 0; y < Resolution; y++)
                {
                    Texture.SetPixel(x, y, col[0]);
                }
            }
        }
        #region helper

        void DrawLine(Vector2 from, Vector2 to, Color color)
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
                _plot(yPixel1, xPixel1, _rfpart(yEnd) * xGap, color);
                _plot(yPixel1 + 1, xPixel1, _fpart(yEnd) * xGap, color);
            }
            else
            {
                _plot(xPixel1, yPixel1, _rfpart(yEnd) * xGap, color);
                _plot(xPixel1, yPixel1 + 1, _fpart(yEnd) * xGap, color);
            }
            float intery = yEnd + gradient;

            xEnd = _round(x1);
            yEnd = y1 + gradient * (xEnd - x1);
            xGap = _fpart(x1 + 0.5f);
            float xPixel2 = xEnd;
            float yPixel2 = _ipart(yEnd);
            if (steep)
            {
                _plot(yPixel2, xPixel2, _rfpart(yEnd) * xGap, color);
                _plot(yPixel2 + 1, xPixel2, _fpart(yEnd) * xGap, color);
            }
            else
            {
                _plot(xPixel2, yPixel2, _rfpart(yEnd) * xGap, color);
                _plot(xPixel2, yPixel2 + 1, _fpart(yEnd) * xGap, color);
            }

            if (steep)
            {
                for (int x = (int)(xPixel1 + 1); x <= xPixel2 - 1; x++)
                {
                    _plot(_ipart(intery), x, _rfpart(intery), color);
                    _plot(_ipart(intery) + 1, x, _fpart(intery), color);
                    intery += gradient;
                }
            }
            else
            {
                for (int x = (int)(xPixel1 + 1); x <= xPixel2 - 1; x++)
                {
                    _plot(x, _ipart(intery), _rfpart(intery), color);
                    _plot(x, _ipart(intery) + 1, _fpart(intery), color);
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
        private void _plot(float x, float y, float c, Color color)
        {
            int alpha = (int)(c * 255);
            if (alpha > 255) alpha = 255;
            if (alpha < 0) alpha = 0;



            if (_checkIfInside((int)x, (int)y))
            {
                //Color color = LineColor;
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
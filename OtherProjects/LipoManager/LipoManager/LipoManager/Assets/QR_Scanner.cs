using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using BarcodeScanner;
using BarcodeScanner.Scanner;
using System;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using Wizcorp.Utils.Logger;
using System.Xml.Serialization;
using System.IO;
using TMPro;
using System.Globalization;

public class QR_Scanner : MonoBehaviour
{
	public GameObject New_Ope_Panel;
    private IScanner BarcodeScanner;
    public Text TextHeader;
    public RawImage Image;
	public TextMeshProUGUI Date_Picker;


	public GameObject Element_Container;


	public GameObject Element_Type_Operation;
	public GameObject Element_Appareil;
	public GameObject Element_Programme;
	public TMP_InputField Tps_Vol;

	public List<GameObject> Elements_Batteries = new List<GameObject>();

	public GameObject prefab_Batterie;

	private float RestartTime;

	private Operation new_operation = new Operation();

	private void Awake()
    {
		New_Ope_Panel.gameObject.SetActive(false);
	}

    void Start()
	{
		// Create a basic scanner
		BarcodeScanner = new Scanner();
		BarcodeScanner.Camera.Play();

		// Display the camera texture through a RawImage
		BarcodeScanner.OnReady += (sender, arg) => {
			// Set Orientation & Texture
			Image.transform.localEulerAngles = BarcodeScanner.Camera.GetEulerAngles();
			Image.transform.localScale = BarcodeScanner.Camera.GetScale();
			Image.texture = BarcodeScanner.Camera.Texture;

			// Keep Image Aspect Ratio
			var rect = Image.GetComponent<RectTransform>();
			var newHeight = rect.sizeDelta.x * BarcodeScanner.Camera.Height / BarcodeScanner.Camera.Width;
			rect.sizeDelta = new Vector2(rect.sizeDelta.x, newHeight);
		};

		// Track status of the scanner
		BarcodeScanner.StatusChanged += (sender, arg) => {
			TextHeader.text = "Status: " + BarcodeScanner.Status;
		};

		Set_New_Date(DateTime.Now);
		/*QRCode_Data data = new QRCode_Data();
		data.Name = "Hacker_5s_5100_11";
		data.Type = QRCode_Data.QRType.Batterie;
		Serialize(data, "./test.xml");*/

		/*QRCode_Data data = new QRCode_Data();
		data = XMLDeserialize("./test.xml");*/
	}

	#region Date Management
	public void Set_New_Date(string date)
	{
		System.DateTime new_date = System.DateTime.Parse(date);

		Set_New_Date(new_date);
	}

	public void Set_New_Date(DateTime date)
	{
		Date_Picker.text = date.ToString("dd/MM/yyyy");

		new_operation.date = date;
	}


    #endregion

    #region Elements_Management
    public void QRCode_Found(string value)
	{
		QRCode_Data data = new QRCode_Data();
		data = XMLDeserialize(value);

		TextHeader.text = "Found: " + data.Type.ToString() + " / " + data.Name.ToString();

		//Vérifie si cet element n'est pas déjà présent dans le dictionnaire
		// ContainsKey can be used to test keys before inserting them.

		switch (data.Type)
		{
			case QRCode_Data.Element_Type.Avion:
				Add_Avion(data);
				break;

			case QRCode_Data.Element_Type.Batterie:
				Add_Batterie(data);
				break;

			case QRCode_Data.Element_Type.Programme:
				Add_Programme(data);
				break;
		}
	}

	private void Add_Avion(QRCode_Data data)
	{
		if (!new_operation.Elements_Presents.ContainsKey(data.Name))
		{
			new_operation.Ope_Type = Operation.Operation_Type.Vol;
			Element_Appareil.SetActive(true);
			Element_Programme.SetActive(true);

			Element_Appareil.GetComponent<Element_Avion>().Set_Name(data.Name);

			Debug.Log("Add Avion");

			new_operation.Elements_Presents.Add(data.Name, data);
			new_operation.Elements_Presents_List.Add(data);
		}
	}

	private void Add_Programme(QRCode_Data data)
	{
		if (!new_operation.Elements_Presents.ContainsKey(data.Name))
		{
			new_operation.Ope_Type = Operation.Operation_Type.Vol;
			Element_Appareil.SetActive(true);
			Element_Programme.SetActive(true);

			Element_Programme.GetComponent<Element_Programme>().Set_Name(data.Name);

			Debug.Log("Add Programme");

			new_operation.Elements_Presents.Add(data.Name, data);
			new_operation.Elements_Presents_List.Add(data);
		}
	}

	private void Add_Batterie(QRCode_Data data)
    {
		if (!new_operation.Elements_Presents.ContainsKey(data.Name))
		{
			GameObject new_Batterie = Instantiate(prefab_Batterie, Element_Container.transform);

			new_Batterie.GetComponent<Element_Batterie>().Set_Name(data.Name);

			Elements_Batteries.Add(new_Batterie);

			Debug.Log("Add Batterie");

			//Si cet element n'est pas déjà présent
			//Ajoute le dans la liste des objets de cet evenement
			new_operation.Elements_Presents.Add(data.Name, data);
			new_operation.Elements_Presents_List.Add(data);
		}
	}
    #endregion

    /// <summary>
    /// The Update method from unity need to be propagated to the scanner
    /// </summary>
    void Update()
	{
		if (BarcodeScanner != null)
		{
			BarcodeScanner.Update();
		}

		// Check if the Scanner need to be started or restarted
		if (RestartTime != 0 && RestartTime < Time.realtimeSinceStartup)
		{
			Start_Scanning();
			RestartTime = 0;
		}
	}

	#region UI Buttons

	public void Start_Scanning()
	{
		if (BarcodeScanner == null)
		{
			Log.Warning("No valid camera");
			return;
		}

		// Start Scanning
		BarcodeScanner.Scan((barCodeType, barCodeValue) => {
			BarcodeScanner.Stop();
			QRCode_Found(barCodeValue);

			RestartTime += Time.realtimeSinceStartup + 1f;

		#if UNITY_ANDROID || UNITY_IOS
			Handheld.Vibrate();
		#endif
		});
	}

	public void StopScanning()
	{
		if (BarcodeScanner == null)
		{
			Log.Warning("No valid camera");
			return;
		}

		// Stop Scanning
		BarcodeScanner.Stop();
	}

	#endregion


	#region Cancel Validate
	public void Cancel_New_Ope()
    {
		Element_Appareil.SetActive(false);
		Element_Programme.SetActive(false);

		foreach (GameObject batt_to_destroy in Elements_Batteries)
			DestroyImmediate(batt_to_destroy);

		Elements_Batteries.Clear();
		new_operation.Elements_Presents.Clear();
		new_operation.Elements_Presents_List.Clear();
	}


	public void Validate_New_Ope()
    {
		foreach(QRCode_Data element in new_operation.Elements_Presents_List)
        {
			if(element.Type == QRCode_Data.Element_Type.Programme)
            {
				element.Temps_Vol = float.Parse(Tps_Vol.text, CultureInfo.CreateSpecificCulture("fr-FR"));
            }

			if(element.Type == QRCode_Data.Element_Type.Batterie)
            {
				element.Restant = int.Parse(FindChildByRecursion(element.gameobj.transform, "InputField (%restant)").GetComponentInChildren<TMP_InputField>().text);

				element.DeltaV = int.Parse(FindChildByRecursion(element.gameobj.transform, "InputField (dV)").GetComponentInChildren<TMP_InputField>().text);
			}
        }


		this.GetComponent<XML_Data_Manager>().Add_New_Operation(new_operation);

		this.GetComponent<XML_Data_Manager>().Save_XML_File();

		Cancel_New_Ope();
	}

	#endregion

	#region Operation
	[Serializable]
	public class Operation
    {
        public enum Operation_Type
        {
			Charge,
			Balance,
			Stockage,
			Decharge,
			Vol
        }

		public Operation_Type Ope_Type = new Operation_Type();

		public DateTime date;

		[XmlIgnore]
		public Dictionary<string, QRCode_Data> Elements_Presents = new Dictionary<string, QRCode_Data>();
		public List<QRCode_Data> Elements_Presents_List = new List<QRCode_Data>();

		public string Commentaire;
	}

	#endregion


	#region XML
	[Serializable]
	public class QRCode_Data
    {
		public enum Element_Type
        {
			Batterie,
			Avion,
			Programme
        }

		[XmlIgnore]
		public GameObject gameobj;

		public Element_Type Type = new Element_Type();
		public string Name;

		public float Courant;
		public int Capacite;
		public int Restant;
		public int DeltaV;
		public float Temps_Vol;
	}

    private void Serialize(object data, string path)
    {
		XMLSerializer(data, path);
	}

    private void XMLSerializer(object data, string path)
	{
		XmlSerializer serialiser = new XmlSerializer(typeof(QRCode_Data));
		TextWriter writer = new StreamWriter(path);

		serialiser.Serialize(writer, data);
	}


	private QRCode_Data XMLDeserialize(string value)
    {
		XmlSerializer serialiser = new XmlSerializer(typeof(QRCode_Data));
		TextReader reader = new StringReader(value);

		object obj = null;
		obj = serialiser.Deserialize(reader);
		return (QRCode_Data)obj;
	}


	#endregion


	internal static Transform FindChildByRecursion(Transform aParent, string aName)
	{
		if (aParent == null) return null;

		var result = aParent.transform.Find(aName);

		if (result != null)
			return result;

		foreach (Transform child in aParent)
		{
			result = FindChildByRecursion(child, aName);
			if (result != null)
				return result;
		}
		return null;
	}
}

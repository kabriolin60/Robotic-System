using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Xml.Serialization;
using System.IO;
using System;

[Serializable]
public class XML_Datas
{	public enum State
	{
		In_Use,
		Sold,
		HS
	}

	//Liste des Elements connus
	public KnownElements Known_Elements = new KnownElements();


	//Liste des operations
	public List<Operation> Operations = new List<Operation>();
}

[Serializable]
public class KnownElements
{
	//Liste des appareils connus
	public List<Appareil> Known_Appareils = new List<Appareil>();

	//Liste des batteries connues
	public List<Batterie> Known_Batteries = new List<Batterie>();

	//Liste des programmes connues
	public List<Programme> Known_Programmes = new List<Programme>();
}

public class Liste_Operations
{
	public List<Operation> Operations = new List<Operation>();
}


[Serializable]
public class Appareil
{
	public int Id;		
	public string Name;

	[XmlElement(IsNullable = true)]
	public string Marque;

	[XmlElement(IsNullable = true)]
	public string Modele;		
	public DateTime Buy_Date;

	public float Price;		
	public XML_Datas.State State;
}

[Serializable]
public class Programme
{
	public int Id;
	public string Name;
}

[Serializable]
public class Batterie
{
	public int Id;
	public string Name;

	[XmlElement(IsNullable = true)]
	public string Marque;

	[XmlElement(IsNullable = true)]
	public string Modele;
	public int Capacity;

	public int C_Rate;

	public int S_Config;

	[XmlElement(IsNullable = true)]
	public string Fournisseur;

	[XmlElement(IsNullable = true)]
	public string Reference;
	public DateTime Buy_Date;

	public float Price;

	public XML_Datas.State State;
}




[Serializable]
public class Operation
{
	public enum Operation_Type
    {
		Charge,
		Balance,
		Decharge,
		Stockage,
		Vol
    }

	public int Id;
	public DateTime date;
	public Operation_Type Ope_Type;

	public List<Element> Elements_List = new List<Element>();
}

[Serializable]
public class Element
{
	public enum Element_Type
    {
		Appareil,
		Programme,
		Batterie
    }

	public Element_Type Type;

	public string Name;

	[XmlElement(IsNullable = false)]
	public float Courant;

	[XmlElement(IsNullable = false)]
	public int Capacite;

	[XmlElement(IsNullable = false)]
	public float Tps_Vol;

	[XmlElement(IsNullable = false)]
	public string Remarque;

	[XmlElement(IsNullable = false)]
	public int Restant;

	[XmlElement(IsNullable = false)]
	public int DeltaV;
}
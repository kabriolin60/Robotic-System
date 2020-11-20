using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class camera_render : MonoBehaviour
{
	public RawImage Rendering_image;


	private RenderTexture panelTexture;

	private void Start()
	{
		Change_Rendering_Panel(Rendering_image);

		panelTexture = new RenderTexture(this.GetComponent<Camera>().pixelWidth, this.GetComponent<Camera>().pixelHeight, 24, RenderTextureFormat.ARGB32);
		panelTexture.antiAliasing = 2;
	}

	public void Change_Rendering_Panel(RawImage new_image)
	{
		Rendering_image = new_image;
	}

	public void OnPostRender()
	{
		// Create a render texture
		try
		{			
			panelTexture.Create();
			Rendering_image.texture = panelTexture;

			this.GetComponent<Camera>().targetTexture = panelTexture;
		}
		catch
		{

		}
	}

}

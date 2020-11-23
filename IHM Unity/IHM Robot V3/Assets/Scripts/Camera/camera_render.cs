using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class camera_render : MonoBehaviour
{
	public RawImage Rendering_image;
	public RawImage Rendering_Big_Image;


	private RenderTexture panelTexture;

	private void Start()
	{
		panelTexture = new RenderTexture(this.GetComponent<Camera>().pixelWidth, this.GetComponent<Camera>().pixelHeight, 24, RenderTextureFormat.ARGB32);
		panelTexture.antiAliasing = 2;
	}

	public void OnPostRender()
	{
		// Create a render texture
		try
		{			
			panelTexture.Create();
			Rendering_image.texture = panelTexture;
			if(Rendering_Big_Image != null)
				Rendering_Big_Image.texture = panelTexture;

			this.GetComponent<Camera>().targetTexture = panelTexture;
		}
		catch
		{

		}
	}

}

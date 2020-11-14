using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class FPS : MonoBehaviour
{
    [SerializeField] private TextMeshProUGUI _fpsText;

    public void Start()
    {
        _fpsText = GetComponent<TextMeshProUGUI>();
    }

    private void Update()
    {
        int fps = (int)(1f / Time.unscaledDeltaTime);
        _fpsText.text = "FPS: " + fps;
    }
}

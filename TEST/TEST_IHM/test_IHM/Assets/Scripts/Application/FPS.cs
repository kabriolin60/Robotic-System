using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class FPS : MonoBehaviour
{
    public int Max_FrameRate;

    /*[SerializeField]*/ private TextMeshProUGUI _fpsText;

    void Awake()
    {
        QualitySettings.vSyncCount = 0;  // VSync must be disabled
        Application.targetFrameRate = Max_FrameRate;
    }

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

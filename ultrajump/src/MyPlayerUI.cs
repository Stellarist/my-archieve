using UnityEngine;
using TMPro;

public class MyPlayerUI : MonoBehaviour
{
    static MyPlayerUI instance;
    public TextMeshProUGUI orb_text, time_text, death_text, gameover_text;

    private void Awake()
    {
        if (instance != null)
        {
            Destroy(gameObject);
            return;
        }
        instance = this;
    }

    public static void UpdateOrbUI(int orb_num)
    {
        instance.orb_text.text=orb_num.ToString();
    }

    public static void UpdateDeathUI(int death_times)
    {
        instance.death_text.text=death_times.ToString();
    }

    public static void UpdateTimeUI(float time)
    {
        int minutes = (int)(time / 60);
        float seconds = time % 60;

        instance.time_text.text=minutes.ToString("00")+":"+seconds.ToString("00");
    }

    public static void ConcealStatus()
    {
        instance.gameover_text.enabled = false;
    }

    public static void DisplayStatus()
    {
        instance.gameover_text.enabled=true;
    }
}

using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections.Generic;

public class GameManager : MonoBehaviour
{
    static GameManager instance;
    MyPlayerScene fader;
    List<MyPlayerOrb> orbs;
    MyPlayerDoor locked_door;
    static float game_time;
    static int dead_times;
    bool is_over;

    private void Start()
    {
        MyPlayerUI.ConcealStatus();
        MyPlayerUI.UpdateTimeUI(game_time);
    }

    private void Awake()
    {
        if (instance != null)
        {
            Destroy(gameObject);
            return;
        }

        instance = this;
        orbs = new List<MyPlayerOrb>();
        MyPlayerUI.UpdateDeathUI(dead_times);
    }

    private void Update()
    {
        if (is_over)
            return;
        game_time += Time.deltaTime;
        MyPlayerUI.UpdateTimeUI(game_time);
        MyPlayerUI.UpdateDeathUI(dead_times);
    }

    // 注册场景
    public static void RegisterSceneFader(MyPlayerScene fader)
    {
        instance.fader = fader;
    }

    // 注册宝珠
    public static void RegisterOrb(MyPlayerOrb orb)
    {
        if (instance == null)
            return;
        if(!instance.orbs.Contains(orb))
            instance.orbs.Add(orb);
        MyPlayerUI.UpdateOrbUI(instance.orbs.Count);
    }

    // 注册门
    public static void RegisterDoor(MyPlayerDoor door)
    {
        instance.locked_door = door;
    }

    // 处理角色胜利机制
    public static void PlayerWon()
    {
        instance.is_over = true;
        MyPlayerUI.DisplayStatus();
        MyPlayerAudio.PlayWinAudio();
    }

    // 处理角色死亡机制
    public static void PlayerDied()
    {
        instance.fader.FadeOut();
        dead_times++;
        MyPlayerUI.UpdateDeathUI(dead_times);
        instance.Invoke("RestartScene", 1.5f);
    }

    // 处理角色获得宝珠机制
    public static void PlayerGrabbedOrb(MyPlayerOrb orb)
    {
        if (!instance.orbs.Contains(orb))
            return;
        instance.orbs.Remove(orb);

        if(instance.orbs.Count==0)
            instance.locked_door.Open();

        MyPlayerUI.UpdateOrbUI(instance.orbs.Count);
    }

    // 重新显示场景
    private void RestartScene()
    {
        instance.orbs.Clear();
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex); 
    }

    // 游戏结束
    public static bool GameIsOver()
    {
        return instance.is_over;
    }
}

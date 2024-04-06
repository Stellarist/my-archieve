using UnityEngine;

public class MyPlayerScene : MonoBehaviour
{
    Animator anim;
    int fader_id;

    void Start()
    {
        anim=GetComponent<Animator>();
        fader_id = Animator.StringToHash("Fade");
        GameManager.RegisterSceneFader(this);
    }

    public void FadeOut()
    {
        anim.SetTrigger(fader_id);
    }
}

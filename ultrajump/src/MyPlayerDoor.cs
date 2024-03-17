using UnityEngine;

public class MyPlayerDoor : MonoBehaviour
{
    Animator anim;
    int open_id;

    void Start()
    {
        anim=GetComponent<Animator>();
        open_id = Animator.StringToHash("Open");
        GameManager.RegisterDoor(this);
    }

    public void Open()
    {
        anim.SetTrigger(open_id);
    }
}

using UnityEngine;

public class MyPlayerAnimation : MonoBehaviour
{
    Animator anim;
    MyPlayerMovement move;
    Rigidbody2D rigid_body;

    // 动画参数
    int ground_id;
    int hanging_id;
    int crouch_id;
    int speed_id;
    int fall_id;

    void Start()
    {
        anim=GetComponent<Animator>(); 

        // 获得父组件的参数
        move=GetComponentInParent<MyPlayerMovement>();
        rigid_body=GetComponentInParent<Rigidbody2D>();

        // 获得动画编号
        ground_id = Animator.StringToHash("isOnGround");
        hanging_id = Animator.StringToHash("isHanging");
        crouch_id = Animator.StringToHash("isCrouching");
        speed_id = Animator.StringToHash("speed");
        fall_id = Animator.StringToHash("verticalVelocity");
    }

    void Update()
    {
        anim.SetFloat(speed_id, Mathf.Abs(move.x_velocity));
        anim.SetBool(ground_id, move.is_onground);
        anim.SetBool(hanging_id, move.is_hanging);
        anim.SetBool(crouch_id, move.is_crouch);
        anim.SetFloat(fall_id, rigid_body.velocity.y);
    }

    // 走路声音
    public void StepAudio()
    {
        MyPlayerAudio.PlayFootStepAudio();
    }

    // 下蹲走路声音
    public void CrouchStepAudio()
    {
        MyPlayerAudio.PlayCrouchFootstepAudio();
    }
}

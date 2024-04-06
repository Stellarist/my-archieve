using UnityEngine;

public class MyPlayerMovement : MonoBehaviour
{
    private Rigidbody2D rigid_body;
    private BoxCollider2D box_collider;

    [Header("Movement")]
    public float speed = 8f;
    public float crouch_speed_divisor = 3f;

    [Header("Jump")]
    public float jump_force = 6.3f;
    public float jump_hold_force = 1.9f;
    public float jump_hold_duration = 0.1f;
    public float jump_crouch_boost = 2.5f;
    public float hanging_jump_force = 15;

    [Header("Status")]
    public bool is_crouch;
    public bool is_onground;
    public bool is_jump;
    public bool is_head_blocked;
    public bool is_hanging;
    public bool jump_judge;

    [Header("Environment")]
    public LayerMask ground_layer;
    public float foot_offset = 0.4f;
    public float head_clearance = 0.5f;
    public float ground_distance = 0.2f;
    public float player_height;
    public float eye_sight = 1.5f;
    public float grab_distance = 0.4f;
    public float reach_offset = 0.7f;

    public float x_velocity;
    public float jump_time;

    bool jump_pressed;
    bool jump_held;
    bool crouch_held;
    bool crouch_pressed;

    Vector2 collider_stand_size;
    Vector2 collider_stand_offset;
    Vector2 collider_crouch_size;
    Vector2 collider_crouch_offset;

    void Start()
    {
        rigid_body = GetComponent<Rigidbody2D>();
        box_collider = GetComponent<BoxCollider2D>();

        player_height = box_collider.size.y;
        collider_stand_size = box_collider.size;
        collider_stand_offset = box_collider.offset;
        collider_crouch_size = new Vector2(box_collider.size.x, box_collider.size.y / 2f);
        collider_crouch_offset = new Vector2(box_collider.offset.x, box_collider.offset.y / 2f);
    }

    void Update()
    {
        if (GameManager.GameIsOver())
            return;

        jump_pressed = Input.GetButtonDown("Jump");
        if (jump_pressed)
            jump_judge = true;
        jump_held = Input.GetButton("Jump");
        crouch_held = Input.GetButton("Crouch");
        crouch_pressed = Input.GetButtonDown("Crouch");
    }

    private void FixedUpdate()
    {
        if (GameManager.GameIsOver())
            return;

        PhysicsCheck();
        GroundMovement();
        MidairMovement();
    }

    void FlipDirection()
    {
        if (x_velocity < 0)
            transform.localScale = new Vector3(-1, 1, 1);
        else
            transform.localScale = new Vector3(1, 1, 1);
    }

    void Crouch()
    {
        is_crouch = true;
        box_collider.size = collider_crouch_size;
        box_collider.offset = collider_crouch_offset;
    }

    void Stand()
    {
        is_crouch = false;
        box_collider.size = collider_stand_size;
        box_collider.offset = collider_stand_offset;
    }

    void PhysicsCheck()
    {
        float direction = transform.localScale.x;
        Vector2 grab_direction = new Vector2(direction, 0f);

        RaycastHit2D left_check = Raycast(new Vector2(-foot_offset, 0f), Vector2.down, ground_distance, ground_layer);
        RaycastHit2D right_check = Raycast(new Vector2(foot_offset, 0f), Vector2.down, ground_distance, ground_layer);
        is_onground = left_check || right_check;

        RaycastHit2D head_check = Raycast(new Vector2(0f, box_collider.size.y), Vector2.up, head_clearance, ground_layer);
        is_head_blocked = head_check;

        RaycastHit2D blocked_check = Raycast(new Vector2(foot_offset * direction, player_height), grab_direction, grab_distance, ground_layer);

        RaycastHit2D wall_check = Raycast(new Vector2(foot_offset * direction, eye_sight), grab_direction, grab_distance, ground_layer);

        RaycastHit2D ledge_check = Raycast(new Vector2(reach_offset * direction, player_height), Vector2.down, grab_distance, ground_layer);

        if (!is_onground && rigid_body.velocity.y < 0f && ledge_check && wall_check && !blocked_check)
        {
            Vector3 pos = transform.position;
            pos.x += (wall_check.distance - 0.05f) * direction;
            pos.y -= ledge_check.distance;
            transform.position = pos;

            rigid_body.bodyType = RigidbodyType2D.Static;
            is_hanging = true;
        }
    }

    void GroundMovement()
    {
        if (is_hanging)
            return;

        if (crouch_held && !is_crouch && is_onground)
            Crouch();
        else if (!crouch_held && is_crouch && !is_head_blocked)
            Stand();
        else if (!is_onground && is_crouch)
            Stand();

        x_velocity = Input.GetAxis("Horizontal");

        if (is_crouch)
            x_velocity /= crouch_speed_divisor;

        rigid_body.velocity = new Vector2(x_velocity * speed, rigid_body.velocity.y);
        FlipDirection();
    }

    void MidairMovement()
    {
        if (is_hanging)
        {
            if (jump_judge)
            {
                rigid_body.bodyType = RigidbodyType2D.Dynamic;
                rigid_body.velocity = new Vector2(rigid_body.velocity.x, hanging_jump_force);
                is_hanging = false;
                jump_judge = false;
            }

            if (crouch_pressed)
            {
                rigid_body.bodyType = RigidbodyType2D.Dynamic;
                is_hanging = false;
            }
        }


        if (jump_judge && is_onground && !is_jump && !is_head_blocked)
        {
            if (is_crouch)
            {
                Stand();
                rigid_body.AddForce(new Vector2(0f, jump_crouch_boost), ForceMode2D.Impulse);
            }
            is_onground = false;
            is_jump = true;
            jump_judge = false;

            jump_time = Time.time + jump_hold_duration;

            rigid_body.AddForce(new Vector2(0f, jump_force), ForceMode2D.Impulse);

            MyPlayerAudio.PlayJumpAudio();
        }
        else if (is_jump)
        {
            if (jump_held)
                rigid_body.AddForce(new Vector2(0f, jump_hold_force), ForceMode2D.Impulse);

            if (jump_time < Time.time)
                is_jump = false;
        }
    }

    RaycastHit2D Raycast(Vector2 offset, Vector2 ray_direction, float length, LayerMask layer)
    {
        Vector2 position = transform.position;
        RaycastHit2D hit = Physics2D.Raycast(position + offset, ray_direction, length, layer);

        Color color = hit ? Color.red : Color.green;
        Debug.DrawRay(position + offset, ray_direction * length, color);

        return hit;
    }
}

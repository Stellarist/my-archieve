using UnityEngine;

public class MyPlayerZone : MonoBehaviour
{
    int player_layer;

    void Start()
    {
        player_layer = LayerMask.NameToLayer("Player");
    }

    // implement abstracts
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.layer == player_layer)
            GameManager.PlayerWon();
    }
}

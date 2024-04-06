using UnityEngine;

public class MyPlayerHealth : MonoBehaviour
{
    public GameObject death_prefab;
    int traps_layer;

    void Start()
    {
        traps_layer = LayerMask.NameToLayer("Traps");
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.layer == traps_layer)
        {
            Instantiate(death_prefab, transform.position, transform.rotation);

            gameObject.SetActive(false);

            MyPlayerAudio.PlayDeathAudio();

            GameManager.PlayerDied();
        }
    }
}

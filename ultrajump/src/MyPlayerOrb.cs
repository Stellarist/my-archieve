using UnityEngine;

public class MyPlayerOrb : MonoBehaviour
{
    int player;
    public GameObject explosion;

    void Start()
    {
        player = LayerMask.NameToLayer("Player");

        // ×¢²á±¦Öé
        GameManager.RegisterOrb(this);
    }

    // override
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.layer==player)
        {
            gameObject.SetActive(false);
            
            Instantiate(explosion, transform.position, transform.rotation);

            gameObject.SetActive(false);

            MyPlayerAudio.PlayOrbAudio();

            GameManager.PlayerGrabbedOrb(this);
        }
    }
}

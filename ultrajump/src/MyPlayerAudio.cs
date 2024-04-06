using UnityEngine;

public class MyPlayerAudio : MonoBehaviour
{
    static MyPlayerAudio current_audio;

    [Header("environment")]
    public AudioClip ambient_clip;
    public AudioClip music_clip;

    [Header("FX")]
    public AudioClip death_fx_clip;
    public AudioClip orb_fx_clip;
    public AudioClip door_fx_clip;
    public AudioClip start_level_clip;
    public AudioClip end_level_clip;

    [Header("player")]
    public AudioClip[] walk_step_clips;
    public AudioClip[] crouch_step_clips;
    public AudioClip jump_clip;
    public AudioClip jump_voice_clip;
    public AudioClip death_clip;
    public AudioClip death_voice_clip;
    public AudioClip orb_voice_clip;

    AudioSource ambient_source;
    AudioSource music_source;
    AudioSource player_source;
    AudioSource fx_source;
    AudioSource voice_source;

    private void Awake()
    {
        if(current_audio != null)
        {
            Destroy(gameObject);
            return;
        }
        current_audio = this;

        ambient_source=gameObject.AddComponent<AudioSource>();
        music_source=gameObject.AddComponent<AudioSource>();
        fx_source=gameObject.AddComponent<AudioSource>();
        player_source=gameObject.AddComponent<AudioSource>();
        voice_source=gameObject.AddComponent<AudioSource>();

        StartLevelAudio();
    }

    void StartLevelAudio()
    {
        current_audio.ambient_source.clip = current_audio.ambient_clip;
        current_audio.ambient_source.loop = true;
        current_audio.ambient_source.Play();

        current_audio.music_source.clip = current_audio.music_clip;
        current_audio.music_source.loop = true;
        current_audio.music_source.Play();

        current_audio.fx_source.clip = current_audio.start_level_clip;
        current_audio.fx_source.Play();
    }

    public static void PlayFootStepAudio()
    {
        int index=Random.Range(0, current_audio.walk_step_clips.Length);
        current_audio.player_source.clip = current_audio.walk_step_clips[index];
        current_audio.player_source.Play();
    }

    public static void PlayCrouchFootstepAudio()
    {
        int index=Random.Range(0, current_audio.crouch_step_clips.Length);
        current_audio.player_source.clip = current_audio.crouch_step_clips[index];
        current_audio.player_source.Play();
    }

    public static void PlayJumpAudio()
    {
        current_audio.player_source.clip=current_audio.jump_clip;
        current_audio.player_source.Play();

        current_audio.voice_source.clip=current_audio.jump_voice_clip;
        current_audio.voice_source.Play();
    }

    public static void PlayDeathAudio()
    {
        current_audio.player_source.clip = current_audio.death_clip;
        current_audio.player_source.Play();

        current_audio.voice_source.clip = current_audio.death_voice_clip;
        current_audio.voice_source.Play();

        current_audio.fx_source.clip=current_audio.death_fx_clip;
        current_audio.fx_source.Play();
    }

    public static void PlayOrbAudio()
    {
        current_audio.fx_source.clip = current_audio.orb_voice_clip;
        current_audio.fx_source.Play();

        current_audio.voice_source.clip= current_audio.orb_voice_clip;
        current_audio.voice_source.Play();
    }

    public static void PlayDoorOpenAudio()
    {
        current_audio.fx_source.clip = current_audio.door_fx_clip;
        current_audio.fx_source.PlayDelayed(1f);
    }
    public static void PlayWinAudio()
    {
        current_audio.fx_source.clip = current_audio.end_level_clip;
        current_audio.fx_source.Play();
        current_audio.player_source.Stop();
    }
}

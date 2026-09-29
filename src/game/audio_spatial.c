#include <kf/lib/address.h>
#include <kf/game/audio.h>

ADDRESS(0x80013f50, 0x34)
KfAudioPlaybackResult audio_play_spatial_default_range(
    s32 sound, const VECTOR *position, s16 volume, s32 note_offset)
{
    return audio_play_spatial(sound, position, volume, KF_AUDIO_DEFAULT_MAX_DISTANCE,
        KF_AUDIO_DEFAULT_ATTENUATION_DISTANCE, note_offset);
}

ADDRESS(0x80013f84, 0x34)
KfAudioPlaybackResult audio_play_spatial_range(
    s32 sound, const VECTOR *position, s16 volume, s32 max_distance,
    s32 attenuation_distance, s32 note_offset)
{
    return audio_play_spatial(
        sound, position, volume, max_distance, attenuation_distance, note_offset);
}

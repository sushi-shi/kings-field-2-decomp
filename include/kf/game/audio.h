#ifndef KF_GAME_AUDIO_H
#define KF_GAME_AUDIO_H
#include <kf/lib/types.h>
#include <psyq/sdk.h>

/* GAME.EXE sound effects: sound ids index the loaded sound table; bit 15 of
 * a spatial sound id selects an alternate panning mode. */
enum {
    KF_AUDIO_NOT_PLAYED = 0,
    KF_AUDIO_PLAYED = 1
};

enum {
    KF_AUDIO_DEFAULT_MAX_DISTANCE = 0x4800,
    KF_AUDIO_DEFAULT_ATTENUATION_DISTANCE = 0x6000
};

typedef s32 KfAudioPlaybackResult;

void audio_shutdown(void);
KfAudioPlaybackResult audio_play_spatial(
    s32 sound, const VECTOR *position, s16 volume, s32 max_distance,
    s32 attenuation_distance, s32 note_offset);
KfAudioPlaybackResult audio_play_spatial_default_range(
    s32 sound, const VECTOR *position, s16 volume, s32 note_offset);
KfAudioPlaybackResult audio_play_spatial_range(
    s32 sound, const VECTOR *position, s16 volume, s32 max_distance,
    s32 attenuation_distance, s32 note_offset);
void audio_play_sound(s32 sound, s32 volume);
void audio_key_on(s32 sound, s32 left_volume, s32 right_volume, s32 note_offset);

#endif

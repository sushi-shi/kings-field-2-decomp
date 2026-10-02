#ifndef KF_GAME_AUDIO_H
#define KF_GAME_AUDIO_H
#include <kf/lib/types.h>
#include <kf/game/cd.h>
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
struct KfEulerAngles;

typedef struct KfAudioVabStreamSlot {
    s16 state;
    u8 unknown_02[2];
    u8 *buffer;
} KfAudioVabStreamSlot;

typedef char kf_audio_vab_stream_slot_size[
    sizeof(KfAudioVabStreamSlot) == 8 ? 1 : -1];

typedef struct {
    s16 vab_id;
    u8 unknown_02[2];
    KfAudioVabStreamSlot *stream_slot;
} KfAudioVabSlot;

typedef char kf_audio_vab_slot_size[sizeof(KfAudioVabSlot) == 8 ? 1 : -1];

typedef struct {
    s16 voice_id;
    s16 sound_id;
} KfAudioVoiceHandle;

typedef struct {
    s16 vab_slot_index;
    s16 program;
    s16 tone;
    s16 note;
    u16 age;
} KfAudioVoiceParams;

typedef char kf_audio_voice_params_size[sizeof(KfAudioVoiceParams) == 10 ? 1 : -1];

typedef struct {
    KfAudioVoiceHandle handles[10];
    /* Sound IDs are byte-indexed; the loaded parameter rows fill this span. */
    KfAudioVoiceParams params[256];
} KfAudioVoiceState;

/* The startup clear bounds this GAME audio state at 0xe9c bytes. Sequence
 * fields and the 130-entry VAB slot stride have direct retail witnesses. */
typedef struct {
    u_long *sequence_buffer;
    s16 sequence_id;
    u8 unknown_06[2];
    s32 sequence_active;
    s32 sequence_ready;
    VECTOR listener_position;
    u16 listener_layer;
    SVECTOR listener_rotation;
    u8 unknown_2a[2];
    KfAudioVabSlot vab_slots[130];
    KfAudioVoiceState voices;
    KfAudioVabStreamSlot vab_stream_slots[7];
} KfGameAudioState;

typedef char kf_game_audio_state_size[sizeof(KfGameAudioState) == 0xe9c ? 1 : -1];
typedef char kf_game_audio_vab_offset[
    (u32)&((KfGameAudioState *)0)->vab_slots == 0x2c ? 1 : -1];
typedef char kf_game_audio_listener_position_offset[
    (u32)&((KfGameAudioState *)0)->listener_position == 0x10 ? 1 : -1];
typedef char kf_game_audio_listener_layer_offset[
    (u32)&((KfGameAudioState *)0)->listener_layer == 0x20 ? 1 : -1];
typedef char kf_game_audio_listener_rotation_offset[
    (u32)&((KfGameAudioState *)0)->listener_rotation == 0x22 ? 1 : -1];
typedef char kf_game_audio_handles_offset[
    (u32)&((KfGameAudioState *)0)->voices.handles == 0x43c ? 1 : -1];
typedef char kf_game_audio_params_offset[
    (u32)&((KfGameAudioState *)0)->voices.params == 0x464 ? 1 : -1];
typedef char kf_game_audio_params_236_offset[
    (u32)&((KfGameAudioState *)0)->voices.params[236].vab_slot_index == 0xd9c ? 1 : -1];
typedef char kf_game_audio_params_239_offset[
    (u32)&((KfGameAudioState *)0)->voices.params[239].vab_slot_index == 0xdba ? 1 : -1];
typedef char kf_game_audio_stream_slots_offset[
    (u32)&((KfGameAudioState *)0)->vab_stream_slots == 0xe64 ? 1 : -1];

extern KfGameAudioState audio_state;

void func_800139c4(void);
void audio_shutdown(void);
void audio_start_sequence(void);
void audio_stop_sequence(void);
KfAudioPlaybackResult audio_play_spatial(
    s32 sound, const VECTOR *position, s32 volume, s32 max_distance,
    s32 attenuation_distance, s32 note_offset);
KfAudioPlaybackResult audio_play_spatial_default_range(
    s32 sound, const VECTOR *position, s16 volume, s32 note_offset);
KfAudioPlaybackResult audio_play_spatial_range(
    s32 sound, const VECTOR *position, s16 volume, s32 max_distance,
    s32 attenuation_distance, s32 note_offset);
void audio_play_sound(s32 sound, s32 volume);
void audio_update_listener(const VECTOR *position, const SVECTOR *rotation);
void audio_play_sound_64(void);
void audio_play_sound_at_volume_100(s32 sound);
s32 func_80045e5c(const VECTOR *position, const struct KfEulerAngles *angles);
void audio_refresh_voice_handles(void);
void audio_key_off_handle(KfAudioVoiceHandle *handle);
KfAudioVoiceHandle *audio_allocate_voice_handle(s32 sound_id);
void audio_key_on(s32 sound, s32 left_volume, s32 right_volume, s32 note_offset);
KfAudioVabStreamSlot *audio_acquire_vab_stream_slot(void);
void audio_vab_stream_callback(KfCdRequest *request);
void audio_queue_vab_stream(s32 archive_slot, s32 entry, s32 vab_slot_index);

#endif

#ifndef KF_GAME_EFFECT_H
#define KF_GAME_EFFECT_H

#include <kf/lib/types.h>
#include <kf/game/audio.h>
#include <psyq/sdk.h>

enum {
    KF_EFFECT_CAPACITY = 128,
    KF_MAGIC_RECORD_COUNT = 64,
    KF_EFFECT_SLOT_FREE = 0xff,
    KF_EFFECT_USE_PLAYER_MAGIC = 0x10
};

/* The pool scan and reset visit 128 records at a 72-byte stride. */
typedef struct KfEffectRecord {
    u8 type;
    u8 kind;
    u8 base_render_id;
    u8 render_id;
    u8 animation_clip;
    u8 unknown_05;
    u8 unknown_06;
    u8 phase;
    u8 unknown_08;
    u8 unknown_09;
    u8 unknown_0a;
    u8 cooldown;
    u8 unknown_0c;
    u8 unknown_0d;
    s16 updates_remaining;
    s16 unknown_10;
    u16 unknown_12;
    VECTOR position;
    SVECTOR rotation;
    u16 scale_x;
    u16 scale_y;
    u16 scale_z;
    u8 unknown_32[2];
    SVECTOR direction;
    u8 unknown_3c[12];
} KfEffectRecord;

typedef char kf_effect_record_size[sizeof(KfEffectRecord) == 72 ? 1 : -1];
typedef char kf_effect_position_offset[(u32)&((KfEffectRecord *)0)->position == 0x14 ? 1 : -1];
typedef char kf_effect_scale_offset[(u32)&((KfEffectRecord *)0)->scale_x == 0x2c ? 1 : -1];
typedef char kf_effect_direction_offset[(u32)&((KfEffectRecord *)0)->direction == 0x34 ? 1 : -1];

/* Kind 6 copies a position and rotation into each 24-byte trail row. */
typedef struct KfEffectTrailRow {
    VECTOR position;
    SVECTOR rotation;
} KfEffectTrailRow;

typedef char kf_effect_trail_row_size[sizeof(KfEffectTrailRow) == 24 ? 1 : -1];
typedef char kf_effect_trail_rotation_offset[(u32)&((KfEffectTrailRow *)0)->rotation == 16 ? 1 : -1];

/* The effect sweep indexes this 26-byte row family by the record kind. */
typedef struct KfMagicRecord {
    u8 menu_available;
    u8 unknown_01[3];
    u8 unknown_04;
    u8 unknown_05;
    u16 unknown_06;
    u16 unknown_08;
    u16 unknown_0a;
    u16 unknown_0c;
    u16 unknown_0e;
    u16 unknown_10;
    u16 unknown_12;
    u16 unknown_14;
    u16 mp_cost;
    u8 unknown_18[2];
} KfMagicRecord;

typedef char kf_magic_record_size[sizeof(KfMagicRecord) == 26 ? 1 : -1];
typedef char kf_magic_record_mp_cost_offset[(u32)&((KfMagicRecord *)0)->mp_cost == 0x16 ? 1 : -1];

/* game_main_loop clears this complete region at startup. */
typedef struct KfEffectState {
    KfMagicRecord magic_records[KF_MAGIC_RECORD_COUNT];
    KfEffectRecord records[KF_EFFECT_CAPACITY];
    KfMagicRecord *current_magic;
    KfEffectRecord *current_record;
    s32 current_index;
} KfEffectState;

typedef char kf_effect_state_size[sizeof(KfEffectState) == 0x2a8c ? 1 : -1];
typedef char kf_effect_records_offset[(u32)&((KfEffectState *)0)->records == 0x680 ? 1 : -1];
typedef char kf_effect_current_magic_offset[(u32)&((KfEffectState *)0)->current_magic == 0x2a80 ? 1 : -1];
typedef char kf_effect_current_index_offset[(u32)&((KfEffectState *)0)->current_index == 0x2a88 ? 1 : -1];

extern KfEffectState effect_state;

int effect_magic_power(KfEffectRecord *effect);
s32 func_8003fa68(const VECTOR *position, s32 arg1, s32 angle);
s32 func_8004177c(s32 scale, s32 max_length, s32 probe_radius, s32 probe_angle,
                  SVECTOR *motion);
KfAudioPlaybackResult effect_play_spatial_sound(KfEffectRecord *effect, s32 sound);
KfEffectRecord *effect_pool_find_free(void);
KfEffectRecord *func_80040308(u8 id, u8 type, u8 kind, const VECTOR *position,
                              const SVECTOR *direction, ...);
void func_800400c0(KfEffectRecord *record, s32 mode, VECTOR *output,
                   const SVECTOR *scale);
void func_800401b4(KfEffectRecord *record, s32 mode, VECTOR *position,
                   const SVECTOR *scale);
void effect_pool_initialize_scaled(KfEffectRecord *record, u8 render_id, u16 scale);
void effect_pool_initialize_fixed(KfEffectRecord *record, u8 render_id);
void effect_pool_reset(void);
void effect_rotate_scale_offset_y(const SVECTOR *offset, VECTOR *output, s16 angle, s32 scale);
void magic_load_records(const KfMagicRecord *records);
void effect_pool_sweep(void);
void effect_update_dispatch(void);

#endif

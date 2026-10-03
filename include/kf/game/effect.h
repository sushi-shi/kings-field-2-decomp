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
    u8 damage_multiplier_tenths;
    u8 phase;
    u8 render_flags;
    u8 render_queue_mode;
    u8 map_layer_mask;
    u8 cooldown;
    u8 lighting_override_index;
    u8 midpoint_collision_enabled;
    s16 updates_remaining;
    s16 lighting_blend_q12;
    u16 animation_phase_q12;
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
typedef char kf_effect_damage_multiplier_offset[(u32)&((KfEffectRecord *)0)->damage_multiplier_tenths == 0x06 ? 1 : -1];
typedef char kf_effect_render_flags_offset[(u32)&((KfEffectRecord *)0)->render_flags == 0x08 ? 1 : -1];
typedef char kf_effect_map_layer_mask_offset[(u32)&((KfEffectRecord *)0)->map_layer_mask == 0x0a ? 1 : -1];
typedef char kf_effect_render_queue_mode_offset[(u32)&((KfEffectRecord *)0)->render_queue_mode == 0x09 ? 1 : -1];
typedef char kf_effect_lighting_override_offset[(u32)&((KfEffectRecord *)0)->lighting_override_index == 0x0c ? 1 : -1];
typedef char kf_effect_midpoint_collision_offset[(u32)&((KfEffectRecord *)0)->midpoint_collision_enabled == 0x0d ? 1 : -1];
typedef char kf_effect_lighting_blend_offset[(u32)&((KfEffectRecord *)0)->lighting_blend_q12 == 0x10 ? 1 : -1];
typedef char kf_effect_animation_phase_offset[(u32)&((KfEffectRecord *)0)->animation_phase_q12 == 0x12 ? 1 : -1];
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

/* Kinds 6 and 107 share this view of kind 6's payload at record +0x40. */
typedef struct KfEffectTrailState {
    KfEffectTrailRow *rows;
    u8 frame_index;
    u8 phase_counter;
    u8 actor_index;
    u8 unknown_07;
} KfEffectTrailState;

typedef char kf_effect_trail_state_size[sizeof(KfEffectTrailState) == 8 ? 1 : -1];
typedef char kf_effect_trail_payload_offset[(u32)&((KfEffectRecord *)0)->unknown_3c[4] == 0x40 ? 1 : -1];
typedef char kf_effect_trail_state_frame_offset[(u32)&((KfEffectTrailState *)0)->frame_index == 4 ? 1 : -1];
typedef char kf_effect_trail_state_actor_offset[(u32)&((KfEffectTrailState *)0)->actor_index == 6 ? 1 : -1];

/* Kind 107 follows one kind-6 record at an offset of three frames per row. */
typedef struct KfEffectTrailChildLink {
    u8 parent_index;
    u8 lag_index;
} KfEffectTrailChildLink;

typedef char kf_effect_trail_child_link_size[sizeof(KfEffectTrailChildLink) == 2 ? 1 : -1];
typedef char kf_effect_trail_child_lag_offset[(u32)&((KfEffectTrailChildLink *)0)->lag_index == 1 ? 1 : -1];

/* The effect sweep indexes this 26-byte row family by the record kind. */
typedef struct KfMagicRecord {
    u8 menu_available;
    u8 unknown_01[3];
    u8 player_status_flags;
    u8 unknown_05;
    u16 damage_components[8];
    u16 mp_cost;
    u8 unknown_18[2];
} KfMagicRecord;

typedef char kf_magic_record_size[sizeof(KfMagicRecord) == 26 ? 1 : -1];
typedef char kf_magic_record_damage_components_offset[(u32)&((KfMagicRecord *)0)->damage_components == 0x06 ? 1 : -1];
typedef char kf_magic_record_status_flags_offset[(u32)&((KfMagicRecord *)0)->player_status_flags == 0x04 ? 1 : -1];
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
void effect_dispatch_magic_impact(s32 kind, s32 record_type, s32 radius, u16 power,
                   u8 damage_multiplier_tenths, u16 magic_06, u16 magic_08, u16 magic_0a,
                   u16 magic_04, u16 magic_0c, u16 magic_0e, u16 magic_10,
                   u16 magic_12, u16 magic_14, const VECTOR *position);
s32 effect_probe_collision_by_type(const VECTOR *position, s32 radius,
    s32 height_flags);
void effect_apply_current_magic(s32 kind, s32 radius, const VECTOR *position);
void effect_apply_current_magic_backstep(s32 kind);
void effect_apply_radial_magic_damage(VECTOR *position, s32 start, s32 end,
                                      s32 arg3, s32 arg4, s32 arg5);
s32 effect_move_probe(s32 scale, s32 max_length, s32 probe_radius,
                      s32 probe_angle, SVECTOR *motion);
s32 effect_aim_and_move(s32 max_length, s32 scale, s32 turn_step,
                        s32 probe_radius, s32 probe_angle, s32 proximity,
                        s32 close_scale, s32 target_filter);
s32 effect_target_motion(const VECTOR *target, s32 max_length, s32 scale,
                         s32 settle_distance, s32 min_distance,
                         s32 probe_radius, s32 probe_angle);
void effect_scale_step(s32 multiplier, s32 limit, s32 increment,
                       s32 arg3, s32 arg5);
void effect_spawn_zero_direction(KfEffectRecord *record, s32 mode);
s32 effect_spawn_at_lower_bound(const VECTOR *position, s32 arg1, s32 arg2,
                                s32 vertical_window);
void effect_spawn_motion(KfEffectRecord *record, s32 position_mode,
                   s32 motion_mode, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, ...);
s32 effect_scatter_lower_bound(const VECTOR *origin, s32 count, s32 spread,
                  s32 scale_x, s32 scale_z, s32 variation);
s32 effect_collision_step(s32 radius, s32 angle, s32 step);
void effect_collision_backtrack(void);
void effect_spawn_radial_ring(s32 count, s32 radius, s32 vertical_angle, s32 arg3);
KfAudioPlaybackResult effect_play_spatial_sound(KfEffectRecord *effect, s32 sound);
KfEffectRecord *effect_pool_find_free(void);
KfEffectRecord *effect_construct_record(u8 damage_multiplier_tenths, u8 type, u8 kind, const VECTOR *position,
                              const SVECTOR *direction, ...);
void effect_sample_rotated_vertex(KfEffectRecord *record, s32 mode, VECTOR *output,
                   const SVECTOR *scale);
void effect_sample_world_vertex(KfEffectRecord *record, s32 mode, VECTOR *position,
                   const SVECTOR *scale);
void effect_pool_initialize_scaled(KfEffectRecord *record, u8 render_id, u16 scale);
void effect_pool_initialize_fixed(KfEffectRecord *record, u8 render_id);
void effect_pool_reset(void);
void effect_rotate_scale_offset_y(const SVECTOR *offset, VECTOR *output, s16 angle, s32 scale);
void magic_load_records(const KfMagicRecord *records);
void effect_pool_sweep(void);
void effect_update_dispatch(void);

#endif

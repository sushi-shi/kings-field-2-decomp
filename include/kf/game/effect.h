#ifndef KF_GAME_EFFECT_H
#define KF_GAME_EFFECT_H

#include <kf/lib/types.h>
#include <kf/game/audio.h>
#include <psyq/sdk.h>

struct KfPoolRecord;

enum {
    KF_EFFECT_CAPACITY = 128,
    KF_MAGIC_RECORD_COUNT = 64,
    KF_EFFECT_SLOT_FREE = 0xff,
    KF_EFFECT_USE_PLAYER_MAGIC = 0x10,
    KF_EFFECT_KIND_DEFENSE_BOOST = 15,
    KF_EFFECT_KIND_ATTACK_BOOST = 17,
    KF_EFFECT_STATIC_OBJECT_ZERO = 0x80,
    KF_EFFECT_RENDER_TRANSFORM_MASK = 0x0c,
    KF_EFFECT_RENDER_SCREEN_SPACE = 0x0c
};

typedef union KfEffectScaleThreshold {
    s16 interpolation_start_y;
    s16 next_probe_phase;
} KfEffectScaleThreshold;

typedef char kf_effect_scale_threshold_size[sizeof(KfEffectScaleThreshold) == 2 ? 1 : -1];

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
typedef char kf_effect_trail_state_frame_offset[(u32)&((KfEffectTrailState *)0)->frame_index == 4 ? 1 : -1];
typedef char kf_effect_trail_state_actor_offset[(u32)&((KfEffectTrailState *)0)->actor_index == 6 ? 1 : -1];

typedef struct KfEffectKind102Payload {
    s16 amplitude;
} KfEffectKind102Payload;

typedef char kf_effect_kind102_payload_size[sizeof(KfEffectKind102Payload) == 2 ? 1 : -1];

/* Ballistic kinds keep the launch Y and elapsed update count in the payload. */
typedef struct KfEffectBallisticState {
    s16 origin_y;
    s16 age;
} KfEffectBallisticState;

typedef char kf_effect_ballistic_state_size[sizeof(KfEffectBallisticState) == 4 ? 1 : -1];
typedef char kf_effect_ballistic_age_offset[(u32)&((KfEffectBallisticState *)0)->age == 2 ? 1 : -1];

/* Kind 23 tracks an actor vertex until its animation reaches a phase threshold. */
typedef struct KfEffectKind23Attachment {
    s16 actor_index;
    s16 vertex_index;
    u16 release_animation_phase;
} KfEffectKind23Attachment;

typedef char kf_effect_kind23_attachment_size[sizeof(KfEffectKind23Attachment) == 6 ? 1 : -1];
typedef char kf_effect_kind23_vertex_offset[(u32)&((KfEffectKind23Attachment *)0)->vertex_index == 2 ? 1 : -1];
typedef char kf_effect_kind23_phase_offset[(u32)&((KfEffectKind23Attachment *)0)->release_animation_phase == 4 ? 1 : -1];

/* Kind 46 tracks a linked effect before fading its captured Y scale. */
typedef struct KfEffectKind46State {
    s8 phase;
    s8 linked_effect_index;
    u16 age_q12;
} KfEffectKind46State;

typedef char kf_effect_kind46_state_size[sizeof(KfEffectKind46State) == 4 ? 1 : -1];
typedef char kf_effect_kind46_age_offset[(u32)&((KfEffectKind46State *)0)->age_q12 == 2 ? 1 : -1];

/* Kind 5 fans out into kind-105 children attached to one actor. */
typedef struct KfEffectKind5Fanout {
    u8 initial_child_count;
    u8 actor_index;
    s16 children_remaining;
} KfEffectKind5Fanout;

typedef char kf_effect_kind5_fanout_size[sizeof(KfEffectKind5Fanout) == 4 ? 1 : -1];
typedef char kf_effect_kind5_remaining_offset[(u32)&((KfEffectKind5Fanout *)0)->children_remaining == 2 ? 1 : -1];

/* Kind 111 follows an actor, with 0xff selecting the effect's own position. */
typedef struct KfEffectKind111Target {
    s16 actor_index;
} KfEffectKind111Target;

typedef char kf_effect_kind111_target_size[sizeof(KfEffectKind111Target) == 2 ? 1 : -1];

/* Kinds 103 and 121 decrement this count between fanout spawns. */
typedef struct KfEffectKind103Counter {
    u16 remaining;
} KfEffectKind103Counter;

typedef char kf_effect_kind103_counter_size[sizeof(KfEffectKind103Counter) == 2 ? 1 : -1];

/* Kind 2 grows a radial effect until it reaches the requested scale. */
typedef struct KfEffectKind2Scale {
    u16 max_scale;
    u16 scale_step;
    u16 radial_damage_parameter;
} KfEffectKind2Scale;

typedef char kf_effect_kind2_scale_size[sizeof(KfEffectKind2Scale) == 6 ? 1 : -1];
typedef char kf_effect_kind2_step_offset[(u32)&((KfEffectKind2Scale *)0)->scale_step == 2 ? 1 : -1];
typedef char kf_effect_kind2_damage_offset[(u32)&((KfEffectKind2Scale *)0)->radial_damage_parameter == 4 ? 1 : -1];

/* Several collision effects apply the current magic once per contact. */
typedef struct KfEffectCollisionLatch {
    u8 impact_handled;
} KfEffectCollisionLatch;

typedef char kf_effect_collision_latch_size[sizeof(KfEffectCollisionLatch) == 1 ? 1 : -1];

/* Kind 42 counts down before its scale step. */
typedef struct KfEffectKind42Countdown {
    s8 ticks_remaining;
} KfEffectKind42Countdown;

typedef char kf_effect_kind42_countdown_size[sizeof(KfEffectKind42Countdown) == 1 ? 1 : -1];

/* Kinds 1 and 28 advance through initial, collision, and fade stages. */
typedef struct KfEffectKind1Stage {
    u8 collision_stage;
} KfEffectKind1Stage;

typedef char kf_effect_kind1_stage_size[sizeof(KfEffectKind1Stage) == 1 ? 1 : -1];

/* Kind 107 follows one kind-6 record at an offset of three frames per row. */
typedef struct KfEffectTrailChildLink {
    u8 parent_index;
    u8 lag_index;
} KfEffectTrailChildLink;

typedef char kf_effect_trail_child_link_size[sizeof(KfEffectTrailChildLink) == 2 ? 1 : -1];
typedef char kf_effect_trail_child_lag_offset[(u32)&((KfEffectTrailChildLink *)0)->lag_index == 1 ? 1 : -1];

/* Kind 8 follows a parent effect and applies a per-tick vertical step. */
typedef struct KfEffectKind8State {
    u8 parent_index;
    u8 unknown_01;
    u16 vertical_step;
} KfEffectKind8State;

typedef char kf_effect_kind8_state_size[sizeof(KfEffectKind8State) == 4 ? 1 : -1];
typedef char kf_effect_kind8_vertical_step_offset[(u32)&((KfEffectKind8State *)0)->vertical_step == 2 ? 1 : -1];

/* Kind 9 targets an actor; 0xfe selects the player camera, 0xff no target. */
enum {
    KF_EFFECT_KIND9_TARGET_PLAYER = 0xfe,
    KF_EFFECT_KIND9_TARGET_NONE = 0xff
};

typedef struct KfEffectKind9Target {
    u8 actor_index;
} KfEffectKind9Target;

typedef char kf_effect_kind9_target_size[sizeof(KfEffectKind9Target) == 1 ? 1 : -1];

/* Kind 109 follows another record in the effect pool. */
typedef struct KfEffectKind109Target {
    u8 effect_index;
} KfEffectKind109Target;

typedef char kf_effect_kind109_target_size[sizeof(KfEffectKind109Target) == 1 ? 1 : -1];

typedef struct KfEffectKind101Motion {
    u16 scale_step;
    u16 vertical_step;
} KfEffectKind101Motion;

typedef char kf_effect_kind101_motion_size[sizeof(KfEffectKind101Motion) == 4 ? 1 : -1];
typedef char kf_effect_kind101_vertical_step_offset[(u32)&((KfEffectKind101Motion *)0)->vertical_step == 2 ? 1 : -1];

typedef struct KfEffectKind10Targeting {
    u8 actor_index;
    u8 emissions_remaining;
} KfEffectKind10Targeting;

typedef char kf_effect_kind10_targeting_size[sizeof(KfEffectKind10Targeting) == 2 ? 1 : -1];

/* Kind 105 follows an animation vertex and reports to its parent effect. */
typedef struct KfEffectKind105Attachment {
    u8 parent_index;
    u8 actor_index;
    s16 vertex_index;
} KfEffectKind105Attachment;

typedef char kf_effect_kind105_attachment_size[sizeof(KfEffectKind105Attachment) == 4 ? 1 : -1];
typedef char kf_effect_kind105_vertex_offset[(u32)&((KfEffectKind105Attachment *)0)->vertex_index == 2 ? 1 : -1];

typedef struct KfEffectKind12Aim {
    s16 max_length;
    s16 scale;
    s16 turn_step;
    s16 close_scale;
} KfEffectKind12Aim;

typedef char kf_effect_kind12_aim_size[sizeof(KfEffectKind12Aim) == 8 ? 1 : -1];
typedef char kf_effect_kind12_close_scale_offset[(u32)&((KfEffectKind12Aim *)0)->close_scale == 6 ? 1 : -1];

typedef union KfEffectKindPayload {
    u8 raw[8];
    KfEffectTrailState trail;
    KfEffectKind102Payload kind102;
    KfEffectBallisticState ballistic;
    KfEffectKind23Attachment kind23;
    KfEffectKind46State kind46;
    KfEffectKind5Fanout kind5;
    KfEffectKind111Target kind111;
    KfEffectKind103Counter kind103;
    KfEffectKind2Scale kind2;
    KfEffectCollisionLatch collision_latch;
    KfEffectKind42Countdown kind42;
    KfEffectKind1Stage kind1;
    KfEffectTrailChildLink trail_child;
    KfEffectKind8State kind8;
    KfEffectKind9Target kind9;
    KfEffectKind109Target kind109;
    KfEffectKind101Motion kind101;
    KfEffectKind10Targeting kind10;
    KfEffectKind105Attachment kind105;
    KfEffectKind12Aim kind12;
} KfEffectKindPayload;

typedef char kf_effect_kind_payload_size[sizeof(KfEffectKindPayload) == 8 ? 1 : -1];

/* Renderer-owned cache slot followed by the effect kind's variant payload. */
typedef struct KfEffectCacheTail {
    struct KfPoolRecord *animation_cache;
    KfEffectKindPayload payload;
} KfEffectCacheTail;

typedef char kf_effect_cache_tail_size[sizeof(KfEffectCacheTail) == 12 ? 1 : -1];
typedef char kf_effect_cache_payload_offset[(u32)&((KfEffectCacheTail *)0)->payload == 4 ? 1 : -1];

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
    KfEffectScaleThreshold scale_threshold;
    SVECTOR direction;
    KfEffectCacheTail cache_tail;
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
typedef char kf_effect_scale_threshold_offset[(u32)&((KfEffectRecord *)0)->scale_threshold == 0x32 ? 1 : -1];
typedef char kf_effect_direction_offset[(u32)&((KfEffectRecord *)0)->direction == 0x34 ? 1 : -1];
typedef char kf_effect_cache_tail_offset[(u32)&((KfEffectRecord *)0)->cache_tail == 0x3c ? 1 : -1];

typedef char kf_effect_trail_payload_offset[(u32)&((KfEffectRecord *)0)->cache_tail.payload.trail == 0x40 ? 1 : -1];

/* The effect sweep indexes this 26-byte row family by the record kind. */
typedef struct KfMagicRecord {
    u8 menu_available;
    u8 charge_rate;
    u8 unknown_02[2];
    u8 player_status_flags;
    u8 unknown_05;
    u16 damage_components[8];
    u16 mp_cost;
    u8 unknown_18[2];
} KfMagicRecord;

typedef char kf_magic_record_size[sizeof(KfMagicRecord) == 26 ? 1 : -1];
typedef char kf_magic_record_charge_rate_offset[(u32)&((KfMagicRecord *)0)->charge_rate == 1 ? 1 : -1];
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

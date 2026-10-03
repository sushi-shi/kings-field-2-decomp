#ifndef KF_GAME_MAP_OBJECT_H
#define KF_GAME_MAP_OBJECT_H

#include <kf/lib/types.h>
#include <kf/game/audio.h>

enum {
    KF_MAP_OBJECT_ACTION_NONE = 0xff,
    KF_MAP_OBJECT_ACTION_TIMER_INIT = 0,
    KF_MAP_OBJECT_ID_NONE = 0xff,
    KF_MAP_OBJECT_SPAWN_SEQUENCE_MODULUS = 0x10000,
    KF_MAP_OBJECT_TEMPLATE_CAPACITY = 320,
    KF_MAP_OBJECT_CAPACITY = 0x18c
};

typedef struct KfMapObjectTemplate {
    u8 collision_kind;
    u8 kind;
    u8 unknown_02[2];
    u16 collision_radius;
    u16 interaction_radius;
    u16 interaction_height;
    u16 unknown_0a;
    u8 marker_action_05;
    u8 unknown_0d[10];
    u8 marker_action_51;
} KfMapObjectTemplate;

typedef char kf_map_object_template_size[sizeof(KfMapObjectTemplate) == 24 ? 1 : -1];
typedef char kf_map_object_template_radius_offset[
    (u32)&((KfMapObjectTemplate *)0)->collision_radius == 4 ? 1 : -1];
typedef char kf_map_object_template_interaction_radius_offset[
    (u32)&((KfMapObjectTemplate *)0)->interaction_radius == 6 ? 1 : -1];
typedef char kf_map_object_template_interaction_height_offset[
    (u32)&((KfMapObjectTemplate *)0)->interaction_height == 8 ? 1 : -1];
typedef char kf_map_object_template_unknown_0a_offset[
    (u32)&((KfMapObjectTemplate *)0)->unknown_0a == 0x0a ? 1 : -1];

/* The scene pose path reads two signed offsets through the same template bytes
 * used by marker actions. Keep both interpretations of the 24-byte record. */
typedef struct KfMapObjectTemplatePoseView {
    u8 unknown_00[0x0c];
    s16 height_offset;
    s16 depth_offset;
    u16 unknown_10;
    u8 unknown_12[6];
} KfMapObjectTemplatePoseView;

typedef char kf_map_object_template_pose_size[
    sizeof(KfMapObjectTemplatePoseView) == sizeof(KfMapObjectTemplate) ? 1 : -1];
typedef char kf_map_object_template_pose_height_offset[
    (u32)&((KfMapObjectTemplatePoseView *)0)->height_offset == 0x0c ? 1 : -1];
typedef char kf_map_object_template_pose_depth_offset[
    (u32)&((KfMapObjectTemplatePoseView *)0)->depth_offset == 0x0e ? 1 : -1];
typedef char kf_map_object_template_pose_unknown_10_offset[
    (u32)&((KfMapObjectTemplatePoseView *)0)->unknown_10 == 0x10 ? 1 : -1];

/* The placement's final two words copy together into the object tail. */
typedef struct KfMapObjectTailCopyWords {
    u32 first;
    u32 second;
} KfMapObjectTailCopyWords;
typedef char kf_map_object_tail_copy_words_size[
    sizeof(KfMapObjectTailCopyWords) == 8 ? 1 : -1];

/* Map resource placements consumed in 24-byte rows by map_object_initialize_from_placements. */
typedef struct KfMapObjectPlacement {
    u8 layer_mask;
    u8 region_z;
    u8 region_x;
    u8 unknown_03;
    u16 object_id;
    s16 rotation_y;
    s16 local_z;
    s16 local_x;
    s16 height;
    u16 unknown_0e;
    KfMapObjectTailCopyWords tail_words;
} KfMapObjectPlacement;

typedef char kf_map_object_placement_size[
    sizeof(KfMapObjectPlacement) == 24 ? 1 : -1];
typedef char kf_map_object_placement_height_offset[
    (u32)&((KfMapObjectPlacement *)0)->height == 12 ? 1 : -1];
typedef char kf_map_object_placement_tail_words_offset[
    (u32)&((KfMapObjectPlacement *)0)->tail_words == 16 ? 1 : -1];

typedef struct KfMapObjectTailHalfwordBytes {
    u8 low;
    u8 high;
} KfMapObjectTailHalfwordBytes;

typedef union KfMapObjectTailHalfword {
    u16 value;
    s16 signed_value;
    KfMapObjectTailHalfwordBytes bytes;
} KfMapObjectTailHalfword;

typedef struct KfMapObjectTailFields {
    u32 unknown_34;
    u8 unknown_38;
    u8 unknown_39;
    KfMapObjectTailHalfword unknown_3a;
    u16 spawn_sequence;
    KfMapObjectTailHalfword unknown_3e;
} KfMapObjectTailFields;

/* Event archive commands read the two state bytes at +0x38 as one halfword. */
typedef struct KfMapObjectTailPair38View {
    u32 unknown_34;
    u16 value_38;
    u8 unknown_3a[6];
} KfMapObjectTailPair38View;

typedef char kf_map_object_tail_pair38_size[
    sizeof(KfMapObjectTailPair38View) == 12 ? 1 : -1];
typedef char kf_map_object_tail_pair38_offset[
    (u32)&((KfMapObjectTailPair38View *)0)->value_38 == 4 ? 1 : -1];

/* Action 0xf0 uses the two copied tail bytes as a view-cell radius. */
typedef struct KfMapObjectTailVisibilityView {
    u32 unknown_34;
    u8 radius_x;
    u8 radius_z;
    u8 unknown_3a[6];
} KfMapObjectTailVisibilityView;
typedef char kf_map_object_tail_visibility_size[
    sizeof(KfMapObjectTailVisibilityView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_visibility_radius_x_offset[
    (u32)&((KfMapObjectTailVisibilityView *)0)->radius_x == 4 ? 1 : -1];
typedef char kf_map_object_tail_visibility_radius_z_offset[
    (u32)&((KfMapObjectTailVisibilityView *)0)->radius_z == 5 ? 1 : -1];

typedef struct KfMapObjectTailSpawnByteFields {
    u32 unknown_34;
    u8 unknown_38;
    u8 unknown_39;
    KfMapObjectTailHalfword unknown_3a;
    KfMapObjectTailHalfwordBytes spawn_sequence;
    KfMapObjectTailHalfword unknown_3e;
} KfMapObjectTailSpawnByteFields;
typedef char kf_map_object_tail_spawn_byte_fields_size[
    sizeof(KfMapObjectTailSpawnByteFields) == 12 ? 1 : -1];
typedef char kf_map_object_tail_spawn_bytes_offset[
    (u32)&((KfMapObjectTailSpawnByteFields *)0)->spawn_sequence == 8 ? 1 : -1];

typedef union KfMapObjectTail {
    KfMapObjectTailFields fields;
    KfMapObjectTailPair38View pair_38;
    KfMapObjectTailVisibilityView visibility;
    KfMapObjectTailSpawnByteFields spawn_bytes;
    u32 reset_words[3];
    struct {
        u32 unknown_34;
        KfMapObjectTailCopyWords copy_words;
    } placement;
} KfMapObjectTail;
typedef char kf_map_object_tail_size[sizeof(KfMapObjectTail) == 12 ? 1 : -1];

/* The +0x40 word is a pointer in the player reaction path and byte state in
 * map-object motion. The pointed object's complete extent is unresolved. */
typedef struct KfMapObjectRecord40 KfMapObjectRecord40;

typedef union KfMapObjectExtra40 {
    KfMapObjectRecord40 *record;
    u32 raw;
    u8 bytes[4];
    u16 object_index;
    u16 halfwords[2];
    s16 signed_halfwords[2];
} KfMapObjectExtra40;

typedef char kf_map_object_extra40_size[sizeof(KfMapObjectExtra40) == 4 ? 1 : -1];

/* The map-object pool is traversed in 0x44-byte records. */
typedef struct KfMapObject {
    u8 layer_mask;
    u8 asset_clip_selector;
    u8 render_queue_mode;
    u8 collision_flags;
    u8 action;
    u8 lighting_override_index;
    u16 object_id;
    u16 action_timer;
    u16 phase_q12;
    u16 collision_height;
    s16 unknown_0e;
    u16 lighting_blend_q12;
    u8 unknown_12[2];
    VECTOR position;
    SVECTOR rotation;
    SVECTOR scale;
    KfMapObjectTail tail;
    KfMapObjectExtra40 extra_40;
} KfMapObject;

typedef char kf_map_object_size[sizeof(KfMapObject) == 0x44 ? 1 : -1];
typedef char kf_map_object_asset_clip_selector_offset[
    (u32)&((KfMapObject *)0)->asset_clip_selector == 1 ? 1 : -1];
typedef char kf_map_object_action_offset[(u32)&((KfMapObject *)0)->action == 4 ? 1 : -1];
typedef char kf_map_object_render_queue_mode_offset[
    (u32)&((KfMapObject *)0)->render_queue_mode == 2 ? 1 : -1];
typedef char kf_map_object_lighting_override_index_offset[
    (u32)&((KfMapObject *)0)->lighting_override_index == 5 ? 1 : -1];
typedef char kf_map_object_action_timer_offset[(u32)&((KfMapObject *)0)->action_timer == 8 ? 1 : -1];
typedef char kf_map_object_phase_q12_offset[
    (u32)&((KfMapObject *)0)->phase_q12 == 0x0a ? 1 : -1];
typedef char kf_map_object_collision_height_offset[
    (u32)&((KfMapObject *)0)->collision_height == 0x0c ? 1 : -1];
typedef char kf_map_object_lighting_blend_q12_offset[
    (u32)&((KfMapObject *)0)->lighting_blend_q12 == 0x10 ? 1 : -1];
typedef char kf_map_object_position_offset[(u32)&((KfMapObject *)0)->position == 0x14 ? 1 : -1];
typedef char kf_map_object_rotation_offset[(u32)&((KfMapObject *)0)->rotation == 0x24 ? 1 : -1];
typedef char kf_map_object_scale_offset[(u32)&((KfMapObject *)0)->scale == 0x2c ? 1 : -1];
typedef char kf_map_object_tail_offset[(u32)&((KfMapObject *)0)->tail == 0x34 ? 1 : -1];
typedef char kf_map_object_spawn_sequence_offset[
    (u32)&((KfMapObject *)0)->tail.fields.spawn_sequence == 0x3c ? 1 : -1];
typedef char kf_map_object_record40_offset[
    (u32)&((KfMapObject *)0)->extra_40 == 0x40 ? 1 : -1];

/* game_main_loop clears the whole region containing the 0x18c-object pool. */
typedef struct KfMapObjectStateGame {
    KfMapObjectTemplate templates[KF_MAP_OBJECT_TEMPLATE_CAPACITY];
    KfMapObject objects[KF_MAP_OBJECT_CAPACITY];
    u8 unknown_8730[4];
    KfMapObjectTemplate *current_template;
    KfMapObject *current_collision_object;
    u8 unknown_873c[2];
    u16 spawn_sequence_pool_15e;
    u16 spawn_sequence_pool_168;
    u16 spawn_sequence_pool_172;
} KfMapObjectStateGame;

typedef char kf_map_object_state_size[sizeof(KfMapObjectStateGame) == 0x8744 ? 1 : -1];
typedef char kf_map_object_state_objects_offset[(u32)&((KfMapObjectStateGame *)0)->objects == 0x1e00 ? 1 : -1];
typedef char kf_map_object_state_current_template_offset[
    (u32)&((KfMapObjectStateGame *)0)->current_template == 0x8734 ? 1 : -1];
typedef char kf_map_object_state_current_collision_offset[
    (u32)&((KfMapObjectStateGame *)0)->current_collision_object == 0x8738 ? 1 : -1];
typedef char kf_map_object_state_counter_873e_offset[
    (u32)&((KfMapObjectStateGame *)0)->spawn_sequence_pool_15e == 0x873e ? 1 : -1];
typedef char kf_map_object_state_counter_8742_offset[
    (u32)&((KfMapObjectStateGame *)0)->spawn_sequence_pool_172 == 0x8742 ? 1 : -1];

extern KfMapObjectStateGame map_object_state;

void map_object_start_action_if_idle(KfMapObject *object, u8 action);
void map_object_initialize_from_placements(const KfMapObjectPlacement *placements);
s32 map_object_find_collision_at_point(s32 x, s32 y, s32 z, s32 radius, s32 height);
KfMapObject *map_object_effect_pool_acquire(s32 first_index, s32 count, s32 sequence);
KfAudioPlaybackResult map_object_play_spatial_sound(KfMapObject *object, s32 sound);
void map_object_reset(KfMapObject *object);
void map_object_pool_reset(void);
void map_object_set_property(s32 index, s32 property, ...);
void map_object_set_cell_marker(KfMapObject *object, s32 mode, u8 marker);
void map_object_apply_marker_signal(u8 identifier);
s32 map_object_check_and_consume_marker(KfMapObject *object, s32 marker);
s32 player_camera_within_map_region(s32 x, s32 z, s32 width, s32 depth, s32 height);
s32 map_object_step_offset_motion(KfMapObject *source, KfMapObject *target,
                  SVECTOR *start_offset, SVECTOR *end_offset,
                  s32 brighten, s32 duration);
void map_object_sample_world_vertex(KfMapObject *object, s32 vertex_index, VECTOR *result);
void map_object_spawn_scattered_effect(u16 effect_id, const VECTOR *origin,
                                       s32 height_offset);
void map_object_spawn_effect(u8 source, u8 object_id, const VECTOR *position,
                             s32 height_offset);
void map_object_update_actions(void);
void map_object_refresh_cell_markers(s32 mode);
s32 map_object_find_interaction_target(s32 first_index, const VECTOR *position, s32 radius,
    s32 point_height, s32 angle, s32 tolerance);

#endif

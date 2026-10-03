#ifndef KF_GAME_MAP_OBJECT_H
#define KF_GAME_MAP_OBJECT_H

#include <kf/lib/types.h>
#include <kf/game/audio.h>
#include <kf/game/pool.h>

enum {
    KF_MAP_OBJECT_ACTION_NONE = 0xff,
    KF_MAP_OBJECT_ACTION_TIMER_INIT = 0,
    KF_MAP_OBJECT_ID_NONE = 0xff,
    KF_MAP_OBJECT_INDEX_NONE = 0xffff,
    KF_MAP_OBJECT_STATIC_OBJECT_ZERO = 0x80,
    KF_MAP_OBJECT_SPAWN_SEQUENCE_MODULUS = 0x10000,
    KF_MAP_OBJECT_TEMPLATE_CAPACITY = 320,
    KF_MAP_OBJECT_PLACED_COUNT = 0x15e,
    KF_MAP_OBJECT_SCATTER_POOL_FIRST = 0x15e,
    KF_MAP_OBJECT_DEFINITION_DROP_FIRST = 0x168,
    KF_MAP_OBJECT_PLACEMENT_DROP_FIRST = 0x172,
    KF_MAP_OBJECT_EFFECT_POOL_SIZE = 10,
    KF_MAP_OBJECT_DROP_FROM_PLACEMENT = 0,
    KF_MAP_OBJECT_DROP_FROM_DEFINITION = 1,
    KF_MAP_OBJECT_INTERACTION_ANY_ANGLE = 0x04,
    KF_MAP_REGION_HEIGHT_ANY = 0x8000,
    KF_MAP_OBJECT_CAPACITY = 0x18c
};

enum {
    KF_MAP_OBJECT_PROPERTY_CLEAR_LAYER_AND_STATE = 0,
    KF_MAP_OBJECT_PROPERTY_SET_LAYER_MASK = 1,
    KF_MAP_OBJECT_PROPERTY_SET_RENDER_DEPTH = 3
};

typedef struct KfMapObjectTemplate {
    u8 collision_kind;
    u8 kind;
    u8 vab_resource_index;
    u8 collision_flags;
    u16 collision_radius;
    u16 interaction_radius;
    u16 interaction_height;
    u16 initial_render_depth_offset;
    u8 marker_action_05;
    u8 unknown_0d[2];
    u8 sound_id;
    u8 unknown_10[7];
    u8 marker_action_51;
} KfMapObjectTemplate;

typedef char kf_map_object_template_size[sizeof(KfMapObjectTemplate) == 24 ? 1 : -1];
typedef char kf_map_object_template_vab_resource_index_offset[
    (u32)&((KfMapObjectTemplate *)0)->vab_resource_index == 2 ? 1 : -1];
typedef char kf_map_object_template_collision_flags_offset[
    (u32)&((KfMapObjectTemplate *)0)->collision_flags == 3 ? 1 : -1];
typedef char kf_map_object_template_radius_offset[
    (u32)&((KfMapObjectTemplate *)0)->collision_radius == 4 ? 1 : -1];
typedef char kf_map_object_template_interaction_radius_offset[
    (u32)&((KfMapObjectTemplate *)0)->interaction_radius == 6 ? 1 : -1];
typedef char kf_map_object_template_interaction_height_offset[
    (u32)&((KfMapObjectTemplate *)0)->interaction_height == 8 ? 1 : -1];
typedef char kf_map_object_template_initial_render_depth_offset_offset[
    (u32)&((KfMapObjectTemplate *)0)->initial_render_depth_offset == 0x0a ? 1 : -1];
typedef char kf_map_object_template_sound_id_offset[
    (u32)&((KfMapObjectTemplate *)0)->sound_id == 0x0f ? 1 : -1];

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

/* The collision-probe action interprets the same template bytes as unsigned
 * vertex, reach, and height values. */
typedef struct KfMapObjectTemplateCollisionView {
    u8 unknown_00[0x0c];
    u16 vertex_index;
    u16 reach;
    u16 height;
    u8 impact_magic_values[4];
    u8 sound_id;
    u8 marker_action_51;
} KfMapObjectTemplateCollisionView;

typedef char kf_map_object_template_collision_view_size[
    sizeof(KfMapObjectTemplateCollisionView) == sizeof(KfMapObjectTemplate) ? 1 : -1];
typedef char kf_map_object_template_collision_vertex_offset[
    (u32)&((KfMapObjectTemplateCollisionView *)0)->vertex_index == 0x0c ? 1 : -1];
typedef char kf_map_object_template_collision_reach_offset[
    (u32)&((KfMapObjectTemplateCollisionView *)0)->reach == 0x0e ? 1 : -1];
typedef char kf_map_object_template_collision_height_offset[
    (u32)&((KfMapObjectTemplateCollisionView *)0)->height == 0x10 ? 1 : -1];
typedef char kf_map_object_template_collision_magic_values_offset[
    (u32)&((KfMapObjectTemplateCollisionView *)0)->impact_magic_values == 0x12 ? 1 : -1];
typedef char kf_map_object_template_collision_sound_offset[
    (u32)&((KfMapObjectTemplateCollisionView *)0)->sound_id == 0x16 ? 1 : -1];

/* Rotated map-cell actions use two dimensions and a sound selector from the
 * template bytes that other actions interpret differently. */
typedef struct KfMapObjectTemplateCellActionView {
    u8 unknown_00[0x0d];
    u8 cell_width;
    u8 cell_height;
    u8 sound_id;
    u8 unknown_10[8];
} KfMapObjectTemplateCellActionView;

typedef char kf_map_object_template_cell_action_view_size[
    sizeof(KfMapObjectTemplateCellActionView) == sizeof(KfMapObjectTemplate) ? 1 : -1];
typedef char kf_map_object_template_cell_action_width_offset[
    (u32)&((KfMapObjectTemplateCellActionView *)0)->cell_width == 0x0d ? 1 : -1];
typedef char kf_map_object_template_cell_action_height_offset[
    (u32)&((KfMapObjectTemplateCellActionView *)0)->cell_height == 0x0e ? 1 : -1];
typedef char kf_map_object_template_cell_action_sound_offset[
    (u32)&((KfMapObjectTemplateCellActionView *)0)->sound_id == 0x0f ? 1 : -1];

/* Action 0x54 selects a pair of map-cell patterns, then uses one state bit
 * to choose which of the two rows to apply. */
typedef struct KfMapObjectTemplatePatternView {
    u8 unknown_00[0x0e];
    u8 pattern_pair_index;
    u8 unknown_0f[9];
} KfMapObjectTemplatePatternView;
typedef char kf_map_object_template_pattern_view_size[
    sizeof(KfMapObjectTemplatePatternView) == sizeof(KfMapObjectTemplate) ? 1 : -1];
typedef char kf_map_object_template_pattern_pair_offset[
    (u32)&((KfMapObjectTemplatePatternView *)0)->pattern_pair_index == 0x0e ? 1 : -1];

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
    u16 object_id;
    s16 rotation_y;
    s16 local_z;
    s16 local_x;
    s16 height;
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

/* Actions 0x60 and 0x62 reuse this halfword for vertical and angular motion. */
typedef struct KfMapObjectTailMotionView {
    u32 unknown_34;
    u8 unknown_38;
    u8 unknown_39;
    KfMapObjectTailHalfword unknown_3a;
    u16 unknown_3c;
    KfMapObjectTailHalfword motion_velocity;
} KfMapObjectTailMotionView;
typedef char kf_map_object_tail_motion_velocity_offset[
    (u32)&((KfMapObjectTailMotionView *)0)->motion_velocity == 10 ? 1 : -1];

typedef struct KfMapObjectTailNotificationView {
    u32 unknown_34;
    u8 unknown_38;
    u8 unknown_39;
    KfMapObjectTailHalfword unknown_3a;
    u16 unknown_3c;
    u8 linked_notification;
    u8 default_notification;
} KfMapObjectTailNotificationView;
typedef char kf_map_object_tail_notification_size[
    sizeof(KfMapObjectTailNotificationView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_linked_notification_offset[
    (u32)&((KfMapObjectTailNotificationView *)0)->linked_notification == 10 ? 1 : -1];
typedef char kf_map_object_tail_default_notification_offset[
    (u32)&((KfMapObjectTailNotificationView *)0)->default_notification == 11 ? 1 : -1];

typedef struct KfMapObjectTailTransitionView {
    u32 unknown_34;
    u8 region_x;
    u8 region_z;
    u8 region_width;
    u8 region_depth;
    u8 destination_cell_x;
    u8 destination_cell_z;
    u8 destination_layer_code;
    u8 destination_yaw_code;
} KfMapObjectTailTransitionView;
typedef char kf_map_object_tail_transition_size[
    sizeof(KfMapObjectTailTransitionView) == 12 ? 1 : -1];

typedef struct KfMapObjectTailResourceTriggerView {
    u32 unknown_34;
    u8 region_width;
    u8 region_depth;
    u8 resource_selectors[5];
    u8 unknown_3f;
} KfMapObjectTailResourceTriggerView;
typedef char kf_map_object_tail_resource_trigger_size[
    sizeof(KfMapObjectTailResourceTriggerView) == 12 ? 1 : -1];

typedef struct KfMapObjectTailAnimatedView {
    KfPoolRecord *animation_cache;
    u8 radius_x;
    u8 radius_z;
    u8 depth_code;
    u8 lighting_flags;
    u8 blend_mode;
    u8 unknown_3d;
    u16 unknown_3e;
} KfMapObjectTailAnimatedView;
typedef char kf_map_object_tail_animated_size[
    sizeof(KfMapObjectTailAnimatedView) == 12 ? 1 : -1];

/* Action 0x1f schedules an ambient sound for a rectangular map region. */
typedef struct KfMapObjectTailAmbientSoundView {
    u32 unknown_34;
    u8 region_width;
    u8 region_depth;
    u8 sound_id;
    u8 maximum_volume;
    u8 audible_radius_code;
    u8 vertical_attenuation_flags;
    u16 repeat_delay_units;
} KfMapObjectTailAmbientSoundView;
typedef char kf_map_object_tail_ambient_sound_size[
    sizeof(KfMapObjectTailAmbientSoundView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_ambient_sound_id_offset[
    (u32)&((KfMapObjectTailAmbientSoundView *)0)->sound_id == 6 ? 1 : -1];
typedef char kf_map_object_tail_ambient_repeat_delay_offset[
    (u32)&((KfMapObjectTailAmbientSoundView *)0)->repeat_delay_units == 10 ? 1 : -1];

/* Actions 3 and 4 copy a rotated region between map-cell coordinates. */
typedef struct KfMapObjectTailCellCopyView {
    u32 unknown_34;
    u8 unknown_38;
    u8 destination_x;
    u8 destination_z;
    u8 source_x;
    u8 source_z;
    u8 linked_object_index;
    u16 unknown_3e;
} KfMapObjectTailCellCopyView;
typedef char kf_map_object_tail_cell_copy_size[
    sizeof(KfMapObjectTailCellCopyView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_cell_copy_destination_offset[
    (u32)&((KfMapObjectTailCellCopyView *)0)->destination_x == 5 ? 1 : -1];
typedef char kf_map_object_tail_cell_copy_source_offset[
    (u32)&((KfMapObjectTailCellCopyView *)0)->source_x == 7 ? 1 : -1];
typedef char kf_map_object_tail_cell_copy_link_offset[
    (u32)&((KfMapObjectTailCellCopyView *)0)->linked_object_index == 9 ? 1 : -1];

/* Action 88 stores its copy coordinates and dimensions at different offsets. */
typedef struct KfMapObjectTailAction88CellCopyView {
    u32 unknown_34;
    u8 transition_mode;
    u8 marker_id;
    u8 destination_x;
    u8 destination_z;
    u8 source_x;
    u8 source_z;
    u8 width;
    u8 height;
} KfMapObjectTailAction88CellCopyView;
typedef char kf_map_object_tail_action88_cell_copy_size[
    sizeof(KfMapObjectTailAction88CellCopyView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_action88_mode_offset[
    (u32)&((KfMapObjectTailAction88CellCopyView *)0)->transition_mode == 4 ? 1 : -1];
typedef char kf_map_object_tail_action88_marker_offset[
    (u32)&((KfMapObjectTailAction88CellCopyView *)0)->marker_id == 5 ? 1 : -1];
typedef char kf_map_object_tail_action88_source_offset[
    (u32)&((KfMapObjectTailAction88CellCopyView *)0)->source_x == 8 ? 1 : -1];
typedef char kf_map_object_tail_action88_width_offset[
    (u32)&((KfMapObjectTailAction88CellCopyView *)0)->width == 10 ? 1 : -1];

/* Action 0x59 waits for a marker, then restores its layer for a timed fade. */
typedef struct KfMapObjectTailAction89LayerFadeView {
    u32 unknown_34;
    u8 marker_id;
    u8 unknown_39;
    u16 delay_frames;
    u8 unknown_3c[4];
} KfMapObjectTailAction89LayerFadeView;
typedef char kf_map_object_tail_action89_layer_fade_size[
    sizeof(KfMapObjectTailAction89LayerFadeView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_action89_marker_offset[
    (u32)&((KfMapObjectTailAction89LayerFadeView *)0)->marker_id == 4 ? 1 : -1];
typedef char kf_map_object_tail_action89_delay_offset[
    (u32)&((KfMapObjectTailAction89LayerFadeView *)0)->delay_frames == 6 ? 1 : -1];

/* Action 84 checks a camera region and alternates two pattern rows. */
typedef struct KfMapObjectTailAction84PatternView {
    u32 unknown_34;
    u8 marker_id;
    u8 center_x;
    u8 center_z;
    u8 region_width;
    u8 region_depth;
    u8 depth_code;
    u8 pattern_flags;
    u8 unknown_3f;
} KfMapObjectTailAction84PatternView;
typedef char kf_map_object_tail_action84_pattern_size[
    sizeof(KfMapObjectTailAction84PatternView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_action84_marker_offset[
    (u32)&((KfMapObjectTailAction84PatternView *)0)->marker_id == 4 ? 1 : -1];
typedef char kf_map_object_tail_action84_region_width_offset[
    (u32)&((KfMapObjectTailAction84PatternView *)0)->region_width == 7 ? 1 : -1];
typedef char kf_map_object_tail_action84_pattern_flags_offset[
    (u32)&((KfMapObjectTailAction84PatternView *)0)->pattern_flags == 10 ? 1 : -1];

/* Action 81 uses a camera gate and dispatches a magic impact on collision. */
typedef struct KfMapObjectTailCollisionProbeView {
    u32 unknown_34;
    u8 unknown_38;
    u8 damage_multiplier_tenths;
    u8 phase_step_code;
    u8 camera_region_width;
    u8 camera_region_depth;
    u8 unknown_3d;
    u16 unknown_3e;
} KfMapObjectTailCollisionProbeView;
typedef char kf_map_object_tail_collision_probe_size[
    sizeof(KfMapObjectTailCollisionProbeView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_collision_damage_offset[
    (u32)&((KfMapObjectTailCollisionProbeView *)0)->damage_multiplier_tenths == 5 ? 1 : -1];
typedef char kf_map_object_tail_collision_region_offset[
    (u32)&((KfMapObjectTailCollisionProbeView *)0)->camera_region_width == 7 ? 1 : -1];

/* Actions 5, 8, and 22 direct property changes to a linked map object. */
typedef struct KfMapObjectTailLinkedPropertyView {
    u32 unknown_34;
    u8 unknown_38;
    u8 unknown_39;
    u16 linked_object_index;
    u32 unknown_3c;
} KfMapObjectTailLinkedPropertyView;
typedef char kf_map_object_tail_linked_property_size[
    sizeof(KfMapObjectTailLinkedPropertyView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_linked_property_index_offset[
    (u32)&((KfMapObjectTailLinkedPropertyView *)0)->linked_object_index == 6 ? 1 : -1];

/* Action 83 emits its marker after each opening or closing phase. */
typedef struct KfMapObjectTailAction83View {
    u32 unknown_34;
    u8 transition_mode;
    u8 completion_marker;
    u8 unknown_3a[6];
} KfMapObjectTailAction83View;
typedef char kf_map_object_tail_action83_size[
    sizeof(KfMapObjectTailAction83View) == 12 ? 1 : -1];
typedef char kf_map_object_tail_action83_mode_offset[
    (u32)&((KfMapObjectTailAction83View *)0)->transition_mode == 4 ? 1 : -1];
typedef char kf_map_object_tail_action83_marker_offset[
    (u32)&((KfMapObjectTailAction83View *)0)->completion_marker == 5 ? 1 : -1];

/* Action 19 grows and animates a linked map object in 1/32-scale steps. */
typedef struct KfMapObjectTailScaleLinkView {
    u32 unknown_34;
    u8 scale_step_code;
    u8 unknown_39;
    u16 linked_object_index;
    u32 unknown_3c;
} KfMapObjectTailScaleLinkView;
typedef char kf_map_object_tail_scale_link_size[
    sizeof(KfMapObjectTailScaleLinkView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_scale_step_offset[
    (u32)&((KfMapObjectTailScaleLinkView *)0)->scale_step_code == 4 ? 1 : -1];
typedef char kf_map_object_tail_scale_link_index_offset[
    (u32)&((KfMapObjectTailScaleLinkView *)0)->linked_object_index == 6 ? 1 : -1];

/* Event archive commands read the two state bytes at +0x38 as one halfword. */
typedef struct KfMapObjectTailPair38View {
    u32 unknown_34;
    u16 value_38;
    u8 unknown_3a[6];
} KfMapObjectTailPair38View;

/* Marker-driven actions compare this byte with an incoming signal, then
 * consume or arm the action. Other actions give the byte different meanings. */
typedef struct KfMapObjectTailMarkerView {
    u32 unknown_34;
    u8 marker_id;
    u8 unknown_39[7];
} KfMapObjectTailMarkerView;
typedef char kf_map_object_tail_marker_size[
    sizeof(KfMapObjectTailMarkerView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_marker_id_offset[
    (u32)&((KfMapObjectTailMarkerView *)0)->marker_id == 4 ? 1 : -1];

/* Action 0x51 uses the high byte of the effect spawn-sequence slot as its
 * incoming marker identifier. */
typedef struct KfMapObjectTailAction51MarkerView {
    u32 unknown_34;
    u8 unknown_38;
    u8 unknown_39;
    KfMapObjectTailHalfword unknown_3a;
    u8 unknown_3c;
    u8 marker_id;
    KfMapObjectTailHalfword unknown_3e;
} KfMapObjectTailAction51MarkerView;
typedef char kf_map_object_tail_action51_marker_size[
    sizeof(KfMapObjectTailAction51MarkerView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_action51_marker_offset[
    (u32)&((KfMapObjectTailAction51MarkerView *)0)->marker_id == 9 ? 1 : -1];

/* Two placement kinds seed three object rotation axes from byte codes;
 * 0xff leaves an axis at its default value. The final code overlaps the
 * spawn-sequence halfword used by other object kinds. */
typedef struct KfMapObjectTailInitialRotationView {
    u32 unknown_34;
    u8 unknown_38[2];
    u8 rotation_x_code;
    u8 rotation_y_code;
    u8 rotation_z_code;
    u8 unknown_3d[3];
} KfMapObjectTailInitialRotationView;
typedef char kf_map_object_tail_initial_rotation_size[
    sizeof(KfMapObjectTailInitialRotationView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_initial_rotation_x_offset[
    (u32)&((KfMapObjectTailInitialRotationView *)0)->rotation_x_code == 6 ? 1 : -1];
typedef char kf_map_object_tail_initial_rotation_z_offset[
    (u32)&((KfMapObjectTailInitialRotationView *)0)->rotation_z_code == 8 ? 1 : -1];

typedef struct KfMapObjectTailEventEffectView {
    u32 unknown_34;
    u8 pending_event_command;
    u8 effect_object_index;
    u8 linked_object_flag_mask;
    u8 linked_object_index;
    u8 unknown_3c[4];
} KfMapObjectTailEventEffectView;
typedef char kf_map_object_tail_event_effect_size[
    sizeof(KfMapObjectTailEventEffectView) == 12 ? 1 : -1];
typedef char kf_map_object_tail_event_command_offset[
    (u32)&((KfMapObjectTailEventEffectView *)0)->pending_event_command == 4 ? 1 : -1];
typedef char kf_map_object_tail_event_effect_index_offset[
    (u32)&((KfMapObjectTailEventEffectView *)0)->effect_object_index == 5 ? 1 : -1];
typedef char kf_map_object_tail_event_linked_mask_offset[
    (u32)&((KfMapObjectTailEventEffectView *)0)->linked_object_flag_mask == 6 ? 1 : -1];
typedef char kf_map_object_tail_event_linked_index_offset[
    (u32)&((KfMapObjectTailEventEffectView *)0)->linked_object_index == 7 ? 1 : -1];

typedef char kf_map_object_tail_pair38_size[
    sizeof(KfMapObjectTailPair38View) == 12 ? 1 : -1];
typedef char kf_map_object_tail_pair38_offset[
    (u32)&((KfMapObjectTailPair38View *)0)->value_38 == 4 ? 1 : -1];

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
    KfMapObjectTailMotionView motion;
    KfMapObjectTailNotificationView notification;
    KfMapObjectTailTransitionView transition;
    KfMapObjectTailResourceTriggerView resource_trigger;
    KfMapObjectTailAnimatedView animated;
    KfMapObjectTailAmbientSoundView ambient_sound;
    KfMapObjectTailCellCopyView cell_copy;
    KfMapObjectTailAction88CellCopyView action_88_cell_copy;
    KfMapObjectTailAction89LayerFadeView action_89_layer_fade;
    KfMapObjectTailAction84PatternView action_84_pattern;
    KfMapObjectTailCollisionProbeView collision_probe;
    KfMapObjectTailLinkedPropertyView linked_property;
    KfMapObjectTailAction83View action_83;
    KfMapObjectTailScaleLinkView scale_link;
    KfMapObjectTailPair38View pair_38;
    KfMapObjectTailMarkerView marker;
    KfMapObjectTailAction51MarkerView action_51_marker;
    KfMapObjectTailInitialRotationView initial_rotation;
    KfMapObjectTailEventEffectView event_effect;
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

typedef struct KfMapObjectHingeMotion {
    u16 progress_ticks;
    u16 base_yaw;
} KfMapObjectHingeMotion;
typedef char kf_map_object_hinge_motion_size[
    sizeof(KfMapObjectHingeMotion) == 4 ? 1 : -1];

typedef struct KfMapObjectOffsetMotionState {
    u8 elapsed_frames;
    u8 unknown_41[3];
} KfMapObjectOffsetMotionState;
typedef char kf_map_object_offset_motion_state_size[
    sizeof(KfMapObjectOffsetMotionState) == 4 ? 1 : -1];

typedef struct KfMapObjectResourceOffsets {
    s8 offset_x;
    s8 offset_z;
    s8 offset_y;
    u8 unknown_43;
} KfMapObjectResourceOffsets;
typedef char kf_map_object_resource_offsets_size[
    sizeof(KfMapObjectResourceOffsets) == 4 ? 1 : -1];

typedef struct KfMapObjectLayerFadeState {
    u16 delay_frames_left;
    u8 original_layer_mask;
    u8 unknown_43;
} KfMapObjectLayerFadeState;
typedef char kf_map_object_layer_fade_state_size[
    sizeof(KfMapObjectLayerFadeState) == 4 ? 1 : -1];

/* Placement kinds 9, 0x15, 0x54, and 0xe2 save the layer before changing
 * visibility; action 0x54 later passes it to map-cell pattern updates. */
typedef struct KfMapObjectSavedLayerState {
    u8 layer_mask;
    u8 unknown_41[3];
} KfMapObjectSavedLayerState;
typedef char kf_map_object_saved_layer_state_size[
    sizeof(KfMapObjectSavedLayerState) == 4 ? 1 : -1];

typedef union KfMapObjectExtra40 {
    KfMapObjectRecord40 *record;
    u32 raw;
    s32 bob_base_y;
    u32 next_sound_frame;
    u8 bytes[4];
    u16 object_index;
    s16 angular_velocity_x;
    u16 movement_frames_left;
    KfMapObjectHingeMotion hinge;
    KfMapObjectOffsetMotionState offset_motion;
    KfMapObjectResourceOffsets resource_offsets;
    KfMapObjectLayerFadeState layer_fade;
    KfMapObjectSavedLayerState saved_layer;
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
    s16 render_depth_offset;
    u16 lighting_blend_q12;
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
    u16 definition_drop_sequence;
    u16 placement_drop_sequence;
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
    (u32)&((KfMapObjectStateGame *)0)->placement_drop_sequence == 0x8742 ? 1 : -1];

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

#ifndef KF_GAME_PLAYER_H
#define KF_GAME_PLAYER_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

struct KfMagicRecord;
struct KfEffectRecord;
struct KfActor;

/* Runtime progression limits (King's Field capped vitals at 9999 and
 * experience at 99999 and loaded forty growth rows). */
enum {
    KF_PLAYER_POWER_MAX = 999,
    KF_PLAYER_VITAL_MAX = 999,
    KF_PLAYER_EXPERIENCE_MAX = 999999,
    KF_PLAYER_LEVEL_MAX = 255,
    KF_PLAYER_LEVEL_GROWTH_COUNT = 100,
    KF_PLAYER_TRAINING_POINTS_PER_GAIN = 100
};

/* Vertical extent of the player in distance tests (King's Field's
 * KF_COLLISION_PLAYER_HEIGHT). */
enum {
    KF_PLAYER_HEIGHT = 1700
};

/* The byte at player +0xcd selects the camera, overlap, damage, and death
 * reaction paths in player_update_frame. */
enum {
    KF_PLAYER_REACTION_NORMAL = 0,
    KF_PLAYER_REACTION_MAP_OBJECT_FOLLOW = 1,
    KF_PLAYER_REACTION_MAP_OBJECT_APPROACH = 2,
    KF_PLAYER_REACTION_ROTATION = 3,
    KF_PLAYER_REACTION_OVERLAP_BOB = 4,
    KF_PLAYER_REACTION_POSITION_RECOVERY = 5,
    KF_PLAYER_REACTION_MOVING_DAMAGE = 0x10,
    KF_PLAYER_REACTION_DEATH = 0x11,
    KF_PLAYER_REACTION_ROTATION_DAMAGE = 0x12
};

/* Low-halfword motion bits kept when the player's motion is cleared. */
enum {
    KF_PLAYER_MOTION_FLAGS_KEPT = 0x8b0
};

/*
 * Record zero seeds a new player's values. Later records provide the next
 * absolute HP/MP values, stat steps, and cumulative experience threshold.
 * Past the last record the game extrapolates from the final two.
 */
typedef struct KfPlayerLevelGrowth {
    u16 maximum_hp;
    u16 maximum_mp;
    u16 physical_power_step;
    u16 magic_step;
    u32 experience_threshold;
} KfPlayerLevelGrowth;

typedef char kf_player_level_growth_size[sizeof(KfPlayerLevelGrowth) == 12 ? 1 : -1];

typedef struct KfPlayerVitals {
    u16 maximum_hp;
    u16 current_hp;
    u16 maximum_mp;
    u16 current_mp;
} KfPlayerVitals;

typedef struct KfEquipmentRecord {
    u8 unknown_00[2];
    u16 bonus_components[9];
    u16 hp_regen_interval;
    u16 hp_drain_interval;
    u8 unknown_18[8];
} KfEquipmentRecord;

typedef char kf_equipment_record_size[sizeof(KfEquipmentRecord) == 0x20 ? 1 : -1];
typedef char kf_equipment_record_bonuses_offset[
    (u32)&((KfEquipmentRecord *)0)->bonus_components == 2 ? 1 : -1];
typedef char kf_equipment_record_hp_regen_offset[
    (u32)&((KfEquipmentRecord *)0)->hp_regen_interval == 0x14 ? 1 : -1];
typedef char kf_equipment_record_hp_drain_offset[
    (u32)&((KfEquipmentRecord *)0)->hp_drain_interval == 0x16 ? 1 : -1];

/* The fourth equipment defense component resists poison application. */
enum { KF_PLAYER_COMBAT_POISON_RESISTANCE = 3 };

typedef struct KfWeaponRecordGame {
    u8 sound_id;
    u8 charge_rank;
    u8 unknown_02;
    u8 initial_effect_id;
    u8 release_effect_id;
    u8 magic_shots;
    u16 attack_components[8];
    u16 hp_regen_interval;
    u16 mp_regen_interval;
    u16 attack_angle;
    u16 attack_phase_step;
    u16 normal_attack_end_phase;
    u16 magic_window_start;
    u16 magic_window_end;
    u16 alternate_attack_phase_step;
    u16 alternate_attack_window_start;
    u16 alternate_attack_end_phase;
    u16 magic_phase_step;
    u16 normal_attack_sound_phase;
    u16 alternate_attack_sound_start_phase;
    u16 alternate_attack_sound_end_phase;
    u16 alternate_attack_sound_phase_step;
    u16 position_offset_x;
    u16 position_offset_y;
    u16 position_offset_z;
    s16 initial_vertex_index;
    u16 rotation_offset_x;
    u16 rotation_offset_y;
    u16 rotation_offset_z;
    s16 final_vertex_index;
} KfWeaponRecordGame;

typedef struct KfWeaponAssetBuffer {
    u8 bytes[0xc000];
} KfWeaponAssetBuffer;

typedef char kf_weapon_asset_buffer_size[sizeof(KfWeaponAssetBuffer) == 0xc000 ? 1 : -1];

typedef struct KfPlayerViewRotation {
    s16 angles[3];
    u16 unknown_06;
} KfPlayerViewRotation;

typedef union KfPlayerViewRotationOffset {
    SVECTOR vector;
    u16 components[4];
} KfPlayerViewRotationOffset;
typedef char kf_player_view_rotation_offset_size[sizeof(KfPlayerViewRotationOffset) == 8 ? 1 : -1];

typedef struct KfQueuedMagicAction {
    u8 magic_id;
    u8 casts_remaining;
    u8 repeat_interval;
    u8 countdown;
} KfQueuedMagicAction;
typedef char kf_queued_magic_action_size[sizeof(KfQueuedMagicAction) == 4 ? 1 : -1];

typedef struct KfPlayerDamageReaction {
    SVECTOR rotation;
    SVECTOR motion;
} KfPlayerDamageReaction;

typedef struct KfPlayerViewReaction {
    u8 mode;
    u8 step;
    KfPlayerViewRotation rotation;
    u8 unknown_0a[6];
} KfPlayerViewReaction;

typedef struct KfPlayerPositionReaction {
    u8 mode;
    u8 unknown_01[3];
    VECTOR position;
} KfPlayerPositionReaction;

typedef struct KfPlayerFlags140Halves {
    u16 low;
    u16 high;
} KfPlayerFlags140Halves;

typedef union KfPlayerFlags140 {
    u32 word;
    u16 low;
    KfPlayerFlags140Halves halves;
} KfPlayerFlags140;

typedef char kf_player_flags140_halves_size[
    sizeof(KfPlayerFlags140Halves) == 4 ? 1 : -1];
typedef char kf_player_flags140_high_offset[
    (u32)&((KfPlayerFlags140 *)0)->halves.high == 2 ? 1 : -1];
typedef char kf_player_flags140_size[sizeof(KfPlayerFlags140) == 4 ? 1 : -1];

typedef union KfPlayerReactionOverlay {
    KfPlayerDamageReaction damage;
    KfPlayerViewReaction view;
    KfPlayerPositionReaction position;
    u16 angle_phase;
} KfPlayerReactionOverlay;

typedef char kf_player_reaction_overlay_size[
    sizeof(KfPlayerReactionOverlay) == 0x14 ? 1 : -1];
typedef char kf_player_reaction_view_rotation_offset[
    (u32)&((KfPlayerReactionOverlay *)0)->view.rotation == 2 ? 1 : -1];
typedef char kf_player_reaction_damage_motion_offset[
    (u32)&((KfPlayerReactionOverlay *)0)->damage.motion == 8 ? 1 : -1];
typedef char kf_player_reaction_position_offset[
    (u32)&((KfPlayerReactionOverlay *)0)->position.position == 4 ? 1 : -1];

typedef union KfPlayerMovementSpeed {
    u16 unsigned_value;
    s16 signed_value;
} KfPlayerMovementSpeed;

typedef char kf_player_movement_speed_size[
    sizeof(KfPlayerMovementSpeed) == 2 ? 1 : -1];

typedef char kf_weapon_record_game_size[sizeof(KfWeaponRecordGame) == 0x44 ? 1 : -1];
typedef char kf_weapon_record_game_hp_regen_offset[
    (u32)&((KfWeaponRecordGame *)0)->hp_regen_interval == 0x16 ? 1 : -1];
typedef char kf_weapon_record_game_mp_regen_offset[
    (u32)&((KfWeaponRecordGame *)0)->mp_regen_interval == 0x18 ? 1 : -1];
typedef char kf_weapon_record_game_normal_end_offset[
    (u32)&((KfWeaponRecordGame *)0)->normal_attack_end_phase == 0x1e ? 1 : -1];
typedef char kf_weapon_record_game_alternate_end_offset[
    (u32)&((KfWeaponRecordGame *)0)->alternate_attack_end_phase == 0x28 ? 1 : -1];
typedef char kf_weapon_record_game_attacks_offset[
    (u32)&((KfWeaponRecordGame *)0)->attack_components == 6 ? 1 : -1];
typedef char kf_weapon_record_game_alternate_phase_step_offset[
    (u32)&((KfWeaponRecordGame *)0)->alternate_attack_phase_step == 0x24 ? 1 : -1];
typedef char kf_weapon_record_game_alternate_window_offset[
    (u32)&((KfWeaponRecordGame *)0)->alternate_attack_window_start == 0x26 ? 1 : -1];
typedef char kf_weapon_record_game_normal_sound_offset[
    (u32)&((KfWeaponRecordGame *)0)->normal_attack_sound_phase == 0x2c ? 1 : -1];
typedef char kf_weapon_record_game_alternate_sound_step_offset[
    (u32)&((KfWeaponRecordGame *)0)->alternate_attack_sound_phase_step == 0x32 ? 1 : -1];

typedef struct KfPlayerMagicIdSequence {
    u8 effect_ids[12];
    u16 attack_masks[4];
} KfPlayerMagicIdSequence;

typedef char kf_player_magic_id_sequence_size[
    sizeof(KfPlayerMagicIdSequence) == 0x14 ? 1 : -1];
typedef char kf_player_magic_attack_masks_offset[
    (u32)&((KfPlayerMagicIdSequence *)0)->attack_masks == 0x0c ? 1 : -1];

typedef struct KfMapOccupancyLayer {
    u8 object_index;
    u8 elevation;
    u8 quarter_turns;
    u8 collision_shape_id;
    u8 lighting_index;
} KfMapOccupancyLayer;

typedef KfMapOccupancyLayer KfMapCellShape;
typedef char kf_map_cell_shape_size[sizeof(KfMapCellShape) == 5 ? 1 : -1];
typedef char kf_map_cell_orientation_offset[
    (u32)&((KfMapCellShape *)0)->quarter_turns == 2 ? 1 : -1];
typedef char kf_map_cell_lighting_offset[
    (u32)&((KfMapCellShape *)0)->lighting_index == 4 ? 1 : -1];

typedef struct KfMapOccupancyCell {
    KfMapOccupancyLayer layer[2];
} KfMapOccupancyCell;

typedef char kf_map_occupancy_cell_size[sizeof(KfMapOccupancyCell) == 10 ? 1 : -1];

/* Startup clears this complete region. The map-object helper addresses its
 * leading cells at an 800-byte row stride and a 10-byte column stride. */
typedef struct KfBss801c7540 {
    KfMapOccupancyCell map_cells[88][80];
    u8 unknown_11300[0x2a8];
    KfEquipmentRecord equipment_records[20];
    u8 unknown_11828[0x1c];
} KfBss801c7540;

typedef char kf_bss_801c7540_size[sizeof(KfBss801c7540) == 0x11844 ? 1 : -1];
typedef char kf_bss_801c7540_equipment_offset[
    (u32)&((KfBss801c7540 *)0)->equipment_records == 0x115a8 ? 1 : -1];

enum {
    KF_EQUIPMENT_SLOT_HEAD = 0,
    KF_EQUIPMENT_SLOT_BODY = 1,
    KF_EQUIPMENT_SLOT_LEG = 2,
    KF_EQUIPMENT_SLOT_SHIELD = 3,
    KF_EQUIPMENT_SLOT_ARM = 4,
    KF_EQUIPMENT_SLOT_ACCESSORY = 5,
    KF_EQUIPMENT_SLOT_EXTRA = 6,
    KF_EQUIPMENT_NONE = 0xff
};

/*
 * The player object: game_main_loop clears 0x160 bytes at its base and the
 * experience routine addresses members from a registered base. Unknown spans
 * remain opaque.
 */
typedef struct KfPlayerState {
    s32 experience;
    s32 next_level_experience;
    u8 level;
    u8 unknown_09;
    u8 force_actor_lifecycle_refresh;
    u8 weapon_charge_delay;
    u8 unknown_0c;
    u8 movement_speed_adjustment_decay_latch;
    u16 movement_speed_adjustment_q12;
    s16 damage_scale;
    KfPlayerVitals vitals;
    u16 attack_charge_current;
    u16 attack_charge_committed;
    u16 magic_charge;
    u16 physical_power_training;
    u16 magic_training;
    u16 base_physical_power;
    u16 base_magic;
    u16 physical_power;
    u16 magic;
    u32 gold;
    u16 attack_components[8];
    u16 unknown_40;
    u16 combat_components[9];
    s16 poison_timer;
    s16 curse_strength;
    u16 curse_phase_limit;
    s16 darkness_phase;
    u16 darkness_phase_limit;
    s16 slow_timer;
    s16 paralysis_timer;
    s16 defense_boost_timer;
    s16 attack_boost_timer;
    s16 magic_tint_phase;
    s16 magic_tint_phase_limit;
    s16 map_marker_visual_effect_timer;
    s16 full_mp_timer;
    s16 magic_boost_timer;
    u8 unknown_70[4];
    u32 equipment_effect_ticks;
    const u16 *magic_attack_mask_cursor;
    struct KfMagicRecord *selected_magic_record;
    KfWeaponRecordGame *equipped_weapon_record;
    struct KfAssetHeader *weapon_asset_buffer;
    struct KfPoolRecord *weapon_animation_cache;
    struct KfEffectRecord *weapon_effect;
    s16 weapon_attack_phase;
    s16 weapon_attack_window;
    s16 weapon_next_sound_phase;
    u8 weapon_magic_shots_remaining;
    u8 primary_magic_shortcut_id;
    u8 secondary_magic_shortcut_id;
    u8 secondary_item_shortcut_id;
    u8 weapon_attack_mode;
    u8 equipped_weapon_id;
    u8 unknown_9c[2];
    u8 weapon_magic_shots_configured;
    u8 weapon_attack_fully_charged;
    u8 weapon_guard_active;
    u8 unknown_a1[3];
    KfEquipmentRecord *equipped_head_record;
    KfEquipmentRecord *equipped_body_record;
    KfEquipmentRecord *equipped_arm_record;
    KfEquipmentRecord *equipped_leg_record;
    KfEquipmentRecord *equipped_shield_record;
    KfEquipmentRecord *equipped_accessory_record;
    KfEquipmentRecord *equipped_extra_record;
    u8 equipped_head_id;
    u8 equipped_body_id;
    u8 equipped_arm_id;
    u8 equipped_leg_id;
    u8 equipped_shield_id;
    u8 equipped_accessory_id;
    u8 equipped_extra_id;
    u8 audio_effects_enabled;
    u8 audio_music_enabled;
    u8 hud_gauges_enabled;
    u8 compass_enabled;
    u8 item_preview_enabled;
    u8 walking_bob_enabled;
    u8 death_state;
    u8 unknown_ce[2];
    u8 vertical_motion_state;
    KfQueuedMagicAction queued_magic_action;
    u8 fatal_fall_latch;
    u8 unknown_d6[2];
    VECTOR camera_position;
    SVECTOR frame_displacement;
    KfPlayerViewRotation camera_rotation;
    KfPlayerViewRotation camera_rotation_target;
    s16 reaction_rotation_offset[3];
    s16 death_transition_frame;
    KfPlayerViewRotationOffset view_rotation_offset;
    s16 vertical_motion_pitch_offset;
    s16 camera_yaw_roll_offsets[2];
    u8 unknown_116[2];
    SVECTOR magic_origin_offset;
    s32 collision_lower_clearance;
    s32 collision_upper_clearance;
    u16 map_layer_index;
    s16 strafe_velocity;
    s16 forward_velocity;
    KfPlayerMovementSpeed movement_speed;
    s16 yaw_step;
    s16 pitch_step;
    s16 camera_vertical_offset;
    s16 walking_bob_phase;
    s16 landing_vertical_offset;
    s16 vertical_velocity;
    s16 damage_red_overlay_scale;
    s16 damage_red_overlay_decay;
    KfPlayerFlags140 flags_140;
    s32 movement_step_limit;
    s32 turn_step_limit;
    KfPlayerReactionOverlay reaction;
} KfPlayerState;

typedef char kf_player_state_size[sizeof(KfPlayerState) == 0x160 ? 1 : -1];
typedef char kf_player_movement_speed_adjustment_decay_latch_offset[
    (u32)&((KfPlayerState *)0)->movement_speed_adjustment_decay_latch == 0x0d ? 1 : -1];
typedef char kf_player_lifecycle_refresh_offset[
    (u32)&((KfPlayerState *)0)->force_actor_lifecycle_refresh == 0x0a ? 1 : -1];
typedef char kf_player_unknown_97_offset[
    (u32)&((KfPlayerState *)0)->primary_magic_shortcut_id == 0x97 ? 1 : -1];
typedef char kf_player_combat_components_offset[
    (u32)&((KfPlayerState *)0)->combat_components == 0x42 ? 1 : -1];
typedef char kf_player_magic_attack_mask_cursor_offset[
    (u32)&((KfPlayerState *)0)->magic_attack_mask_cursor == 0x78 ? 1 : -1];
typedef char kf_player_weapon_next_sound_phase_offset[
    (u32)&((KfPlayerState *)0)->weapon_next_sound_phase == 0x94 ? 1 : -1];
typedef char kf_player_selected_magic_record_offset[
    (u32)&((KfPlayerState *)0)->selected_magic_record == 0x7c ? 1 : -1];
typedef char kf_player_equipped_head_record_offset[
    (u32)&((KfPlayerState *)0)->equipped_head_record == 0xa4 ? 1 : -1];
typedef char kf_player_equipped_head_id_offset[
    (u32)&((KfPlayerState *)0)->equipped_head_id == 0xc0 ? 1 : -1];
typedef char kf_player_death_state_offset[
    (u32)&((KfPlayerState *)0)->death_state == 0xcd ? 1 : -1];
typedef char kf_player_queued_magic_action_offset[
    (u32)&((KfPlayerState *)0)->queued_magic_action == 0xd1 ? 1 : -1];
typedef char kf_player_fatal_fall_latch_offset[
    (u32)&((KfPlayerState *)0)->fatal_fall_latch == 0xd5 ? 1 : -1];
typedef char kf_player_frame_displacement_offset[
    (u32)&((KfPlayerState *)0)->frame_displacement == 0xe8 ? 1 : -1];
typedef char kf_player_vertical_motion_pitch_offset[
    (u32)&((KfPlayerState *)0)->vertical_motion_pitch_offset == 0x110 ? 1 : -1];
typedef char kf_player_camera_yaw_roll_offsets_offset[
    (u32)&((KfPlayerState *)0)->camera_yaw_roll_offsets == 0x112 ? 1 : -1];
typedef char kf_player_death_rotation_offset[
    (u32)&((KfPlayerState *)0)->reaction == 0x14c ? 1 : -1];
typedef char kf_player_movement_speed_offset[
    (u32)&((KfPlayerState *)0)->movement_speed == 0x12e ? 1 : -1];
typedef char kf_player_flags140_offset[
    (u32)&((KfPlayerState *)0)->flags_140 == 0x140 ? 1 : -1];
typedef char kf_player_turn_step_limit_offset[
    (u32)&((KfPlayerState *)0)->turn_step_limit == 0x148 ? 1 : -1];
typedef char kf_player_movement_step_limit_offset[
    (u32)&((KfPlayerState *)0)->movement_step_limit == 0x144 ? 1 : -1];

extern KfPlayerLevelGrowth player_level_growth_table[KF_PLAYER_LEVEL_GROWTH_COUNT];
extern KfPlayerState player_state;
extern KfPlayerMagicIdSequence player_magic_id_sequence;
extern KfBss801c7540 bss_801c7540;
extern KfWeaponRecordGame player_weapon_records[18];

s32 player_move_horizontal(s32 heading, s32 distance);
void player_recalculate_combat_stats(void);
void player_increment_physical_power_training(void);
void player_increment_magic_training(void);
void player_add_experience(s16 amount);
void player_clear_motion(void);
s32 player_distance_to_point_in_cone(
    const VECTOR *point, s16 facing, s32 max_distance, s32 angle_tolerance);
s32 player_distance_to_point(
    s32 point_x, s32 point_y, s32 point_z, s32 max_distance, s32 point_height);
s32 player_distance_to_point_with_margin(
    s32 point_x, s32 point_y, s32 point_z, s32 max_distance, s32 point_height);
void player_update_collision_bounds(void);
void player_reload_map_resources(s32 first, s32 second, s32 third, s32 fourth,
                   s32 fifth, s32 optional_resource);
s32 player_charge_gain_for_rank(s32 value, s32 rank);
void player_add_equipment_bonuses(s32 item_id);
s32 player_calculate_damage_component(s32 base_power, s32 defense, s32 attack);
void player_apply_damage_reaction(const VECTOR *origin, s32 damage, s32 reaction_flags);
void player_apply_damage(u16 damage0, u16 damage1, u16 damage2, u16 status_flags,
                   u16 damage3, u16 damage4, u16 damage5, u16 damage6,
                   u16 damage7, u16 scale_q16, u16 multiplier_tenths,
                   const VECTOR *origin);
void player_apply_radial_damage(VECTOR *position, s32 start, s32 end, s32 mode,
                   u16 falloff, u16 damage0, u16 damage1, u16 damage2,
                   u16 damage3, u16 damage4, u16 damage5, u16 damage6,
                   u16 damage7, u16 damage8, s32 scale_and_flags, u16 record_id);
void player_adjust_hp_unclamped(s32 delta);
void player_cap_status_components(u32 mask);
void player_death_begin(const SVECTOR *rotation);
void player_adjust_hp(s32 delta);
void player_adjust_mp(s32 delta);
void player_set_primary_magic_shortcut_id(u8 value);
void player_set_secondary_magic_shortcut_id(u8 value);
void player_set_secondary_item_shortcut_id(u8 value);
void player_set_equipment_slot(u8 item_id, u8 slot);
void player_equip_weapon(u8 weapon_id);
struct KfActor *player_probe_view_target_and_vectors(s32 scale, VECTOR *position,
                              SVECTOR *direction, s32 *distance);
void player_dispatch_magic_effect(s32 effect_id, ...);
void player_sample_weapon_world_vertex(s32 vertex_index, VECTOR *output);
void player_update_weapon_attack(void);
void player_select_magic_action(s32 magic_id);
void player_update_vertical_motion(void);
s32 player_move_reaction_with_collision(void);
void player_update_camera_rotation(void);
void player_update_horizontal_motion(void);
s32 item_id_is_71_to_80(s32 value);
void player_update_actions_and_charge(void);
void player_render_frame_and_release_pool(void);
void player_begin_view_reaction(u8 mode);
void player_begin_rotation_reaction(const SVECTOR *rotation);
void player_begin_moving_damage_reaction(const SVECTOR *rotation, const SVECTOR *motion,
                   s16 duration);
void player_begin_rotation_only_damage_reaction(const SVECTOR *rotation, const SVECTOR *motion,
                   s16 duration);
void player_update_frame(void);
void render_frames_with_color_overlay(s32 mode, s32 phase, s32 last_phase,
    s32 step);
void player_begin_weapon_attack(s32 mode);
void player_reset_status(void);
void player_get_camera_pose(VECTOR *position, SVECTOR *angles);
void player_reset_view(void);
void player_restore_equipment_effects(void);
void player_sync_position_to_map(void);
s32 player_has_power_and_magic_60(void);
void player_initialize_state(void);
void game_initialize_session(void);

#endif

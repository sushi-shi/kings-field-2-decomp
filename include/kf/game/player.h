#ifndef KF_GAME_PLAYER_H
#define KF_GAME_PLAYER_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

struct KfMagicRecord;
struct KfEffectRecord;

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

typedef struct KfWeaponRecordGame {
    u8 sound_id;
    u8 charge_rank;
    u8 unknown_02;
    u8 initial_effect_id;
    u8 release_effect_id;
    u8 magic_shots;
    u16 attack_components[8];
    u16 unknown_16;
    u16 unknown_18;
    s16 attack_angle;
    s16 attack_phase_step;
    u16 unknown_1e;
    u16 magic_window_start;
    u16 magic_window_end;
    u16 unknown_24;
    u16 unknown_26;
    u16 unknown_28;
    u16 magic_phase_step;
    u16 unknown_2c;
    u16 unknown_2e;
    u16 unknown_30;
    s16 release_phase_step;
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
typedef char kf_weapon_record_game_attacks_offset[
    (u32)&((KfWeaponRecordGame *)0)->attack_components == 6 ? 1 : -1];
typedef char kf_weapon_record_game_unknown_24_offset[
    (u32)&((KfWeaponRecordGame *)0)->unknown_24 == 0x24 ? 1 : -1];

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
    u8 unknown_03;
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
    u8 unknown_09[2];
    u8 weapon_charge_delay;
    u8 unknown_0c[2];
    u16 unknown_0e;
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
    s16 unknown_54;
    s16 curse_strength;
    u16 unknown_58;
    s16 unknown_5a;
    u16 unknown_5c;
    s16 unknown_5e;
    s16 unknown_60;
    s16 unknown_62;
    s16 unknown_64;
    s16 unknown_66;
    s16 unknown_68;
    s16 unknown_6a;
    s16 unknown_6c;
    s16 unknown_6e;
    u8 unknown_70[4];
    u32 equipment_effect_ticks;
    const u16 *unknown_78;
    struct KfMagicRecord *selected_magic_record;
    KfWeaponRecordGame *equipped_weapon_record;
    struct KfAssetHeader *weapon_asset_buffer;
    struct KfPoolRecord *weapon_animation_cache;
    struct KfEffectRecord *weapon_effect;
    s16 weapon_attack_phase;
    u16 weapon_attack_window;
    u16 weapon_attack_recovery;
    u8 weapon_magic_shots_remaining;
    u8 unknown_97;
    u8 unknown_98;
    u8 unknown_99;
    u8 weapon_attack_mode;
    u8 equipped_weapon_id;
    u8 unknown_9c[2];
    u8 weapon_magic_shots_configured;
    u8 weapon_attack_fully_charged;
    u8 unknown_a0;
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
    u8 unknown_c9[4];
    u8 death_state;
    u8 unknown_ce[2];
    u8 unknown_d0;
    u8 unknown_d1[7];
    VECTOR camera_position;
    u16 unknown_e8;
    u16 unknown_ea;
    u16 unknown_ec;
    u16 unknown_ee;
    KfPlayerViewRotation camera_rotation;
    KfPlayerViewRotation camera_rotation_target;
    u16 unknown_100[3];
    u16 unknown_106;
    u16 unknown_108[3];
    u8 unknown_10e[2];
    s16 unknown_110[3];
    u8 unknown_116[2];
    SVECTOR unknown_118;
    s32 unknown_120;
    s32 unknown_124;
    u16 unknown_128;
    s16 strafe_velocity;
    s16 forward_velocity;
    KfPlayerMovementSpeed movement_speed;
    s16 yaw_step;
    s16 pitch_step;
    s16 unknown_134;
    s16 unknown_136;
    s16 unknown_138;
    s16 unknown_13a;
    s16 unknown_13c;
    s16 unknown_13e;
    KfPlayerFlags140 flags_140;
    s32 movement_step_limit;
    s32 turn_step_limit;
    KfPlayerReactionOverlay reaction;
} KfPlayerState;

typedef char kf_player_state_size[sizeof(KfPlayerState) == 0x160 ? 1 : -1];
typedef char kf_player_unknown_97_offset[
    (u32)&((KfPlayerState *)0)->unknown_97 == 0x97 ? 1 : -1];
typedef char kf_player_combat_components_offset[
    (u32)&((KfPlayerState *)0)->combat_components == 0x42 ? 1 : -1];
typedef char kf_player_unknown_78_offset[
    (u32)&((KfPlayerState *)0)->unknown_78 == 0x78 ? 1 : -1];
typedef char kf_player_selected_magic_record_offset[
    (u32)&((KfPlayerState *)0)->selected_magic_record == 0x7c ? 1 : -1];
typedef char kf_player_equipped_head_record_offset[
    (u32)&((KfPlayerState *)0)->equipped_head_record == 0xa4 ? 1 : -1];
typedef char kf_player_equipped_head_id_offset[
    (u32)&((KfPlayerState *)0)->equipped_head_id == 0xc0 ? 1 : -1];
typedef char kf_player_death_state_offset[
    (u32)&((KfPlayerState *)0)->death_state == 0xcd ? 1 : -1];
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
extern const KfPlayerMagicIdSequence DAT_800667e8;
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
s32 func_80023814(s32 value, s32 rank);
void player_add_equipment_bonuses(s32 item_id);
s32 player_calculate_damage_component(s32 base_power, s32 defense, s32 attack);
void player_adjust_hp_unclamped(s32 delta);
void player_cap_status_components(u32 mask);
void player_death_begin(const SVECTOR *rotation);
void player_adjust_hp(s32 delta);
void player_adjust_mp(s32 delta);
void player_set_unknown_97(u8 value);
void player_set_unknown_98(u8 value);
void player_set_unknown_99(u8 value);
void player_set_equipment_slot(u8 item_id, u8 slot);
void player_equip_weapon(u8 weapon_id);
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

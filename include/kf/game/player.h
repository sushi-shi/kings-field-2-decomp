#ifndef KF_GAME_PLAYER_H
#define KF_GAME_PLAYER_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

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

/* Bits of unknown_140 kept when the player's motion is cleared. */
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

/*
 * The player object: game_main_loop clears 0x160 bytes at its base and the
 * experience routine addresses members from a registered base. Unknown spans
 * remain opaque.
 */
typedef struct KfPlayerState {
    s32 experience;
    s32 next_level_experience;
    u8 level;
    u8 unknown_09[9];
    KfPlayerVitals vitals;
    u8 unknown_1a[6];
    u16 physical_power_training;
    u16 magic_training;
    u16 base_physical_power;
    u16 base_magic;
    u8 unknown_28[0xb0];
    VECTOR camera_position;
    u8 unknown_e8[0x42];
    s16 strafe_velocity;
    s16 forward_velocity;
    u16 movement_speed;
    s16 yaw_step;
    s16 pitch_step;
    u8 unknown_134[0xc];
    u16 unknown_140;
    u8 unknown_142[0x1e];
} KfPlayerState;

typedef char kf_player_state_size[sizeof(KfPlayerState) == 0x160 ? 1 : -1];

extern KfPlayerLevelGrowth player_level_growth_table[KF_PLAYER_LEVEL_GROWTH_COUNT];
extern KfPlayerState player_state;

void player_recalculate_combat_stats(void);
void player_increment_physical_power_training(void);
void player_increment_magic_training(void);
void player_add_experience(s16 amount);
void player_clear_motion(void);
s32 player_distance_to_point_in_cone(
    const VECTOR *point, s16 facing, s32 max_distance, s32 angle_tolerance);
s32 player_distance_to_point(
    s32 point_x, s32 point_y, s32 point_z, s32 max_distance, s32 point_height);
void player_death_begin(const SVECTOR *rotation);
void player_adjust_hp(s32 delta);
void player_adjust_mp(s32 delta);

#endif

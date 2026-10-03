#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/lib/math.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>

enum {
    COLLISION_CACHE_BIAS = 1600,
    COLLISION_LOWER_DEATH_LIMIT = -1000
};

ADDRESS(0x80023384, 0xac)
void player_update_collision_bounds(void)
{
    s32 lower;
    s32 upper;

    lower = KF_COLLISION_CACHE_LOWER_BOUND + COLLISION_CACHE_BIAS;
    lower -= player_state.camera_vertical_offset + player_state.camera_position.vy
           + player_state.landing_vertical_offset;
    player_state.collision_lower_clearance = lower;
    if (lower < COLLISION_LOWER_DEATH_LIMIT) {
        player_death_begin(NULL);
    }

    upper = KF_COLLISION_CACHE_UPPER_BOUND + COLLISION_CACHE_BIAS;
    upper -= player_state.camera_vertical_offset + player_state.camera_position.vy
           + player_state.landing_vertical_offset;
    player_state.collision_upper_clearance = upper;
    if (upper <= 0) {
        player_death_begin(NULL);
    }
}

enum {
    KF_PLAYER_DISTANCE_MARGIN = 800
};

ADDRESS(0x80023430, 0x54)
s32 player_distance_to_point_with_margin(
    s32 point_x, s32 point_y, s32 point_z, s32 max_distance,
    s32 point_height)
{
    return vector_distance_to_point(
        &player_state.camera_position, point_x, point_y, point_z,
        max_distance + KF_PLAYER_DISTANCE_MARGIN, KF_PLAYER_HEIGHT,
        point_height);
}

enum {
    PLAYER_VIEW_SCALE_INITIAL = 0x1000,
    PLAYER_VIEW_TIMER_INITIAL = 10000
};

ADDRESS(0x80023484, 0xec)
void player_reset_view(void)
{
    player_state.camera_vertical_offset = 0;
    player_state.walking_bob_phase = 0;
    player_state.landing_vertical_offset = 0;
    player_state.vertical_motion_state = 0;
    player_state.vertical_velocity = 0;
    player_state.death_state = 0;
    player_state.camera_yaw_roll_offsets[1] = 0;
    player_state.camera_yaw_roll_offsets[0] = 0;
    player_state.vertical_motion_pitch_offset = 0;
    player_state.view_rotation_offset.components[2] = 0;
    player_state.view_rotation_offset.components[1] = 0;
    player_state.view_rotation_offset.components[0] = 0;
    player_state.reaction_rotation_offset[2] = 0;
    player_state.reaction_rotation_offset[1] = 0;
    player_state.reaction_rotation_offset[0] = 0;
    player_state.camera_rotation = player_state.camera_rotation_target;
    player_state.movement_speed_adjustment_q12 = 0;
    player_state.movement_speed_adjustment_decay_latch = 0;
    player_state.damage_scale = PLAYER_VIEW_SCALE_INITIAL;
    player_state.queued_magic_action.magic_id = 0xff;
    player_state.collision_lower_clearance = PLAYER_VIEW_TIMER_INITIAL;
    player_state.collision_upper_clearance = PLAYER_VIEW_TIMER_INITIAL;
}

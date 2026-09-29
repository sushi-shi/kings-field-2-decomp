#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/player.h>

ADDRESS(0x800252e4, 0xc8)
s32 player_distance_to_point_in_cone(
    const VECTOR *point, s16 facing, s32 max_distance, s32 angle_tolerance)
{
    s32 distance;
    s16 delta;

    distance = player_distance_to_point(point->vx, KF_DISTANCE_IGNORE_HEIGHT, point->vz, max_distance, 0);
    if (distance != KF_DISTANCE_NONE) {
        delta = (vector_xz_to_angle(
                     player_state.camera_position.vx - point->vx,
                     player_state.camera_position.vz - point->vz)
                 - facing) & KF_ANGLE_WRAP_MASK;
        delta = angle_error_magnitude(delta);
        if (angle_tolerance < delta) {
            distance = KF_DISTANCE_NONE;
        }
    }
    return distance;
}

ADDRESS(0x800253ac, 0x50)
s32 player_distance_to_point(
    s32 point_x, s32 point_y, s32 point_z, s32 max_distance, s32 point_height)
{
    return vector_distance_to_point(
        &player_state.camera_position, point_x, point_y, point_z, max_distance,
        KF_PLAYER_HEIGHT, point_height);
}

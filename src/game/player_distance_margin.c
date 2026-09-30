#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/player.h>

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

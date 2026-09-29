#include <kf/lib/address.h>
#include <kf/lib/math.h>

ADDRESS(0x800155a4, 0xf4)
s32 vector_distance_to_point(
    const VECTOR *position, s32 point_x, s32 point_y, s32 point_z,
    s32 max_distance, s32 height, s32 point_height)
{
    s32 dx;
    s32 dz;
    s32 distance;

    dx = position->vx - point_x;
    if (dx < -max_distance || max_distance < dx) {
        return KF_DISTANCE_NONE;
    }
    dz = position->vz - point_z;
    if (dz < -max_distance || max_distance < dz) {
        return KF_DISTANCE_NONE;
    }
    if (point_y != KF_DISTANCE_IGNORE_HEIGHT) {
        if (position->vy < point_y) {
            if (position->vy < point_y - point_height) {
                return KF_DISTANCE_NONE;
            }
        } else if (point_y < position->vy - height) {
            return KF_DISTANCE_NONE;
        }
    }
    dx >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    dz >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    distance = SquareRoot0(dx * dx + dz * dz) << KF_LENGTH_SQUARE_DOWNSHIFT;
    if (max_distance < distance) {
        return KF_DISTANCE_NONE;
    }
    return distance;
}

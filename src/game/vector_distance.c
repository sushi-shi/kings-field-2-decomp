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
        goto reject;
    }
    dz = position->vz - point_z;
    if (dz < -max_distance || max_distance < dz) {
        goto reject;
    }
    if (point_y != KF_DISTANCE_IGNORE_HEIGHT) {
        if (position->vy < point_y) {
            if (position->vy >= point_y - point_height) {
                goto horizontal_distance;
            }
            goto reject;
        }
        if (point_y < position->vy - height) {
            goto reject;
        }
    }
horizontal_distance:
    dx >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    dz >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    distance = SquareRoot0(dx * dx + dz * dz) << KF_LENGTH_SQUARE_DOWNSHIFT;
    if (max_distance < distance) {
        goto reject;
    }
    return distance;
reject:
    return KF_DISTANCE_NONE;
}

enum {
    KF_DISTANCE_OUTSIDE_REACH = -9999999
};

ADDRESS(0x80015698, 0x114)
s32 vector_distance_between_with_reach(const VECTOR *first, s32 reach, const VECTOR *second,
                  s32 offset, s32 height)
{
    s32 limit = reach + offset;
    s32 x = second->vx - first->vx;
    s32 z;
    s32 y;
    s32 distance;

    if (x < -limit || limit < x) {
        return KF_DISTANCE_OUTSIDE_REACH;
    }
    z = second->vz - first->vz;
    if (z < -limit || limit < z) {
        return KF_DISTANCE_OUTSIDE_REACH;
    }
    y = second->vy - first->vy - (height >> 1);
    if (y < -limit || limit < y) {
        return KF_DISTANCE_OUTSIDE_REACH;
    }
    x >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    z >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    y >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    distance = SquareRoot0(x * x + z * z + y * y)
             << KF_LENGTH_SQUARE_DOWNSHIFT;
    if (limit < distance) {
        goto out_of_range;
    }
    if (distance > offset) {
        return distance - offset;
    }
    return 0;

out_of_range:
    return KF_DISTANCE_OUTSIDE_REACH;
}

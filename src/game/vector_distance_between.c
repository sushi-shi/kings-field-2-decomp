#include <kf/lib/address.h>
#include <kf/lib/math.h>

enum {
    KF_DISTANCE_OUTSIDE_REACH = -9999999
};

ADDRESS(0x80015698, 0x114)
s32 func_80015698(const VECTOR *first, s32 reach, const VECTOR *second,
                  s32 offset, s32 height)
{
    s32 limit = reach + offset;
    s32 x = second->vx - first->vx;
    s32 z;
    s32 y;
    s32 distance;

    if (x < -limit || limit < x) {
        goto out_of_range;
    }
    z = second->vz - first->vz;
    if (z < -limit || limit < z) {
        goto out_of_range;
    }
    y = second->vy - first->vy - (height >> 1);
    if (y < -limit || limit < y) {
        goto out_of_range;
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

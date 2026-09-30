#include <kf/lib/address.h>
#include <kf/lib/math.h>

ADDRESS(0x80015918, 0x2b0)
s32 func_80015918(s32 mode, s32 horizontal_distance,
    s32 vertical_distance, s32 speed, s32 amplitude,
    s32 *travel_time, s32 *angle)
{
    s32 amplitude_squared = amplitude * amplitude;
    s32 speed_squared = speed * speed;
    s32 acceleration = amplitude_squared >> 6;
    s32 discriminant = acceleration * acceleration
        - (((speed_squared * horizontal_distance) >> 10)
           * (horizontal_distance >> 2))
        - (((speed * acceleration) >> 3) * (vertical_distance >> 2));
    s32 square_root;
    s32 midpoint;
    s32 longer_time;
    s32 shorter_time;
    s32 chosen_time;
    s32 result_time;
    s32 horizontal_component;
    s32 vertical_component;

    if (discriminant < 0) {
        return -1;
    }
    square_root = SquareRoot0(discriminant << 4) << 2;
    midpoint = (amplitude_squared >> 2) - ((vertical_distance * speed) >> 2);
    longer_time = ((midpoint + square_root) << 1) / speed_squared;
    shorter_time = ((midpoint - square_root) << 1) / speed_squared;
    if (longer_time <= 0 && shorter_time <= 0) {
        return -1;
    }
    if (mode == 0 && longer_time <= 0) {
        chosen_time = shorter_time;
    } else if (mode == 0) {
        chosen_time = shorter_time;
        if (shorter_time >= longer_time) {
            chosen_time = longer_time;
        }
    } else {
        chosen_time = shorter_time;
        if (longer_time >= shorter_time) {
            chosen_time = longer_time;
        }
    }
    result_time = SquareRoot0(chosen_time << 14);
    horizontal_component = ((horizontal_distance << 14) / amplitude) / result_time;
    vertical_component = (((vertical_distance + ((speed * chosen_time) << 1)) << 12)
        / (amplitude >> 1)) / (result_time >> 1);
    *travel_time = result_time;
    *angle = vector_xz_to_angle(vertical_component, horizontal_component);
    return 0;
}

ADDRESS(0x80015bc8, 0x118)
s32 func_80015bc8(s32 mode, s32 source_x, s32 source_y,
    s32 source_z, s32 target_x, s32 target_y, s32 target_z,
    s32 speed, s32 amplitude, s16 *result, s16 *motion_x, s16 *motion_z)
{
    union {
        s32 word;
        u16 half;
    } result_value;
    s32 angle;
    s32 status;
    s32 distance = fixed_vector2_length(target_x - source_x,
                                       target_z - source_z);

    status = func_80015918(mode, distance, source_y - target_y,
                           speed, amplitude, &result_value.word, &angle);
    if (status == 0) {
        *motion_x = (amplitude * rcos(angle)) >> KF_FIXED12_BITS;
        *motion_z = (amplitude * -rsin(angle)) >> KF_FIXED12_BITS;
        *result = result_value.half;
    }
    return status;
}

ADDRESS(0x80015ce0, 0x70)
void func_80015ce0(const VECTOR *origin, const SVECTOR *delta, s32 scale,
    VECTOR *output)
{
    setVector(output, delta->vx * scale + origin->vx,
              delta->vy * scale + origin->vy,
              delta->vz * scale + origin->vz);
}

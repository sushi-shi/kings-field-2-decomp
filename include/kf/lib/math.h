#ifndef KF_LIB_MATH_H
#define KF_LIB_MATH_H

/* Game-owned fixed-point angle, vector, and matrix types and helpers. */

#include <kf/lib/bool.h>
#include <kf/lib/types.h>
#include <psyq/sdk.h>

enum {
    KF_FIXED11_BITS = 11,
    KF_FIXED12_BITS = 12,
    KF_FIXED12_ONE = 0x1000,
    KF_ANGLE_QUARTER_TURN = 0x400,
    KF_ANGLE_HALF_TURN = 0x800,
    KF_ANGLE_THREE_QUARTER_TURN = 0xc00,
    KF_ANGLE_FULL_TURN = 0x1000,
    KF_ANGLE_WRAP_MASK = 0xfff,
    KF_LENGTH_SQUARE_DOWNSHIFT = 3
};

/* vector_distance_to_point: a point_y of KF_DISTANCE_IGNORE_HEIGHT skips the
 * vertical test; KF_DISTANCE_NONE reports a point out of range. */
enum {
    KF_DISTANCE_IGNORE_HEIGHT = 0xffff,
    KF_DISTANCE_NONE = -1
};

/* Quarter turns about Y applied by matrix_rotate_quarter_turns and
 * svector_rotate_quarter_turns. */
enum {
    KF_QUARTER_TURN_0 = 0,
    KF_QUARTER_TURN_1 = 1,
    KF_QUARTER_TURN_2 = 2,
    KF_QUARTER_TURN_3 = 3
};

/* 32-bit X/Z pair; King's Field (SLPS-00017) used an s16 pair here. */
struct KfVecXZi {
    s32 x;
    s32 z;
};

struct KfEulerAngles {
    s16 x;
    s16 y;
    s16 z;
};

s16 angle_approach(s16 current, s16 target, s32 step);
s32 value_approach(s32 current, s32 target, s32 step);
void vector_direction_scaled(VECTOR *from, VECTOR *to, s32 scale, SVECTOR *direction);
s32 angle_velocity_step(s32 current, s32 target, s32 velocity, s32 acceleration, s32 damping);
void angle_to_forward_xz(s16 angle, struct KfVecXZi *direction);
void matrix_rotate_quarter_turns(MATRIX *source, MATRIX *destination, s32 turns);
void svector_rotate_quarter_turns(SVECTOR *source, SVECTOR *destination, s32 turns);
void matrix_set_rotation_x(s16 angle, MATRIX *matrix);
void matrix_set_rotation_y(s16 angle, MATRIX *matrix);
void matrix_set_rotation_z(s16 angle, MATRIX *matrix);
void matrix_set_rotation_yxz(const struct KfEulerAngles *angles, MATRIX *matrix);
void matrix_set_rotation_xzy(const struct KfEulerAngles *angles, MATRIX *matrix);
void pitch_yaw_to_forward_vector(const struct KfEulerAngles *angles, SVECTOR *direction);
void vector_rotate_yxz(const struct KfEulerAngles *angles, SVECTOR *vector, VECTOR *result);
void vector2i_scale_shift11(s16 scale, struct KfVecXZi *vector);
void vector3s_scale_shift12(s16 scale, SVECTOR *vector);
void vector2i_scale_shift12(s16 scale, s32 *vector);
void vector3s_scale_shift12_alt(s16 scale, s16 *vector);
void vector3i_add_xz(VECTOR *destination, const struct KfVecXZi *delta);
KfBool angle_within_tolerance(int lhs, int rhs, s16 range);
KfBool angle_mod_delta_le_half_turn(int lhs, int rhs);
s32 angle_shortest_delta(s32 from, s32 to);
s32 vector_xz_to_angle(s32 x, s32 z);
s32 fixed_vector2_length(s32 x, s32 y);
s32 fixed_vector3_length(s32 x, s32 y, s32 z);
s32 vector_distance_to_point(
    const VECTOR *position, s32 point_x, s32 point_y, s32 point_z,
    s32 max_distance, s32 height, s32 point_height);
void func_800154fc(s32 x, s32 y, s32 z, struct KfEulerAngles *angles);
KfBool func_80015574(s32 first, s32 first_width, s32 second, s32 second_width);
s32 vector_distance_between_with_reach(const VECTOR *first, s32 reach, const VECTOR *second,
                  s32 offset, s32 height);
s32 random_triangular_scaled(s32 amplitude);
s32 random_centered_triangular_scaled(s32 amplitude);
s32 fixed_lerp_q12(s32 start, s32 end, s32 fraction);
s32 angle_lerp_shortest_q12(s32 start, s32 end, s32 fraction);
void fixed_lerp_nine_halfwords_q12(const u16 *start, const u16 *end, u16 *output, s16 fraction);
s32 func_80015918(s32 mode, s32 horizontal_distance,
    s32 vertical_distance, s32 speed, s32 amplitude,
    s32 *travel_time, s32 *angle);
s32 func_80015bc8(s32 mode, s32 source_x, s32 source_y,
    s32 source_z, s32 target_x, s32 target_y, s32 target_z,
    s32 speed, s32 amplitude, s16 *result, s16 *motion_x, s16 *motion_z);
void func_80015ce0(const VECTOR *origin, const SVECTOR *delta, s32 scale,
    VECTOR *output);

static inline s16 angle_error_magnitude(s16 difference)
{
    s16 folded;
    difference &= KF_ANGLE_WRAP_MASK;
    folded = difference;
    if (difference > KF_ANGLE_HALF_TURN) {
        folded = KF_ANGLE_FULL_TURN - difference;
    }
    return folded;
}

#endif

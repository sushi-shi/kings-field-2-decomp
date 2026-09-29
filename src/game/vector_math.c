#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <psyq/libc.h>

/*
 * Fixed-point direction and angle helpers adjacent to the rotation builders
 * in matrix_rotation.c.
 */

ADDRESS(0x80015034, 0xd0)
void pitch_yaw_to_forward_vector(const struct KfEulerAngles *angles, SVECTOR *direction)
{
    MATRIX pitch_matrix;
    MATRIX yaw_matrix;
    SVECTOR source;
    VECTOR result;

    setVector(&source, 0, 0, KF_FIXED12_ONE);
    matrix_set_rotation_x(-angles->x, &pitch_matrix);
    ApplyMatrix(&pitch_matrix, &source, &result);
    copyVector(&source, &result);
    matrix_set_rotation_y(angles->y, &yaw_matrix);
    ApplyMatrix(&yaw_matrix, &source, &result);
    copyVector(direction, &result);
}

ADDRESS(0x80015104, 0x44)
void vector_rotate_yxz(const struct KfEulerAngles *angles, SVECTOR *vector, VECTOR *result)
{
    MATRIX rotation;

    matrix_set_rotation_yxz(angles, &rotation);
    ApplyMatrix(&rotation, vector, result);
}

ADDRESS(0x80015148, 0x40)
void vector2i_scale_shift11(s16 scale, struct KfVecXZi *vector)
{
    s32 x = scale * vector->x;
    s32 z = scale * vector->z;

    vector->x = x >> KF_FIXED11_BITS;
    vector->z = z >> KF_FIXED11_BITS;
}

ADDRESS(0x80015188, 0x5c)
void vector3s_scale_shift12(s16 scale, SVECTOR *vector)
{
    s32 x = vector->vx * scale;
    s32 y = vector->vy * scale;
    s32 z = vector->vz * scale;

    setVector(vector, x >> KF_FIXED12_BITS, y >> KF_FIXED12_BITS, z >> KF_FIXED12_BITS);
}

ADDRESS(0x800151e4, 0x40)
void vector2i_scale_shift12(s16 scale, s32 *vector)
{
    s32 x = scale * vector[0];
    s32 y = scale * vector[1];

    vector[0] = x >> KF_FIXED12_BITS;
    vector[1] = y >> KF_FIXED12_BITS;
}

ADDRESS(0x80015224, 0x5c)
void vector3s_scale_shift12_alt(s16 scale, s16 *vector)
{
    s32 x = vector[0] * scale;
    s32 y = vector[1] * scale;
    s32 z = vector[2] * scale;

    vector[0] = x >> KF_FIXED12_BITS;
    vector[1] = y >> KF_FIXED12_BITS;
    vector[2] = z >> KF_FIXED12_BITS;
}

ADDRESS(0x80015280, 0x2c)
void vector3i_add_xz(VECTOR *destination, const struct KfVecXZi *delta)
{
    destination->vx += delta->x;
    destination->vz += delta->z;
}

ADDRESS(0x800152ac, 0x3c)
KfBool angle_within_tolerance(int lhs, int rhs, s16 range)
{
    int delta = (lhs - rhs) & KF_ANGLE_WRAP_MASK;

    return delta <= range || KF_ANGLE_FULL_TURN - range <= delta;
}

ADDRESS(0x800152e8, 0x10)
KfBool angle_mod_delta_le_half_turn(int lhs, int rhs)
{
    return ((lhs - rhs) & KF_ANGLE_WRAP_MASK) < (KF_ANGLE_HALF_TURN + 1);
}

/* Signed turn from FROM to TO in -0x7ff..0x800. */
ADDRESS(0x800152f8, 0x20)
s32 angle_shortest_delta(s32 from, s32 to)
{
    s32 difference = (to - from) & KF_ANGLE_WRAP_MASK;

    if (difference > KF_ANGLE_HALF_TURN) {
        return difference - KF_ANGLE_FULL_TURN;
    }
    return difference;
}

/* Psy-Q LIBGTE: catan(long) returns a 12-bit angle for a 12-bit fixed ratio.
 * The larger component is the divisor, keeping the ratio within one. */
ADDRESS(0x80015318, 0x150)
s32 vector_xz_to_angle(s32 x, s32 z)
{
    if (abs(x) <= abs(z)) {
        if (z > 0) {
            return -catan((x << KF_FIXED12_BITS) / z) & KF_ANGLE_WRAP_MASK;
        }
        if (z < 0) {
            return KF_ANGLE_HALF_TURN - catan((x << KF_FIXED12_BITS) / z);
        }
        return 0;
    }
    if (x >= 0) {
        return catan((z << KF_FIXED12_BITS) / x) + KF_ANGLE_THREE_QUARTER_TURN;
    }
    return catan((z << KF_FIXED12_BITS) / x) + KF_ANGLE_QUARTER_TURN;
}

ADDRESS(0x80015468, 0x40)
s32 fixed_vector2_length(s32 x, s32 y)
{
    x >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    y >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    return SquareRoot0(x * x + y * y) << KF_LENGTH_SQUARE_DOWNSHIFT;
}

ADDRESS(0x800154a8, 0x54)
s32 fixed_vector3_length(s32 x, s32 y, s32 z)
{
    x >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    y >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    z >>= KF_LENGTH_SQUARE_DOWNSHIFT;
    return SquareRoot0(x * x + y * y + z * z) << KF_LENGTH_SQUARE_DOWNSHIFT;
}

#include <kf/lib/address.h>
#include <kf/lib/math.h>

/*
 * Fixed-point angle and rotation-matrix builders. The direction and angle
 * helpers continue in vector_math.c; the module boundary remains WIP.
 */

/*
 * Angles are 12-bit (0..0xfff). Within a half turn the step is applied
 * directly and clamped at the target; beyond it the angle wraps the other
 * way and the wrapped value is clamped only while it stays on the target's
 * side of the half-turn boundary.
 */
ADDRESS(0x800147a0, 0xc4)
s16 angle_approach(s16 current, s16 target, s32 step)
{
    s16 result;

    current &= KF_ANGLE_WRAP_MASK;
    target &= KF_ANGLE_WRAP_MASK;
    if (target == current) {
        return target;
    }
    if (current < target) {
        if (target - current <= KF_ANGLE_HALF_TURN) {
            result = current + step;
            if (target < result) {
                return target;
            }
        } else {
            result = (current - step) & KF_ANGLE_WRAP_MASK;
            if (result >= KF_ANGLE_HALF_TURN && result <= target) {
                return target;
            }
        }
    } else {
        if (current - target <= KF_ANGLE_HALF_TURN) {
            result = current - step;
            if (result < target) {
                return target;
            }
        } else {
            result = (current + step) & KF_ANGLE_WRAP_MASK;
            if (result <= KF_ANGLE_HALF_TURN && result >= target) {
                return target;
            }
        }
    }
    return result;
}

/* Moves CURRENT toward TARGET by |STEP| without passing it; a negative STEP
 * is applied with the opposite sign. */
ADDRESS(0x80014864, 0x68)
s32 value_approach(s32 current, s32 target, s32 step)
{
    if (step > 0) {
        if (current < target) {
            current += step;
            if (target < current) {
                current = target;
            }
        } else if (target < current) {
            current -= step;
            if (current < target) {
                current = target;
            }
        }
    } else {
        if (target < current) {
            current += step;
            if (current < target) {
                current = target;
            }
        } else if (current < target) {
            current -= step;
            if (target < current) {
                current = target;
            }
        }
    }
    return current;
}

/* DIRECTION = (TO - FROM) scaled to length SCALE. */
ADDRESS(0x800148cc, 0x13c)
void vector_direction_scaled(VECTOR *from, VECTOR *to, s32 scale, SVECTOR *direction)
{
    s32 dx = to->vx - from->vx;
    s32 dy = to->vy - from->vy;
    s32 dz = to->vz - from->vz;
    s32 length = fixed_vector3_length(dx, dy, dz);

    direction->vx = dx * scale / length;
    direction->vy = dy * scale / length;
    direction->vz = dz * scale / length;
}

/* Turns VELOCITY toward TARGET by ACCELERATION and damps it, unless CURRENT
 * already faces TARGET and VELOCITY is within one ACCELERATION of rest. */
ADDRESS(0x80014a08, 0xb8)
s32 angle_velocity_step(s32 current, s32 target, s32 velocity, s32 acceleration, s32 damping)
{
    if (angle_within_tolerance(current, target, acceleration >> 1)
        && velocity <= acceleration && -acceleration <= velocity) {
        return velocity;
    }
    if (target != current) {
        if (angle_mod_delta_le_half_turn(target, current)) {
            velocity -= acceleration;
        } else {
            velocity += acceleration;
        }
    }
    if (velocity > 0) {
        velocity -= damping;
    } else if (velocity < 0) {
        velocity += damping;
    }
    return velocity;
}

ADDRESS(0x80014ac0, 0x4c)
void angle_to_forward_xz(s16 angle, struct KfVecXZi *direction)
{
    direction->x = -rsin(angle);
    direction->z = rcos(angle);
}

/* DESTINATION = SOURCE turned by TURNS quarter turns about Y. */
ADDRESS(0x80014b0c, 0x224)
void matrix_rotate_quarter_turns(MATRIX *source, MATRIX *destination, s32 turns)
{
    switch (turns) {
    case KF_QUARTER_TURN_0:
        destination->m[0][0] = source->m[0][0];
        destination->m[0][1] = source->m[0][1];
        destination->m[0][2] = source->m[0][2];
        destination->m[1][0] = source->m[1][0];
        destination->m[1][1] = source->m[1][1];
        destination->m[1][2] = source->m[1][2];
        destination->m[2][0] = source->m[2][0];
        destination->m[2][1] = source->m[2][1];
        destination->m[2][2] = source->m[2][2];
        break;
    case KF_QUARTER_TURN_1:
        destination->m[0][0] = -source->m[0][2];
        destination->m[0][1] = source->m[0][1];
        destination->m[0][2] = source->m[0][0];
        destination->m[1][0] = -source->m[1][2];
        destination->m[1][1] = source->m[1][1];
        destination->m[1][2] = source->m[1][0];
        destination->m[2][0] = -source->m[2][2];
        destination->m[2][1] = source->m[2][1];
        destination->m[2][2] = source->m[2][0];
        break;
    case KF_QUARTER_TURN_2:
        destination->m[0][0] = -source->m[0][0];
        destination->m[0][1] = source->m[0][1];
        destination->m[0][2] = -source->m[0][2];
        destination->m[1][0] = -source->m[1][0];
        destination->m[1][1] = source->m[1][1];
        destination->m[1][2] = -source->m[1][2];
        destination->m[2][0] = -source->m[2][0];
        destination->m[2][1] = source->m[2][1];
        destination->m[2][2] = -source->m[2][2];
        break;
    case KF_QUARTER_TURN_3:
        destination->m[0][0] = source->m[0][2];
        destination->m[0][1] = source->m[0][1];
        destination->m[0][2] = -source->m[0][0];
        destination->m[1][0] = source->m[1][2];
        destination->m[1][1] = source->m[1][1];
        destination->m[1][2] = -source->m[1][0];
        destination->m[2][0] = source->m[2][2];
        destination->m[2][1] = source->m[2][1];
        destination->m[2][2] = -source->m[2][0];
        break;
    }
}

ADDRESS(0x80014d30, 0xe4)
void svector_rotate_quarter_turns(SVECTOR *source, SVECTOR *destination, s32 turns)
{
    switch (turns) {
    case KF_QUARTER_TURN_0:
        destination->vx = source->vx;
        destination->vy = source->vy;
        destination->vz = source->vz;
        break;
    case KF_QUARTER_TURN_1:
        destination->vx = -source->vz;
        destination->vy = source->vy;
        destination->vz = source->vx;
        break;
    case KF_QUARTER_TURN_2:
        destination->vx = -source->vx;
        destination->vy = source->vy;
        destination->vz = -source->vz;
        break;
    case KF_QUARTER_TURN_3:
        destination->vx = source->vz;
        destination->vy = source->vy;
        destination->vz = -source->vx;
        break;
    }
}

ADDRESS(0x80014e14, 0x70)
void matrix_set_rotation_x(s16 angle, MATRIX *matrix)
{
    s32 sin = rsin(angle);
    s32 cos = rcos(angle);

    matrix->m[0][0] = KF_FIXED12_ONE;
    matrix->m[0][1] = 0;
    matrix->m[0][2] = 0;
    matrix->m[1][0] = 0;
    matrix->m[1][1] = cos;
    matrix->m[1][2] = -sin;
    matrix->m[2][0] = 0;
    matrix->m[2][1] = sin;
    matrix->m[2][2] = cos;
}

ADDRESS(0x80014e84, 0x70)
void matrix_set_rotation_y(s16 angle, MATRIX *matrix)
{
    s32 sin = rsin(angle);
    s32 cos = rcos(angle);

    matrix->m[0][0] = cos;
    matrix->m[0][1] = 0;
    matrix->m[0][2] = -sin;
    matrix->m[1][0] = 0;
    matrix->m[1][1] = KF_FIXED12_ONE;
    matrix->m[1][2] = 0;
    matrix->m[2][0] = sin;
    matrix->m[2][1] = 0;
    matrix->m[2][2] = cos;
}

ADDRESS(0x80014ef4, 0x70)
void matrix_set_rotation_z(s16 angle, MATRIX *matrix)
{
    s32 sin = rsin(angle);
    s32 cos = rcos(angle);

    matrix->m[0][0] = cos;
    matrix->m[0][1] = -sin;
    matrix->m[0][2] = 0;
    matrix->m[1][0] = sin;
    matrix->m[1][1] = cos;
    matrix->m[1][2] = 0;
    matrix->m[2][0] = 0;
    matrix->m[2][1] = 0;
    matrix->m[2][2] = KF_FIXED12_ONE;
}

/* MATRIX = Ry * Rx * Rz. */
ADDRESS(0x80014f64, 0x68)
void matrix_set_rotation_yxz(const struct KfEulerAngles *angles, MATRIX *matrix)
{
    MATRIX temporary;

    matrix_set_rotation_z(angles->z, &temporary);
    matrix_set_rotation_x(angles->x, matrix);
    MulMatrix(matrix, &temporary);
    matrix_set_rotation_y(angles->y, &temporary);
    MulMatrix2(&temporary, matrix);
}

/* MATRIX = Rx * Rz * Ry. */
ADDRESS(0x80014fcc, 0x68)
void matrix_set_rotation_xzy(const struct KfEulerAngles *angles, MATRIX *matrix)
{
    MATRIX temporary;

    matrix_set_rotation_y(angles->y, &temporary);
    matrix_set_rotation_z(angles->z, matrix);
    MulMatrix(matrix, &temporary);
    matrix_set_rotation_x(angles->x, &temporary);
    MulMatrix2(&temporary, matrix);
}

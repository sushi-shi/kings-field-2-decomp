#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>

extern s32 func_8003ae50(s16 *motion, s32 target);
extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius,
                          s32 height_and_flags, s32 mode);

ADDRESS(0x8003b9a4, 0x140)
s32 func_8003b9a4(s32 decay, s32 target)
{
    KfActor *actor = actor_state.current;
    s32 length;

    if (actor->unknown_0d == 0) {
        length = SquareRoot0(actor->unknown_50 * actor->unknown_50
                           + actor->unknown_54 * actor->unknown_54);
        if (length == 0) {
            return 0;
        }
        actor->unknown_50 = value_approach(actor->unknown_50, 0,
            (actor->unknown_50 * decay * 2) / length);
        actor->unknown_54 = value_approach(actor->unknown_54, 0,
            (actor->unknown_54 * decay * 2) / length);
    }
    return func_8003ae50(&actor->unknown_50, target);
}

ADDRESS(0x8003bae4, 0xbc)
s32 func_8003bae4(s16 angle, s32 speed, s32 step, s32 target)
{
    KfActor *actor = actor_state.current;
    struct KfVecXZi direction;
    struct KfVecXZi step_direction;

    angle_to_forward_xz(angle, &direction);
    step_direction = direction;
    vector2i_scale_shift11(speed, &direction);
    vector2i_scale_shift11(step, &step_direction);
    actor->unknown_50 = value_approach(actor->unknown_50,
                                        direction.x, step_direction.x);
    actor->unknown_54 = value_approach(actor->unknown_54,
                                        direction.z, step_direction.z);
    return func_8003ae50(&actor->unknown_50, target);
}

ADDRESS(0x8003bba0, 0x130)
void func_8003bba0(KfActor *actor, s32 target_angle, s32 max_speed,
    s32 acceleration)
{
    if (angle_shortest_delta(target_angle, actor->rotation.y) != 0) {
        s32 old_angle;

        if (angle_mod_delta_le_half_turn(target_angle, actor->rotation.y)) {
            actor->unknown_58 += acceleration;
            if (max_speed < (s16)actor->unknown_58) {
                actor->unknown_58 = max_speed;
            }
        } else {
            actor->unknown_58 -= acceleration;
            if ((s16)actor->unknown_58 < -max_speed) {
                actor->unknown_58 = -max_speed;
            }
        }

        old_angle = actor->rotation.y;
        actor->rotation.y += actor->unknown_58;
        if ((s16)actor->unknown_58 > 0) {
            if (angle_mod_delta_le_half_turn(target_angle, old_angle) &&
                !angle_mod_delta_le_half_turn(target_angle, actor->rotation.y)) {
                actor->rotation.y = target_angle;
            }
        } else {
            if (angle_mod_delta_le_half_turn(old_angle, target_angle) &&
                !angle_mod_delta_le_half_turn(actor->rotation.y, target_angle)) {
                actor->rotation.y = target_angle;
            }
        }
    } else {
        actor->unknown_58 = 0;
    }
}

ADDRESS(0x8003bcd0, 0x70)
s32 func_8003bcd0(s16 angle, s32 speed, s32 range, s32 step,
    s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;

    func_8003bba0(actor, angle, range, mode);
    return func_8003bae4(actor->rotation.y, speed, step, target);
}

ADDRESS(0x8003bd40, 0xf8)
s32 func_8003bd40(s32 world_x, s32 world_z, s32 speed, s32 range,
                  s16 reference_angle, s32 step, s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;
    s32 dx = world_x - actor->position.vx;
    s32 dz = world_z - actor->position.vz;
    s32 angle = vector_xz_to_angle(dx, dz);

    if (dx < 0) {
        dx = -dx;
    }
    if (dz < 0) {
        dz = -dz;
    }
    if (reference_angle != -1 && dx + dz <= 600
        && !angle_within_tolerance(angle, reference_angle, 0x320)) {
        return -1;
    }
    func_8003bcd0(angle, speed, range, step, mode, target);
    return angle & KF_ANGLE_WRAP_MASK;
}

ADDRESS(0x8003be38, 0x13c)
s32 func_8003be38(const struct KfEulerAngles *angles, s32 speed,
                  s32 step, s32 target)
{
    KfActor *actor = actor_state.current;
    SVECTOR direction;
    SVECTOR step_direction;
    s32 moved;
    s32 proposed_y;
    s32 radius;
    s32 height_and_flags;

    pitch_yaw_to_forward_vector(angles, &direction);
    step_direction = direction;
    vector3s_scale_shift12(speed, &direction);
    vector3s_scale_shift12(step, &step_direction);
    actor->unknown_50 = value_approach(actor->unknown_50,
                                        direction.vx, step_direction.vx);
    actor->unknown_52 = value_approach(actor->unknown_52,
                                        direction.vy, step_direction.vy);
    actor->unknown_54 = value_approach(actor->unknown_54,
                                        direction.vz, step_direction.vz);
    moved = func_8003ae50(&actor->unknown_50, target) != 0;
    proposed_y = actor->position.vy + actor->unknown_52;
    radius = actor->unknown_1c;
    height_and_flags = actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16);
    if (func_8002b9d4(actor->position.vx, proposed_y, actor->position.vz,
                      radius, height_and_flags,
                      actor_state.unknown_93a4) == 0) {
        actor->position.vy = proposed_y;
    } else {
        moved |= 2;
    }
    return moved;
}

ADDRESS(0x8003bf74, 0x8c)
s32 func_8003bf74(const struct KfEulerAngles *angles, s32 speed,
    s32 range, s32 step, s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;

    func_8003bba0(actor, angles->y, range, mode);
    actor->rotation.x = angle_approach(actor->rotation.x, angles->x, 8);
    return func_8003be38(&actor->rotation, speed, step, target);
}

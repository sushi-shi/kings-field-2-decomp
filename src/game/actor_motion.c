#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>
#include <kf/lib/math.h>

ADDRESS(0x8003b33c, 0x1e4)
s32 actor_move_with_collision(SVECTOR *motion)
{
    KfActor *actor = actor_state.current;
    VECTOR proposed;
    s32 result;

    proposed.vx = actor->position.vx + motion->vx;
    proposed.vy = actor->position.vy + motion->vy;
    proposed.vz = actor->position.vz + motion->vz;
    result = collision_query_world(proposed.vx, proposed.vy, proposed.vz,
        actor->unknown_1c,
        actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16),
        actor_state.unknown_93a4);
    if (result == 0) {
        copyVector(&actor->position, &proposed);
    } else if (collision_query_world(proposed.vx, actor->position.vy,
                             actor->position.vz, actor->unknown_1c,
                             actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16),
                             actor_state.unknown_93a4) != 0) {
        motion->vx = -(u16)motion->vx;
    } else if (collision_query_world(actor->position.vx, proposed.vy,
                             actor->position.vz, actor->unknown_1c,
                             actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16),
                             actor_state.unknown_93a4) != 0) {
        motion->vy = -(u16)motion->vy;
    } else if (collision_query_world(actor->position.vx, actor->position.vy,
                             proposed.vz, actor->unknown_1c,
                             actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16),
                             actor_state.unknown_93a4) != 0) {
        motion->vz = -(u16)motion->vz;
    }
    return result;
}

ADDRESS(0x8003b520, 0x9c)
s32 actor_start_ballistic_motion(s32 mode, s32 target_x, s32 target_y,
    s32 target_z, s32 trajectory_parameter, s32 trajectory_speed)
{
    KfActor *actor = actor_state.current;
    s16 result;

    if (func_80015bc8(mode, actor->position.vx, actor->position.vy,
        actor->position.vz, target_x, target_y, target_z,
        trajectory_parameter, trajectory_speed, &result,
        &actor->unknown_68, &actor->unknown_6a) != 0) {
        return -1;
    }
    actor->unknown_0d = 0x30;
    actor->unknown_52 = 1;
    actor->unknown_6c = trajectory_parameter;
    actor->unknown_3c = actor->position.vy;
    return result;
}

ADDRESS(0x8003b5bc, 0x14)
void actor_suspend_vertical_motion(void)
{
    actor_state.current->unknown_0d = 0x60;
}

ADDRESS(0x8003b5d0, 0x3d4)
void actor_update_vertical_motion(void)
{
    KfActor *actor = actor_state.current;
    KfTargetGroup *group = actor_state.active_group;
    s32 vertical_state;

    func_8002b604(actor->position.vx, actor->position.vy, actor->position.vz,
                   actor->unknown_1c,
                   actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16));
    actor->unknown_03 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
    if (actor->unknown_28 & 0x400) {
        KF_COLLISION_CACHE_RESULT = KF_COLLISION_CACHE_HEIGHT;
    }

    vertical_state = actor->unknown_0d;
    if (vertical_state == 0x20) goto state_20;
    if (vertical_state < 33) {
        if (vertical_state == 0) goto state_0;
        if (vertical_state == 0x10) goto state_10;
        return;
    }
    if (vertical_state == 0x30) goto state_30;
    return;

state_0: {
        s32 next_y;
        next_y = KF_COLLISION_CACHE_RESULT - actor->position.vy;
        if (next_y < 0) {
            actor->unknown_0d = 0x20;
            actor->unknown_52 = -100;
        } else if (next_y > 0) {
            actor->unknown_0d = 0x10;
            actor->unknown_52 = 0;
        }
        return;
    }

state_10: {
        s32 next_y;
        s32 collision;
        next_y = actor->position.vy + actor->unknown_52;
        collision = collision_query_world(actor->position.vx, next_y,
                                  actor->position.vz, actor->unknown_1c,
                                  actor->unknown_1e |
                                      ((actor->unknown_28 & 0xc000) << 16),
                                  actor_state.unknown_93a4);
        if (collision == 0) {
        advance_rise:
            actor->position.vy = next_y;
            actor->unknown_52 += group->unknown_05;
            return;
        }
        if (collision == 0x80 && actor->unknown_52 > 40) {
            player_apply_damage(0, group->unknown_06, 0, 0, 0, 0, 0, 0, 0,
                          0x1000, 10, &actor->position);
        }
        if (collision & 4) {
            if (actor->unknown_28 & 0x400) {
                s32 floor_y = KF_COLLISION_CACHE_HEIGHT;
                if (actor->position.vy < floor_y) goto advance_rise;
                actor->position.vy = floor_y;
            } else {
                actor->position.vy = KF_COLLISION_CACHE_RESULT;
            }
            actor->unknown_52 = 0;
            actor->unknown_0d = 0;
            return;
        }
        if (actor->unknown_28 & 0x400) goto advance_rise;
        actor->unknown_0d = 0;
        return;
    }

state_20:
        actor->position.vy += actor->unknown_52;
        actor->unknown_52 += 5;
        if (KF_COLLISION_CACHE_RESULT < actor->position.vy &&
            actor->unknown_52 < 0) {
            return;
        }
        actor->position.vy = KF_COLLISION_CACHE_RESULT;
        actor->unknown_0d = 0;
        return;

state_30: {
        s32 phase;
        s32 next_y;
        s32 collision;
        phase = actor->unknown_52;
        next_y = actor->unknown_3c - actor->unknown_6a * phase +
                 ((actor->unknown_6c * phase * phase) >> 1);
        collision = collision_query_world(actor->position.vx, next_y,
                                  actor->position.vz, actor->unknown_1c,
                                  actor->unknown_1e |
                                      ((actor->unknown_28 & 0xc000) << 16),
                                  actor_state.unknown_93a4);
        if (collision == 0) {
            actor->position.vy = next_y;
            actor->unknown_52++;
            actor->unknown_03 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
            return;
        }
        if (collision == 0x80) {
            player_apply_damage(0, group->unknown_06, 0, 0, 0, 0, 0, 0, 0,
                          0x1000, 10, &actor->position);
        }
        actor->unknown_0d = 0x10;
        actor->unknown_52 = 0;
        return;
    }
}

ADDRESS(0x8003b9a4, 0x140)
s32 actor_damp_horizontal_motion(s32 decay, s32 target)
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
    return actor_move_horizontal_with_collision((SVECTOR *)&actor->unknown_50, target);
}

ADDRESS(0x8003bae4, 0xbc)
s32 actor_move_along_heading(s16 angle, s32 speed, s32 step, s32 target)
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
    return actor_move_horizontal_with_collision((SVECTOR *)&actor->unknown_50, target);
}

ADDRESS(0x8003bba0, 0x130)
void actor_turn_toward_angle(KfActor *actor, s32 target_angle, s32 max_speed,
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
s32 actor_turn_and_move_along_heading(s16 angle, s32 speed, s32 range, s32 step,
    s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;

    actor_turn_toward_angle(actor, angle, range, mode);
    return actor_move_along_heading(actor->rotation.y, speed, step, target);
}

ADDRESS(0x8003bd40, 0xf8)
s32 actor_turn_and_move_toward_point(s32 world_x, s32 world_z, s32 speed, s32 range,
                  s16 reference_angle, s32 step, s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;
    s32 dx = (s32)((u32)world_x - (u32)actor->position.vx);
    s32 dz = (s32)((u32)world_z - (u32)actor->position.vz);
    s32 angle = vector_xz_to_angle(dx, dz);

    if (dx < 0) {
        dx = (s32)(0u - (u32)dx);
    }
    if (dz < 0) {
        dz = (s32)(0u - (u32)dz);
    }
    if (reference_angle != -1 && (s32)((u32)dx + (u32)dz) <= 600
        && !angle_within_tolerance(angle, reference_angle, 0x320)) {
        return -1;
    }
    actor_turn_and_move_along_heading(angle, speed, range, step, mode, target);
    return angle & KF_ANGLE_WRAP_MASK;
}

ADDRESS(0x8003be38, 0x13c)
s32 actor_move_along_euler_angles(const struct KfEulerAngles *angles, s32 speed,
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
    moved = actor_move_horizontal_with_collision((SVECTOR *)&actor->unknown_50, target) != 0;
    proposed_y = actor->position.vy + actor->unknown_52;
    radius = actor->unknown_1c;
    height_and_flags = actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16);
    if (collision_query_world(actor->position.vx, proposed_y, actor->position.vz,
                      radius, height_and_flags,
                      actor_state.unknown_93a4) == 0) {
        actor->position.vy = proposed_y;
    } else {
        moved |= 2;
    }
    return moved;
}

ADDRESS(0x8003bf74, 0x8c)
s32 actor_turn_and_move_along_euler_angles(const struct KfEulerAngles *angles, s32 speed,
    s32 range, s32 step, s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;

    actor_turn_toward_angle(actor, angles->y, range, mode);
    actor->rotation.x = angle_approach(actor->rotation.x, angles->x, 8);
    return actor_move_along_euler_angles(&actor->rotation, speed, step, target);
}

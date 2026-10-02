#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>

extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius,
                          s32 height_and_flags, s32 mode);
extern s32 func_8002b7f8(s32 x, s32 y, s32 z, s32 radius,
                          s32 height_and_flags);
extern void func_8002b874(void);

ADDRESS(0x8003a9f4, 0x168)
s32 func_8003a9f4(s32 x, s32 y, s32 z, s32 radius, s32 height)
{
    KfActor *actor = actor_state.actors;
    s32 index;

    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        VECTOR alternate;

        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE || actor->target_type == 3
            || (actor_state.unknown_93a0 & actor->unknown_28)
            || actor == actor_state.current) {
            continue;
        }
        if (actor->unknown_28 & 0x10) {
            if (actor->unknown_22 == actor_state.unknown_93b8) {
                continue;
            }
            alternate.vx = actor->position.vx;
            alternate.vz = actor->position.vz;
            alternate.vy = actor->position.vy + actor->unknown_26;
            if (vector_distance_to_point(&alternate, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        } else {
            if (vector_distance_to_point(&actor->position, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        }
    }
    return -1;
}

ADDRESS(0x8003ab5c, 0x158)
s32 func_8003ab5c(s32 x, s32 y, s32 z, s32 radius, s32 height)
{
    KfActor *actor = actor_state.actors;
    s32 index;

    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        VECTOR alternate;

        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE
            || (actor_state.unknown_93a0 & actor->unknown_28)
            || actor == actor_state.current) {
            continue;
        }
        if (actor->unknown_28 & 0x10) {
            if (actor->unknown_22 == actor_state.unknown_93b8) {
                continue;
            }
            alternate.vx = actor->position.vx;
            alternate.vz = actor->position.vz;
            alternate.vy = actor->position.vy + actor->unknown_26;
            if (vector_distance_to_point(&alternate, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        } else {
            if (vector_distance_to_point(&actor->position, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        }
    }
    return -1;
}

ADDRESS(0x8003acb4, 0xdc)
void actor_bind_current(KfActor *actor)
{
    KfTargetGroup *group;
    KfActor *other;

    if (actor != NULL) {
        actor_state.current = actor;
        group = &actor_state.target_groups[actor->group_index];
        actor_state.active_group = group;
        actor_state.current_group_index = actor->group_index;
        if (group->unknown_34 & 0x10) {
            other = &actor_state.actors[actor->unknown_22];
            actor_state.other_actor = other;
            actor_state.other_group = &actor_state.target_groups[other->group_index];
        }
    } else {
        actor_state.current = NULL;
        actor_state.active_group = NULL;
        actor_state.current_group_index = 0;
        actor_state.other_actor = NULL;
        actor_state.other_group = NULL;
    }
}

ADDRESS(0x8003ad90, 0x34)
void actor_advance_animation_wrapped(KfActor *actor, s16 delta)
{
    if (delta < 0) {
        actor->animation_step = -delta;
    } else {
        actor->animation_step = delta;
    }
    actor->animation_phase = (actor->animation_phase + delta) & KF_ACTOR_ANIMATION_PHASE_MAX;
}

ADDRESS(0x8003adc4, 0x5c)
void actor_advance_animation_clamped(KfActor *actor, s16 delta)
{
    s16 phase;

    if (delta < 0) {
        actor->animation_step = -delta;
    } else {
        actor->animation_step = delta;
    }
    phase = actor->animation_phase + delta;
    actor->animation_phase = phase;
    if (phase >= KF_ACTOR_ANIMATION_PHASE_PERIOD) {
        actor->animation_phase = KF_ACTOR_ANIMATION_PHASE_MAX;
    } else if (phase < 0) {
        actor->animation_phase = 0;
    }
}

ADDRESS(0x8003ae20, 0x30)
KfBool32 actor_animation_crossed_phase(const KfActor *actor, u16 phase)
{
    return phase < actor->animation_phase
        && phase >= actor->animation_phase - actor->animation_step;
}

ADDRESS(0x8003ae50, 0x4ec)
s32 func_8003ae50(SVECTOR *motion, s32 flags)
{
    KfActor *actor = actor_state.current;
    s32 motion_x;
    s32 motion_z;
    s32 original_x;
    s32 original_z;
    VECTOR proposed;
    s32 collision;
    s32 axis_attempted;
    s32 diagonal_attempted;
    s32 retry_count;
    s32 result = 0;

    if (motion->vx == 0 && motion->vz == 0) {
        return 0;
    }
    retry_count = 0;
    diagonal_attempted = 0;
    axis_attempted = 0;
    original_x = motion_x = motion->vx;
    original_z = motion_z = motion->vz;

retry_move:
    proposed.vx = actor->position.vx + motion_x;
    proposed.vz = actor->position.vz + motion_z;
    collision = func_8002b9d4(proposed.vx, actor->position.vy, proposed.vz,
        actor->unknown_1c,
        actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16),
        actor_state.unknown_93a4);
    if (collision == 0) {
    check_floor:
        if (actor->unknown_0d == 0 && (flags & 0x24)) {
            s32 floor_height = KF_COLLISION_CACHE_RESULT;
            s32 probe_x;
            s32 probe_z;

            if (motion_x == 0) {
                probe_x = 0;
            } else if (motion_x > 0) {
                probe_x = actor->unknown_1c * 2;
            } else {
                probe_x = -(s32)actor->unknown_1c * 2;
            }
            if (motion_z == 0) {
                probe_z = 0;
            } else if (motion_z > 0) {
                probe_z = actor->unknown_1c * 2;
            } else {
                probe_z = -(s32)actor->unknown_1c * 2;
            }
            func_8002b7f8(actor->position.vx, actor->position.vy,
                actor->position.vz + probe_z, actor->unknown_1c,
                actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16));
            if (floor_height < KF_COLLISION_CACHE_RESULT) {
                floor_height = KF_COLLISION_CACHE_RESULT;
            }
            func_8002b7f8(actor->position.vx + probe_x, actor->position.vy,
                actor->position.vz, actor->unknown_1c,
                actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16));
            if (floor_height < KF_COLLISION_CACHE_RESULT) {
                floor_height = KF_COLLISION_CACHE_RESULT;
            }
            if (actor->position.vy < floor_height - 1100) {
                result |= 0x100;
                if (flags & 4) {
                    goto try_axis;
                }
            }
        }
        actor->position.vx = proposed.vx;
        actor->position.vz = proposed.vz;
        goto finish;
    }

    result |= collision;
    if (!(flags & 3)) {
        goto finish;
    }
    if (collision & 0xb0) {
        s32 obstacle_angle;
        s32 movement_angle;
        s32 length;

        retry_count++;
        if (retry_count == 2) {
            goto finish;
        }
        func_8002b874();
        obstacle_angle = vector_xz_to_angle(
            KF_COLLISION_CACHE_POSITION.vx - actor->position.vx,
            KF_COLLISION_CACHE_POSITION.vz - actor->position.vz);
        movement_angle = vector_xz_to_angle(motion_x, motion_z);
        obstacle_angle = (angle_mod_delta_le_half_turn(
            movement_angle, obstacle_angle)
            ? obstacle_angle + 1024 : obstacle_angle - 1024);
        obstacle_angle &= KF_ANGLE_WRAP_MASK;
        length = SquareRoot0(motion_x * motion_x + motion_z * motion_z);
        motion_x = -(rsin(obstacle_angle) * length) >> 13;
        motion_z = (rcos(obstacle_angle) * length) >> 13;
        goto retry_move;
    }
    if (!(flags & 0x10)) {
        if (actor->unknown_28 & 0x4000) {
            goto try_axis;
        }
        if (!(collision & ~5)) {
            if (actor->position.vy <= KF_COLLISION_CACHE_RESULT + 800) {
                goto check_floor;
            }
            goto try_axis;
        }
    }
    if (!(collision & 1)) {
        goto check_diagonal;
    }

try_axis:
    if (axis_attempted) {
        motion_z = 0;
        motion_x = 0;
        goto finish;
    }
    if (motion_x != 0) {
        motion_x = 0;
        if (flags & 1) {
            s32 length = SquareRoot0(original_x * original_x
                                    + original_z * original_z);
            motion_z = length;
            if (original_z <= 0) {
                motion_z = -motion_z;
            }
        } else {
            motion_z = original_z;
        }
        goto retry_move;
    }
    if (motion_z != 0) {
        motion_z = 0;
        if (flags & 1) {
            s32 length = SquareRoot0(original_x * original_x
                                    + original_z * original_z);
            motion_x = length;
            if (original_x <= 0) {
                motion_x = -motion_x;
            }
        } else {
            motion_z = original_z;
        }
        axis_attempted = 1;
        goto retry_move;
    }
check_diagonal:
    if (!(collision & 2)) {
        goto finish;
    }
    if (diagonal_attempted) {
        goto try_axis;
    }
    switch (KF_COLLISION_CACHE_SHAPE[2] & 3) {
    case 0:
    case 2:
        motion_x = (original_x + original_z) >> 1;
        motion_z = motion_x;
        break;
    default:
        motion_x = (original_x - original_z) >> 1;
        motion_z = -motion_x;
        break;
    }
    diagonal_attempted = 1;
    goto retry_move;

finish:
    if (flags & 8) {
        motion->vx = motion_x;
        motion->vz = motion_z;
    }
    return result;
}

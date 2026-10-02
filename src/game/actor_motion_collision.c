#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>
#include <kf/lib/math.h>

ADDRESS(0x8003b33c, 0x1e4)
s32 func_8003b33c(SVECTOR *motion)
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
s32 func_8003b520(s32 first, s32 target_x, s32 target_y,
    s32 target_z, s32 trajectory_parameter, s32 trajectory_speed)
{
    KfActor *actor = actor_state.current;
    s16 result;

    if (func_80015bc8(first, actor->position.vx, actor->position.vy,
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
void func_8003b5bc(void)
{
    actor_state.current->unknown_0d = 0x60;
}

ADDRESS(0x8003b5d0, 0x3d4)
void func_8003b5d0(void)
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
            func_800248a8(0, group->unknown_06, 0, 0, 0, 0, 0, 0, 0,
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
            func_800248a8(0, group->unknown_06, 0, 0, 0, 0, 0, 0, 0,
                          0x1000, 10, &actor->position);
        }
        actor->unknown_0d = 0x10;
        actor->unknown_52 = 0;
        return;
    }
}

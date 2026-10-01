#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern void func_800248a8(u16 value0, u16 value1, u16 value2, u16 value3,
                          u16 value4, u16 value5, u16 value6, u16 value7,
                          u16 value8, u16 value9, u16 value10,
                          const VECTOR *position);
extern void func_80039c94(s32 actor_index, u16 power, u16 magic_06,
                          u16 magic_08, u16 magic_0a, u16 magic_0c,
                          u16 magic_0e, u16 magic_10, u16 magic_12,
                          u16 magic_14, s32 radius, s32 effect_flags,
                          const VECTOR *position);

ADDRESS(0x8003a318, 0x2fc)
void func_8003a318(VECTOR *position, s32 minimum_distance, s32 reach,
                   s32 mode, u16 falloff, u16 power, u16 magic_06,
                   u16 magic_08, u16 magic_0a, u16 magic_0c, u16 magic_0e,
                   u16 magic_10, u16 magic_12, u16 magic_14,
                   s32 amount_and_flags, u16 effect_flags)
{
    KfActor *actor;
    s16 index;
    const VECTOR *damage_position = position;
    u32 amount;

    if ((amount_and_flags & 0x8000) != 0) {
        damage_position = 0;
    }
    amount = (u32)amount_and_flags & 0x7fff;
    actor = actor_state.actors;
    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        s32 distance;
        u32 scaled_amount;

        if (actor->lifecycle != 1 || actor == actor_state.current) {
            continue;
        }
        if (mode == 0x8000) {
            distance = func_80015698(position, reach, &actor->position,
                                     actor->unknown_1c, actor->unknown_1e);
        } else if (mode == 0x8001) {
            if (position->vy < actor->position.vy - actor->unknown_1e) {
                distance = -9999999;
            } else {
                distance = func_80015698(position, reach, &actor->position,
                                         actor->unknown_1c, actor->unknown_1e);
            }
        } else {
            distance = vector_distance_to_point(
                &actor->position, position->vx, position->vy, position->vz,
                reach, actor->unknown_1e, mode);
            if (distance == KF_DISTANCE_NONE) {
                distance = -9999999;
            }
        }
        if (distance < minimum_distance) {
            continue;
        }

        if ((u16)falloff != KF_FIXED12_ONE) {
            u32 ratio = (u16)((distance << KF_FIXED12_BITS) / reach);
            u16 weight = KF_FIXED12_ONE -
                         ((ratio * (KF_FIXED12_ONE - (u16)falloff)) >> KF_FIXED12_BITS);
            scaled_amount = (amount * weight) >> KF_FIXED12_BITS;
        } else {
            scaled_amount = amount;
        }
        func_80039c94(index, power, magic_06, magic_08, magic_0a,
                      magic_0c, magic_0e, magic_10, magic_12, magic_14,
                      (u16)scaled_amount, (u16)effect_flags,
                      damage_position);
    }
}

ADDRESS(0x8003a614, 0x164)
s32 func_8003a614(s32 minimum_distance, s32 maximum_distance, s32 y_offset,
                  s32 angle_tolerance, u16 damage0, u16 damage1,
                  u16 damage2, u16 damage3)
{
    s32 maximum = maximum_distance << 6;
    s32 offset = y_offset << 5;
    s32 tolerance = angle_tolerance << 4;
    KfActor *actor = actor_state.current;
    VECTOR origin;
    s32 distance;
    s32 angle;

    minimum_distance <<= 6;
    origin.vx = actor->position.vx;
    origin.vy = actor->position.vy - offset;
    origin.vz = actor->position.vz;
    distance = vector_distance_to_point(&origin,
                                        player_state.camera_position.vx,
                                        player_state.camera_position.vy,
                                        player_state.camera_position.vz,
                                        maximum, 0, 1700);
    if (distance == -1 || distance < minimum_distance) {
        return 0;
    }

    angle = vector_xz_to_angle(player_state.camera_position.vx - origin.vx,
                               player_state.camera_position.vz - origin.vz);
    if (!angle_within_tolerance(actor->rotation.y, angle, tolerance)) {
        return 0;
    }

    func_800248a8(damage0, damage1, damage2, damage3,
                  0, 0, 0, 0, 0, 0x1000, 10, &origin);
    return 1;
}

ADDRESS(0x8003a778, 0x27c)
KfActor *func_8003a778(const VECTOR *position, s16 yaw, s16 pitch,
                       s32 max_distance, s32 yaw_limit, s32 pitch_limit,
                       s32 *distance, s32 variation)
{
    KfActor *best = 0;
    s32 best_score = 30000;
    s32 best_distance = -1;
    KfActor *actor = actor_state.actors;
    u16 remaining = KF_ACTOR_CAPACITY - 1;
    struct KfEulerAngles direction;
    s32 actor_distance;
    s32 score;
    s32 reach;

    do {
        if (actor->lifecycle != 1 || actor->target_type == 3 ||
            actor == actor_state.current) {
            continue;
        }

        reach = max_distance;
        if ((actor->unknown_28 & 0x20000) != 0) {
            reach <<= 1;
        }
        actor_distance = vector_distance_to_point(
            &actor->position, position->vx, KF_DISTANCE_IGNORE_HEIGHT,
            position->vz, reach, 0, 0);
        if (actor_distance == KF_DISTANCE_NONE) {
            continue;
        }

        func_800154fc(actor->position.vx - position->vx,
                      actor->position.vy - position->vy,
                      actor->position.vz - position->vz, &direction);
        direction.y = (direction.y - (u16)yaw) & KF_ANGLE_WRAP_MASK;
        if (direction.y >= KF_ANGLE_HALF_TURN) {
            direction.y = KF_ANGLE_FULL_TURN - direction.y;
        }
        direction.x = (direction.x - (u16)pitch) & KF_ANGLE_WRAP_MASK;
        if (direction.x >= KF_ANGLE_HALF_TURN) {
            direction.x = KF_ANGLE_FULL_TURN - direction.x;
        }

        if ((actor->unknown_28 & 0x20000) != 0 && variation >= 0) {
            actor_distance >>= 2;
            direction.x -= 512;
            direction.y -= 512;
        } else if (direction.y > yaw_limit || direction.x > pitch_limit) {
            continue;
        }

        score = direction.y + direction.x + (actor_distance >> 7);
        if (variation > 0) {
            score += (rand() * variation) >> 15;
        }
        if (score < best_score) {
            best_score = score;
            best = actor;
            best_distance = actor_distance;
        }
    } while (actor++, remaining-- != 0);

    *distance = best_distance;
    return best;
}

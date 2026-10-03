#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/types.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

ADDRESS(0x80039c14, 0x80)
s32 actor_magic_component_curve(s32 base, s32 amount, s32 divisor)
{
    amount <<= 4;
    divisor <<= 4;
    base <<= 4;
    if (amount == 0) {
        return 0;
    }
    amount += base;
    base = amount - divisor;
    if (base < 0) {
        base = 0;
    }
    if (divisor == 0) {
        divisor = 16;
    }
    return base + (amount * amount) / (divisor << 1);
}

typedef void (*KfMagicRecipientCallback)(KfActor *actor, s32 amount,
    u16 magic_06, u16 magic_08, u16 magic_0a, u16 magic_0c,
    u16 magic_0e, u16 magic_10, u16 magic_12, u16 magic_14);

ADDRESS(0x80039c94, 0x684)
void actor_apply_magic_to_actor(s32 actor_index, u16 power, u16 magic_06,
    u16 magic_08, u16 magic_0a, u16 magic_0c, u16 magic_0e,
    u16 magic_10, u16 magic_12, u16 magic_14, u16 amount,
    s32 effect_flags, const VECTOR *position)
{
    KfActor *actor = &actor_state.actors[(u16)actor_index];
    KfTargetGroup *group;
    KfTargetCandidate *candidate;
    KfActor *linked;
    s32 total;
    s32 applied;
    s32 remaining;
    s32 mode;
    s32 kind;
    s32 remaining_slots;
    KfTargetReference *target_slot;
    s32 motion_divisor;

    if (actor->slot_state == 3) {
        actor = &actor_state.actors[actor->unknown_22];
    }
    group = &actor_state.target_groups[actor->group_index];
    if (actor->target_type == 3 && actor->animation_phase >= 1548) {
        return;
    }
    if (actor->target_type == 0x15) {
        if (actor->state_70.signed_state != 0) {
            return;
        }
        actor->state_70.signed_state = 1;
        return;
    }
    if (actor->target_type == 0x1a) {
        return;
    }

    kind = effect_flags & 0x30;
    mode = effect_flags & 3;
    if (kind == 0x20 && (actor->unknown_28 & 0x100000)) {
        return;
    }

    total = actor_magic_component_curve(power, magic_06, group->magic_component_divisors[0]);
    total += actor_magic_component_curve(power, magic_08, group->magic_component_divisors[1]);
    total += actor_magic_component_curve(power, magic_0a, group->magic_component_divisors[2]);
    total += actor_magic_component_curve(power, magic_0c, group->magic_component_divisors[3]);
    total += actor_magic_component_curve(power, magic_0e, group->magic_component_divisors[4]);
    total += actor_magic_component_curve(power, magic_10, group->magic_component_divisors[5]);
    total += actor_magic_component_curve(power, magic_12, group->magic_component_divisors[6]);
    total += actor_magic_component_curve(power, magic_14, group->magic_component_divisors[7]);
    if (total > 0x68db7) {
        total = 0x68db7;
    }
    applied = ((total * (u16)amount) / 5000 + 128) >> 4;
    /* Slot 18's target is unresolved; its O32 arguments are observed. */
    ((KfMagicRecipientCallback)state_8017d118.active_table[18])(
        actor, applied, magic_06, magic_08, magic_0a, magic_0c,
        magic_0e, magic_10, magic_12, magic_14);
    if (applied == 0) {
        return;
    }

    if (actor->health != 0 && kind == 0x10) {
        if (mode == 2) {
            player_increment_magic_training();
        } else if (mode == 1 && (u16)amount >= 2500) {
            player_increment_physical_power_training();
        }
    }
    if (mode == 1) {
        if (actor->target_type == 0x13 && actor->state_70.signed_state == 0x10) {
            goto update_motion;
        }
    } else if (mode == 2) {
        if (actor->unknown_28 & 0x40000) {
            goto update_motion;
        }
        candidate = actor_find_target_of_type(group, 0x16);
        if (candidate != 0) {
            s32 angle = vector_xz_to_angle(
                actor->position.vx - position->vx,
                actor->position.vz - position->vz);
            if (angle_within_tolerance(actor->rotation.y, angle + 0x800,
                                       0x200)) {
                actor_set_target(actor, candidate);
                goto update_motion;
            }
        }
    }

    remaining = (u16)actor->health - applied;
    if (remaining <= 0) {
        if (actor->health != 0 && kind == 0x10) {
            player_add_experience(group->experience_reward);
        }
        actor_select_target_type_in_own_group(actor, 3);
        remaining = 0;
    } else {
        target_slot = group->targets;
        remaining_slots = 15;
        do {
            candidate = (target_slot++)->pointer;
            if (candidate == 0) {
                break;
            }
            if (candidate->type == 2 && candidate->word_0c.value <= applied) {
                u8 chance = candidate->unknown_01[1];
                if (chance == 0xff || (rand() >> 7) < chance) {
                    actor_set_target(actor, candidate);
                    actor->health = remaining;
                    goto update_motion;
                }
            }
        } while (--remaining_slots != -1);
    }
    actor->health = remaining;

update_motion:
    if (actor->unknown_28 & 0x10) {
        KfActorStateGame *state = &actor_state;
        linked = &state->actors[actor->unknown_22];
        motion_divisor =
            state->target_groups[linked->group_index].unknown_01[1];
    } else {
        motion_divisor = group->unknown_01[1];
    }
    if (position != 0 && motion_divisor < 0xf0) {
        struct KfEulerAngles angles;
        SVECTOR *motion = &actor->motion.vector;
        s32 speed;

        vector_displacement_to_pitch_yaw(actor->position.vx - position->vx,
                      actor->position.vy - (actor->collision_height >> 1) - position->vy,
                      actor->position.vz - position->vz, &angles);
        pitch_yaw_to_forward_vector(&angles, motion);
        speed = SquareRoot0(SquareRoot0(applied << 11));
        speed = (((speed << 10) / motion_divisor) << 5) / motion_divisor;
        if (speed > 512) {
            speed = 512;
        }
        vector3s_scale_shift12(speed, motion);
        actor->motion.vector.vx >>= 3;
        actor->motion.vector.vz >>= 3;
        actor->motion.vector.vy >>= 6;
        if (actor->unknown_28 & 0x10) {
            linked->motion.vector.vx = actor->motion.vector.vx;
            linked->motion.vector.vy = actor->motion.vector.vy;
            linked->motion.vector.vz = actor->motion.vector.vz;
            if (linked->target_type != 3) {
                actor_select_target_type_in_own_group(linked, 2);
            }
        }
    }
    actor->vertical_motion_state = 0x10;
}

ADDRESS(0x8003a318, 0x2fc)
void actor_apply_area_magic(VECTOR *position, s32 minimum_distance, s32 reach,
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

        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE || actor == actor_state.current) {
            continue;
        }
        if (mode == 0x8000) {
            distance = vector_distance_between_with_reach(position, reach, &actor->position,
                                     actor->collision_radius, actor->collision_height);
        } else if (mode == 0x8001) {
            if (position->vy < actor->position.vy - actor->collision_height) {
                distance = -9999999;
            } else {
                distance = vector_distance_between_with_reach(position, reach, &actor->position,
                                         actor->collision_radius, actor->collision_height);
            }
        } else {
            distance = vector_distance_to_point(
                &actor->position, position->vx, position->vy, position->vz,
                reach, actor->collision_height, mode);
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
        actor_apply_magic_to_actor(index, power, magic_06, magic_08, magic_0a,
                      magic_0c, magic_0e, magic_10, magic_12, magic_14,
                      (u16)scaled_amount, (u16)effect_flags,
                      damage_position);
    }
}

ADDRESS(0x8003a614, 0x164)
s32 actor_try_damage_player_in_cone(s32 minimum_distance, s32 maximum_distance, s32 y_offset,
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

    player_apply_damage(damage0, damage1, damage2, damage3,
                  0, 0, 0, 0, 0, 0x1000, 10, &origin);
    return 1;
}

ADDRESS(0x8003a778, 0x27c)
KfActor *actor_find_best_in_cone(const VECTOR *position, s16 yaw, s16 pitch,
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
        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE || actor->target_type == 3 ||
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

        vector_displacement_to_pitch_yaw(actor->position.vx - position->vx,
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

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
        SVECTOR *motion = (SVECTOR *)&actor->unknown_50;
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
        actor->unknown_50 >>= 3;
        actor->unknown_54 >>= 3;
        actor->unknown_52 >>= 6;
        if (actor->unknown_28 & 0x10) {
            linked->unknown_50 = actor->unknown_50;
            linked->unknown_52 = actor->unknown_52;
            linked->unknown_54 = actor->unknown_54;
            if (linked->target_type != 3) {
                actor_select_target_type_in_own_group(linked, 2);
            }
        }
    }
    actor->vertical_motion_state = 0x10;
}

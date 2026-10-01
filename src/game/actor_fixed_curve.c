#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/types.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

ADDRESS(0x80039c14, 0x80)
s32 func_80039c14(s32 base, s32 amount, s32 divisor)
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
void func_80039c94(s32 actor_index, u16 power, u16 magic_06,
    u16 magic_08, u16 magic_0a, u16 magic_0c, u16 magic_0e,
    u16 magic_10, u16 magic_12, u16 magic_14, s32 amount,
    s32 effect_flags, const VECTOR *position)
{
    KfActor *actor = &actor_state.actors[(u16)actor_index];
    KfTargetGroup *group;
    KfTargetCandidate *candidate;
    KfActor *linked = 0;
    s32 total;
    s32 applied;
    s32 remaining;
    s32 mode;
    s32 kind;
    s32 remaining_slots;
    KfTargetReference *target_slot;
    u8 group_index;

    if (actor->slot_state == 3) {
        actor = &actor_state.actors[actor->unknown_22];
    }
    group_index = actor->group_index;
    group = &actor_state.target_groups[group_index];
    if (actor->target_type == 3 && actor->animation_phase >= 1548) {
        return;
    }
    if (actor->target_type == 0x15) {
        if (actor->unknown_70 != 0) {
            return;
        }
        actor->unknown_70 = 1;
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

    total = func_80039c14(power, magic_06, group->unknown_20[0]);
    total += func_80039c14(power, magic_08, group->unknown_20[1]);
    total += func_80039c14(power, magic_0a, group->unknown_20[2]);
    total += func_80039c14(power, magic_0c, group->unknown_20[3]);
    total += func_80039c14(power, magic_0e, group->unknown_20[4]);
    total += func_80039c14(power, magic_10, group->unknown_20[5]);
    total += func_80039c14(power, magic_12, group->unknown_20[6]);
    total += func_80039c14(power, magic_14, group->unknown_20[7]);
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

    if (actor->unknown_1a != 0 && kind == 0x10) {
        if (mode == 2) {
            player_increment_magic_training();
        } else if (mode == 1 && (u16)amount >= 2500) {
            player_increment_physical_power_training();
        }
    }
    if (mode == 1) {
        if (actor->target_type == 0x13 && actor->unknown_70 == 0x10) {
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

    remaining = (u16)actor->unknown_1a - applied;
    if (remaining <= 0) {
        if (actor->unknown_1a != 0 && kind == 0x10) {
            player_add_experience(group->unknown_1e);
        }
        actor_select_target_type_in_own_group(actor, 3);
        actor->unknown_1a = 0;
    } else {
        target_slot = group->targets;
        remaining_slots = 15;
        do {
            candidate = target_slot->pointer;
            if (candidate == 0) {
                break;
            }
            if (candidate->type == 2 && candidate->unknown_0c <= applied &&
                (candidate->unknown_01[1] == 0xff ||
                 (rand() >> 7) < candidate->unknown_01[1])) {
                actor_set_target(actor, candidate);
                actor->unknown_1a = remaining;
                goto update_motion;
            }
            target_slot++;
        } while (remaining_slots-- != 0);
        actor->unknown_1a = remaining;
    }

update_motion:
    if (actor->unknown_28 & 0x10) {
        linked = &actor_state.actors[actor->unknown_22];
        group_index = linked->group_index;
    }
    if (position != 0 && group_index < 0xf0) {
        struct KfEulerAngles angles;
        SVECTOR *motion = (SVECTOR *)&actor->unknown_50;
        s32 speed;

        func_800154fc(actor->position.vx - position->vx,
                      actor->position.vy - (actor->unknown_1e >> 1) - position->vy,
                      actor->position.vz - position->vz, &angles);
        pitch_yaw_to_forward_vector(&angles, motion);
        speed = SquareRoot0(SquareRoot0(applied << 11));
        speed = (((speed << 10) / group_index) << 5) / group_index;
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
    actor->unknown_0d = 0x10;
}

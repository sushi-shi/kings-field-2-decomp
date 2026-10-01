#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern void func_8002b73c(s32 x, s32 z, s32 radius, s32 amount);
extern void func_8003d0e8(KfActor *actor);
extern s32 func_8003b9a4(s32 decay, s32 target);
extern void func_8003b5bc(void);
extern s32 func_8003a614(s32 minimum_distance, s32 maximum_distance,
                          s32 y_offset, s32 angle_tolerance, u16 damage0,
                          u16 damage1, u16 damage2, u16 damage3);
extern s32 func_8003bcd0(s16 angle, s32 speed, s32 range, s32 step,
                         s32 mode, s32 target);
extern s32 func_8003bf74(const struct KfEulerAngles *angles, s32 speed,
                         s32 range, s32 step, s32 mode, s32 target);
extern s32 func_8003bd40(s32 world_x, s32 world_z, s32 speed, s32 range,
                         s16 reference_angle, s32 step, s32 mode, s32 target);
extern void func_8003bba0(KfActor *actor, s32 target_angle, s32 max_speed,
                          s32 acceleration);
extern void actor_reset_target_and_reselect(void);

RODATA(0x800120d8, 0x3c4)

ADDRESS(0x8003d184, 0x248c)
void func_8003d184(void)
{
    KfActor *actor = actor_state.current;
    KfTargetCandidate *target = actor->target;
    u16 trigger;
    u16 interval;
    s32 should_play_sound = 0;

    actor_state.unknown_93a4 = (actor->unknown_28 & 4) != 0 ? 3 : 0x93;
    func_8002b73c(actor->position.vx, actor->position.vz,
                   actor->unknown_1c, -1);

    if (target->unknown_04 != 0xff) {
        trigger = target->unknown_0a;
        interval = trigger & 0x3fff;
        switch (trigger & 0xc000) {
        case 0:
            should_play_sound = actor_animation_crossed_phase(actor, interval);
            break;
        case 0x4000:
            should_play_sound =
                (interval * actor_state.unknown_93b8 / 3) % interval ==
                actor_state.unknown_93c4 % interval;
            break;
        case 0x8000:
            should_play_sound = (rand() >> 3) < interval;
            break;
        }
        if (should_play_sound) {
            func_8003d0e8(actor);
        }
    }

    switch (actor->target_type) {
    case 0:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
        }
        actor_advance_animation_wrapped(actor, target->unknown_08);
        if (angle_within_tolerance(actor->animation_phase, 0,
                                   target->unknown_08 - 1)) {
            actor->unknown_0f = 0xf1;
        }
        func_8003b9a4(actor_state.active_group->unknown_01[2], 10);
        break;
    case 1:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            func_80039804(target->unknown_01[0]);
            *(s16 *)actor->unknown_64 = rand() >> 3;
        } else if (func_8003bcd0(*(s16 *)actor->unknown_64,
                                  target->word_0c.value, target->word_0e.value,
                                  actor_state.active_group->unknown_01[2],
                                  actor_state.active_group->unknown_01[3], 5) != 0 ||
                   (rand() >> 5) < target->word_10.bytes.fallback_offset) {
            *(s16 *)actor->unknown_64 = rand() >> 3;
        }
        actor_advance_animation_wrapped(actor, target->unknown_08);
        break;
    case 2:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        if (actor->animation_phase < 0x800 ||
            (actor->unknown_28 & 0x800) == 0) {
            actor_advance_animation_clamped(actor, target->unknown_08);
        }
        if (actor->animation_phase > 0xffe) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(actor_state.active_group->unknown_01[2] * 2, 10);
        break;
    case 4:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(target->word_0c.bytes.high, 10);
        break;
    case 6:
    case 7:
    case 8:
        state_8017d118.active_table[17]();
        break;
    case 14:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor->unknown_70 = 0;
        }
        switch (actor->unknown_70) {
        case 0:
            if ((s16)actor_state.other_actor->unknown_58 > 0) {
                func_800397d8(target->word_0c.bytes.high);
                actor->unknown_70 = 1;
            } else if ((s16)actor_state.other_actor->unknown_58 < 0) {
                func_800397d8(target->word_0c.bytes.low);
                actor->unknown_70 = 2;
            }
            break;
        case 1:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)actor_state.other_actor->unknown_58 <= 0) {
                actor->unknown_70 = 3;
            }
            break;
        case 2:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)actor_state.other_actor->unknown_58 >= 0) {
                actor->unknown_70 = 3;
            }
            break;
        case 3:
            actor->animation_phase -= 128;
            if (actor->animation_phase == 0) {
                actor->unknown_70 = 0;
            }
            break;
        }
        break;
    case 15:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
        }
        func_8003bcd0(*(s16 *)actor->unknown_64,
                      target->word_0c.value, target->word_0e.value,
                      actor_state.active_group->unknown_01[2],
                      actor_state.active_group->unknown_01[3], 5);
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    case 18: {
        s32 delta_x;
        s32 delta_y;
        s32 delta_z;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
        }
        delta_x = player_state.camera_position.vx - actor->position.vx;
        delta_y = player_state.camera_position.vy - actor->position.vy - 1600;
        delta_z = player_state.camera_position.vz - actor->position.vz;
        fixed_vector3_length(delta_x, delta_y, delta_z);
        func_800154fc(delta_x, delta_y, delta_z, &actor->tail_72.angles);
        func_8003bf74(&actor->tail_72.angles, target->unknown_1c,
                      target->unknown_1e,
                      target->word_0c.bytes.high,
                      actor_state.active_group->unknown_01[3], 6);
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 19:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->unknown_70 = 0;
            func_80039804(target->unknown_01[0]);
        }
        switch (actor->unknown_70) {
        case 0:
            actor_advance_animation_clamped(actor, target->unknown_08);
            if (actor->animation_phase >= target->word_10.value) {
                actor->unknown_70 = 16;
            }
            break;
        case 16:
            if (player_state.weapon_attack_phase == -1) {
                actor->unknown_70 = 32;
            }
            break;
        case 32:
            actor_advance_animation_clamped(actor, target->unknown_08);
            if (actor->animation_phase >= 0xfff) {
                actor_reset_target_and_reselect();
            }
            break;
        }
        func_8003b9a4(actor_state.active_group->unknown_01[3], 10);
        break;
    case 20:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->unknown_70 = target->word_12.value;
            func_80039804(target->unknown_01[0]);
        }
        if (func_8003bae4(actor->rotation.y + 2048,
                          target->word_10.value, 1000, 5) != 0) {
            actor->unknown_70 = 1;
        }
        actor_advance_animation_wrapped(actor, target->unknown_08);
        if (actor->animation_phase >= 0xfff - target->unknown_08) {
            actor->unknown_70--;
            if (actor->unknown_70 == 0) {
                actor->animation_phase = 0xfff;
                actor_reset_target_and_reselect();
            }
        }
        break;
    case 21:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->unknown_70 = 0;
            func_800397d8(target->unknown_01[0]);
            actor->unknown_1c = target->word_14.value;
            actor->unknown_1e = target->word_16.value;
        }
        switch (actor->unknown_70) {
        case 0:
            if (fixed_vector2_length(player_state.camera_position.vx - actor->position.vx,
                                     player_state.camera_position.vz - actor->position.vz)
                < target->word_0c.value) {
                actor->unknown_70 = 1;
            }
            break;
        case 1:
            actor_advance_animation_clamped(actor, target->unknown_08);
            actor->unknown_1c = func_8001584c(target->word_14.value,
                actor_state.active_group->unknown_12, actor->animation_phase);
            actor->unknown_1e = func_8001584c(target->word_16.value,
                actor_state.active_group->unknown_14, actor->animation_phase);
            if (actor->animation_phase >= 0xfff) {
                actor->unknown_1c = actor_state.active_group->unknown_12;
                actor->unknown_1e = actor_state.active_group->unknown_14;
                actor->unknown_70 = 2;
                actor->tail_72.signed_state = target->word_10.value;
                func_800397d8(target->word_18.bytes.low);
            }
            if (actor->animation_phase >= target->word_12.value) {
                s32 saved_flags = actor_state.unknown_93a4;
                actor_state.unknown_93a4 &= ~0x20;
                func_8003bcd0(*(s16 *)actor->unknown_64,
                               target->word_0e.value, 0, 0xff, 0, 5);
                actor_state.unknown_93a4 = saved_flags;
            }
            break;
        case 2: {
            s32 saved_flags = actor_state.unknown_93a4;
            actor_state.unknown_93a4 &= ~0x20;
            func_8003bcd0(*(s16 *)actor->unknown_64,
                           target->word_0e.value, 0, 0xff, 0, 5);
            actor->tail_72.signed_state--;
            if (actor->tail_72.signed_state == -1) {
                actor_reset_target_and_reselect();
            }
            actor_state.unknown_93a4 = saved_flags;
            actor_advance_animation_wrapped(actor, target->word_18.bytes.high);
            break;
        }
        }
        break;
    case 22:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(actor_state.active_group->unknown_01[3], 10);
        break;
    case 26:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->unknown_70 = 0;
            func_800397d8(target->unknown_01[0]);
            actor->unknown_48 = 0;
            actor->unknown_4a.value = 0;
            actor->unknown_4c = 0;
        }
        switch (actor->unknown_70) {
        case 0: {
            KfMapObject *object = &map_object_state.objects[actor->unknown_20];
            if (object->action_timer < 2 || object->rotation.vx <= 0 ||
                object->rotation.vx >= 3072) {
                break;
            }
            actor->unknown_70 = 1;
            actor->tail_72.signed_state = 8;
            actor->unknown_1c = actor_state.active_group->unknown_12;
            actor->position.vy += 2048;
            actor->unknown_1e = actor_state.active_group->unknown_14;
            actor->unknown_48 = actor_state.active_group->unknown_32;
            actor->unknown_4a.value = actor_state.active_group->unknown_32;
            actor->unknown_4c = actor_state.active_group->unknown_32;
            actor->rotation.y = object->rotation.vy;
            actor->unknown_0d = 0;
        }
            /* fall through */
        case 1:
            actor->position.vy -= 256;
            actor->tail_72.signed_state--;
            if (actor->tail_72.signed_state == 0) {
                actor_reset_target_and_reselect();
            }
            break;
        }
        break;
    case 27:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor->unknown_70 = 0;
            actor->tail_72.signed_state = -1;
            func_800397d8(target->word_16.bytes.low);
        }
        switch (actor->unknown_70) {
        case 0:
            if (target->word_0e.value == 0) {
                actor->unknown_70 = 1;
            } else {
                actor->tail_72.signed_state = func_8003bd40(
                    actor->unknown_24 + (actor->unknown_07[1] << 11),
                    actor->unknown_22 + (actor->unknown_07[0] << 11),
                    target->word_0e.value, target->word_10.value,
                    actor->tail_72.signed_state,
                    actor_state.active_group->unknown_01[2],
                    actor_state.active_group->unknown_01[3], 5);
                if (actor->tail_72.signed_state == -1) {
                    actor->unknown_70 = 1;
                }
            }
            actor_advance_animation_wrapped(actor, target->word_16.bytes.high);
            break;
        case 1:
            func_8003bba0(actor, actor->unknown_20, target->word_10.value,
                          actor_state.active_group->unknown_01[3]);
            if (actor->rotation.y == actor->unknown_20) {
                func_800397d8(target->unknown_01[0]);
                actor->unknown_70 = 2;
            }
            break;
        case 2: {
            KfTargetCandidate *next_target;
            actor_advance_animation_clamped(actor, target->unknown_08);
            actor->unknown_1c = func_8001584c(
                actor_state.active_group->unknown_12,
                target->word_12.value, actor->animation_phase);
            actor->unknown_1e = func_8001584c(
                actor_state.active_group->unknown_14,
                target->word_14.value, actor->animation_phase);
            if (actor->animation_phase >= 0xfff) {
                actor->unknown_1c = target->word_12.value;
                actor->unknown_1e = target->word_14.value;
                next_target = actor_find_target_of_type(actor_state.active_group, 21);
                if (next_target != 0) {
                    actor_set_target(actor, next_target);
                } else {
                    actor_reset_target_and_reselect();
                }
            }
            break;
        }
        }
        break;
    case 29:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
            func_8003b5bc();
            actor->tail_72.signed_state = 16;
        }
        actor->rotation.x += 128;
        actor->rotation.y += 64;
        actor->position.vy -= 256;
        actor->tail_72.signed_state--;
        if (actor->tail_72.signed_state == 0) {
            actor_reset_target_and_reselect();
        }
        break;
    case 240:
        func_8003b5bc();
        break;
    /* Selectors 3, 5, 9–13, 16–17, 23–25, 28, and 30 have separate WIP paths. */
    default:
        if (actor->target_type >= 31 && actor->target_type != 240) {
            state_8017d118.active_table[17]();
        }
        break;
    }
}

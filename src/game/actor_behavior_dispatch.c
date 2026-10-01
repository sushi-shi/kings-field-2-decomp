#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
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
                                  target->unknown_0c, target->word_0e.value,
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
        if (actor_animation_crossed_phase(actor, target->unknown_18)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->unknown_16,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(target->unknown_0c >> 8, 10);
        break;
    case 6:
    case 7:
    case 8:
        state_8017d118.active_table[17]();
        break;
    case 15:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
        }
        func_8003bcd0(*(s16 *)actor->unknown_64,
                      target->unknown_0c, target->word_0e.value,
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
                      ((const KfTargetCandidateWord0c *)&target->unknown_0c)->bytes.high,
                      actor_state.active_group->unknown_01[3], 6);
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor, target->unknown_18)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->unknown_16,
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
    /* Selectors 3, 5, 9–14, 16–17, 21, 23–28, and 30 have separate WIP paths. */
    default:
        if (actor->target_type >= 31 && actor->target_type != 240) {
            state_8017d118.active_table[17]();
        }
        break;
    }
}

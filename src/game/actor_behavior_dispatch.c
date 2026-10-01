#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
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
    case 240:
        func_8003b5bc();
        break;
    /* Selectors 3, 5, and 9–30 have separate WIP paths. */
    default:
        if (actor->target_type >= 31 && actor->target_type != 240) {
            state_8017d118.active_table[17]();
        }
        break;
    }
}

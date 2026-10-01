#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <psyq/libc.h>

extern void func_8002b73c(s32 x, s32 z, s32 radius, s32 amount);
extern void func_8003d0e8(KfActor *actor);
extern void actor_reset_target_and_reselect(void);

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
            if (interval != 0) {
                should_play_sound =
                    (interval * actor_state.unknown_93b8 / 3) % interval ==
                    actor_state.unknown_93c4 % interval;
            }
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
        break;
    case 1:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            func_80039804(target->unknown_01[0]);
            *(s16 *)actor->unknown_64 = rand() >> 3;
        }
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
        break;
    /* Further actor target states are WIP. The in-body indirect dispatch
     * remains a candidate edge until its value chain is proved. */
    }
}

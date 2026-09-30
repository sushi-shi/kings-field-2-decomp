#include <kf/game/actor.h>
#include <kf/lib/address.h>
#include <kf/lib/math.h>

extern void func_800335a0(s32 mode, s32 argument);

ADDRESS(0x800460a0, 0xa4)
void func_800460a0(KfActor *actor, u8 state, u16 phase, s32 target_phase, s32 phase_step)
{
    s32 step;
    u32 half_step;
    s32 final_phase;

    if ((u16)phase_step == 0) {
        return;
    }

    step = phase_step & 0xfffe;
    half_step = (u32)step >> 1;
    final_phase = target_phase - half_step;
    actor->unknown_0c = state;
    actor->animation_phase = phase;

    while (!angle_within_tolerance(actor->animation_phase, (u16)final_phase, half_step)) {
        actor->animation_phase = (step + actor->animation_phase) & 0xfff;
        func_800335a0(0, 0);
    }

    actor->animation_phase = final_phase & 0xfff;
    func_800335a0(0, 0);
}

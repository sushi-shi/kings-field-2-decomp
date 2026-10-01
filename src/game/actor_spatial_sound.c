#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <psyq/libc.h>

ADDRESS(0x8003d084, 0x64)
s32 func_8003d084(KfActor *actor)
{
    s32 offset = 16 - actor->unknown_4a.bytes.high;

    if (offset > 12) {
        offset = 12;
    } else if (offset < -12) {
        offset = -12;
    }
    return offset + (((rand() * 5) >> 15) - 2);
}

ADDRESS(0x8003d0e8, 0x9c)
void func_8003d0e8(KfActor *actor)
{
    KfTargetCandidate *target = actor->target;

    if (target->unknown_04 & 0x80) {
        audio_play_spatial_range((target->unknown_04 & 0x7f) + 96,
            &actor->position, 0x7f, 0x6000, 0x7800,
            func_8003d084(actor));
    } else {
        audio_play_spatial_default_range(target->unknown_04 + 96,
            &actor->position, 0x6e, func_8003d084(actor));
    }
}

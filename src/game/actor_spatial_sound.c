#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <psyq/libc.h>

ADDRESS(0x8003d084, 0x64)
s32 actor_sound_note_offset(KfActor *actor)
{
    s32 offset = 16 - actor->model_scale_y.bytes.high;

    if (offset > 12) {
        offset = 12;
    } else if (offset < -12) {
        offset = -12;
    }
    return offset + (((rand() * 5) >> 15) - 2);
}

ADDRESS(0x8003d0e8, 0x9c)
void actor_play_target_sound(KfActor *actor)
{
    KfTargetCandidate *target = actor->target;

    if (target->sound_code & 0x80) {
        audio_play_spatial_range((target->sound_code & 0x7f) + 96,
            &actor->position, 0x7f, 0x6000, 0x7800,
            actor_sound_note_offset(actor));
    } else {
        audio_play_spatial_default_range(target->sound_code + 96,
            &actor->position, 0x6e, actor_sound_note_offset(actor));
    }
}

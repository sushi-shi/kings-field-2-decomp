#include <kf/lib/address.h>
#include <kf/game/actor.h>

ADDRESS(0x80039804, 0x38)
void func_80039804(u8 value)
{
    KfActor *actor = actor_state.current;

    if (value != 0xff && actor->unknown_0c != value) {
        actor->unknown_0c = value;
        actor->animation_phase = 0;
    }
}

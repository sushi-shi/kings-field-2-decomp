#include <kf/lib/address.h>
#include <kf/game/actor.h>

ADDRESS(0x800397d8, 0x2c)
void func_800397d8(u8 value)
{
    if (value != 0xff) {
        KfActor *actor = actor_state.current;

        actor->unknown_0c = value;
        actor->animation_phase = 0;
    }
}

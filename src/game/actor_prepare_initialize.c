#include <kf/lib/address.h>
#include <kf/game/actor.h>

ADDRESS(0x80038ff0, 0x58)
void actor_prepare_and_initialize(KfActor *actor)
{
    actor->rotation.z = 0;
    actor->rotation.x = 0;
    actor->unknown_03 = actor->unknown_06;
    actor->rotation.y = actor->unknown_20;
    actor_set_home_position(actor);
    actor_initialize_from_group(actor);
    if (actor->slot_state == 3) {
        actor->unknown_03 = 0;
    }
}

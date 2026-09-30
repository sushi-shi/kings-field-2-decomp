#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>

ADDRESS(0x80039080, 0x50)
void actor_pool_clear(void)
{
    KfActor *actor;
    u16 index;

    actor_state.unknown_93c4 = 0;
    actor = actor_state.actors;
    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        actor->slot_state = KF_ACTOR_SLOT_FREE;
        actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
        actor->animation_cache = NULL;
    }
    actor_state.unknown_93a0 = 0;
}

#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>

ADDRESS(0x80038cc8, 0x3c)
KfActor *actor_pool_find_free(void)
{
    KfActor *actor = &actor_state.actors[KF_ACTOR_DYNAMIC_START];
    KfActor *found;
    s32 count = KF_ACTOR_DYNAMIC_COUNT - 1;

    do {
        if (actor->slot_state == KF_ACTOR_SLOT_FREE) {
            found = actor;
            goto done;
        }
        actor++;
    } while (--count != -1);
    found = NULL;
done:
    return found;
}

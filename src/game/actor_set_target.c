#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>

ADDRESS(0x800390d0, 0x38)
void actor_set_target(KfActor *actor, KfTargetCandidate *target)
{
    actor->previous_target_type = actor->target_type;
    if (target != NULL) {
        u8 target_type;

        actor->target = target;
        target_type = target->type;
        actor->unknown_0f = 0;
        actor->target_type = target_type;
    } else {
        actor->target = NULL;
        actor->target_type = 0xff;
        actor->unknown_0f = 0xff;
    }
}

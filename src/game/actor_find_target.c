#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>

ADDRESS(0x80039710, 0x48)
KfTargetCandidate *actor_find_target_of_type(const KfTargetGroup *group, u8 type)
{
    KfTargetCandidate *target;
    const KfTargetReference *slot = group->targets;
    s32 remaining = 15;

    do {
        target = (slot++)->pointer;
        if (target == NULL) {
            break;
        }
        if (target->type == type) {
            return target;
        }
    } while (--remaining != -1);
    return NULL;
}

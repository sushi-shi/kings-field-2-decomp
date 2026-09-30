#include <kf/lib/address.h>
#include <kf/game/actor.h>

ADDRESS(0x80039758, 0x50)
void actor_select_target_type_in_own_group(KfActor *actor, u8 type)
{
    KfTargetGroup *group = &actor_state.target_groups[actor->group_index];
    KfTargetCandidate *target = actor_find_target_of_type(group, type);

    actor_set_target(actor, target);
}

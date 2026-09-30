#include <kf/lib/address.h>
#include <kf/game/actor.h>

ADDRESS(0x80038dc4, 0x74)
void actor_copy_group_defaults(KfActor *actor)
{
    const KfTargetGroup *group = &actor_state.target_groups[actor->group_index];
    u16 value;

    actor->unknown_01 = group->unknown_00;
    actor->unknown_1a = group->unknown_1a;
    actor->unknown_1c = group->unknown_12;
    actor->unknown_1e = group->unknown_14;
    actor->unknown_28 = group->unknown_34;
    value = group->unknown_32;
    actor->unknown_4c = value;
    actor->unknown_4a.value = value;
    actor->unknown_48 = value;
}

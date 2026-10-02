#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>

ADDRESS(0x80038efc, 0x24)
void actor_set_lifecycle_and_home_position(KfActor *actor)
{
    actor->lifecycle = KF_ACTOR_LIFECYCLE_DISABLED;
    actor_set_home_position(actor);
}

ADDRESS(0x80038f20, 0xd0)
void func_80038f20(void)
{
    KfActor *actor = actor_state.actors;
    s32 remaining = KF_ACTOR_CAPACITY - 1;

    do {
        if (actor->slot_state == 1 &&
            actor->lifecycle == KF_ACTOR_LIFECYCLE_ACTIVE &&
            ((*(u32 *)&actor->unknown_0c & 0xffff0000) == 0xf0030000) &&
            (actor->state_70.signed_state != 0 || actor->animation_phase > 2048)) {
            state_8017d118.active_table[19](actor);
            actor_set_lifecycle_and_home_position(actor);
        }
        actor++;
    } while (--remaining != -1);
}

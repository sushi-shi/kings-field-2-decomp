#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>

ADDRESS(0x800397a8, 0x30)
void actor_reset_target_and_reselect(void)
{
    KfActor *actor = actor_state.current;

    actor->target = NULL;
    actor->unknown_0f = 0xff;
    actor_select_target_for_player_distance();
}

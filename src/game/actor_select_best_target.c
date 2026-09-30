#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>

ADDRESS(0x800395c8, 0xfc)
void actor_select_best_target(s32 player_distance)
{
    KfActor *actor = actor_state.current;
    KfTargetGroup *group = actor_state.active_group;
    KfTargetCandidate *target;
    KfTargetCandidate *best_target;
    const KfTargetReference *slot;
    s32 best_score;
    s32 score;
    s32 remaining;

    if (actor->unknown_0f == 0xf0 || actor->unknown_0f == 0) {
        return;
    }

    slot = group->targets;
    best_score = -2;
    best_target = NULL;
    remaining = 15;
    do {
        target = (slot++)->pointer;
        if (target == NULL) {
            break;
        }
        score = func_80039108(target, player_distance);
        if (score > best_score) {
            best_score = score;
            best_target = target;
        }
    } while (--remaining != -1);

    if (best_target != NULL &&
        (best_target != actor->target || actor->unknown_0f == 0xff)) {
        actor_set_target(actor, best_target);
    }
}

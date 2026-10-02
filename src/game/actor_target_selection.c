#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>
#include <kf/game/player.h>

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

ADDRESS(0x800396c4, 0x4c)
void actor_select_target_for_player_distance(void)
{
    KfActor *actor = actor_state.current;
    s32 distance = fixed_vector2_length(
        actor->position.vx - player_state.camera_position.vx,
        actor->position.vz - player_state.camera_position.vz);

    actor_select_best_target(distance);
}

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

ADDRESS(0x80039758, 0x50)
void actor_select_target_type_in_own_group(KfActor *actor, u8 type)
{
    KfTargetGroup *group = &actor_state.target_groups[actor->group_index];
    KfTargetCandidate *target = actor_find_target_of_type(group, type);

    actor_set_target(actor, target);
}

ADDRESS(0x800397a8, 0x30)
void actor_reset_target_and_reselect(void)
{
    KfActor *actor = actor_state.current;

    actor->target = NULL;
    actor->unknown_0f = 0xff;
    actor_select_target_for_player_distance();
}

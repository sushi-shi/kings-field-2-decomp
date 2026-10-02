#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/map_cell.h>
#include <kf/game/player.h>

extern s32 func_8003a9f4(s32 x, s32 y, s32 z, s32 radius, s32 height);

ADDRESS(0x8003983c, 0x31c)
void func_8003983c(void)
{
    KfActor *actor = actor_state.current;
    KfTargetGroup *group = actor_state.active_group;
    u8 slot_state = actor->slot_state;
    s32 distance;

    switch (actor->lifecycle) {
    case KF_ACTOR_LIFECYCLE_DORMANT:
        distance = vector_distance_to_point(
            &actor->position, player_state.camera_position.vx,
            KF_DISTANCE_IGNORE_HEIGHT, player_state.camera_position.vz,
            (group->unknown_0a[0] + 1) << KF_FIXED11_BITS, 0, 0);
        if (distance == KF_DISTANCE_NONE) {
            return;
        }

        if (slot_state == 3 || slot_state == 4) {
            if (actor_state.other_actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE) {
                return;
            }
            actor_prepare_and_initialize(actor_state.current);
            actor_select_best_target(distance);
            return;
        }

        if (slot_state == 2) {
            u8 chance = actor->unknown_0a[0];
            if (chance != 0xff && chance < (rand() >> 4)) {
                return;
            }
        } else {
            if (distance < (group->unknown_0a[0] << KF_FIXED11_BITS) &&
                player_state.unknown_09[1] == 0) {
                goto set_dormant;
            }
            if (slot_state == 1) {
                goto check_actor_overlap;
            }
            if (slot_state != 0) {
                goto set_dormant;
            }
            if (actor->unknown_0a[0] == 0 ||
                actor->unknown_0a[0] < (rand() >> 7)) {
                goto set_dormant;
            }
        }

    check_actor_overlap:
        if (func_8003a9f4(actor->position.vx, actor->position.vy,
                          actor->position.vz, group->unknown_12,
                          group->unknown_14) != -1) {
            goto set_dormant;
        }

    activate:
        actor_prepare_and_initialize(actor_state.current);
        {
            KfTargetCandidate *target = actor_find_target_of_type(group, 0x15);
            if (target == 0) {
                target = actor_find_target_of_type(group, 0x1a);
            }
            if (target != 0) {
                actor_set_target(actor, target);
                return;
            }
        }
        actor_select_best_target(distance);
        return;

    set_dormant:
        if (slot_state != 2) {
            actor->lifecycle = KF_ACTOR_LIFECYCLE_WAIT_FOR_RANGE_EXIT;
        }
        return;

    case KF_ACTOR_LIFECYCLE_ACTIVE:
        distance = vector_distance_to_point(
            &actor->position, player_state.camera_position.vx,
            KF_DISTANCE_IGNORE_HEIGHT, player_state.camera_position.vz,
            group->unknown_0a[1] << KF_FIXED11_BITS, 0, 0);
        if (distance != KF_DISTANCE_NONE) {
            return;
        }
        func_8002b73c(actor->position.vx, actor->position.vz,
                       actor->unknown_1c, -1);
        actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
        actor_set_home_position(actor);
        return;

    case KF_ACTOR_LIFECYCLE_WAIT_FOR_RANGE_EXIT:
        if (slot_state == 3 || slot_state == 4) {
            if (actor_state.other_actor->lifecycle == KF_ACTOR_LIFECYCLE_ACTIVE) {
                return;
            }
        } else {
            distance = vector_distance_to_point(
                &actor->position, player_state.camera_position.vx,
                KF_DISTANCE_IGNORE_HEIGHT, player_state.camera_position.vz,
                group->unknown_0a[1] << KF_FIXED11_BITS, 0, 0);
            if (distance != KF_DISTANCE_NONE) {
                return;
            }
        }
        actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
        actor_set_home_position(actor);
        return;
    }
}

ADDRESS(0x80039b58, 0xbc)
void func_80039b58(s32 group_index)
{
    KfActor *actor = actor_state.actors;
    s16 remaining = KF_ACTOR_CAPACITY - 1;

    do {
        if (actor->slot_state != 0xff &&
            actor->group_index == (u16)group_index) {
            if (actor->lifecycle == KF_ACTOR_LIFECYCLE_ACTIVE) {
                actor_select_target_type_in_own_group(actor, 3);
            } else {
                actor_set_lifecycle_and_home_position(actor);
            }
        }
        actor++;
    } while (--remaining != -1);
}

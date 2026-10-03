#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/collision_cache.h>
#include <kf/game/map_cell.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

DATA(0x8016b600, 0x93cc)
KfActorStateGame actor_state;

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

enum { ACTOR_HOME_CELL_SHIFT = 11 };

ADDRESS(0x80038d04, 0xc0)
void actor_set_home_position(KfActor *actor)
{
    actor->position.vx = (actor->unknown_07[1] << ACTOR_HOME_CELL_SHIFT)
                       + actor->unknown_24;
    actor->position.vz = (actor->unknown_07[0] << ACTOR_HOME_CELL_SHIFT)
                       + actor->unknown_22;
    actor->position.vy = collision_sample_map_layer_height(actor->unknown_06,
                                        actor->position.vx,
                                        actor->position.vz,
                                        actor->unknown_1c,
                                        actor->unknown_1e);
    if (actor->position.vy >= 0) {
        actor->position.vy = 0;
    }
    if (actor->unknown_28 & 0x400) {
        actor->position.vy = KF_COLLISION_CACHE_HEIGHT;
    }
    if (!(actor->unknown_28 & 0x10)) {
        actor->position.vy += actor->unknown_26;
    }
}

ADDRESS(0x80038dc4, 0x74)
void actor_copy_group_defaults(KfActor *actor)
{
    const KfTargetGroup *group = &actor_state.target_groups[actor->group_index];
    u16 value;

    actor->unknown_01 = group->unknown_00;
    actor->unknown_1a = group->unknown_1a;
    actor->unknown_1c = group->collision_radius;
    actor->unknown_1e = group->collision_height;
    actor->unknown_28 = group->unknown_34;
    value = group->unknown_32;
    actor->unknown_4c = value;
    actor->unknown_4a.value = value;
    actor->unknown_48 = value;
}

ADDRESS(0x80038e38, 0xc4)
void actor_initialize_from_group(KfActor *actor)
{
    actor_copy_group_defaults(actor);
    actor->lifecycle = KF_ACTOR_LIFECYCLE_ACTIVE;
    actor->unknown_0c = 0;
    actor->animation_phase = 0;
    actor->unknown_11 = 0;
    actor->unknown_0d = 0;
    actor->target_type = 0;
    actor->unknown_0f = 0xff;
    actor->target = NULL;
    if ((actor->unknown_05 & 1) == 0) {
        actor->rotation.y = rand() >> 3;
    }
    actor->unknown_54 = 0;
    actor->unknown_52 = 0;
    actor->unknown_50 = 0;
    actor->unknown_58 = 0;
    actor->unknown_14 = 0x47;
    actor->unknown_16 = 0x800;
    if (actor->unknown_28 & 0x80) {
        actor->unknown_13 = 1;
    } else {
        actor->unknown_13 = 0xff;
    }
    map_cell_add_layer_occupancy(actor->position.vx, actor->position.vz, actor->unknown_1c, 1);
}

ADDRESS(0x80038efc, 0x24)
void actor_set_lifecycle_and_home_position(KfActor *actor)
{
    actor->lifecycle = KF_ACTOR_LIFECYCLE_DISABLED;
    actor_set_home_position(actor);
}

ADDRESS(0x80038f20, 0xd0)
void actor_disable_type3_transition_actors(void)
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

ADDRESS(0x80038ff0, 0x58)
void actor_prepare_and_initialize(KfActor *actor)
{
    actor->rotation.z = 0;
    actor->rotation.x = 0;
    actor->unknown_03 = actor->unknown_06;
    actor->rotation.y = actor->unknown_20;
    actor_set_home_position(actor);
    actor_initialize_from_group(actor);
    if (actor->slot_state == 3) {
        actor->unknown_03 = 0;
    }
}

ADDRESS(0x80039048, 0x38)
void actor_prepare_and_initialize_by_index(u16 actor_index)
{
    actor_prepare_and_initialize(&actor_state.actors[actor_index]);
}

ADDRESS(0x80039080, 0x50)
void actor_pool_clear(void)
{
    KfActor *actor;
    u16 index;

    actor_state.unknown_93c4 = 0;
    actor = actor_state.actors;
    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        actor->slot_state = KF_ACTOR_SLOT_FREE;
        actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
        actor->animation_cache = NULL;
    }
    actor_state.unknown_93a0 = 0;
}

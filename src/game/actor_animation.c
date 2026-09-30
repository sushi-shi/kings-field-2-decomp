#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>

ADDRESS(0x8003a9f4, 0x168)
s32 func_8003a9f4(s32 x, s32 y, s32 z, s32 radius, s32 height)
{
    KfActor *actor = actor_state.actors;
    s32 index;

    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        VECTOR alternate;

        if (actor->lifecycle != 1 || actor->target_type == 3
            || (actor_state.unknown_93a0 & actor->unknown_28)
            || actor == actor_state.current) {
            continue;
        }
        if (actor->unknown_28 & 0x10) {
            if (actor->unknown_22 == actor_state.unknown_93b8) {
                continue;
            }
            alternate.vx = actor->position.vx;
            alternate.vz = actor->position.vz;
            alternate.vy = actor->position.vy + actor->unknown_26;
            if (vector_distance_to_point(&alternate, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        } else {
            if (vector_distance_to_point(&actor->position, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        }
    }
    return -1;
}

ADDRESS(0x8003ab5c, 0x158)
s32 func_8003ab5c(s32 x, s32 y, s32 z, s32 radius, s32 height)
{
    KfActor *actor = actor_state.actors;
    s32 index;

    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        VECTOR alternate;

        if (actor->lifecycle != 1
            || (actor_state.unknown_93a0 & actor->unknown_28)
            || actor == actor_state.current) {
            continue;
        }
        if (actor->unknown_28 & 0x10) {
            if (actor->unknown_22 == actor_state.unknown_93b8) {
                continue;
            }
            alternate.vx = actor->position.vx;
            alternate.vz = actor->position.vz;
            alternate.vy = actor->position.vy + actor->unknown_26;
            if (vector_distance_to_point(&alternate, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        } else {
            if (vector_distance_to_point(&actor->position, x, y, z,
                actor->unknown_1c + radius, actor->unknown_1e, height) != -1) {
                return index;
            }
        }
    }
    return -1;
}

ADDRESS(0x8003acb4, 0xdc)
void actor_bind_current(KfActor *actor)
{
    KfTargetGroup *group;
    KfActor *other;

    if (actor != NULL) {
        actor_state.current = actor;
        group = &actor_state.target_groups[actor->group_index];
        actor_state.active_group = group;
        actor_state.current_group_index = actor->group_index;
        if (group->unknown_34 & 0x10) {
            other = &actor_state.actors[actor->unknown_22];
            actor_state.other_actor = other;
            actor_state.other_group = &actor_state.target_groups[other->group_index];
        }
    } else {
        actor_state.current = NULL;
        actor_state.active_group = NULL;
        actor_state.current_group_index = 0;
        actor_state.other_actor = NULL;
        actor_state.other_group = NULL;
    }
}

ADDRESS(0x8003ad90, 0x34)
void actor_advance_animation_wrapped(KfActor *actor, s16 delta)
{
    if (delta < 0) {
        actor->animation_step = -delta;
    } else {
        actor->animation_step = delta;
    }
    actor->animation_phase = (actor->animation_phase + delta) & KF_ACTOR_ANIMATION_PHASE_MAX;
}

ADDRESS(0x8003adc4, 0x5c)
void actor_advance_animation_clamped(KfActor *actor, s16 delta)
{
    s16 phase;

    if (delta < 0) {
        actor->animation_step = -delta;
    } else {
        actor->animation_step = delta;
    }
    phase = actor->animation_phase + delta;
    actor->animation_phase = phase;
    if (phase >= KF_ACTOR_ANIMATION_PHASE_PERIOD) {
        actor->animation_phase = KF_ACTOR_ANIMATION_PHASE_MAX;
    } else if (phase < 0) {
        actor->animation_phase = 0;
    }
}

ADDRESS(0x8003ae20, 0x30)
KfBool32 actor_animation_crossed_phase(const KfActor *actor, u16 phase)
{
    return phase < actor->animation_phase
        && phase >= actor->animation_phase - actor->animation_step;
}

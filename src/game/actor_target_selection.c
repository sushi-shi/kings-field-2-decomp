#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

typedef s32 (*KfCandidateScoreCallback)(KfTargetCandidate *target,
                                         s32 player_distance);

RODATA(0x80011cd8, 0x20c)

ADDRESS(0x800390d0, 0x38)
void actor_set_target(KfActor *actor, KfTargetCandidate *target)
{
    actor->previous_target_type = actor->target_type;
    if (target != NULL) {
        u8 target_type;

        actor->target = target;
        target_type = target->type;
        actor->unknown_0f = 0;
        actor->target_type = target_type;
    } else {
        actor->target = NULL;
        actor->target_type = 0xff;
        actor->unknown_0f = 0xff;
    }
}

ADDRESS(0x80039108, 0x4c0)
s32 actor_score_target_candidate(KfTargetCandidate *target, s32 player_distance)
{
    KfActor *actor = actor_state.current;
    s32 score;
    s32 angle;

    if (target->type == 0xff) {
        return 0;
    }

    score = 0;

    switch (target->type) {
    case 5:
    case 13:
        if (target == actor->target) {
            if (target->word_12.value >= player_distance) {
                score = random_triangular_scaled(target->unknown_01[2]);
            }
            goto done;
        }
        if (target->word_10.value >= player_distance) {
            score = random_triangular_scaled(target->unknown_01[1]);
        }
        switch (actor->target_type) {
        case 4:
        case 18:
        case 23:
        case 24:
        case 132:
            score *= 2;
            break;
        }
        goto done;

    case 9:
        if (target->word_0c.value < player_distance) {
            goto done;
        }
        if ((u32)(player_state.camera_position.vy - actor->position.vy + 1023) < 2047 &&
            rand() >= 4096) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (!angle_within_tolerance(actor->rotation.y, angle, 0x140)) {
            goto done;
        }
        goto score_target;

    case 4:
    case 18:
    case 23:
    case 24:
    case 132:
        if ((actor->unknown_28 & 0x100) || target->word_1a.value < player_distance ||
            !directed_intervals_overlap(actor->position.vy, actor->unknown_1e,
                            player_state.camera_position.vy + 200, 0x834)) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (!angle_within_tolerance(actor->rotation.y, angle,
                                    target->word_10.bytes.fallback_offset << 4)) {
            goto done;
        }
        if (target == actor->target) {
            score = random_triangular_scaled(target->unknown_01[2]);
            goto done;
        }
        score = target->unknown_01[1];
        {
            s32 near_distance = target->word_0e.bytes.low << 6;
            s32 middle_distance = (near_distance + target->word_1a.value) >> 1;
            if (player_distance >= middle_distance) {
                score -= score / 3;
            } else if (player_distance >= near_distance) {
                score -= score / 6;
            }
        }
        score = random_triangular_scaled(score);
        goto done;

    case 25:
        if ((actor->unknown_28 & 0x100) || target->word_16.value < player_distance ||
            target->word_14.value > player_distance) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (angle_within_tolerance(actor->rotation.y, angle,
                                   target->word_0c.bytes.high << 4)) {
            break;
        }
        goto done;

    case 11:
        if (target->word_1a.value < player_distance) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (angle_within_tolerance(actor->rotation.y, angle, 0x140)) {
            break;
        }
        goto done;

    case 19:
    case 20:
        if (target->word_0e.value < player_distance || player_state.weapon_attack_phase == -1) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (angle_within_tolerance(actor->rotation.y, angle,
                                   target->word_0c.bytes.low << 5)) {
            break;
        }
        goto done;

    case 112:
        score = -1;
        goto done;
    case 27:
        if (player_distance >= target->word_0c.value) {
            break;
        }
        /* Fall through to the zero-score cases. */
    case 2:
    case 3:
    case 22:
        score = 0;
        goto done;
    default:
        if (target->type >= 128 &&
            !((KfCandidateScoreCallback)state_8017d118.active_table[16])(
                target, player_distance)) {
            goto done;
        }
        break;
    }

score_target:
    if (target == actor->target) {
        score = random_triangular_scaled(target->unknown_01[2]);
    } else {
        score = random_triangular_scaled(target->unknown_01[1]);
    }
done:
    return score;
}

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
        score = actor_score_target_candidate(target, player_distance);
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

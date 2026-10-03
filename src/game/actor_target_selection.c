#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/lib/types.h>
#include <kf/game/actor.h>
#include <kf/game/animation.h>
#include <kf/game/callback.h>
#include <kf/game/collision_cache.h>
#include <kf/game/map_cell.h>
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
        actor->target_action_state = 0;
        actor->target_type = target_type;
    } else {
        actor->target = NULL;
        actor->target_type = KF_ACTOR_TARGET_TYPE_NONE;
        actor->target_action_state = KF_ACTOR_TARGET_ACTION_UNSELECTED;
    }
}

ADDRESS(0x80039108, 0x4c0)
s32 actor_score_target_candidate(KfTargetCandidate *target, s32 player_distance)
{
    KfActor *actor = actor_state.current;
    s32 score;
    s32 angle;

    if (target->type == KF_TARGET_CANDIDATE_DISABLED) {
        return 0;
    }

    score = 0;

    switch (target->type) {
    case 5:
    case 13:
        if (target == actor->target) {
            if (target->word_12.value >= player_distance) {
                score = random_triangular_scaled(target->word_02.target_selection.continuing_score_scale);
            }
            goto done;
        }
        if (target->word_10.value >= player_distance) {
            score = random_triangular_scaled(target->word_02.target_selection.initial_score_scale);
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
        if ((actor->unknown_28 & KF_ACTOR_FLAG_BLOCK_PLAYER_TARGETING) ||
            target->word_1a.value < player_distance ||
            !directed_intervals_overlap(actor->position.vy, actor->collision_height,
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
            score = random_triangular_scaled(target->word_02.target_selection.continuing_score_scale);
            goto done;
        }
        score = target->word_02.target_selection.initial_score_scale;
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
        if ((actor->unknown_28 & KF_ACTOR_FLAG_BLOCK_PLAYER_TARGETING) ||
            target->word_16.value < player_distance ||
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
        score = random_triangular_scaled(target->word_02.target_selection.continuing_score_scale);
    } else {
        score = random_triangular_scaled(target->word_02.target_selection.initial_score_scale);
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

    if (actor->target_action_state == KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED ||
        actor->target_action_state == 0) {
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
        (best_target != actor->target ||
         actor->target_action_state == KF_ACTOR_TARGET_ACTION_UNSELECTED)) {
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
    actor->target_action_state = KF_ACTOR_TARGET_ACTION_UNSELECTED;
    actor_select_target_for_player_distance();
}

ADDRESS(0x800397d8, 0x2c)
void actor_set_animation(u8 animation_id)
{
    if (animation_id != KF_ACTOR_ANIMATION_NO_CHANGE) {
        KfActor *actor = actor_state.current;

        actor->animation_id = animation_id;
        actor->animation_phase = 0;
    }
}

ADDRESS(0x80039804, 0x38)
void actor_set_animation_if_changed(u8 animation_id)
{
    KfActor *actor = actor_state.current;

    if (animation_id != KF_ACTOR_ANIMATION_NO_CHANGE &&
        actor->animation_id != animation_id) {
        actor->animation_id = animation_id;
        actor->animation_phase = 0;
    }
}

ADDRESS(0x8003983c, 0x31c)
void actor_update_lifecycle_for_player_range(void)
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
            (group->activation_range_cells + 1) << KF_FIXED11_BITS, 0, 0);
        if (distance == KF_DISTANCE_NONE) {
            return;
        }

        if (slot_state == KF_ACTOR_SLOT_HOMEBOUND || slot_state == 4) {
            if (actor_state.other_actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE) {
                return;
            }
            actor_prepare_and_initialize(actor_state.current);
            actor_select_best_target(distance);
            return;
        }

        if (slot_state == KF_ACTOR_SLOT_RESPAWNING) {
            u8 chance = actor->spawn_chance;
            if (chance != 0xff && chance < (rand() >> 4)) {
                return;
            }
        } else {
            if (distance < (group->activation_range_cells << KF_FIXED11_BITS) &&
                player_state.force_actor_lifecycle_refresh == 0) {
                goto set_dormant;
            }
            if (slot_state != KF_ACTOR_SLOT_PERSISTENT) {
                if (slot_state != 0) {
                    goto set_dormant;
                }
                if (actor->spawn_chance == 0 ||
                    actor->spawn_chance < (rand() >> 7)) {
                    goto set_dormant;
                }
            }
        }

        if (actor_find_overlap_excluding_target_type3(actor->position.vx, actor->position.vy,
                          actor->position.vz, group->collision_radius,
                          group->collision_height) != -1) {
            goto set_dormant;
        }

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
        if (slot_state != KF_ACTOR_SLOT_RESPAWNING) {
            actor->lifecycle = KF_ACTOR_LIFECYCLE_WAIT_FOR_RANGE_EXIT;
        }
        return;

    case KF_ACTOR_LIFECYCLE_ACTIVE:
        distance = vector_distance_to_point(
            &actor->position, player_state.camera_position.vx,
            KF_DISTANCE_IGNORE_HEIGHT, player_state.camera_position.vz,
            group->deactivation_range_cells << KF_FIXED11_BITS, 0, 0);
        if (distance != KF_DISTANCE_NONE) {
            return;
        }
        map_cell_add_layer_occupancy(actor->position.vx, actor->position.vz,
                       actor->collision_radius, -1);
        actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
        actor_set_home_position(actor);
        return;

    case KF_ACTOR_LIFECYCLE_WAIT_FOR_RANGE_EXIT:
        if (slot_state == KF_ACTOR_SLOT_HOMEBOUND || slot_state == 4) {
            if (actor_state.other_actor->lifecycle == KF_ACTOR_LIFECYCLE_ACTIVE) {
                return;
            }
        } else {
            distance = vector_distance_to_point(
                &actor->position, player_state.camera_position.vx,
                KF_DISTANCE_IGNORE_HEIGHT, player_state.camera_position.vz,
                group->deactivation_range_cells << KF_FIXED11_BITS, 0, 0);
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
void actor_retarget_or_disable_group_members(s32 group_index)
{
    KfActor *actor = actor_state.actors;
    s16 remaining = KF_ACTOR_CAPACITY - 1;

    do {
        if (actor->slot_state != KF_ACTOR_SLOT_FREE &&
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

ADDRESS(0x80039c14, 0x80)
s32 actor_magic_component_curve(s32 base, s32 amount, s32 divisor)
{
    amount <<= 4;
    divisor <<= 4;
    base <<= 4;
    if (amount == 0) {
        return 0;
    }
    amount += base;
    base = amount - divisor;
    if (base < 0) {
        base = 0;
    }
    if (divisor == 0) {
        divisor = 16;
    }
    return base + (amount * amount) / (divisor << 1);
}

typedef void (*KfMagicRecipientCallback)(KfActor *actor, s32 amount,
    u16 magic_06, u16 magic_08, u16 magic_0a, u16 magic_0c,
    u16 magic_0e, u16 magic_10, u16 magic_12, u16 magic_14);

ADDRESS(0x80039c94, 0x684)
void actor_apply_magic_to_actor(s32 actor_index, u16 power, u16 magic_06,
    u16 magic_08, u16 magic_0a, u16 magic_0c, u16 magic_0e,
    u16 magic_10, u16 magic_12, u16 magic_14, u16 amount,
    s32 effect_flags, const VECTOR *position)
{
    KfActor *actor = &actor_state.actors[(u16)actor_index];
    KfTargetGroup *group;
    KfTargetCandidate *candidate;
    KfActor *linked;
    s32 total;
    s32 applied;
    s32 remaining;
    s32 mode;
    s32 kind;
    s32 remaining_slots;
    KfTargetReference *target_slot;
    s32 motion_divisor;

    if (actor->slot_state == KF_ACTOR_SLOT_HOMEBOUND) {
        actor = &actor_state.actors[actor->word_22.linked_actor_slot];
    }
    group = &actor_state.target_groups[actor->group_index];
    if (actor->target_type == 3 && actor->animation_phase >= 1548) {
        return;
    }
    if (actor->target_type == 0x15) {
        if (actor->state_70.signed_state != 0) {
            return;
        }
        actor->state_70.signed_state = 1;
        return;
    }
    if (actor->target_type == 0x1a) {
        return;
    }

    kind = effect_flags & 0x30;
    mode = effect_flags & 3;
    if (kind == 0x20 && (actor->unknown_28 & 0x100000)) {
        return;
    }

    total = actor_magic_component_curve(power, magic_06, group->magic_component_divisors[0]);
    total += actor_magic_component_curve(power, magic_08, group->magic_component_divisors[1]);
    total += actor_magic_component_curve(power, magic_0a, group->magic_component_divisors[2]);
    total += actor_magic_component_curve(power, magic_0c, group->magic_component_divisors[3]);
    total += actor_magic_component_curve(power, magic_0e, group->magic_component_divisors[4]);
    total += actor_magic_component_curve(power, magic_10, group->magic_component_divisors[5]);
    total += actor_magic_component_curve(power, magic_12, group->magic_component_divisors[6]);
    total += actor_magic_component_curve(power, magic_14, group->magic_component_divisors[7]);
    if (total > 0x68db7) {
        total = 0x68db7;
    }
    applied = ((total * (u16)amount) / 5000 + 128) >> 4;
    /* Slot 18's target is unresolved; its O32 arguments are observed. */
    ((KfMagicRecipientCallback)state_8017d118.active_table[18])(
        actor, applied, magic_06, magic_08, magic_0a, magic_0c,
        magic_0e, magic_10, magic_12, magic_14);
    if (applied == 0) {
        return;
    }

    if (actor->health != 0 && kind == 0x10) {
        if (mode == 2) {
            player_increment_magic_training();
        } else if (mode == 1 && (u16)amount >= 2500) {
            player_increment_physical_power_training();
        }
    }
    if (mode == 1) {
        if (actor->target_type == 0x13 && actor->state_70.signed_state == 0x10) {
            goto update_motion;
        }
    } else if (mode == 2) {
        if (actor->unknown_28 & 0x40000) {
            goto update_motion;
        }
        candidate = actor_find_target_of_type(group, 0x16);
        if (candidate != 0) {
            s32 angle = vector_xz_to_angle(
                actor->position.vx - position->vx,
                actor->position.vz - position->vz);
            if (angle_within_tolerance(actor->rotation.y, angle + KF_ANGLE_HALF_TURN,
                                       0x200)) {
                actor_set_target(actor, candidate);
                goto update_motion;
            }
        }
    }

    remaining = (u16)actor->health - applied;
    if (remaining <= 0) {
        if (actor->health != 0 && kind == 0x10) {
            player_add_experience(group->experience_reward);
        }
        actor_select_target_type_in_own_group(actor, 3);
        remaining = 0;
    } else {
        target_slot = group->targets;
        remaining_slots = 15;
        do {
            candidate = (target_slot++)->pointer;
            if (candidate == 0) {
                break;
            }
            if (candidate->type == 2 && candidate->word_0c.value <= applied) {
                u8 chance = candidate->word_02.damage_reaction.reaction_chance;
                if (chance == 0xff || (rand() >> 7) < chance) {
                    actor_set_target(actor, candidate);
                    actor->health = remaining;
                    goto update_motion;
                }
            }
        } while (--remaining_slots != -1);
    }
    actor->health = remaining;

update_motion:
    if (actor->unknown_28 & KF_ACTOR_FLAG_LINKED) {
        KfActorStateGame *state = &actor_state;
        linked = &state->actors[actor->word_22.linked_actor_slot];
        motion_divisor =
            state->target_groups[linked->group_index].knockback_divisor;
    } else {
        motion_divisor = group->knockback_divisor;
    }
    if (position != 0 && motion_divisor < 0xf0) {
        struct KfEulerAngles angles;
        SVECTOR *motion = &actor->motion.vector;
        s32 speed;

        vector_displacement_to_pitch_yaw(actor->position.vx - position->vx,
                      actor->position.vy - (actor->collision_height >> 1) - position->vy,
                      actor->position.vz - position->vz, &angles);
        pitch_yaw_to_forward_vector(&angles, motion);
        speed = SquareRoot0(SquareRoot0(applied << 11));
        speed = (((speed << 10) / motion_divisor) << 5) / motion_divisor;
        if (speed > 512) {
            speed = 512;
        }
        vector3s_scale_shift12(speed, motion);
        actor->motion.vector.vx >>= 3;
        actor->motion.vector.vz >>= 3;
        actor->motion.vector.vy >>= 6;
        if (actor->unknown_28 & KF_ACTOR_FLAG_LINKED) {
            linked->motion.vector.vx = actor->motion.vector.vx;
            linked->motion.vector.vy = actor->motion.vector.vy;
            linked->motion.vector.vz = actor->motion.vector.vz;
            if (linked->target_type != 3) {
                actor_select_target_type_in_own_group(linked, 2);
            }
        }
    }
    actor->vertical_motion_state = 0x10;
}

enum {
    KF_AREA_MAGIC_OMIT_DAMAGE_ORIGIN = 0x8000,
    KF_AREA_MAGIC_AMOUNT_MASK = 0x7fff
};

ADDRESS(0x8003a318, 0x2fc)
void actor_apply_area_magic(VECTOR *position, s32 minimum_distance, s32 reach,
                   s32 mode, u16 falloff, u16 power, u16 magic_06,
                   u16 magic_08, u16 magic_0a, u16 magic_0c, u16 magic_0e,
                   u16 magic_10, u16 magic_12, u16 magic_14,
                   s32 amount_and_flags, u16 effect_flags)
{
    KfActor *actor;
    s16 index;
    const VECTOR *damage_position = position;
    u32 amount;

    if ((amount_and_flags & KF_AREA_MAGIC_OMIT_DAMAGE_ORIGIN) != 0) {
        damage_position = 0;
    }
    amount = (u32)amount_and_flags & KF_AREA_MAGIC_AMOUNT_MASK;
    actor = actor_state.actors;
    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        s32 distance;
        u32 scaled_amount;

        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE || actor == actor_state.current) {
            continue;
        }
        if (mode == 0x8000) {
            distance = vector_distance_between_with_reach(position, reach, &actor->position,
                                     actor->collision_radius, actor->collision_height);
        } else if (mode == 0x8001) {
            if (position->vy < actor->position.vy - actor->collision_height) {
                distance = KF_DISTANCE_OUTSIDE_REACH;
            } else {
                distance = vector_distance_between_with_reach(position, reach, &actor->position,
                                         actor->collision_radius, actor->collision_height);
            }
        } else {
            distance = vector_distance_to_point(
                &actor->position, position->vx, position->vy, position->vz,
                reach, actor->collision_height, mode);
            if (distance == KF_DISTANCE_NONE) {
                distance = KF_DISTANCE_OUTSIDE_REACH;
            }
        }
        if (distance < minimum_distance) {
            continue;
        }

        if ((u16)falloff != KF_FIXED12_ONE) {
            u32 ratio = (u16)((distance << KF_FIXED12_BITS) / reach);
            u16 weight = KF_FIXED12_ONE -
                         ((ratio * (KF_FIXED12_ONE - (u16)falloff)) >> KF_FIXED12_BITS);
            scaled_amount = (amount * weight) >> KF_FIXED12_BITS;
        } else {
            scaled_amount = amount;
        }
        actor_apply_magic_to_actor(index, power, magic_06, magic_08, magic_0a,
                      magic_0c, magic_0e, magic_10, magic_12, magic_14,
                      (u16)scaled_amount, (u16)effect_flags,
                      damage_position);
    }
}

ADDRESS(0x8003a614, 0x164)
s32 actor_try_damage_player_in_cone(s32 minimum_distance, s32 maximum_distance, s32 y_offset,
                  s32 angle_tolerance, u16 damage0, u16 damage1,
                  u16 damage2, u16 damage3)
{
    s32 offset = y_offset << 5;
    KfActor *actor = actor_state.current;
    const VECTOR *camera_position;
    VECTOR origin;
    s32 distance;
    s32 angle;

    minimum_distance <<= 6;
    maximum_distance <<= 6;
    angle_tolerance <<= 4;
    origin.vx = actor->position.vx;
    origin.vy = actor->position.vy - offset;
    origin.vz = actor->position.vz;
    distance = vector_distance_to_point(&origin,
                                        player_state.camera_position.vx,
                                        player_state.camera_position.vy,
                                        player_state.camera_position.vz,
                                        maximum_distance, 0, 1700);
    if (distance == -1 || distance < minimum_distance) {
        return 0;
    }

    camera_position = &player_state.camera_position;
    angle = vector_xz_to_angle(camera_position->vx - origin.vx,
                               camera_position->vz - origin.vz);
    if (!angle_within_tolerance(actor->rotation.y, angle, angle_tolerance)) {
        return 0;
    }

    player_apply_damage(damage0, damage1, damage2, damage3,
                  0, 0, 0, 0, 0, 0x1000, 10, &origin);
    return 1;
}

ADDRESS(0x8003a778, 0x27c)
KfActor *actor_find_best_in_cone(const VECTOR *position, s16 yaw, s16 pitch,
                       s32 max_distance, s32 yaw_limit, s32 pitch_limit,
                       s32 *distance, s32 variation)
{
    KfActor *best = 0;
    s32 best_score = 30000;
    s32 best_distance = -1;
    KfActor *actor = actor_state.actors;
    u16 remaining = KF_ACTOR_CAPACITY - 1;
    struct KfEulerAngles direction;
    s32 actor_distance;
    s32 score;
    s32 reach;

    do {
        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE || actor->target_type == 3 ||
            actor == actor_state.current) {
            continue;
        }

        reach = max_distance;
        if ((actor->unknown_28 & KF_ACTOR_FLAG_CONE_TARGET_PRIORITY) != 0) {
            reach <<= 1;
        }
        actor_distance = vector_distance_to_point(
            &actor->position, position->vx, KF_DISTANCE_IGNORE_HEIGHT,
            position->vz, reach, 0, 0);
        if (actor_distance == KF_DISTANCE_NONE) {
            continue;
        }

        vector_displacement_to_pitch_yaw(actor->position.vx - position->vx,
                      actor->position.vy - position->vy,
                      actor->position.vz - position->vz, &direction);
        direction.y = (direction.y - (u16)yaw) & KF_ANGLE_WRAP_MASK;
        if (direction.y >= KF_ANGLE_HALF_TURN) {
            direction.y = KF_ANGLE_FULL_TURN - direction.y;
        }
        direction.x = (direction.x - (u16)pitch) & KF_ANGLE_WRAP_MASK;
        if (direction.x >= KF_ANGLE_HALF_TURN) {
            direction.x = KF_ANGLE_FULL_TURN - direction.x;
        }

        if ((actor->unknown_28 & KF_ACTOR_FLAG_CONE_TARGET_PRIORITY) != 0 && variation >= 0) {
            actor_distance >>= 2;
            direction.x -= 512;
            direction.y -= 512;
        } else if (direction.y > yaw_limit || direction.x > pitch_limit) {
            continue;
        }

        score = direction.y + direction.x + (actor_distance >> 7);
        if (variation > 0) {
            score += (rand() * variation) >> 15;
        }
        if (score < best_score) {
            best_score = score;
            best = actor;
            best_distance = actor_distance;
        }
    } while (actor++, remaining-- != 0);

    *distance = best_distance;
    return best;
}

ADDRESS(0x8003a9f4, 0x168)
s32 actor_find_overlap_excluding_target_type3(s32 x, s32 y, s32 z,
                                             s32 radius, s32 height)
{
    KfActor *actor = actor_state.actors;
    s32 index;

    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        VECTOR alternate;

        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE || actor->target_type == 3
            || (actor_state.actor_overlap_exclusion_flags & actor->unknown_28)
            || actor == actor_state.current) {
            continue;
        }
        if (actor->unknown_28 & KF_ACTOR_FLAG_LINKED) {
            if (actor->word_22.linked_actor_slot == actor_state.current_actor_slot_index) {
                continue;
            }
            alternate.vx = actor->position.vx;
            alternate.vz = actor->position.vz;
            alternate.vy = actor->position.vy + actor->vertical_anchor_offset;
            if (vector_distance_to_point(&alternate, x, y, z,
                actor->collision_radius + radius, actor->collision_height, height) != -1) {
                return index;
            }
        } else {
            if (vector_distance_to_point(&actor->position, x, y, z,
                actor->collision_radius + radius, actor->collision_height, height) != -1) {
                return index;
            }
        }
    }
    return -1;
}

ADDRESS(0x8003ab5c, 0x158)
s32 actor_find_overlap(s32 x, s32 y, s32 z, s32 radius, s32 height)
{
    KfActor *actor = actor_state.actors;
    s32 index;

    for (index = 0; index < KF_ACTOR_CAPACITY; index++, actor++) {
        VECTOR alternate;

        if (actor->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE
            || (actor_state.actor_overlap_exclusion_flags & actor->unknown_28)
            || actor == actor_state.current) {
            continue;
        }
        if (actor->unknown_28 & KF_ACTOR_FLAG_LINKED) {
            if (actor->word_22.linked_actor_slot == actor_state.current_actor_slot_index) {
                continue;
            }
            alternate.vx = actor->position.vx;
            alternate.vz = actor->position.vz;
            alternate.vy = actor->position.vy + actor->vertical_anchor_offset;
            if (vector_distance_to_point(&alternate, x, y, z,
                actor->collision_radius + radius, actor->collision_height, height) != -1) {
                return index;
            }
        } else {
            if (vector_distance_to_point(&actor->position, x, y, z,
                actor->collision_radius + radius, actor->collision_height, height) != -1) {
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
        if (group->initial_actor_flags & KF_ACTOR_FLAG_LINKED) {
            other = &actor_state.actors[actor->word_22.linked_actor_slot];
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

ADDRESS(0x8003ae50, 0x4ec)
s32 actor_move_horizontal_with_collision(SVECTOR *motion, s32 flags)
{
    KfActor *actor = actor_state.current;
    s32 motion_x;
    s32 motion_z;
    s32 original_x;
    s32 original_z;
    VECTOR proposed;
    s32 collision;
    s32 axis_attempted;
    s32 diagonal_attempted;
    s32 retry_count;
    s32 result = 0;

    if (motion->vx == 0 && motion->vz == 0) {
        return 0;
    }
    retry_count = 0;
    diagonal_attempted = 0;
    axis_attempted = 0;
    original_x = motion_x = motion->vx;
    original_z = motion_z = motion->vz;

retry_move:
    proposed.vx = actor->position.vx + motion_x;
    proposed.vz = actor->position.vz + motion_z;
    collision = collision_query_world(proposed.vx, actor->position.vy, proposed.vz,
        actor->collision_radius,
        actor->collision_height | ((actor->unknown_28 & 0xc000) << 16),
        actor_state.actor_collision_query_flags);
    if (collision == 0) {
    check_floor:
        if (actor->vertical_motion_state == 0 && (flags & 0x24)) {
            s32 floor_height = KF_COLLISION_CACHE_RESULT;
            s32 probe_x;
            s32 probe_z;

            if (motion_x == 0) {
                probe_x = 0;
            } else if (motion_x > 0) {
                probe_x = actor->collision_radius * 2;
            } else {
                probe_x = -(s32)actor->collision_radius * 2;
            }
            if (motion_z == 0) {
                probe_z = 0;
            } else if (motion_z > 0) {
                probe_z = actor->collision_radius * 2;
            } else {
                probe_z = -(s32)actor->collision_radius * 2;
            }
            collision_query_shapes_with_layer_sample(actor->position.vx, actor->position.vy,
                actor->position.vz + probe_z, actor->collision_radius,
                actor->collision_height | ((actor->unknown_28 & 0xc000) << 16));
            if (floor_height < KF_COLLISION_CACHE_RESULT) {
                floor_height = KF_COLLISION_CACHE_RESULT;
            }
            collision_query_shapes_with_layer_sample(actor->position.vx + probe_x, actor->position.vy,
                actor->position.vz, actor->collision_radius,
                actor->collision_height | ((actor->unknown_28 & 0xc000) << 16));
            if (floor_height < KF_COLLISION_CACHE_RESULT) {
                floor_height = KF_COLLISION_CACHE_RESULT;
            }
            if (actor->position.vy < floor_height - 1100) {
                result |= 0x100;
                if (flags & 4) {
                    goto try_axis;
                }
            }
        }
        actor->position.vx = proposed.vx;
        actor->position.vz = proposed.vz;
        goto finish;
    }

    result |= collision;
    if (!(flags & 3)) {
        goto finish;
    }
    if (collision & 0xb0) {
        s32 obstacle_angle;
        s32 movement_angle;
        s32 length;

        retry_count++;
        if (retry_count == 2) {
            goto finish;
        }
        collision_cache_load_hit_bounds();
        obstacle_angle = vector_xz_to_angle(
            KF_COLLISION_CACHE_POSITION.vx - actor->position.vx,
            KF_COLLISION_CACHE_POSITION.vz - actor->position.vz);
        movement_angle = vector_xz_to_angle(motion_x, motion_z);
        obstacle_angle = (angle_mod_delta_le_half_turn(
            movement_angle, obstacle_angle)
            ? obstacle_angle + 1024 : obstacle_angle - 1024) & KF_ANGLE_WRAP_MASK;
        length = SquareRoot0(motion_x * motion_x + motion_z * motion_z);
        motion_x = -(rsin(obstacle_angle) * length) >> 13;
        motion_z = (rcos(obstacle_angle) * length) >> 13;
        goto retry_move;
    }
    if (!(flags & 0x10)) {
        if (actor->unknown_28 & 0x4000) {
            goto try_axis;
        }
        if (!(collision & ~5)) {
            if (actor->position.vy <= KF_COLLISION_CACHE_RESULT + 800) {
                goto check_floor;
            }
            goto try_axis;
        }
    }
    if (!(collision & KF_COLLISION_HIT_AXIS)) {
        goto check_diagonal;
    }

try_axis:
    if (axis_attempted) {
        motion_z = 0;
        motion_x = 0;
        goto finish;
    }
    if (motion_x != 0) {
        motion_x = 0;
        if (flags & 1) {
            s32 length = SquareRoot0(original_x * original_x
                                    + original_z * original_z);
            motion_z = length;
            if (original_z <= 0) {
                motion_z = -motion_z;
            }
        } else {
            motion_z = original_z;
        }
        goto retry_move;
    }
    if (motion_z != 0) {
        motion_z = 0;
        if (flags & 1) {
            s32 length = SquareRoot0(original_x * original_x
                                    + original_z * original_z);
            motion_x = length;
            if (original_x <= 0) {
                motion_x = -motion_x;
            }
        } else {
            motion_z = original_z;
        }
        axis_attempted = 1;
        goto retry_move;
    }
check_diagonal:
    if (!(collision & KF_COLLISION_HIT_DIAGONAL)) {
        goto finish;
    }
    if (diagonal_attempted) {
        goto try_axis;
    }
    switch (((KfMapOccupancyLayer *)KF_COLLISION_CACHE_SHAPE)->quarter_turns & 3) {
    case 0:
    case 2:
        motion_x = (original_x + original_z) >> 1;
        motion_z = motion_x;
        break;
    default:
        motion_x = (original_x - original_z) >> 1;
        motion_z = -motion_x;
        break;
    }
    diagonal_attempted = 1;
    goto retry_move;

finish:
    if (flags & 8) {
        motion->vx = motion_x;
        motion->vz = motion_z;
    }
    return result;
}

ADDRESS(0x8003b33c, 0x1e4)
s32 actor_move_with_collision(SVECTOR *motion)
{
    KfActor *actor = actor_state.current;
    VECTOR proposed;
    s32 result;

    proposed.vx = actor->position.vx + motion->vx;
    proposed.vy = actor->position.vy + motion->vy;
    proposed.vz = actor->position.vz + motion->vz;
    result = collision_query_world(proposed.vx, proposed.vy, proposed.vz,
        actor->collision_radius,
        actor->collision_height | ((actor->unknown_28 & 0xc000) << 16),
        actor_state.actor_collision_query_flags);
    if (result == 0) {
        copyVector(&actor->position, &proposed);
    } else if (collision_query_world(proposed.vx, actor->position.vy,
                             actor->position.vz, actor->collision_radius,
                             actor->collision_height | ((actor->unknown_28 & 0xc000) << 16),
                             actor_state.actor_collision_query_flags) != 0) {
        motion->vx = -(u16)motion->vx;
    } else if (collision_query_world(actor->position.vx, proposed.vy,
                             actor->position.vz, actor->collision_radius,
                             actor->collision_height | ((actor->unknown_28 & 0xc000) << 16),
                             actor_state.actor_collision_query_flags) != 0) {
        motion->vy = -(u16)motion->vy;
    } else if (collision_query_world(actor->position.vx, actor->position.vy,
                             proposed.vz, actor->collision_radius,
                             actor->collision_height | ((actor->unknown_28 & 0xc000) << 16),
                             actor_state.actor_collision_query_flags) != 0) {
        motion->vz = -(u16)motion->vz;
    }
    return result;
}

ADDRESS(0x8003b520, 0x9c)
s32 actor_start_ballistic_motion(s32 mode, s32 target_x, s32 target_y,
    s32 target_z, s32 trajectory_parameter, s32 trajectory_speed)
{
    KfActor *actor = actor_state.current;
    s16 result;

    if (trajectory_solve_motion_between_points(mode, actor->position.vx, actor->position.vy,
        actor->position.vz, target_x, target_y, target_z,
        trajectory_parameter, trajectory_speed, &result,
        &actor->ballistic_horizontal_speed, &actor->ballistic_launch_speed_y) != 0) {
        return -1;
    }
    actor->vertical_motion_state = KF_ACTOR_VERTICAL_MOTION_BALLISTIC;
    actor->motion.ballistic.phase = 1;
    actor->ballistic_acceleration = trajectory_parameter;
    actor->ballistic_origin_y = actor->position.vy;
    return result;
}

ADDRESS(0x8003b5bc, 0x14)
void actor_suspend_vertical_motion(void)
{
    actor_state.current->vertical_motion_state = KF_ACTOR_VERTICAL_MOTION_SUSPENDED;
}

ADDRESS(0x8003b5d0, 0x3d4)
void actor_update_vertical_motion(void)
{
    KfActor *actor = actor_state.current;
    KfTargetGroup *group = actor_state.active_group;
    s32 vertical_state;

    collision_probe_floor_height(actor->position.vx, actor->position.vy, actor->position.vz,
                   actor->collision_radius,
                   actor->collision_height | ((actor->unknown_28 & 0xc000) << 16));
    actor->current_map_layer = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
    if (actor->unknown_28 & KF_ACTOR_FLAG_USE_MAP_LAYER_FLOOR) {
        KF_COLLISION_CACHE_RESULT = KF_COLLISION_CACHE_HEIGHT;
    }

    vertical_state = actor->vertical_motion_state;
    if (vertical_state == 0x20) goto state_20;
    if (vertical_state < 33) {
        if (vertical_state == 0) goto state_0;
        if (vertical_state == 0x10) goto state_10;
        return;
    }
    if (vertical_state == KF_ACTOR_VERTICAL_MOTION_BALLISTIC) goto state_30;
    return;

state_0: {
        s32 next_y;
        next_y = KF_COLLISION_CACHE_RESULT - actor->position.vy;
        if (next_y < 0) {
            actor->vertical_motion_state = 0x20;
            actor->motion.vector.vy = -100;
        } else if (next_y > 0) {
            actor->vertical_motion_state = 0x10;
            actor->motion.vector.vy = 0;
        }
        return;
    }

state_10: {
        s32 next_y;
        s32 collision;
        next_y = actor->position.vy + actor->motion.vector.vy;
        collision = collision_query_world(actor->position.vx, next_y,
                                  actor->position.vz, actor->collision_radius,
                                  actor->collision_height |
                                      ((actor->unknown_28 & 0xc000) << 16),
                                  actor_state.actor_collision_query_flags);
        if (collision == 0) {
        advance_rise:
            actor->position.vy = next_y;
            actor->motion.vector.vy += group->vertical_acceleration;
            return;
        }
        if (collision == 0x80 && actor->motion.vector.vy > 40) {
            player_apply_damage(0, group->unknown_06, 0, 0, 0, 0, 0, 0, 0,
                          0x1000, 10, &actor->position);
        }
        if (collision & 4) {
            if (actor->unknown_28 & KF_ACTOR_FLAG_USE_MAP_LAYER_FLOOR) {
                s32 floor_y = KF_COLLISION_CACHE_HEIGHT;
                if (actor->position.vy < floor_y) goto advance_rise;
                actor->position.vy = floor_y;
            } else {
                actor->position.vy = KF_COLLISION_CACHE_RESULT;
            }
            actor->motion.vector.vy = 0;
            actor->vertical_motion_state = 0;
            return;
        }
        if (actor->unknown_28 & KF_ACTOR_FLAG_USE_MAP_LAYER_FLOOR) goto advance_rise;
        actor->vertical_motion_state = 0;
        return;
    }

state_20:
        actor->position.vy += actor->motion.vector.vy;
        actor->motion.vector.vy += 5;
        if (KF_COLLISION_CACHE_RESULT < actor->position.vy &&
            actor->motion.vector.vy < 0) {
            return;
        }
        actor->position.vy = KF_COLLISION_CACHE_RESULT;
        actor->vertical_motion_state = 0;
        return;

state_30: {
        s32 phase;
        s32 next_y;
        s32 collision;
        phase = actor->motion.ballistic.phase;
        next_y = actor->ballistic_origin_y - actor->ballistic_launch_speed_y * phase +
                 ((actor->ballistic_acceleration * phase * phase) >> 1);
        collision = collision_query_world(actor->position.vx, next_y,
                                  actor->position.vz, actor->collision_radius,
                                  actor->collision_height |
                                      ((actor->unknown_28 & 0xc000) << 16),
                                  actor_state.actor_collision_query_flags);
        if (collision == 0) {
            actor->position.vy = next_y;
            actor->motion.ballistic.phase++;
            actor->current_map_layer = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
            return;
        }
        if (collision == 0x80) {
            player_apply_damage(0, group->unknown_06, 0, 0, 0, 0, 0, 0, 0,
                          0x1000, 10, &actor->position);
        }
        actor->vertical_motion_state = 0x10;
        actor->motion.ballistic.phase = 0;
        return;
    }
}

ADDRESS(0x8003b9a4, 0x140)
s32 actor_damp_horizontal_motion(s32 decay, s32 target)
{
    KfActor *actor = actor_state.current;
    s32 length;

    if (actor->vertical_motion_state == 0) {
        length = SquareRoot0(actor->motion.vector.vx * actor->motion.vector.vx
                           + actor->motion.vector.vz * actor->motion.vector.vz);
        if (length == 0) {
            return 0;
        }
        actor->motion.vector.vx = value_approach(actor->motion.vector.vx, 0,
            (actor->motion.vector.vx * decay * 2) / length);
        actor->motion.vector.vz = value_approach(actor->motion.vector.vz, 0,
            (actor->motion.vector.vz * decay * 2) / length);
    }
    return actor_move_horizontal_with_collision(&actor->motion.vector, target);
}

ADDRESS(0x8003bae4, 0xbc)
s32 actor_move_along_heading(s16 angle, s32 speed, s32 step, s32 target)
{
    KfActor *actor = actor_state.current;
    struct KfVecXZi direction;
    struct KfVecXZi step_direction;

    angle_to_forward_xz(angle, &direction);
    step_direction = direction;
    vector2i_scale_shift11(speed, &direction);
    vector2i_scale_shift11(step, &step_direction);
    actor->motion.vector.vx = value_approach(actor->motion.vector.vx,
                                        direction.x, step_direction.x);
    actor->motion.vector.vz = value_approach(actor->motion.vector.vz,
                                        direction.z, step_direction.z);
    return actor_move_horizontal_with_collision(&actor->motion.vector, target);
}

ADDRESS(0x8003bba0, 0x130)
void actor_turn_toward_angle(KfActor *actor, s32 target_angle, s32 max_speed,
    s32 acceleration)
{
    if (angle_shortest_delta(target_angle, actor->rotation.y) != 0) {
        s32 old_angle;

        if (angle_mod_delta_le_half_turn(target_angle, actor->rotation.y)) {
            actor->turn_rate += acceleration;
            if (max_speed < (s16)actor->turn_rate) {
                actor->turn_rate = max_speed;
            }
        } else {
            actor->turn_rate -= acceleration;
            if ((s16)actor->turn_rate < -max_speed) {
                actor->turn_rate = -max_speed;
            }
        }

        old_angle = actor->rotation.y;
        actor->rotation.y += actor->turn_rate;
        if ((s16)actor->turn_rate > 0) {
            if (angle_mod_delta_le_half_turn(target_angle, old_angle) &&
                !angle_mod_delta_le_half_turn(target_angle, actor->rotation.y)) {
                actor->rotation.y = target_angle;
            }
        } else {
            if (angle_mod_delta_le_half_turn(old_angle, target_angle) &&
                !angle_mod_delta_le_half_turn(actor->rotation.y, target_angle)) {
                actor->rotation.y = target_angle;
            }
        }
    } else {
        actor->turn_rate = 0;
    }
}

ADDRESS(0x8003bcd0, 0x70)
s32 actor_turn_and_move_along_heading(s16 angle, s32 speed, s32 range, s32 step,
    s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;

    actor_turn_toward_angle(actor, angle, range, mode);
    return actor_move_along_heading(actor->rotation.y, speed, step, target);
}

ADDRESS(0x8003bd40, 0xf8)
s32 actor_turn_and_move_toward_point(s32 world_x, s32 world_z, s32 speed, s32 range,
                  s16 reference_angle, s32 step, s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;
    s32 dx = (s32)((u32)world_x - (u32)actor->position.vx);
    s32 dz = (s32)((u32)world_z - (u32)actor->position.vz);
    s32 angle = vector_xz_to_angle(dx, dz);

    dx = dx < 0 ? (s32)(0u - (u32)dx) : dx;
    dz = dz < 0 ? (s32)(0u - (u32)dz) : dz;
    if (reference_angle != -1 && (s32)((u32)dx + (u32)dz) <= 600
        && !angle_within_tolerance(angle, reference_angle, 0x320)) {
        return -1;
    }
    actor_turn_and_move_along_heading(angle, speed, range, step, mode, target);
    return angle & KF_ANGLE_WRAP_MASK;
}

ADDRESS(0x8003be38, 0x13c)
s32 actor_move_along_euler_angles(const struct KfEulerAngles *angles, s32 speed,
                  s32 step, s32 target)
{
    KfActor *actor = actor_state.current;
    SVECTOR direction;
    SVECTOR step_direction;
    s32 moved;
    s32 proposed_y;
    s32 radius;
    s32 height_and_flags;

    pitch_yaw_to_forward_vector(angles, &direction);
    step_direction = direction;
    vector3s_scale_shift12(speed, &direction);
    vector3s_scale_shift12(step, &step_direction);
    actor->motion.vector.vx = value_approach(actor->motion.vector.vx,
                                        direction.vx, step_direction.vx);
    actor->motion.vector.vy = value_approach(actor->motion.vector.vy,
                                        direction.vy, step_direction.vy);
    actor->motion.vector.vz = value_approach(actor->motion.vector.vz,
                                        direction.vz, step_direction.vz);
    moved = actor_move_horizontal_with_collision(&actor->motion.vector, target) != 0;
    proposed_y = actor->position.vy + actor->motion.vector.vy;
    radius = actor->collision_radius;
    height_and_flags = actor->collision_height | ((actor->unknown_28 & 0xc000) << 16);
    if (collision_query_world(actor->position.vx, proposed_y, actor->position.vz,
                      radius, height_and_flags,
                      actor_state.actor_collision_query_flags) == 0) {
        actor->position.vy = proposed_y;
    } else {
        moved |= 2;
    }
    return moved;
}

ADDRESS(0x8003bf74, 0x8c)
s32 actor_turn_and_move_along_euler_angles(const struct KfEulerAngles *angles, s32 speed,
    s32 range, s32 step, s32 mode, s32 target)
{
    KfActor *actor = actor_state.current;

    actor_turn_toward_angle(actor, angles->y, range, mode);
    actor->rotation.x = angle_approach(actor->rotation.x, angles->x, 8);
    return actor_move_along_euler_angles(&actor->rotation, speed, step, target);
}

ADDRESS(0x8003c000, 0x10c)
s32 actor_sample_rotated_animation_vertex(KfActor *actor, s32 vertex_index, VECTOR *output)
{
    struct KfEulerAngles rotation;
    SVECTOR offset;

    if (animation_sample_vertex(actor->definition_id + 128, actor->animation_id,
                      actor->animation_phase, vertex_index, &offset) != 0) {
        offset.vx = 0;
        offset.vy = -(s32)actor->collision_height >> 1;
        offset.vz = -(s32)actor->collision_radius;
    } else {
        offset.vx = ((s32)offset.vx * (s16)actor->model_scale_x) >> KF_FIXED12_BITS;
        offset.vy = ((s32)offset.vy * (s16)actor->model_scale_y.value) >> KF_FIXED12_BITS;
        offset.vz = ((s32)offset.vz * (s16)actor->model_scale_z) >> KF_FIXED12_BITS;
    }
    rotation.x = actor->rotation.x;
    rotation.y = actor->rotation.y + KF_ANGLE_HALF_TURN;
    rotation.z = actor->rotation.z;
    vector_rotate_yxz(&rotation, &offset, output);
    return 1;
}

ADDRESS(0x8003c10c, 0x114)
VECTOR *actor_resolve_group_position(KfActor *actor, VECTOR *output)
{
    KfTargetGroup *group;
    switch (actor->unknown_28 & 3) {
    case 0:
        return &actor->position;
    case 1:
        group = &actor_state.target_groups[actor->group_index];
        setVector(output, actor->position.vx + group->position_offset_x,
                  actor->position.vy + group->position_offset_y,
                  actor->position.vz + group->position_offset_z);
        return output;
    case 2:
        group = &actor_state.target_groups[actor->group_index];
        vector_rotate_yxz(&actor->rotation,
                          (SVECTOR *)&group->position_offset_x, output);
        addVector(output, &actor->position);
        return output;
    }
    /* Retail leaves the return register unspecified for mode 3. */
}

ADDRESS(0x8003c220, 0x1c0)
void actor_update_motion_animation(s32 first, s32 reverse, s32 forward, s32 fast,
                   s32 slow, s32 phase_step)
{
    KfActor *actor = actor_state.current;
    s32 selected = first;
    s32 magnitude = 0;
    s16 motion = (s16)actor->turn_rate;

    if (motion > 0) {
        selected = forward;
        magnitude = motion;
    } else if (motion < 0) {
        selected = reverse;
        magnitude = -motion;
    }
    if (actor->motion.vector.vy >= 11) {
        if (magnitude < actor->motion.vector.vy) {
            selected = fast;
        }
    } else if (actor->motion.vector.vy < 10) {
        if (magnitude < -actor->motion.vector.vy) {
            selected = slow;
        }
    }

    if (actor->animation_id == first) {
        actor_advance_animation_wrapped(actor, phase_step);
        if (actor_animation_crossed_phase(actor, 0)) {
            actor_set_animation_if_changed(selected);
        }
    } else if (actor->animation_id == reverse) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->animation_id) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    } else if (actor->animation_id == forward) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->animation_id) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    } else if (actor->animation_id == fast) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->animation_id) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    } else if (actor->animation_id == slow) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->animation_id) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    }
}

ADDRESS(0x8003c3e0, 0x234)
s32 actor_compute_target_direction(KfActor *actor, const VECTOR *origin, s32 step,
                  const VECTOR *target, SVECTOR *direction,
                  s32 pitch_override, u16 yaw_limit, s32 iterations)
{
    VECTOR position = *origin;
    struct KfEulerAngles angles;
    s32 pitch = pitch_override;
    s32 yaw_fraction;
    s32 yaw_error;
    s32 pitch_error;
    s32 distance;
    s32 target_y;

    for (;;) {
        target_y = target->vy + 1600;
        vector_displacement_to_pitch_yaw(position.vx - target->vx,
                      position.vy - target_y,
                      position.vz - target->vz, &angles);
        yaw_error = ((s16)angles.y - (s16)actor->rotation.y) & KF_ANGLE_WRAP_MASK;
        yaw_fraction = yaw_error << KF_FIXED12_BITS;
        if (yaw_error >= KF_ANGLE_HALF_TURN) {
            yaw_error = KF_ANGLE_FULL_TURN - yaw_error;
            yaw_fraction = yaw_error << KF_FIXED12_BITS;
        }
        yaw_fraction /= (s16)yaw_limit;
        if (yaw_fraction > KF_FIXED12_ONE) {
            yaw_fraction = KF_FIXED12_ONE;
        }
        angles.y = angle_lerp_shortest_q12(angles.y, actor->rotation.y, yaw_fraction);

        if ((s16)pitch == -1) {
            pitch_error = ((s16)angles.x - (s16)actor->rotation.x) & KF_ANGLE_WRAP_MASK;
            if (pitch_error >= KF_ANGLE_HALF_TURN) {
                pitch_error = KF_ANGLE_FULL_TURN - pitch_error;
            }
            angles.x = angle_lerp_shortest_q12(angles.x, actor->rotation.x, pitch_error);
        } else {
            angles.x = pitch;
        }

        pitch_yaw_to_forward_vector(&angles, direction);
        vector3s_scale_shift12(step, direction);
        if (--iterations == 0) {
            break;
        }

        distance = fixed_vector3_length(target->vx - position.vx,
                                        target->vy - position.vy,
                                        target->vz - position.vz);
        vector_add_scaled_delta(origin, &player_state.frame_displacement,
                      distance / step, &position);
    }
    return angles.y;
}

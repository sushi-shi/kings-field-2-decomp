#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <psyq/libc.h>
#include <kf/game/collision_cache.h>
#include <kf/game/callback.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <kf/lib/null.h>

RODATA(0x800120d8, 0x3c4)

enum {
    KF_TARGET_SOUND_BASE_ID = 96,
    KF_TARGET_SOUND_ALTERNATE_RANGE = 0x80,
    KF_TARGET_SOUND_INDEX_MASK = 0x7f,
    KF_TARGET_SOUND_TRIGGER_INTERVAL_MASK = 0x3fff,
    KF_TARGET_SOUND_TRIGGER_MODE_MASK = 0xc000,
    KF_TARGET_SOUND_TRIGGER_STAGGERED = 0x4000,
    KF_TARGET_SOUND_TRIGGER_RANDOM = 0x8000
};

ADDRESS(0x8003d084, 0x64)
s32 actor_sound_note_offset(KfActor *actor)
{
    s32 offset = 16 - actor->model_scale_y.bytes.high;

    if (offset > 12) {
        offset = 12;
    } else if (offset < -12) {
        offset = -12;
    }
    return offset + (((rand() * 5) >> 15) - 2);
}

ADDRESS(0x8003d0e8, 0x9c)
void actor_play_target_sound(KfActor *actor)
{
    KfTargetCandidate *target = actor->target;

    if (target->sound_code & KF_TARGET_SOUND_ALTERNATE_RANGE) {
        audio_play_spatial_range((target->sound_code & KF_TARGET_SOUND_INDEX_MASK) + KF_TARGET_SOUND_BASE_ID,
            &actor->position, 0x7f, 0x6000, 0x7800,
            actor_sound_note_offset(actor));
    } else {
        audio_play_spatial_default_range(target->sound_code + KF_TARGET_SOUND_BASE_ID,
            &actor->position, 0x6e, actor_sound_note_offset(actor));
    }
}

ADDRESS(0x8003d184, 0x248c)
void actor_update_behavior(void)
{
    KfActor *actor = actor_state.current;
    KfTargetGroup *group = actor_state.active_group;
    KfTargetCandidate *target = actor->target;
    u16 trigger;
    s32 interval;

    if ((actor->unknown_28 & 4) != 0) {
        actor_state.actor_collision_query_flags = 3;
    } else {
        actor_state.actor_collision_query_flags = 0x93;
    }
    map_cell_add_layer_occupancy(actor->position.vx, actor->position.vz,
                   actor->collision_radius, -1);

    if (target->sound_code != KF_AUDIO_SOUND_NONE) {
        trigger = target->sound_trigger;
        interval = trigger & KF_TARGET_SOUND_TRIGGER_INTERVAL_MASK;
        switch (trigger & KF_TARGET_SOUND_TRIGGER_MODE_MASK) {
        case 0:
            if (actor_animation_crossed_phase(actor, interval)) {
                goto play_sound;
            }
            break;
        case KF_TARGET_SOUND_TRIGGER_RANDOM:
            if ((rand() >> 3) < interval) {
                goto play_sound;
            }
            break;
        case KF_TARGET_SOUND_TRIGGER_STAGGERED:
            if ((interval * actor_state.current_actor_slot_index / 3) % interval ==
                (s32)actor_state.actor_update_frame_count % interval) {
                goto play_sound;
            }
            break;
        }
    }
    goto dispatch_action;

play_sound:
    actor_play_target_sound(actor);

dispatch_action:

    switch (actor->target_type) {
    case 2:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
        }
        if (actor->animation_phase < 0x800 ||
            (actor->unknown_28 & KF_ACTOR_FLAG_EFFECT_ANIMATION_HOLD) == 0) {
            actor_advance_animation_clamped(actor, target->animation_step);
        }
        if (actor->animation_phase > 0xffe) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(group->movement_step * 2, 10);
        break;
    case 3: {
        s32 old_state = actor->state_70.signed_state;

        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
            actor->state_70.signed_state = 0;
            actor->unknown_28 &= ~KF_ACTOR_FLAG_MAP_OBJECT_ATTACHED;
        }
        if (actor->state_70.signed_state == 0) {
            if (actor->animation_phase >= 0x400 &&
                (actor->unknown_28 & KF_ACTOR_FLAG_EFFECT_ANIMATION_HOLD) != 0) {
                break;
            }
            actor_advance_animation_clamped(actor, target->animation_step);
            if (actor_animation_crossed_phase(actor, 0x800)) {
                u16 effect_id = group->scattered_effect_id_center +
                    random_centered_triangular_scaled(group->scattered_effect_id_center);

                if (effect_id != 0) {
                    map_object_spawn_scattered_effect(effect_id, &actor->position,
                                   -(actor->collision_height >> 1));
                }
                if (actor->slot_state == 0 || actor->slot_state == 4) {
                    if (target->word_0c.bytes.low != 0xff &&
                        (rand() >> 7) < target->word_0c.bytes.high) {
                        map_object_spawn_effect(
                            1, target->word_0c.bytes.low, &actor->position,
                            -(actor->collision_height >> 1));
                    }
                } else if (actor->slot_state == KF_ACTOR_SLOT_PERSISTENT &&
                           actor->death_drop_object_id != 0xff) {
                    map_object_spawn_effect(
                        0, actor->death_drop_object_id, &actor->position,
                        -(actor->collision_height >> 1));
                }
            }
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX ||
                target->animation_step == 0) {
                actor->state_70.signed_state = 15;
            }
        } else {
            s32 current_state = actor->state_70.signed_state;

            if (current_state == 99) {
                break;
            }
            actor->state_70.signed_state++;
            if (old_state < 20) {
                goto case3_motion;
            }
            if (old_state == 20) {
                actor->lighting_override = 0x42;
                actor->lighting_blend = 0x400;
                actor->render_mode = 1;
            } else if (old_state < 38) {
                if (actor->lighting_blend < 0x1000) {
                    actor->lighting_blend += 192;
                } else {
                    actor->lighting_blend = 0x1000;
                }
            } else {
                state_8017d118.active_table[19](actor);
                if (actor->slot_state == KF_ACTOR_SLOT_PERSISTENT) {
                    actor_set_lifecycle_and_home_position(actor);
                } else if (actor->slot_state == KF_ACTOR_SLOT_RESPAWNING) {
                    actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
                    actor_set_home_position(actor);
                } else if (actor->slot_state == KF_ACTOR_SLOT_EFFECT_SPAWNED) {
                    actor->slot_state = KF_ACTOR_SLOT_FREE;
                    actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
                } else {
                    actor->lifecycle = KF_ACTOR_LIFECYCLE_WAIT_FOR_RANGE_EXIT;
                    actor_set_home_position(actor);
                }
            }
        }
case3_motion:
        actor_damp_horizontal_motion(group->movement_step * 2, 10);
        break;
    }
    case 0:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation_if_changed(target->animation_id);
        }
        actor_advance_animation_wrapped(actor, target->animation_step);
        if (angle_within_tolerance(actor->animation_phase, 0,
                                   target->animation_step - 1)) {
            actor->target_action_state = 0xf1;
        }
        actor_damp_horizontal_motion(group->movement_step, 10);
        break;
    case 1:
        if (actor->target_action_state == 0) {
            actor->target_action_state = 0xf1;
            actor_set_animation_if_changed(target->animation_id);
            actor->movement_yaw = rand() >> KF_RANDOM_ANGLE_SHIFT;
        } else if (actor_turn_and_move_along_heading(actor->movement_yaw,
                                  target->word_0c.value, target->word_0e.value,
                                  group->movement_step,
                                  group->turn_acceleration, 5) != 0 ||
                   (rand() >> 5) < target->word_10.bytes.fallback_offset) {
            actor->movement_yaw = rand() >> KF_RANDOM_ANGLE_SHIFT;
        }
        actor_advance_animation_wrapped(actor, target->animation_step);
        break;
    case 12:
    case 16: {
        s32 motion_flags;

        if (actor->target_action_state == 0) {
            actor->target_action_state = 0xf1;
            actor->tail_72.angles.z = 0;
            actor->tail_72.angles.y = 0;
            actor->tail_72.angles.x = 0;
            actor->tail_72.motion.baseline = (u16)actor->vertical_anchor_offset +
                collision_sample_map_layer_height(actor->home_map_layer,
                    (actor->home_cell_x << 11) + actor->word_24.home_local_x,
                    (actor->home_cell_z << 11) + actor->word_22.home_local_z,
                    actor->collision_radius, actor->collision_height);
            actor_set_animation_if_changed(target->animation_id);
            actor_suspend_vertical_motion();
            motion_flags = 3;
        } else {
            motion_flags = actor_turn_and_move_along_euler_angles(
                &actor->tail_72.angles, target->word_0c.value,
                target->word_0e.value,
                group->movement_step,
                group->turn_acceleration, 17);
            if ((rand() >> 5) < target->word_12.flight.orientation_change_threshold) {
                motion_flags |= 1;
            }
            if ((rand() >> 5) < target->word_12.flight.orientation_change_threshold) {
                motion_flags |= 2;
            }
            if (actor->tail_72.motion.baseline + target->word_10.value <
                actor->position.vy) {
                actor->motion.vector.vy -= target->word_12.flight.vertical_velocity_step;
            } else if (actor->position.vy <
                       actor->tail_72.motion.baseline - target->word_10.value) {
                actor->motion.vector.vy += target->word_12.flight.vertical_velocity_step;
            }
        }
        if (motion_flags & 1) {
            actor->tail_72.angles.y = rand() >> KF_RANDOM_ANGLE_SHIFT;
        }
        if (motion_flags & 2) {
            actor->tail_72.angles.x = (rand() >> 5) - 512;
        }
        if (actor->target_type == 16) {
            actor_update_motion_animation(target->animation_id, target->word_14.bytes[0],
                           target->word_14.bytes[1], target->word_16.bytes.low,
                           target->word_16.bytes.high, target->animation_step);
        } else {
            actor_advance_animation_wrapped(actor, target->animation_step);
        }
        break;
    }
    case 5: {
        s32 distance;
        s32 angle;
        s32 mode;
        s32 motion_flags;

        if (actor->target_action_state == 0) {
            actor->target_action_state = 0xf1;
            actor_set_animation_if_changed(target->animation_id);
            switch (actor->previous_target_type) {
            case 4:
            case 18:
            case 23:
            case 24:
                actor->state_70.bytes.low = 1;
                break;
            default:
                actor->state_70.bytes.low = 0;
                break;
            }
            actor->state_70.bytes.high = 0;
        }
        if (rand() < 6000) {
            distance = fixed_vector2_length(
                player_state.camera_position.vx - actor->position.vx,
                player_state.camera_position.vz - actor->position.vz);
            if (distance >= target->word_16.value) {
                actor->state_70.bytes.low = 0;
            } else if (distance <= target->word_14.value) {
                actor->state_70.bytes.low = 1;
            }
        }
        angle = vector_xz_to_angle(
            player_state.camera_position.vx - actor->position.vx,
            player_state.camera_position.vz - actor->position.vz);
        actor->movement_yaw = angle;
        mode = actor->state_70.bytes.high == 0 ? 5 : 33;
        if (actor->state_70.bytes.low == 0) {
            motion_flags = actor_turn_and_move_along_heading((s16)angle,
                target->word_0c.value, target->word_0e.value,
                group->movement_step,
                group->turn_acceleration, mode);
        } else {
            motion_flags = actor_move_along_heading((s16)angle + 2048,
                target->word_0c.value,
                group->movement_step, mode);
        }
        if (motion_flags & 0x100) {
            if (actor->state_70.bytes.high != 0) {
                goto case5_advance;
            }
            if (rand() < target->word_18.value) {
                actor->state_70.bytes.high = 1;
                goto case5_advance;
            }
        } else {
            actor->state_70.bytes.high = 0;
        }
        if (motion_flags != 0) {
            actor->state_70.bytes.low = actor->state_70.bytes.low == 0;
        }
    case5_advance:
        actor_advance_animation_wrapped(actor, target->animation_step);
        break;
    }
    case 13:
    case 17: {
        s32 delta_x;
        s32 delta_y;
        s32 delta_z;
        s32 distance;
        struct KfEulerAngles opposite;

        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation_if_changed(target->animation_id);
            actor->state_70.signed_state = 0;
        }
        delta_x = player_state.camera_position.vx - actor->position.vx;
        delta_y = player_state.camera_position.vy - actor->position.vy - 1600;
        delta_z = player_state.camera_position.vz - actor->position.vz;
        distance = fixed_vector3_length(delta_x, delta_y, delta_z);
        vector_displacement_to_pitch_yaw(delta_x, delta_y, delta_z, &actor->tail_72.angles);
        if (actor->tail_72.angles.x >= 3585) {
            actor->tail_72.angles.x = 3584;
        } else if (actor->tail_72.angles.x > 512) {
            actor->tail_72.angles.x = 512;
        }
        if (distance >= target->word_16.value) {
            actor->state_70.signed_state = 0;
            actor->target_action_state = 0xf1;
        } else if (distance <= target->word_14.value) {
            actor->state_70.signed_state = 1;
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
        }
        if (actor->state_70.signed_state == 0) {
            actor_turn_and_move_along_euler_angles(&actor->tail_72.angles, target->word_0c.value,
                          target->word_0e.value,
                          group->movement_step,
                          group->turn_acceleration, 17);
        } else {
            opposite.x = -512;
            opposite.y = actor->tail_72.angles.y + 2048;
            opposite.z = 0;
            actor_turn_and_move_along_euler_angles(&opposite, target->word_0c.value,
                          target->word_0e.value,
                          group->movement_step,
                          group->turn_acceleration, 17);
        }
        if (actor->target_type == 17) {
            actor_update_motion_animation(target->animation_id, target->word_18.bytes.low,
                           target->word_18.bytes.high, target->word_1a.bytes[0],
                           target->word_1a.bytes[1], target->animation_step);
        } else {
            actor_advance_animation_wrapped(actor, target->animation_step);
        }
        break;
    }
    case 9:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor->state_70.signed_state = 0;
            actor_set_animation(target->word_14.bytes[0]);
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if (actor_move_along_heading(actor->rotation.y + 2048,
                              target->word_12.value,
                              group->movement_step, 5) == 0) {
                actor_advance_animation_wrapped(actor, target->animation_step);
                if (fixed_vector2_length(
                        player_state.camera_position.vx - actor->position.vx,
                        player_state.camera_position.vz - actor->position.vz)
                    < target->word_0e.value) {
                    break;
                }
            }
            actor->state_70.signed_state = 1;
            actor_set_animation(target->animation_id);
            break;
        case 1:
            actor_advance_animation_clamped(actor, target->word_16.bytes.low);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                if ((s16)actor_start_ballistic_motion(
                        1, player_state.camera_position.vx,
                        player_state.camera_position.vy - 500,
                        player_state.camera_position.vz,
                        target->word_16.bytes.high,
                        target->word_10.value) <= 0) {
                    actor->state_70.signed_state = 3;
                    actor_set_animation(target->word_14.bytes[1]);
                    break;
                }
                actor->state_70.signed_state = 2;
            }
            actor_damp_horizontal_motion(group->movement_step * 2, 10);
            break;
        case 2:
            if (actor->vertical_motion_state == 0) {
                actor->state_70.signed_state = 3;
                actor_set_animation(target->word_14.bytes[1]);
            } else {
                actor_turn_and_move_along_heading(actor->rotation.y,
                              actor->ballistic_horizontal_speed, 0,
                              actor->ballistic_horizontal_speed,
                              group->turn_acceleration, 10);
            }
            break;
        case 3:
            actor_advance_animation_clamped(actor, target->word_16.bytes.low);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_reset_target_and_reselect();
            }
            actor_damp_horizontal_motion(actor->ballistic_horizontal_speed >> 4, 10);
            break;
        }
        break;
    case 10: {
        s32 random_value;

        if (actor->target_action_state == 0) {
            actor->target_action_state = 0xf1;
            actor_set_animation_if_changed(target->animation_id);
            actor_suspend_vertical_motion();
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->motion.vector.vx += target->word_10.value;
            if (actor->motion.vector.vx > target->word_0c.value) {
                actor->motion.vector.vx = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->motion.vector.vx -= target->word_10.value;
            if (actor->motion.vector.vx < -(s32)target->word_0c.value) {
                actor->motion.vector.vx = -target->word_0c.value;
            }
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->motion.vector.vz += target->word_10.value;
            if (actor->motion.vector.vz > target->word_0c.value) {
                actor->motion.vector.vz = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->motion.vector.vz -= target->word_10.value;
            if (actor->motion.vector.vz < -(s32)target->word_0c.value) {
                actor->motion.vector.vz = -target->word_0c.value;
            }
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->motion.vector.vy += target->word_10.value;
            if (actor->motion.vector.vy > target->word_0c.value) {
                actor->motion.vector.vy = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->motion.vector.vy -= target->word_10.value;
            if (actor->motion.vector.vy < -(s32)target->word_0c.value) {
                actor->motion.vector.vy = -target->word_0c.value;
            }
        }
        actor_move_with_collision(&actor->motion.vector);
        actor_advance_animation_wrapped(actor, target->animation_step);
        actor_turn_toward_angle(actor, vector_xz_to_angle(actor->motion.vector.vx,
                                                actor->motion.vector.vz),
                        target->word_0e.value,
                        group->turn_acceleration);
        break;
    }
    case 4:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
        }
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.attack.damage_component3);
        }
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(target->word_0c.bytes.high, 10);
        break;
    case 23:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor->state_70.signed_state = target->word_18.value;
            actor_set_animation(target->animation_id);
        }
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor_animation_crossed_phase(actor, actor->state_70.signed_state)) {
            actor->state_70.signed_state = (u16)actor->state_70.signed_state + target->repeated_attack_phase_step;
            if (target->word_26.unsigned_value < actor->state_70.signed_state) {
                actor->state_70.signed_state = 0;
            }
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.attack.damage_component3 | 0x80);
        }
        if (target->secondary_hit_phase != 0 &&
            actor_animation_crossed_phase(actor, target->secondary_hit_phase)) {
            actor_try_damage_player_in_cone(0, target->word_1c.bytes[0],
                           target->word_1c.bytes[1],
                           target->word_1e.bytes[0], target->word_20.damage_component0,
                           target->word_22.damage_component1, target->word_24.unsigned_value,
                           target->word_1e.bytes[1]);
        }
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(target->word_0c.bytes.high, 10);
        break;
    case 24: {
        s32 speed;
        s32 step;
        s32 angle;
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
        }
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor->animation_phase >= target->word_22.animation_phase_stop) {
            speed = 0;
            step = target->word_26.signed_value;
        } else if (actor->animation_phase >= target->word_20.animation_phase_start) {
            speed = target->word_1c.value;
            step = target->word_24.signed_value;
        } else {
            speed = target->word_1c.value;
            step = target->word_0c.bytes.high;
        }
        angle = vector_xz_to_angle(
            player_state.camera_position.vx - actor->position.vx,
            player_state.camera_position.vz - actor->position.vz);
        actor->movement_yaw = angle;
        actor_turn_and_move_along_heading(actor->movement_yaw, speed,
                      target->word_1e.value, step,
                      group->turn_acceleration, 4);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.attack.damage_component3);
        }
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 11: {
        s32 collision;
        struct KfEulerAngles toward_player;
        SVECTOR forward;
        SVECTOR outer;

        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation_if_changed(target->animation_id);
            actor->state_70.signed_state = 0;
            actor_suspend_vertical_motion();
        }
        switch (actor->state_70.signed_state) {
        case 0:
            actor_advance_animation_clamped(actor, target->animation_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                vector_displacement_to_pitch_yaw(
                    player_state.camera_position.vx - actor->position.vx,
                    player_state.camera_position.vy - actor->position.vy,
                    player_state.camera_position.vz - actor->position.vz,
                    &toward_player);
                pitch_yaw_to_forward_vector(&toward_player,
                                            &actor->tail_72.direction);
                actor->state_70.signed_state = 1;
            }
            break;
        case 1:
            forward = actor->tail_72.direction;
            outer = forward;
            vector3s_scale_shift12(target->word_14.value, &forward);
            vector3s_scale_shift12(target->word_18.value, &outer);
            actor->motion.vector.vx = value_approach(actor->motion.vector.vx,
                                                forward.vx, outer.vx);
            actor->motion.vector.vy = value_approach(actor->motion.vector.vy,
                                                forward.vy, outer.vy);
            actor->motion.vector.vz = value_approach(actor->motion.vector.vz,
                                                forward.vz, outer.vz);
            collision = actor_move_with_collision(&actor->motion.vector);
            if (collision != 0) {
                if (collision & KF_COLLISION_HIT_PLAYER) {
                    player_apply_damage(target->word_0e.value,
                                   target->word_10.value,
                                   target->word_12.value,
                                   target->word_0c.bytes.high,
                                   0, 0, 0, 0, 0, 0x1000, 10,
                                   &actor->position);
                }
                actor->state_70.signed_state = 2;
            }
            actor->rotation.x = (actor->rotation.x + 256) & KF_ANGLE_WRAP_MASK;
            goto case11_turn;
        case 2: {
            u32 pitch_phase = ((u16)actor->rotation.x + 256) & KF_ANGLE_WRAP_MASK;
            actor->rotation.x = pitch_phase;
            if (pitch_phase < 256) {
                actor->rotation.x = 0;
                actor->state_70.signed_state = 3;
            }
            break;
        }
        case 3:
            actor_advance_animation_clamped(actor, -target->animation_step);
            if (actor->animation_phase == 0) {
                actor_reset_target_and_reselect();
            }
            break;
        default:
            goto case11_turn;
        }
        actor_move_with_collision(&actor->motion.vector);
    case11_turn:
        actor_turn_toward_angle(actor, vector_xz_to_angle(actor->motion.vector.vx,
                                                actor->motion.vector.vz),
                        target->word_16.value,
                        group->turn_acceleration);
        break;
    }
    case 18: {
        s32 delta_x;
        s32 delta_y;
        s32 delta_z;

        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation_if_changed(target->animation_id);
        }
        delta_x = player_state.camera_position.vx - actor->position.vx;
        delta_y = player_state.camera_position.vy - actor->position.vy - 1600;
        delta_z = player_state.camera_position.vz - actor->position.vz;
        fixed_vector3_length(delta_x, delta_y, delta_z);
        vector_displacement_to_pitch_yaw(delta_x, delta_y, delta_z, &actor->tail_72.angles);
        actor_turn_and_move_along_euler_angles(&actor->tail_72.angles, target->word_1c.value,
                      target->word_1e.value,
                      target->word_0c.bytes.high,
                      group->turn_acceleration, 6);
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.attack.damage_component3);
        }
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 14: {
        KfActor *other = actor_state.other_actor;
        if (actor->target_action_state == 0) {
            actor->target_action_state = 0xf1;
            actor->state_70.signed_state = 0;
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if ((s16)other->turn_rate > 0) {
                actor_set_animation(target->word_0c.bytes.high);
                actor->state_70.signed_state = 1;
            } else if ((s16)other->turn_rate < 0) {
                actor_set_animation(target->word_0c.bytes.low);
                actor->state_70.signed_state = 2;
            }
            break;
        case 1:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)other->turn_rate <= 0) {
                actor->state_70.signed_state = 3;
            }
            break;
        case 2:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)other->turn_rate >= 0) {
                actor->state_70.signed_state = 3;
            }
            break;
        case 3:
            actor->animation_phase -= 128;
            if (actor->animation_phase == 0) {
                actor->state_70.signed_state = 0;
            }
            break;
        }
        break;
    }
    case 15:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation_if_changed(target->animation_id);
        }
        actor_turn_and_move_along_heading(actor->movement_yaw,
                      target->word_0c.value, target->word_0e.value,
                      group->movement_step,
                      group->turn_acceleration, 5);
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
            actor_reset_target_and_reselect();
        }
        break;
    case 19:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor->state_70.signed_state = 0;
            actor_set_animation_if_changed(target->animation_id);
        }
        switch (actor->state_70.signed_state) {
        case 0:
            actor_advance_animation_clamped(actor, target->animation_step);
            if (actor->animation_phase >= target->word_10.value) {
                actor->state_70.signed_state = 16;
            }
            break;
        case 16:
            if (player_state.weapon_attack_phase == -1) {
                actor->state_70.signed_state = 32;
            }
            break;
        case 32:
        case19_clamped:
            actor_advance_animation_clamped(actor, target->animation_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_reset_target_and_reselect();
            }
            break;
        }
        actor_damp_horizontal_motion(group->movement_step, 10);
        break;
    case 20:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor->state_70.signed_state = target->word_12.value;
            actor_set_animation_if_changed(target->animation_id);
        }
        if (actor_move_along_heading(actor->rotation.y + 2048,
                          target->word_10.value, 1000, 5) != 0) {
            actor->state_70.signed_state = 1;
        }
        actor_advance_animation_wrapped(actor, target->animation_step);
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX - target->animation_step) {
            actor->state_70.signed_state--;
            if (actor->state_70.signed_state == 0) {
                actor->animation_phase = KF_ACTOR_ANIMATION_PHASE_MAX;
                actor_reset_target_and_reselect();
            }
        }
        break;
    case 22:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
        }
        goto case19_clamped;
    case 21:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor->state_70.signed_state = 0;
            actor_set_animation(target->animation_id);
            actor->collision_radius = target->word_14.value;
            actor->collision_height = target->word_16.value;
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if (fixed_vector2_length(player_state.camera_position.vx - actor->position.vx,
                                     player_state.camera_position.vz - actor->position.vz)
                < target->word_0c.value) {
                actor->state_70.signed_state = 1;
            }
            break;
        case 1:
            actor_advance_animation_clamped(actor, target->animation_step);
            actor->collision_radius = fixed_lerp_q12(target->word_14.value,
                group->collision_radius, actor->animation_phase);
            actor->collision_height = fixed_lerp_q12(target->word_16.value,
                group->collision_height, actor->animation_phase);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor->collision_radius = group->collision_radius;
                actor->collision_height = group->collision_height;
                actor->state_70.signed_state = 2;
                actor->tail_72.signed_state = target->word_10.value;
                actor_set_animation(target->word_18.bytes.low);
            }
            if (actor->animation_phase >= target->word_12.value) {
                s32 saved_flags = actor_state.actor_collision_query_flags;
                actor_state.actor_collision_query_flags &= ~KF_COLLISION_QUERY_MAP_OBJECTS;
                actor_turn_and_move_along_heading(actor->movement_yaw,
                               target->word_0e.value, 0, 0xff, 0, 5);
                actor_state.actor_collision_query_flags = saved_flags;
            }
            break;
        case 2: {
            s32 saved_flags = actor_state.actor_collision_query_flags;
            s32 next_timer;
            actor_state.actor_collision_query_flags &= ~KF_COLLISION_QUERY_MAP_OBJECTS;
            actor_turn_and_move_along_heading(actor->movement_yaw,
                           target->word_0e.value, 0, 0xff, 0, 5);
            next_timer = actor->tail_72.unsigned_state - 1;
            actor->tail_72.unsigned_state = next_timer;
            if ((s16)next_timer == -1) {
                actor_reset_target_and_reselect();
            }
            actor_state.actor_collision_query_flags = saved_flags;
            actor_advance_animation_wrapped(actor, target->word_18.bytes.high);
            break;
        }
        }
        break;
    case 26:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor->state_70.signed_state = 0;
            actor_set_animation(target->animation_id);
            actor->model_scale_z = 0;
            actor->model_scale_y.value = 0;
            actor->model_scale_x = 0;
        }
        switch (actor->state_70.signed_state) {
        case 0: {
            KfMapObject *object = &map_object_state.objects[actor->word_20.linked_map_object_slot];
            u16 group_scale;

            if (object->action_timer < 2 || object->rotation.vx == 0 ||
                object->rotation.vx >= 3072) {
                break;
            }
            actor->state_70.signed_state = 1;
            actor->tail_72.signed_state = 8;
            actor->collision_radius = group->collision_radius;
            actor->collision_height = group->collision_height;
            actor->position.vy += 2048;
            group_scale = group->initial_model_scale_q12;
            actor->model_scale_z = group_scale;
            actor->model_scale_y.value = group_scale;
            actor->model_scale_x = group_scale;
            actor->rotation.y = object->rotation.vy;
        }
            /* fall through */
        case 1:
            actor->vertical_motion_state = 0;
            goto case29_shared_motion;
        }
        break;
    case 25: {
        const u16 *cursor;
        s32 repeat;

        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
            actor->state_70.signed_state = target->word_0e.value;
            actor->tail_72.script.word_index = 0;
            actor->tail_72.script.effect_cycle_index = 0;
            if (target->start_vertical_motion_on_entry == 1) {
                actor->vertical_motion_state = 16;
            }
        }
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor_animation_crossed_phase(actor,
                                          actor->state_70.signed_state)) {
            actor->state_70.signed_state =
                (u16)actor->state_70.signed_state + target->word_12.value;
            repeat = 1;
            if (target->word_10.value < actor->state_70.signed_state) {
                actor->state_70.signed_state = 0;
            }
            cursor = ((const KfTargetCandidateAction25 *)target)->stream +
                (s16)actor->tail_72.script.word_index;
            for (;;) {
                s32 opcode = *cursor++;
                u16 index = actor->tail_72.script.word_index;
                actor->tail_72.script.word_index = index + 1;

                if (opcode == 0x8000) {
                    cursor = ((const KfTargetCandidateAction25 *)target)->stream;
                    actor->tail_72.script.word_index = 0;
                    continue;
                } else if (opcode == 0x8001) {
                    repeat = *cursor++;
                    actor->tail_72.script.word_index = index + 2;
                    continue;
                } else if (opcode == 0x8003) {
                    u16 skip = *cursor;
                    cursor += skip + 1;
                    continue;
                } else if (opcode == 0x8002) {
                    s16 x;
                    s16 y;
                    s16 z;

                    x = *cursor++;
                    actor->tail_72.script.word_index = index + 2;
                    y = *cursor++;
                    actor->tail_72.script.word_index = index + 3;
                    z = *cursor++;
                    actor->tail_72.script.word_index = index + 4;
                    actor_dispatch_group_effect(target->word_0c.bytes.low,
                                   target->word_18.value, -1, x, y, z, cursor);
                } else if (opcode == 0x8004) {
                    u16 first = *cursor++;
                    u16 second = *cursor++;
                    u16 third = *cursor++;
                    actor->tail_72.script.word_index = index + 4;
                    actor_dispatch_group_effect(target->word_0c.bytes.low,
                                   target->word_18.value, -2,
                                   first, second, third);
                } else {
                    actor_dispatch_group_effect(target->word_0c.bytes.low,
                                   target->word_18.value, opcode,
                                   cursor + repeat - 1,
                                   (s16)actor->tail_72.script.effect_cycle_index);
                }
                if (--repeat == 0) {
                    break;
                }
            }
            actor->tail_72.script.effect_cycle_index++;
        }
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(group->movement_step, 10);
        break;
    }
    case 27:
        if (actor->target_action_state == 0) {
            actor->target_action_state = 0xf1;
            actor->state_70.signed_state = 0;
            actor->tail_72.signed_state = -1;
            actor_set_animation(target->word_16.bytes.low);
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if (target->word_0e.value == 0) {
                actor->state_70.signed_state = 1;
            } else {
                actor->tail_72.signed_state = actor_turn_and_move_toward_point(
                    (actor->home_cell_x << 11) + actor->word_24.home_local_x,
                    (actor->home_cell_z << 11) + actor->word_22.home_local_z,
                    target->word_0e.value, target->word_10.value,
                    actor->tail_72.signed_state,
                    group->movement_step,
                    group->turn_acceleration, 5);
                if (actor->tail_72.signed_state == -1) {
                    actor->state_70.signed_state = 1;
                }
            }
            actor_advance_animation_wrapped(actor, target->word_16.bytes.high);
            break;
        case 1:
            actor_turn_toward_angle(actor, actor->word_20.home_yaw, target->word_10.value,
                          group->turn_acceleration);
            if (actor->rotation.y == actor->word_20.home_yaw) {
                actor_set_animation(target->animation_id);
                actor->state_70.signed_state = 2;
            }
            break;
        case 2: {
            KfTargetCandidate *next_target;
            actor_advance_animation_clamped(actor, target->animation_step);
            actor->collision_radius = fixed_lerp_q12(
                group->collision_radius,
                target->word_12.value, actor->animation_phase);
            actor->collision_height = fixed_lerp_q12(
                group->collision_height,
                target->word_14.value, actor->animation_phase);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor->collision_radius = target->word_12.value;
                actor->collision_height = target->word_14.value;
                next_target = actor_find_target_of_type(group, 21);
                if (next_target != 0) {
                    actor_set_target(actor, next_target);
                } else {
                    actor_reset_target_and_reselect();
                }
            }
            break;
        }
        }
        break;
    case 29:
        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
            actor_suspend_vertical_motion();
            actor->tail_72.signed_state = 16;
        }
        actor->rotation.x += 128;
        actor->rotation.y += 64;
    case29_shared_motion: {
        s32 next_y = actor->position.vy - 256;
        s32 next_timer = actor->tail_72.unsigned_state - 1;

        actor->position.vy = next_y;
        actor->tail_72.unsigned_state = next_timer;
        if ((s16)next_timer == 0) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 28: {
        s32 collision;

        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
            actor_suspend_vertical_motion();
            audio_play_spatial_default_range(0x1b, &actor->position, 120, 0);
        }
        actor->rotation.x += 128;
        actor->rotation.y += 64;
        collision = actor_move_with_collision(&actor->motion.vector);
        if (collision != 0) {
            if (collision & KF_COLLISION_HIT_PLAYER) {
                player_apply_damage(target->word_0e.value, target->word_10.value,
                               target->word_12.value, target->word_0c.value,
                               target->word_14.value, target->word_16.value,
                               target->word_18.value, target->word_1a.value,
                               target->word_1c.value, 0x1000, 10, &actor->position);
            }
            actor_select_target_type_in_own_group(actor, 3);
            actor->motion.vector.vz = 0;
            actor->motion.vector.vy = 0;
            actor->motion.vector.vx = 0;
            actor->vertical_motion_state = 16;
        }
        break;
    }
    case 30: {
        VECTOR next;
        s32 collision;

        if (actor->target_action_state == 0) {
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
            actor_set_animation(target->animation_id);
            actor_suspend_vertical_motion();
        }
        next.vx = actor->position.vx + actor->motion.vector.vx;
        next.vy = actor->position.vy + actor->motion.vector.vy;
        next.vz = actor->position.vz + actor->motion.vector.vz;
        collision = collision_query_world(
            next.vx, next.vy, next.vz, actor->collision_radius,
            actor->collision_height | ((actor->unknown_28 & 0xc000) << 16),
            actor_state.actor_collision_query_flags);
        if (collision == 0) {
        case30_position:
            actor->position.vx = next.vx;
            actor->position.vy = next.vy;
            actor->position.vz = next.vz;
            goto case30_advance;
        } else if (collision & KF_COLLISION_HIT_PLAYER) {
            player_apply_damage(target->word_0e.value, target->word_10.value,
                           target->word_12.value, target->word_0c.value,
                           target->word_14.value, target->word_16.value,
                           target->word_18.value, target->word_1a.value,
                           target->word_1c.value, 0x1000, 10, &actor->position);
            goto case30_position;
        } else {
            actor->motion.vector.vz = 0;
            actor->motion.vector.vy = 0;
            actor->motion.vector.vx = 0;
        }
    case30_advance:
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
            actor_select_target_type_in_own_group(actor, 3);
            actor->vertical_motion_state = 16;
        }
        break;
    }
    case 240:
        actor_suspend_vertical_motion();
        break;
    case 6:
    case 7:
    case 8:
    default:
        state_8017d118.active_table[17]();
        break;
    }

    if (actor->unknown_28 & KF_ACTOR_FLAG_LINKED) {
        KfActor *other = actor_state.other_actor;
        u8 slot_state = actor->slot_state;

        if (slot_state == KF_ACTOR_SLOT_HOMEBOUND) {
            actor->current_map_layer = 0;
            if (other->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE) {
                actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
                goto behavior_done;
            }
            if (other->target_type == slot_state) {
                actor->target_type = 3;
                actor->target_action_state = KF_ACTOR_TARGET_ACTION_RETARGET_BLOCKED;
                actor->state_70.signed_state = 99;
                goto behavior_done;
            }
        } else {
            s32 collision;

            actor->current_map_layer = other->current_map_layer;
            if (other->lifecycle != KF_ACTOR_LIFECYCLE_ACTIVE) {
                actor->unknown_28 = (actor->unknown_28 & ~KF_ACTOR_FLAG_LINKED) |
                    KF_ACTOR_FLAG_BLOCK_PLAYER_TARGETING;
                collision = collision_query_world(
                    actor->position.vx, actor->position.vy,
                    actor->position.vz, actor->collision_radius,
                    actor->collision_height, KF_COLLISION_QUERY_SHAPES | KF_COLLISION_QUERY_PLAYER);
                if (collision & KF_COLLISION_HIT_PLAYER) {
                    actor->position.vx += 800 + actor->collision_radius;
                }
                if (collision & 0xf) {
                    actor->unknown_28 |= KF_ACTOR_FLAG_USE_MAP_LAYER_FLOOR;
                }
                if (actor->unknown_28 & 0x200) {
                    actor_select_target_type_in_own_group(actor, 3);
                }
                actor->motion.vector.vx = other->motion.vector.vx;
                actor->motion.vector.vy = other->motion.vector.vy;
                actor->motion.vector.vz = other->motion.vector.vz;
                goto behavior_done;
            }
        }

        {
            struct KfEulerAngles rotation;
            VECTOR group_offset;
            VECTOR vertex_offset;

            /* Copy the two trailing orientation bytes with the three angles. */
            *(KfActorOrientation *)&actor->rotation =
                *(const KfActorOrientation *)&other->rotation;
            rotation.x = actor->rotation.x;
            rotation.y = actor->rotation.y;
            rotation.z = actor->rotation.z;
            vector_rotate_yxz(&rotation, (SVECTOR *)&group->position_offset_x,
                              &group_offset);
            actor_sample_rotated_animation_vertex(other, actor->word_24.linked_animation_vertex_index, &vertex_offset);
            actor->position.vx = other->position.vx + vertex_offset.vx -
                                 group_offset.vx;
            actor->position.vy = other->position.vy + vertex_offset.vy -
                                 group_offset.vy;
            actor->position.vz = other->position.vz + vertex_offset.vz -
                                 group_offset.vz;
        }
    } else if ((actor->unknown_28 & KF_ACTOR_FLAG_MAP_OBJECT_ATTACHED) == 0) {
        actor_update_vertical_motion();
    }

behavior_done:
    if (actor->lifecycle == KF_ACTOR_LIFECYCLE_ACTIVE) {
        map_cell_add_layer_occupancy(actor->position.vx, actor->position.vz,
                       actor->collision_radius, 1);
    }
}

ADDRESS(0x8003f610, 0x1dc)
void actor_update_frame(void)
{
    KfActor *actor;

    actor_state.active_actor_count = 0;
    actor = actor_state.actors;
    actor_state.current_actor_slot_index = 0;
    do {
        if (actor->slot_state != KF_ACTOR_SLOT_FREE) {
            actor_bind_current(actor);
            if (((actor_state.actor_update_frame_count & 3) ==
                 (actor_state.current_actor_slot_index & 3)) ||
                player_state.force_actor_lifecycle_refresh != 0 ||
                player_state.death_state == 1) {
                actor_update_lifecycle_for_player_range();
            }

            if (actor->lifecycle == KF_ACTOR_LIFECYCLE_ACTIVE) {
                if ((actor_state.actor_update_frame_count & 3) ==
                    (actor_state.active_actor_count & 3)) {
                    actor_select_target_for_player_distance();
                }
                actor_update_behavior();
                actor_state.active_actor_count++;
            }

            if ((actor_state.active_group->initial_actor_flags &
                 KF_ACTOR_FLAG_MAP_OBJECT_ATTACHED) != 0) {
                KfMapObject *object =
                    &map_object_state.objects[actor->word_22.linked_map_object_slot];
                s32 object_z;

                actor->position.vx = object->position.vx;
                actor->position.vy = object->position.vy - 500;
                object_z = object->position.vz;
                actor->position.vz = object_z;
                if (actor->lifecycle == KF_ACTOR_LIFECYCLE_ACTIVE) {
                    actor->rotation.y = vector_xz_to_angle(
                        player_state.camera_position.vx - actor->position.vx,
                        player_state.camera_position.vz - object_z);
                }
            }
        }
        actor++;
        actor_state.current_actor_slot_index++;
    } while (actor_state.current_actor_slot_index < KF_ACTOR_CAPACITY);

    actor_state.actor_update_frame_count++;
    actor_bind_current(NULL);
}

ADDRESS(0x8003f7ec, 0x74)
void actor_fixup_group_targets(void)
{
    KfTargetGroup *group = actor_state.target_groups;
    KfTargetCandidate *base;
    s32 group_index;
    s32 slot_index;
    KfTargetReference *slot;

    group_index = 0;
    base = (KfTargetCandidate *)actor_state.unknown_73a0;
    while (group_index < 40) {
        if (group->definition_id == 0xff) {
            break;
        }
        slot = group->targets;
        for (slot_index = 0; slot_index < 16; slot_index++, slot++) {
            if (slot->relative_offset == -1) {
                slot->pointer = NULL;
            } else {
                slot->pointer = (KfTargetCandidate *)((u8 *)base + slot->relative_offset);
            }
        }
        group_index++;
        group++;
    }
}

/* The archive loader at 0x80016820 advances across 16-byte records. */
typedef struct KfActorLoadRecord {
    u8 slot_state;
    u8 group_index;
    u8 placement_flags;
    u8 cell_z;
    u8 cell_x;
    u8 spawn_chance;
    u8 death_drop_object_id;
    u8 home_map_layer;
    u16 initial_actor_word_20;
    u16 initial_actor_word_22;
    u16 initial_actor_word_24;
    u16 vertical_anchor_offset;
} KfActorLoadRecord;
typedef char kf_actor_load_record_size[sizeof(KfActorLoadRecord) == 16 ? 1 : -1];

ADDRESS(0x8003f860, 0x1cc)
void actor_load_records(const KfActorLoadRecord *records)
{
    KfActor *actor = actor_state.actors;
    u16 remaining = KF_ACTOR_CAPACITY - 1;

    do {
        actor->slot_state = records->slot_state;
        if (actor->slot_state != KF_ACTOR_SLOT_FREE) {
            const KfTargetGroup *group;

            actor->group_index = records->group_index;
            actor->unknown_04 = 0;
            actor->placement_flags = records->placement_flags;
            actor->home_map_layer = records->home_map_layer;
            actor->home_cell_z = records->cell_z;
            actor->home_cell_x = records->cell_x;
            actor->spawn_chance = records->spawn_chance;
            actor->death_drop_object_id = records->death_drop_object_id;
            actor->word_20.value = records->initial_actor_word_20;
            actor->word_22.value = records->initial_actor_word_22;
            actor->word_24.value = records->initial_actor_word_24;
            actor->vertical_anchor_offset = records->vertical_anchor_offset;
            actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
            actor->target_type = 0;
            actor->target_action_state = KF_ACTOR_TARGET_ACTION_UNSELECTED;
            actor->target = NULL;

            group = &actor_state.target_groups[actor->group_index];
            actor_copy_group_defaults(actor);
            actor_set_home_position(actor);
            actor->render_depth = group->render_depth;
            if ((actor->unknown_28 & KF_ACTOR_FLAG_LINKED) != 0) {
                if (actor->slot_state == KF_ACTOR_SLOT_HOMEBOUND) {
                    if (actor->word_24.value == -1) {
                        actor->word_24.value = group->word_1a.slot3_home_x_fallback;
                    }
                    if (actor->vertical_anchor_offset == -1) {
                        actor->vertical_anchor_offset = group->default_vertical_anchor_offset;
                    }
                } else {
                    actor->slot_state = 4;
                    actor->vertical_anchor_offset = 0;
                }
            }
        } else {
            actor->lifecycle = KF_ACTOR_LIFECYCLE_DORMANT;
        }
        records++;
        actor++;
    } while (remaining-- != 0);
}

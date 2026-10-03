#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

RODATA(0x800120d8, 0x3c4)

ADDRESS(0x8003d184, 0x248c)
void actor_update_behavior(void)
{
    KfActor *actor = actor_state.current;
    KfTargetGroup *group = actor_state.active_group;
    KfTargetCandidate *target = actor->target;
    u16 trigger;
    s32 interval;

    if ((actor->unknown_28 & 4) != 0) {
        actor_state.unknown_93a4 = 3;
    } else {
        actor_state.unknown_93a4 = 0x93;
    }
    map_cell_add_layer_occupancy(actor->position.vx, actor->position.vz,
                   actor->unknown_1c, -1);

    if (target->sound_code != 0xff) {
        trigger = target->sound_trigger;
        interval = trigger & 0x3fff;
        switch (trigger & 0xc000) {
        case 0:
            if (actor_animation_crossed_phase(actor, interval)) {
                goto play_sound;
            }
            break;
        case 0x8000:
            if ((rand() >> 3) < interval) {
                goto play_sound;
            }
            break;
        case 0x4000:
            if ((interval * actor_state.unknown_93b8 / 3) % interval ==
                (s32)actor_state.unknown_93c4 % interval) {
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
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
        }
        if (actor->animation_phase < 0x800 ||
            (actor->unknown_28 & 0x800) == 0) {
            actor_advance_animation_clamped(actor, target->animation_step);
        }
        if (actor->animation_phase > 0xffe) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(group->unknown_01[2] * 2, 10);
        break;
    case 3: {
        s32 old_state = actor->state_70.signed_state;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
            actor->state_70.signed_state = 0;
            actor->unknown_28 &= ~0x10000;
        }
        if (actor->state_70.signed_state == 0) {
            if (actor->animation_phase >= 0x400 &&
                (actor->unknown_28 & 0x800) != 0) {
                break;
            }
            actor_advance_animation_clamped(actor, target->animation_step);
            if (actor_animation_crossed_phase(actor, 0x800)) {
                u16 effect_id = group->unknown_30 +
                    random_centered_triangular_scaled(group->unknown_30);

                if (effect_id != 0) {
                    map_object_spawn_scattered_effect(effect_id, &actor->position,
                                   -(actor->unknown_1e >> 1));
                }
                if (actor->slot_state == 0 || actor->slot_state == 4) {
                    if (target->word_0c.bytes.low != 0xff &&
                        (rand() >> 7) < target->word_0c.bytes.high) {
                        map_object_spawn_effect(
                            1, target->word_0c.bytes.low, &actor->position,
                            -(actor->unknown_1e >> 1));
                    }
                } else if (actor->slot_state == 1 &&
                           actor->unknown_0a[1] != 0xff) {
                    map_object_spawn_effect(
                        0, actor->unknown_0a[1], &actor->position,
                        -(actor->unknown_1e >> 1));
                }
            }
            if (actor->animation_phase >= 0xfff || target->animation_step == 0) {
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
                actor->unknown_14 = 0x42;
                actor->unknown_16 = 0x400;
                actor->unknown_13 = 1;
            } else if (old_state < 38) {
                if (actor->unknown_16 < 0x1000) {
                    actor->unknown_16 += 192;
                } else {
                    actor->unknown_16 = 0x1000;
                }
            } else {
                state_8017d118.active_table[19](actor);
                if (actor->slot_state == 1) {
                    actor_set_lifecycle_and_home_position(actor);
                } else if (actor->slot_state == 2) {
                    actor->lifecycle = 0;
                    actor_set_home_position(actor);
                } else if (actor->slot_state == 5) {
                    actor->slot_state = 0xff;
                    actor->lifecycle = 0;
                } else {
                    actor->lifecycle = 2;
                    actor_set_home_position(actor);
                }
            }
        }
case3_motion:
        actor_damp_horizontal_motion(group->unknown_01[2] * 2, 10);
        break;
    }
    case 0:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation_if_changed(target->unknown_01[0]);
        }
        actor_advance_animation_wrapped(actor, target->animation_step);
        if (angle_within_tolerance(actor->animation_phase, 0,
                                   target->animation_step - 1)) {
            actor->unknown_0f = 0xf1;
        }
        actor_damp_horizontal_motion(group->unknown_01[2], 10);
        break;
    case 1:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor_set_animation_if_changed(target->unknown_01[0]);
            actor->unknown_64 = rand() >> 3;
        } else if (actor_turn_and_move_along_heading(actor->unknown_64,
                                  target->word_0c.value, target->word_0e.value,
                                  group->unknown_01[2],
                                  group->unknown_01[3], 5) != 0 ||
                   (rand() >> 5) < target->word_10.bytes.fallback_offset) {
            actor->unknown_64 = rand() >> 3;
        }
        actor_advance_animation_wrapped(actor, target->animation_step);
        break;
    case 12:
    case 16: {
        s32 motion_flags;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor->tail_72.angles.z = 0;
            actor->tail_72.angles.y = 0;
            actor->tail_72.angles.x = 0;
            actor->tail_72.motion.baseline = (u16)actor->unknown_26 +
                collision_sample_map_layer_height(actor->unknown_06,
                    (actor->unknown_07[1] << 11) + actor->unknown_24,
                    (actor->unknown_07[0] << 11) + actor->unknown_22,
                    actor->unknown_1c, actor->unknown_1e);
            actor_set_animation_if_changed(target->unknown_01[0]);
            actor_suspend_vertical_motion();
            motion_flags = 3;
        } else {
            motion_flags = actor_turn_and_move_along_euler_angles(
                &actor->tail_72.angles, target->word_0c.value,
                target->word_0e.value,
                group->unknown_01[2],
                group->unknown_01[3], 17);
            if ((rand() >> 5) < target->word_12.bytes.marker_state) {
                motion_flags |= 1;
            }
            if ((rand() >> 5) < target->word_12.bytes.marker_state) {
                motion_flags |= 2;
            }
            if (actor->tail_72.motion.baseline + target->word_10.value <
                actor->position.vy) {
                actor->unknown_52 -= target->word_12.bytes.unknown_12;
            } else if (actor->position.vy <
                       actor->tail_72.motion.baseline - target->word_10.value) {
                actor->unknown_52 += target->word_12.bytes.unknown_12;
            }
        }
        if (motion_flags & 1) {
            actor->tail_72.angles.y = rand() >> 3;
        }
        if (motion_flags & 2) {
            actor->tail_72.angles.x = (rand() >> 5) - 512;
        }
        if (actor->target_type == 16) {
            actor_update_motion_animation(target->unknown_01[0], target->word_14.bytes[0],
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

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor_set_animation_if_changed(target->unknown_01[0]);
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
        actor->unknown_64 = angle;
        mode = actor->state_70.bytes.high == 0 ? 5 : 33;
        if (actor->state_70.bytes.low == 0) {
            motion_flags = actor_turn_and_move_along_heading((s16)angle,
                target->word_0c.value, target->word_0e.value,
                group->unknown_01[2],
                group->unknown_01[3], mode);
        } else {
            motion_flags = actor_move_along_heading((s16)angle + 2048,
                target->word_0c.value,
                group->unknown_01[2], mode);
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

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation_if_changed(target->unknown_01[0]);
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
            actor->unknown_0f = 0xf1;
        } else if (distance <= target->word_14.value) {
            actor->state_70.signed_state = 1;
            actor->unknown_0f = 0xf0;
        }
        if (actor->state_70.signed_state == 0) {
            actor_turn_and_move_along_euler_angles(&actor->tail_72.angles, target->word_0c.value,
                          target->word_0e.value,
                          group->unknown_01[2],
                          group->unknown_01[3], 17);
        } else {
            opposite.x = -512;
            opposite.y = actor->tail_72.angles.y + 2048;
            opposite.z = 0;
            actor_turn_and_move_along_euler_angles(&opposite, target->word_0c.value,
                          target->word_0e.value,
                          group->unknown_01[2],
                          group->unknown_01[3], 17);
        }
        if (actor->target_type == 17) {
            actor_update_motion_animation(target->unknown_01[0], target->word_18.bytes.low,
                           target->word_18.bytes.high, target->word_1a.bytes[0],
                           target->word_1a.bytes[1], target->animation_step);
        } else {
            actor_advance_animation_wrapped(actor, target->animation_step);
        }
        break;
    }
    case 9:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            actor_set_animation(target->word_14.bytes[0]);
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if (actor_move_along_heading(actor->rotation.y + 2048,
                              target->word_12.value,
                              group->unknown_01[2], 5) == 0) {
                actor_advance_animation_wrapped(actor, target->animation_step);
                if (fixed_vector2_length(
                        player_state.camera_position.vx - actor->position.vx,
                        player_state.camera_position.vz - actor->position.vz)
                    < target->word_0e.value) {
                    break;
                }
            }
            actor->state_70.signed_state = 1;
            actor_set_animation(target->unknown_01[0]);
            break;
        case 1:
            actor_advance_animation_clamped(actor, target->word_16.bytes.low);
            if (actor->animation_phase >= 0xfff) {
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
            actor_damp_horizontal_motion(group->unknown_01[2] * 2, 10);
            break;
        case 2:
            if (actor->unknown_0d == 0) {
                actor->state_70.signed_state = 3;
                actor_set_animation(target->word_14.bytes[1]);
            } else {
                actor_turn_and_move_along_heading(actor->rotation.y, actor->unknown_68, 0,
                              actor->unknown_68,
                              group->unknown_01[3], 10);
            }
            break;
        case 3:
            actor_advance_animation_clamped(actor, target->word_16.bytes.low);
            if (actor->animation_phase >= 0xfff) {
                actor_reset_target_and_reselect();
            }
            actor_damp_horizontal_motion(actor->unknown_68 >> 4, 10);
            break;
        }
        break;
    case 10: {
        s32 random_value;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor_set_animation_if_changed(target->unknown_01[0]);
            actor_suspend_vertical_motion();
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->unknown_50 += target->word_10.value;
            if (actor->unknown_50 > target->word_0c.value) {
                actor->unknown_50 = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->unknown_50 -= target->word_10.value;
            if (actor->unknown_50 < -(s32)target->word_0c.value) {
                actor->unknown_50 = -target->word_0c.value;
            }
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->unknown_54 += target->word_10.value;
            if (actor->unknown_54 > target->word_0c.value) {
                actor->unknown_54 = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->unknown_54 -= target->word_10.value;
            if (actor->unknown_54 < -(s32)target->word_0c.value) {
                actor->unknown_54 = -target->word_0c.value;
            }
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->unknown_52 += target->word_10.value;
            if (actor->unknown_52 > target->word_0c.value) {
                actor->unknown_52 = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->unknown_52 -= target->word_10.value;
            if (actor->unknown_52 < -(s32)target->word_0c.value) {
                actor->unknown_52 = -target->word_0c.value;
            }
        }
        actor_move_with_collision((SVECTOR *)&actor->unknown_50);
        actor_advance_animation_wrapped(actor, target->animation_step);
        actor_turn_toward_angle(actor, vector_xz_to_angle(actor->unknown_50,
                                                actor->unknown_54),
                        target->word_0e.value,
                        group->unknown_01[3]);
        break;
    }
    case 4:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(target->word_0c.bytes.high, 10);
        break;
    case 23:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = target->word_18.value;
            actor_set_animation(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor_animation_crossed_phase(actor, actor->state_70.signed_state)) {
            actor->state_70.signed_state = (u16)actor->state_70.signed_state + target->unknown_28;
            if (target->word_26.unsigned_value < actor->state_70.signed_state) {
                actor->state_70.signed_state = 0;
            }
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11 | 0x80);
        }
        if (target->unknown_2a != 0 &&
            actor_animation_crossed_phase(actor, target->unknown_2a)) {
            actor_try_damage_player_in_cone(0, target->word_1c.bytes[0],
                           target->word_1c.bytes[1],
                           target->word_1e.bytes[0], target->unknown_20,
                           target->unknown_22, target->word_24.unsigned_value,
                           target->word_1e.bytes[1]);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(target->word_0c.bytes.high, 10);
        break;
    case 24: {
        s32 speed;
        s32 step;
        s32 angle;
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor->animation_phase >= target->unknown_22) {
            speed = 0;
            step = target->word_26.signed_value;
        } else if (actor->animation_phase >= target->unknown_20) {
            speed = target->word_1c.value;
            step = target->word_24.signed_value;
        } else {
            speed = target->word_1c.value;
            step = target->word_0c.bytes.high;
        }
        angle = vector_xz_to_angle(
            player_state.camera_position.vx - actor->position.vx,
            player_state.camera_position.vz - actor->position.vz);
        actor->unknown_64 = angle;
        actor_turn_and_move_along_heading(actor->unknown_64, speed,
                      target->word_1e.value, step,
                      group->unknown_01[3], 4);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 11: {
        s32 collision;
        struct KfEulerAngles toward_player;
        SVECTOR forward;
        SVECTOR outer;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation_if_changed(target->unknown_01[0]);
            actor->state_70.signed_state = 0;
            actor_suspend_vertical_motion();
        }
        switch (actor->state_70.signed_state) {
        case 0:
            actor_advance_animation_clamped(actor, target->animation_step);
            if (actor->animation_phase >= 0xfff) {
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
            actor->unknown_50 = value_approach(actor->unknown_50,
                                                forward.vx, outer.vx);
            actor->unknown_52 = value_approach(actor->unknown_52,
                                                forward.vy, outer.vy);
            actor->unknown_54 = value_approach(actor->unknown_54,
                                                forward.vz, outer.vz);
            collision = actor_move_with_collision((SVECTOR *)&actor->unknown_50);
            if (collision != 0) {
                if (collision & 0x80) {
                    player_apply_damage(target->word_0e.value,
                                   target->word_10.value,
                                   target->word_12.value,
                                   target->word_0c.bytes.high,
                                   0, 0, 0, 0, 0, 0x1000, 10,
                                   &actor->position);
                }
                actor->state_70.signed_state = 2;
            }
            actor->rotation.x = (actor->rotation.x + 256) & 0xfff;
            goto case11_turn;
        case 2: {
            u32 pitch_phase = ((u16)actor->rotation.x + 256) & 0xfff;
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
        actor_move_with_collision((SVECTOR *)&actor->unknown_50);
    case11_turn:
        actor_turn_toward_angle(actor, vector_xz_to_angle(actor->unknown_50,
                                                actor->unknown_54),
                        target->word_16.value,
                        group->unknown_01[3]);
        break;
    }
    case 18: {
        s32 delta_x;
        s32 delta_y;
        s32 delta_z;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation_if_changed(target->unknown_01[0]);
        }
        delta_x = player_state.camera_position.vx - actor->position.vx;
        delta_y = player_state.camera_position.vy - actor->position.vy - 1600;
        delta_z = player_state.camera_position.vz - actor->position.vz;
        fixed_vector3_length(delta_x, delta_y, delta_z);
        vector_displacement_to_pitch_yaw(delta_x, delta_y, delta_z, &actor->tail_72.angles);
        actor_turn_and_move_along_euler_angles(&actor->tail_72.angles, target->word_1c.value,
                      target->word_1e.value,
                      target->word_0c.bytes.high,
                      group->unknown_01[3], 6);
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            actor_try_damage_player_in_cone(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 14: {
        KfActor *other = actor_state.other_actor;
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor->state_70.signed_state = 0;
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if ((s16)other->unknown_58 > 0) {
                actor_set_animation(target->word_0c.bytes.high);
                actor->state_70.signed_state = 1;
            } else if ((s16)other->unknown_58 < 0) {
                actor_set_animation(target->word_0c.bytes.low);
                actor->state_70.signed_state = 2;
            }
            break;
        case 1:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)other->unknown_58 <= 0) {
                actor->state_70.signed_state = 3;
            }
            break;
        case 2:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)other->unknown_58 >= 0) {
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
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation_if_changed(target->unknown_01[0]);
        }
        actor_turn_and_move_along_heading(actor->unknown_64,
                      target->word_0c.value, target->word_0e.value,
                      group->unknown_01[2],
                      group->unknown_01[3], 5);
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    case 19:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            actor_set_animation_if_changed(target->unknown_01[0]);
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
            if (actor->animation_phase >= 0xfff) {
                actor_reset_target_and_reselect();
            }
            break;
        }
        actor_damp_horizontal_motion(group->unknown_01[2], 10);
        break;
    case 20:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = target->word_12.value;
            actor_set_animation_if_changed(target->unknown_01[0]);
        }
        if (actor_move_along_heading(actor->rotation.y + 2048,
                          target->word_10.value, 1000, 5) != 0) {
            actor->state_70.signed_state = 1;
        }
        actor_advance_animation_wrapped(actor, target->animation_step);
        if (actor->animation_phase >= 0xfff - target->animation_step) {
            actor->state_70.signed_state--;
            if (actor->state_70.signed_state == 0) {
                actor->animation_phase = 0xfff;
                actor_reset_target_and_reselect();
            }
        }
        break;
    case 22:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
        }
        goto case19_clamped;
    case 21:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            actor_set_animation(target->unknown_01[0]);
            actor->unknown_1c = target->word_14.value;
            actor->unknown_1e = target->word_16.value;
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
            actor->unknown_1c = fixed_lerp_q12(target->word_14.value,
                group->unknown_12, actor->animation_phase);
            actor->unknown_1e = fixed_lerp_q12(target->word_16.value,
                group->unknown_14, actor->animation_phase);
            if (actor->animation_phase >= 0xfff) {
                actor->unknown_1c = group->unknown_12;
                actor->unknown_1e = group->unknown_14;
                actor->state_70.signed_state = 2;
                actor->tail_72.signed_state = target->word_10.value;
                actor_set_animation(target->word_18.bytes.low);
            }
            if (actor->animation_phase >= target->word_12.value) {
                s32 saved_flags = actor_state.unknown_93a4;
                actor_state.unknown_93a4 &= ~0x20;
                actor_turn_and_move_along_heading(actor->unknown_64,
                               target->word_0e.value, 0, 0xff, 0, 5);
                actor_state.unknown_93a4 = saved_flags;
            }
            break;
        case 2: {
            s32 saved_flags = actor_state.unknown_93a4;
            s32 next_timer;
            actor_state.unknown_93a4 &= ~0x20;
            actor_turn_and_move_along_heading(actor->unknown_64,
                           target->word_0e.value, 0, 0xff, 0, 5);
            next_timer = actor->tail_72.unsigned_state - 1;
            actor->tail_72.unsigned_state = next_timer;
            if ((s16)next_timer == -1) {
                actor_reset_target_and_reselect();
            }
            actor_state.unknown_93a4 = saved_flags;
            actor_advance_animation_wrapped(actor, target->word_18.bytes.high);
            break;
        }
        }
        break;
    case 26:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            actor_set_animation(target->unknown_01[0]);
            actor->unknown_4c = 0;
            actor->unknown_4a.value = 0;
            actor->unknown_48 = 0;
        }
        switch (actor->state_70.signed_state) {
        case 0: {
            KfMapObject *object = &map_object_state.objects[actor->unknown_20];
            u16 group_height;

            if (object->action_timer < 2 || object->rotation.vx == 0 ||
                object->rotation.vx >= 3072) {
                break;
            }
            actor->state_70.signed_state = 1;
            actor->tail_72.signed_state = 8;
            actor->unknown_1c = group->unknown_12;
            actor->unknown_1e = group->unknown_14;
            actor->position.vy += 2048;
            group_height = group->unknown_32;
            actor->unknown_4c = group_height;
            actor->unknown_4a.value = group_height;
            actor->unknown_48 = group_height;
            actor->rotation.y = object->rotation.vy;
        }
            /* fall through */
        case 1:
            actor->unknown_0d = 0;
            goto case29_shared_motion;
        }
        break;
    case 25: {
        const u16 *cursor;
        s32 repeat;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
            actor->state_70.signed_state = target->word_0e.value;
            actor->tail_72.script.word_index = 0;
            actor->tail_72.script.unknown_74 = 0;
            if (target->unknown_05[2] == 1) {
                actor->unknown_0d = 16;
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
                                   (s16)actor->tail_72.script.unknown_74);
                }
                if (--repeat == 0) {
                    break;
                }
            }
            actor->tail_72.script.unknown_74++;
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        actor_damp_horizontal_motion(group->unknown_01[2], 10);
        break;
    }
    case 27:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
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
                    (actor->unknown_07[1] << 11) + actor->unknown_24,
                    (actor->unknown_07[0] << 11) + actor->unknown_22,
                    target->word_0e.value, target->word_10.value,
                    actor->tail_72.signed_state,
                    group->unknown_01[2],
                    group->unknown_01[3], 5);
                if (actor->tail_72.signed_state == -1) {
                    actor->state_70.signed_state = 1;
                }
            }
            actor_advance_animation_wrapped(actor, target->word_16.bytes.high);
            break;
        case 1:
            actor_turn_toward_angle(actor, actor->unknown_20, target->word_10.value,
                          group->unknown_01[3]);
            if (actor->rotation.y == actor->unknown_20) {
                actor_set_animation(target->unknown_01[0]);
                actor->state_70.signed_state = 2;
            }
            break;
        case 2: {
            KfTargetCandidate *next_target;
            actor_advance_animation_clamped(actor, target->animation_step);
            actor->unknown_1c = fixed_lerp_q12(
                group->unknown_12,
                target->word_12.value, actor->animation_phase);
            actor->unknown_1e = fixed_lerp_q12(
                group->unknown_14,
                target->word_14.value, actor->animation_phase);
            if (actor->animation_phase >= 0xfff) {
                actor->unknown_1c = target->word_12.value;
                actor->unknown_1e = target->word_14.value;
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
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
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

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
            actor_suspend_vertical_motion();
            audio_play_spatial_default_range(0x1b, &actor->position, 120, 0);
        }
        actor->rotation.x += 128;
        actor->rotation.y += 64;
        collision = actor_move_with_collision((SVECTOR *)&actor->unknown_50);
        if (collision != 0) {
            if (collision & 0x80) {
                player_apply_damage(target->word_0e.value, target->word_10.value,
                               target->word_12.value, target->word_0c.value,
                               target->word_14.value, target->word_16.value,
                               target->word_18.value, target->word_1a.value,
                               target->word_1c.value, 0x1000, 10, &actor->position);
            }
            actor_select_target_type_in_own_group(actor, 3);
            actor->unknown_54 = 0;
            actor->unknown_52 = 0;
            actor->unknown_50 = 0;
            actor->unknown_0d = 16;
        }
        break;
    }
    case 30: {
        VECTOR next;
        s32 collision;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor_set_animation(target->unknown_01[0]);
            actor_suspend_vertical_motion();
        }
        next.vx = actor->position.vx + actor->unknown_50;
        next.vy = actor->position.vy + actor->unknown_52;
        next.vz = actor->position.vz + actor->unknown_54;
        collision = collision_query_world(
            next.vx, next.vy, next.vz, actor->unknown_1c,
            actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16),
            actor_state.unknown_93a4);
        if (collision == 0) {
        case30_position:
            actor->position.vx = next.vx;
            actor->position.vy = next.vy;
            actor->position.vz = next.vz;
            goto case30_advance;
        } else if (collision & 0x80) {
            player_apply_damage(target->word_0e.value, target->word_10.value,
                           target->word_12.value, target->word_0c.value,
                           target->word_14.value, target->word_16.value,
                           target->word_18.value, target->word_1a.value,
                           target->word_1c.value, 0x1000, 10, &actor->position);
            goto case30_position;
        } else {
            actor->unknown_54 = 0;
            actor->unknown_52 = 0;
            actor->unknown_50 = 0;
        }
    case30_advance:
        actor_advance_animation_clamped(actor, target->animation_step);
        if (actor->animation_phase >= 0xfff) {
            actor_select_target_type_in_own_group(actor, 3);
            actor->unknown_0d = 16;
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

    if (actor->unknown_28 & 0x10) {
        KfActor *other = actor_state.other_actor;
        u8 slot_state = actor->slot_state;

        if (slot_state == 3) {
            actor->unknown_03 = 0;
            if (other->lifecycle != 1) {
                actor->lifecycle = 0;
                goto behavior_done;
            }
            if (other->target_type == slot_state) {
                actor->target_type = 3;
                actor->unknown_0f = 0xf0;
                actor->state_70.signed_state = 99;
                goto behavior_done;
            }
        } else {
            s32 collision;

            actor->unknown_03 = other->unknown_03;
            if (other->lifecycle != 1) {
                actor->unknown_28 = (actor->unknown_28 & ~0x10) | 0x100;
                collision = collision_query_world(
                    actor->position.vx, actor->position.vy,
                    actor->position.vz, actor->unknown_1c,
                    actor->unknown_1e, 0x81);
                if (collision & 0x80) {
                    actor->position.vx += 800 + actor->unknown_1c;
                }
                if (collision & 0xf) {
                    actor->unknown_28 |= 0x400;
                }
                if (actor->unknown_28 & 0x200) {
                    actor_select_target_type_in_own_group(actor, 3);
                }
                actor->unknown_50 = other->unknown_50;
                actor->unknown_52 = other->unknown_52;
                actor->unknown_54 = other->unknown_54;
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
            vector_rotate_yxz(&rotation, (SVECTOR *)&group->unknown_0c,
                              &group_offset);
            actor_sample_rotated_animation_vertex(other, actor->unknown_24, &vertex_offset);
            actor->position.vx = other->position.vx + vertex_offset.vx -
                                 group_offset.vx;
            actor->position.vy = other->position.vy + vertex_offset.vy -
                                 group_offset.vy;
            actor->position.vz = other->position.vz + vertex_offset.vz -
                                 group_offset.vz;
        }
    } else if ((actor->unknown_28 & 0x10000) == 0) {
        actor_update_vertical_motion();
    }

behavior_done:
    if (actor->lifecycle == 1) {
        map_cell_add_layer_occupancy(actor->position.vx, actor->position.vz,
                       actor->unknown_1c, 1);
    }
}

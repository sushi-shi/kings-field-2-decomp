#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/animation.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>
#include <kf/lib/null.h>
#include <kf/game/collision_cache.h>


ADDRESS(0x80026330, 0x134)
void player_sample_weapon_world_vertex(s32 vertex_index, VECTOR *output)
{
    SVECTOR offset;
    struct KfEulerAngles angles;

    angles.x = player_state.camera_rotation.angles[0]
             - player_state.equipped_weapon_record->rotation_offset_x;
    angles.y = player_state.camera_rotation.angles[1]
             - player_state.equipped_weapon_record->rotation_offset_y;
    angles.z = player_state.camera_rotation.angles[2]
             + player_state.equipped_weapon_record->rotation_offset_z;
    animation_sample_vertex(KF_PLAYER_WEAPON_ASSET_INDEX, player_state.weapon_attack_mode,
                  player_state.weapon_attack_phase, vertex_index, &offset);
    offset.vx -= player_state.equipped_weapon_record->position_offset_x;
    offset.vy += player_state.equipped_weapon_record->position_offset_y;
    offset.vz -= player_state.equipped_weapon_record->position_offset_z;
    vector_rotate_yxz(&angles, &offset, output);
    output->vx += player_state.camera_position.vx;
    output->vz += player_state.camera_position.vz;
    {
        s32 y = output->vy - KF_PLAYER_CAMERA_EYE_OFFSET;
        s32 camera_y = player_state.camera_vertical_offset + player_state.camera_position.vy
                     + player_state.landing_vertical_offset;
        output->vy = y + camera_y;
    }
}

enum { PLAYER_WEAPON_MAGIC_POWER_MINIMUM = 60 };

ADDRESS(0x80026464, 0x34)
s32 player_meets_weapon_magic_power_requirement(void)
{
    return player_state.physical_power >= PLAYER_WEAPON_MAGIC_POWER_MINIMUM
        && player_state.magic >= PLAYER_WEAPON_MAGIC_POWER_MINIMUM;
}

RODATA(0x80011260, 0x9c)

enum {
    WEAPON_ATTACK_EVENT_DISABLED_PHASE = 5000,
    WEAPON_EFFECT_HELD_PHASE = 99,
    WEAPON_MAGIC_EFFECT_NONE = 0xff
};

ADDRESS(0x80026498, 0x1c4)
void player_dispatch_weapon_magic(s32 magic_id, s32 consume_mp, s32 effect_parameter)
{
    KfMagicRecord *record = &effect_state.magic_records[magic_id];
    VECTOR position;

    if (player_state.vitals.current_mp < record->mp_cost) {
        return;
    }
    player_state.selected_magic_record = record;
    if (consume_mp != 0) {
        player_state.magic_charge = 0;
        player_state.vitals.current_mp -= record->mp_cost;
    }

    switch (magic_id - 38) {
    case 1:
        player_sample_weapon_world_vertex(player_magic_id_sequence.effect_ids[effect_parameter], &position);
        player_dispatch_magic_effect(magic_id, &position);
        break;
    case 11:
    case 12:
        player_sample_weapon_world_vertex(0, &position);
        player_dispatch_magic_effect(magic_id, &position);
        break;
    case 2:
        effect_parameter <<= 9;
        player_state.magic_origin_offset.vx = rcos(effect_parameter) >> 3;
        player_state.magic_origin_offset.vy = rsin(effect_parameter) >> 3;
        player_state.magic_origin_offset.vz = 600;
        player_dispatch_magic_effect(magic_id);
        break;
    case 0:
        for (effect_parameter = 0; effect_parameter < KF_ANGLE_WRAP_MASK; effect_parameter += 684) {
            player_state.magic_origin_offset.vx = rcos(effect_parameter) >> 3;
            player_state.magic_origin_offset.vy = rsin(effect_parameter) >> 3;
            player_state.magic_origin_offset.vz = 400;
            player_dispatch_magic_effect(magic_id);
        }
        break;
    default:
        player_state.magic_origin_offset.vx = 200;
        player_state.magic_origin_offset.vy = 200;
        player_state.magic_origin_offset.vz = 400;
        player_dispatch_magic_effect(magic_id);
        break;
    }
}

ADDRESS(0x8002665c, 0xbd0)
void player_update_weapon_attack(void)
{
    s32 weapon_id = player_state.equipped_weapon_id;
    KfWeaponRecordGame *weapon = player_state.equipped_weapon_record;
    s16 phase;
    s32 phase_step;
    s32 phase_end;
    s32 sound_end;
    s32 sound_step;
    s32 hit_step;
    SVECTOR initial_vertex;
    struct KfEulerAngles rotation;
    VECTOR world_position;
    VECTOR step;
    VECTOR damage_position;
    VECTOR last_world;
    KfEffectRecord *effect;
    const VECTOR *damage_origin;
    s32 damage_amount;
    s32 index;
    s32 i;

    if (weapon_id < 16) {
        goto regular_weapon;
    }
    if (weapon_id < 18) {
        goto special_weapon;
    }
    if (weapon_id == KF_EQUIPMENT_NONE) {
        return;
    }
    goto regular_weapon;

special_weapon: {
        s32 mode;
        phase = player_state.weapon_attack_phase;
        if (phase == KF_WEAPON_ATTACK_INACTIVE) {
            goto special_idle;
        }
        mode = player_state.weapon_attack_mode;
        if (mode == 0) {
            goto special_mode_zero;
        }
        if (mode == 1) {
            goto special_mode_one;
        }
        return;
special_mode_zero: {
            if (phase == 0) {
                s32 effect_kind;
                s32 counter;

                switch (weapon_id) {
                case 16:
                    effect_kind = 31;
                    counter = 0x75;
                    break;
                case 17:
                    effect_kind = 30;
                    counter = 0x76;
                    break;
                }

                if (player_state.equipped_accessory_id == 59
                    || player_state.equipped_extra_id == 59) {
                    effect_kind += 17;
                }
                if (game_counter_bytes[counter] != 0) {
                    game_counter_bytes[counter]--;
                    player_state.weapon_effect = effect_construct_record(
                        10, 0x12, effect_kind, &player_state.camera_position,
                        0, &player_state.camera_rotation);
                    effect = player_state.weapon_effect;
                    if (effect != 0) {
                        effect->phase = WEAPON_EFFECT_HELD_PHASE;
                    }
                } else {
                    player_state.weapon_effect = 0;
                }
            }

            player_state.weapon_attack_phase += weapon->attack_phase_step;
            if (player_state.weapon_attack_phase >= KF_ANGLE_WRAP_MASK) {
                player_state.weapon_attack_phase = KF_ANGLE_WRAP_MASK;
            }
            if (player_state.weapon_attack_phase >= weapon->normal_attack_end_phase) {
                if (player_state.attack_charge_current == 0
                    && player_state.equipped_weapon_id == 16) {
                    audio_play_sound(3, 110);
                }
                player_state.attack_charge_current =
                    ((player_state.weapon_attack_phase - weapon->normal_attack_end_phase) * KF_PLAYER_CHARGE_FULL)
                    / (KF_ANGLE_WRAP_MASK - weapon->normal_attack_end_phase);
            } else {
                player_state.attack_charge_current = 0;
            }

            effect = player_state.weapon_effect;
            if (effect != 0) {
                rotation.x = player_state.camera_rotation.angles[0] - weapon->rotation_offset_x;
                rotation.y = player_state.camera_rotation.angles[1] - weapon->rotation_offset_y;
                rotation.z = player_state.camera_rotation.angles[2] + weapon->rotation_offset_z;
                animation_sample_vertex(32, player_state.weapon_attack_mode,
                               player_state.weapon_attack_phase,
                               weapon->initial_vertex_index, &initial_vertex);
                initial_vertex.vx -= weapon->position_offset_x;
                initial_vertex.vy += weapon->position_offset_y;
                initial_vertex.vz -= weapon->position_offset_z;
                vector_rotate_yxz(&rotation, &initial_vertex, &world_position);
                effect->position.vx = player_state.camera_position.vx + world_position.vx;
                effect->position.vz = player_state.camera_position.vz + world_position.vz;
                effect->position.vy = player_state.camera_position.vy + world_position.vy
                                    + player_state.camera_vertical_offset + player_state.landing_vertical_offset
                                    - KF_PLAYER_CAMERA_EYE_OFFSET;

                animation_sample_vertex(32, player_state.weapon_attack_mode,
                               player_state.weapon_attack_phase,
                               weapon->final_vertex_index, &initial_vertex);
                initial_vertex.vx -= weapon->position_offset_x;
                initial_vertex.vy += weapon->position_offset_y;
                initial_vertex.vz -= weapon->position_offset_z;
                vector_rotate_yxz(&rotation, &initial_vertex, &last_world);
                vector_displacement_to_pitch_yaw(world_position.vx - last_world.vx,
                              world_position.vy - last_world.vy,
                              world_position.vz - last_world.vz,
                              (struct KfEulerAngles *)&effect->rotation);
            }
            if ((player_state.flags_140.low & 0x10) != 0) {
                return;
            }
            if (effect != 0) {
                effect->phase = 0;
                pitch_yaw_to_forward_vector(
                    (const struct KfEulerAngles *)&effect->rotation,
                    &effect->direction);
                vector3s_scale_shift12((player_state.attack_charge_current * 900) / KF_PLAYER_CHARGE_FULL,
                                       &effect->direction);
                effect->updates_remaining = 50;
                effect->cache_tail.payload.ballistic.origin_y = effect->position.vy;
            }
            player_state.weapon_attack_mode = 1;
            player_state.weapon_attack_phase = 0;
            return;
        }
special_mode_one: {
            phase += 400;
            player_state.weapon_attack_phase = phase;
            if (phase >= KF_ANGLE_WRAP_MASK) {
                player_state.weapon_attack_phase = KF_WEAPON_ATTACK_INACTIVE;
                player_state.attack_charge_current = 0;
            }
        }
        return;
special_idle:
        player_state.attack_charge_current = 0;
        player_state.weapon_charge_delay = 0;
        return;
    }

regular_weapon:
    phase = player_state.weapon_attack_phase;

    if (phase == KF_WEAPON_ATTACK_INACTIVE) {
        goto regular_idle;
    }

    if (player_state.weapon_attack_mode == 0) {
        phase_step = weapon->attack_phase_step;
        phase_end = weapon->normal_attack_end_phase;
        sound_end = weapon->normal_attack_sound_phase;
        sound_step = 0;
        hit_step = 0;
    } else {
        phase_step = weapon->alternate_attack_phase_step;
        phase_end = weapon->alternate_attack_end_phase;
        hit_step = weapon->magic_phase_step;
        sound_end = weapon->alternate_attack_sound_end_phase;
        sound_step = weapon->alternate_attack_sound_phase_step;
    }
    player_state.weapon_attack_phase += phase_step;

    if (player_state.weapon_attack_mode == 0
        && weapon->initial_effect_id != WEAPON_MAGIC_EFFECT_NONE
        && player_state.weapon_attack_fully_charged != 0
        && player_meets_weapon_magic_power_requirement() != 0
        && (player_state.flags_140.low & 0x80) != 0) {
        if (player_state.weapon_attack_phase >= weapon->magic_window_start
            && player_state.weapon_attack_phase <= weapon->magic_window_end) {
            if (player_state.weapon_magic_shots_configured != 0) {
                player_dispatch_weapon_magic(weapon->initial_effect_id,
                               player_state.weapon_magic_shots_configured == weapon->magic_shots,
                               player_state.weapon_magic_shots_configured);
                player_state.weapon_magic_shots_configured--;
            }
        } else {
            player_state.weapon_magic_shots_configured = 0;
        }
    }

    if (player_state.weapon_attack_phase >= player_state.weapon_next_sound_phase
        && player_state.weapon_attack_phase
             < player_state.weapon_next_sound_phase + phase_step) {
        audio_play_sound(weapon->sound_id, 80);
        if (player_state.weapon_next_sound_phase >= sound_end) {
            player_state.weapon_next_sound_phase = WEAPON_ATTACK_EVENT_DISABLED_PHASE;
        } else {
            player_state.weapon_next_sound_phase += sound_step;
        }
    }

    if (player_state.weapon_attack_phase >= player_state.weapon_attack_window
        && player_state.weapon_attack_phase
             < player_state.weapon_attack_window + phase_step) {
        player_state.weapon_guard_active = 0;
        if (player_state.weapon_attack_mode == 1) {
            if (player_state.equipped_weapon_id == 13
                && (player_state.flags_140.low & 0x80) != 0) {
                player_state.weapon_attack_phase -= phase_step;
                player_state.weapon_guard_active = 1;
                return;
            }
            if (weapon->release_effect_id != WEAPON_MAGIC_EFFECT_NONE) {
                player_dispatch_weapon_magic(weapon->release_effect_id,
                               player_state.weapon_attack_phase >= phase_end,
                               (player_state.weapon_attack_phase - weapon->alternate_attack_window_start)
                                   / hit_step);
            }
        }

        if (player_state.weapon_attack_phase >= phase_end) {
            player_state.weapon_attack_window = WEAPON_ATTACK_EVENT_DISABLED_PHASE;
            player_state.weapon_charge_delay = 10;
            damage_amount = player_state.attack_charge_committed;
            damage_origin = &damage_position;
            player_state.attack_charge_current = 0;
            damage_position.vx = player_state.camera_position.vx;
            damage_position.vz = player_state.camera_position.vz;
            damage_position.vy = player_state.camera_position.vy - 1000;
        } else {
            player_state.weapon_attack_window += hit_step;
            damage_origin = 0;
            damage_amount = player_state.attack_charge_committed >> 2;
        }

        initial_vertex.vx = 0;
        initial_vertex.vy = 0;
        initial_vertex.vz = weapon->attack_angle;
        rotation.x = -player_state.camera_rotation_target.angles[0];
        rotation.y = player_state.camera_rotation_target.angles[1];
        rotation.z = 0;
        vector_rotate_yxz(&rotation, &initial_vertex, &step);
        step.vy = ((3600 - weapon->attack_angle) * step.vy) / 1800;
        step.vx /= 4;
        step.vy /= 4;
        step.vz /= 4;
        world_position.vx = player_state.camera_position.vx;
        world_position.vy = player_state.camera_position.vy - 800;
        world_position.vz = player_state.camera_position.vz;
        for (i = 3; i != -1; i--) {
            world_position.vx += step.vx;
            world_position.vy += step.vy;
            world_position.vz += step.vz;
            index = actor_find_overlap_excluding_target_type3(world_position.vx, world_position.vy,
                                   world_position.vz, 400, 600);
            if (index != -1) {
                KfActor *actor = &actor_state.actors[index];
                KfTargetGroup *group = &actor_state.target_groups[actor->group_index];
                s32 bearing = vector_xz_to_angle(actor->position.vx - player_state.camera_position.vx,
                                                  actor->position.vz - player_state.camera_position.vz);
                if (angle_within_tolerance(player_state.camera_rotation.angles[1],
                                           bearing, group->player_facing_tolerance)
                    && angle_within_tolerance(actor->rotation.y,
                                              bearing + 0x800, group->actor_facing_tolerance)) {
                    actor_apply_magic_to_actor(index, player_state.physical_power,
                                   player_state.attack_components[0],
                                   player_state.attack_components[1],
                                   player_state.attack_components[2],
                                   player_state.attack_components[3],
                                   player_state.attack_components[4],
                                   player_state.attack_components[5],
                                   player_state.attack_components[6],
                                   player_state.attack_components[7],
                                   damage_amount,
                                   0x11, damage_origin);
                    break;
                }
            }
        }
    }

    if (player_state.weapon_attack_phase > KF_ANGLE_WRAP_MASK) {
        player_state.weapon_attack_phase = KF_WEAPON_ATTACK_INACTIVE;
        player_state.weapon_magic_shots_configured = 0;
    }
    return;

regular_idle:
    if ((player_state.flags_140.low & 0x10) == 0) {
        if (player_state.weapon_charge_delay == 0) {
            s32 gain = player_charge_gain_for_rank(player_state.physical_power,
                                      weapon->charge_rank) * 2;
            if (player_state.equipped_leg_id == 44) {
                gain >>= 1;
            }
            if ((player_state.equipped_accessory_id == 57
                 || player_state.equipped_extra_id == 57)
                && player_state.equipped_weapon_id == 13) {
                gain *= 2;
            }
            player_state.attack_charge_current += gain;
            if (player_state.attack_charge_current > KF_PLAYER_CHARGE_FULL) {
                player_state.attack_charge_current = KF_PLAYER_CHARGE_FULL;
            }
        } else {
            player_state.weapon_charge_delay--;
        }
    }
}




ADDRESS(0x8002722c, 0x2c0)
void player_select_magic_action(s32 magic_id)
{
    KfMagicRecord *record;
    u16 mp_cost;

    if (player_state.queued_magic_action.magic_id != KF_PLAYER_MAGIC_ACTION_NONE ||
        magic_id == KF_PLAYER_MAGIC_ACTION_NONE) {
        return;
    }

    record = &effect_state.magic_records[magic_id];
    if (player_state.vitals.current_mp < record->mp_cost) {
        return;
    }

    if (player_state.equipped_weapon_id == 12 && magic_id < 11) {
        if (magic_id >= 7) {
            return;
        }
    }
    if (player_state.equipped_body_id == 31 && magic_id >= 11) {
        if (magic_id < 13) {
            return;
        }
        if (magic_id < 20) {
            if (magic_id >= 18) {
                return;
            }
        }
    }

    switch (magic_id - 14) {
    case 0:
    case 2:
    case 5:
        break;
    case 1:
        if (player_state.defense_boost_timer != 0) {
            return;
        }
        break;
    case 3:
        if (player_state.attack_boost_timer != 0) {
            return;
        }
        break;
    case 4:
        player_state.magic_tint_phase_limit = 900;
        player_state.vitals.current_mp -= record->mp_cost;
        return;
    default:
        goto charge_gate;
    }
    player_state.queued_magic_action.casts_remaining = 1;
    player_state.queued_magic_action.repeat_interval = 1;

charge_gate:
    if (player_state.magic_charge < KF_PLAYER_CHARGE_FULL) {
        return;
    }
    player_state.magic_charge = 0;
    mp_cost = record->mp_cost;
    player_state.queued_magic_action.magic_id = magic_id;
    player_state.magic_origin_offset.vx = -200;
    player_state.magic_origin_offset.vy = 200;
    player_state.magic_origin_offset.vz = 400;
    player_state.queued_magic_action.countdown = 1;
    player_state.vitals.current_mp -= mp_cost;

    switch (magic_id) {
    case 10:
        player_state.magic_origin_offset.vx = 0;
        player_state.magic_origin_offset.vy = -512;
        player_state.magic_origin_offset.vz = 2000;
        /* Retail falls through to the shared action-byte stores. */
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
    case 18:
        player_state.queued_magic_action.casts_remaining = 1;
        player_state.queued_magic_action.repeat_interval = 1;
        break;
    case 12:
        player_state.magic_origin_offset.vx = -200;
        player_state.queued_magic_action.casts_remaining = 5;
        player_state.queued_magic_action.repeat_interval = 2;
        break;
    case 9:
        player_state.queued_magic_action.casts_remaining = 6;
        player_state.queued_magic_action.repeat_interval = 1;
        break;
    case 0:
    case 2:
        player_state.queued_magic_action.casts_remaining = 1;
        player_state.queued_magic_action.repeat_interval = 1;
        player_state.magic_origin_offset.vz = 0;
        player_state.magic_origin_offset.vy = 0;
        player_state.magic_origin_offset.vx = 0;
        break;
    case 13:
        player_state.queued_magic_action.casts_remaining = 7;
        player_state.queued_magic_action.repeat_interval = 1;
        break;
    case 3:
        player_state.queued_magic_action.casts_remaining = 6;
        player_state.queued_magic_action.repeat_interval = 2;
        break;
    }

    player_state.selected_magic_record = record;
}

enum {
    PLAYER_MOVE_COLLISION_MODE = KF_COLLISION_QUERY_SHAPES |
                                 KF_COLLISION_QUERY_ACTORS |
                                 KF_COLLISION_QUERY_MAP_OBJECTS,
    PLAYER_MOVE_SLIDE_RADIUS = 880,
    PLAYER_MOVE_DEFLECTION_ANGLE = 32,
    PLAYER_MOVE_STEP = 22,
    PLAYER_MOVE_STEP_UP_TOLERANCE = 1280,
    PLAYER_MOVE_COLLISION_RETRY_LIMIT = 2
};

ADDRESS(0x800274ec, 0x43c)
s32 player_move_horizontal(s32 heading, s32 distance)
{
    s32 dx = (-rsin(heading) * distance) >> 12;
    s32 dz = (rcos(heading) * distance) >> 12;
    s32 initial_dx = dx;
    s32 initial_dz = dz;
    VECTOR next;
    s32 flags;
    s32 angle;
    s32 radius;
    s32 slide_distance;
    s32 result = 0;
    s32 collision_retry = 0;
    s32 slide_attempted = 0;
    s32 diagonal_retry = 0;
    s32 high_collision;
    SVECTOR delta;
    s32 diagonal_kind;

    for (;;) {
        next.vx = player_state.camera_position.vx + dx;
        next.vz = player_state.camera_position.vz + dz;
        flags = collision_query_world(next.vx, player_state.camera_position.vy, next.vz,
                              KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                              PLAYER_MOVE_COLLISION_MODE);
        if (flags == 0) {
        accept_position:
            player_state.camera_position.vx = next.vx;
            player_state.camera_position.vz = next.vz;
            player_state.map_layer_index = KF_COLLISION_CACHE_LAYER;
            result = 1;
            break;
        }

        high_collision = 0;
        if ((flags & ~(KF_COLLISION_HIT_AXIS | KF_COLLISION_HIT_FLOOR)) == 0) {
            s32 collision_height = KF_COLLISION_CACHE_RESULT;
            high_collision = 1;
            if (collision_height + PLAYER_MOVE_STEP_UP_TOLERANCE >= player_state.camera_position.vy
                && player_state.death_state == 0
                && (KF_COLLISION_CACHE_HEIGHT_LIMIT - collision_height)
                       < -KF_PLAYER_HEIGHT) {
                goto accept_position;
            }
        }

        if (flags & (KF_COLLISION_HIT_ACTOR | KF_COLLISION_HIT_MAP_OBJECT)) {
            collision_retry++;
            if (collision_retry == PLAYER_MOVE_COLLISION_RETRY_LIMIT) {
                break;
            }
            collision_cache_load_hit_bounds();
            delta.vx = (u16)KF_COLLISION_CACHE_POSITION.vx
                     - (u16)player_state.camera_position.vx;
            delta.vz = (u16)KF_COLLISION_CACHE_POSITION.vz
                     - (u16)player_state.camera_position.vz;
            angle = vector_xz_to_angle(delta.vx, delta.vz);
            angle = angle_mod_delta_le_half_turn(heading, angle)
                ? angle + (KF_ANGLE_HALF_TURN - PLAYER_MOVE_DEFLECTION_ANGLE)
                : angle + (KF_ANGLE_HALF_TURN + PLAYER_MOVE_DEFLECTION_ANGLE);
            angle &= KF_ANGLE_WRAP_MASK;
            radius = KF_COLLISION_CACHE_RADIUS + PLAYER_MOVE_SLIDE_RADIUS;
            delta.vx = (-rsin(angle) * radius) >> 12;
            delta.vz = (rcos(angle) * radius) >> 12;
            next.vx = KF_COLLISION_CACHE_POSITION.vx + delta.vx;
            next.vz = KF_COLLISION_CACHE_POSITION.vz + delta.vz;
            dx = next.vx - player_state.camera_position.vx;
            dz = next.vz - player_state.camera_position.vz;
            continue;
        }

        if (!slide_attempted) {
            slide_distance = distance - PLAYER_MOVE_STEP;
            if (slide_distance >= 0) {
                VECTOR *camera = &player_state.camera_position;
                do {
                    next.vx = camera->vx
                           + ((-rsin(heading) * slide_distance) >> 12);
                    next.vz = camera->vz
                           + ((rcos(heading) * slide_distance) >> 12);
                    if (collision_query_world(next.vx, camera->vy,
                                       next.vz, KF_PLAYER_COLLISION_RADIUS,
                                       KF_PLAYER_HEIGHT, PLAYER_MOVE_COLLISION_MODE) == 0) {
                        camera->vx = next.vx;
                        camera->vz = next.vz;
                        break;
                    }
                    slide_distance -= PLAYER_MOVE_STEP;
                } while (slide_distance >= 0);
            }
            slide_attempted = 1;
        }

        if (high_collision || (flags & KF_COLLISION_HIT_AXIS)) {
        axis_retry:
            if (dx != 0) {
                dx = 0;
                continue;
            }
            if (dz != 0) {
                dz = 0;
                dx = initial_dx;
                continue;
            }
        }
        if (flags & KF_COLLISION_HIT_DIAGONAL) {
            if (diagonal_retry) {
                goto axis_retry;
            } else {
                diagonal_retry = 1;
                diagonal_kind =
                    ((KfMapOccupancyLayer *)KF_COLLISION_CACHE_SHAPE)->quarter_turns & 3;
                if (diagonal_kind == 0 || diagonal_kind == 2) {
                    dx = (initial_dx + initial_dz) >> 1;
                    dz = dx;
                } else {
                    dx = (initial_dx - initial_dz) >> 1;
                    dz = -dx;
                }
                continue;
            }
        }
        result = 0;
        break;
    }
    player_state.frame_displacement.vx = dx;
    player_state.frame_displacement.vz = dz;
    return result;
}

enum {
    COLLISION_DEPTH_ARM_HEIGHT = 200,
    COLLISION_DEPTH_DEATH_LIMIT = 32000,
    PLAYER_MOTION_COLLISION_MASK = KF_COLLISION_QUERY_SHAPES |
        KF_COLLISION_QUERY_ACTORS | KF_COLLISION_QUERY_MAP_OBJECTS,
    PLAYER_LANDING_SOUND_ID = 12,
    PLAYER_LANDING_SOUND_MIN_MAGNITUDE = 320,
    PLAYER_LANDING_SOUND_MAX_EXCESS = 896,
    PLAYER_LANDING_SOUND_BASE_VOLUME = 32
};

ADDRESS(0x80027928, 0x60)
void player_check_fall_death(void)
{
    if (player_state.vertical_velocity >= COLLISION_DEPTH_ARM_HEIGHT
        && (KF_COLLISION_CACHE_RESULT - player_state.camera_position.vy)
               > COLLISION_DEPTH_DEATH_LIMIT) {
        player_death_begin(NULL);
        player_state.fatal_fall_latch = 1;
    }
}

ADDRESS(0x80027988, 0x44)
void player_play_landing_sound(s32 magnitude)
{
    s32 volume = magnitude;

    if (volume >= PLAYER_LANDING_SOUND_MIN_MAGNITUDE) {
        volume -= PLAYER_LANDING_SOUND_MIN_MAGNITUDE;
        if (volume > PLAYER_LANDING_SOUND_MAX_EXCESS) {
            volume = PLAYER_LANDING_SOUND_MAX_EXCESS;
        }
        audio_play_sound(PLAYER_LANDING_SOUND_ID,
                         (volume >> 3) + PLAYER_LANDING_SOUND_BASE_VOLUME);
    }
}

ADDRESS(0x800279cc, 0x5ac)
void player_update_vertical_motion(void)
{
    s32 next_y;
    s32 height_difference;
    s32 collision_flags;
    s32 impact;
    s32 bob;
    s32 movement_speed;
    const s32 *floor_result;

    collision_probe_floor_height(player_state.camera_position.vx,
                  player_state.camera_position.vy,
                  player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT);
    player_state.frame_displacement.vy = 0;

    switch (player_state.vertical_motion_state) {
    case KF_PLAYER_VERTICAL_GROUNDED:
        break;

    case KF_PLAYER_VERTICAL_FALLING:
        player_check_fall_death();
        next_y = player_state.camera_position.vy + player_state.vertical_velocity;
        player_state.camera_position.vy = next_y;
        player_state.frame_displacement.vy = player_state.vertical_velocity;
        player_state.vertical_velocity += 40;
        if (KF_COLLISION_CACHE_RESULT + 100 < next_y) {
            player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            player_state.vertical_motion_state = KF_PLAYER_VERTICAL_GROUNDED;
        }
        goto finish;

    case KF_PLAYER_VERTICAL_STEP_UP:
        player_check_fall_death();
        player_state.camera_position.vy += player_state.vertical_velocity;
        if (player_state.camera_position.vy <= KF_COLLISION_CACHE_RESULT
            || player_state.vertical_velocity >= 0) {
            if (KF_COLLISION_CACHE_RESULT
                < player_state.camera_position.vy - player_state.vertical_velocity) {
                player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            }
            player_state.vertical_motion_state = KF_PLAYER_VERTICAL_GROUNDED;
        }
        player_state.frame_displacement.vy = player_state.vertical_velocity;
        player_state.vertical_velocity += 5;
        goto finish;

    case KF_PLAYER_VERTICAL_DEEP_FALL:
        player_check_fall_death();
        next_y = player_state.camera_position.vy + player_state.vertical_velocity;
        collision_flags = collision_query_world(player_state.camera_position.vx, next_y,
                                         player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS,
                                         KF_PLAYER_HEIGHT, PLAYER_MOTION_COLLISION_MASK);
        if (collision_flags == 0) {
            player_state.frame_displacement.vy = player_state.vertical_velocity;
            player_state.vertical_velocity += 40;
            player_state.vertical_motion_pitch_offset = player_state.vertical_velocity >> 1;
            player_state.camera_position.vy = next_y;
            goto finish;
        }
        if (player_state.vertical_velocity < 0) {
            player_state.vertical_velocity = 0;
            goto finish;
        }
        player_play_landing_sound(player_state.vertical_velocity);
        if (player_state.vertical_velocity >= 480) {
            impact = (player_state.vertical_velocity * player_state.vertical_velocity) >> 12;
            player_apply_damage_reaction(NULL, (impact * impact * impact) / 0x1ccf0, 0);
        }
        if ((collision_flags & KF_COLLISION_HIT_FLOOR) != 0) {
            player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
        } else {
            collision_cache_load_hit_bounds();
            next_y = KF_COLLISION_CACHE_POSITION.vy
                   - KF_COLLISION_CACHE_INTERACTION_HEIGHT - 1;
            if (collision_query_world(player_state.camera_position.vx, next_y,
                               player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS,
                               KF_PLAYER_HEIGHT, PLAYER_MOTION_COLLISION_MASK) == 0) {
                player_state.camera_position.vy = next_y;
            }
        }
        player_state.landing_vertical_offset = 1;
        player_state.vertical_motion_state = KF_PLAYER_VERTICAL_LANDING;
        /* Enter the landing response in the same frame. */
        goto landing;

    case KF_PLAYER_VERTICAL_LANDING:
landing:
        if (player_state.landing_vertical_offset > 0) {
            player_state.landing_vertical_offset += player_state.vertical_velocity >> 2;
        }
        bob = player_state.vertical_motion_pitch_offset;
        player_state.vertical_velocity -= 100;
        if (bob > 0) {
            if (player_state.vertical_velocity > 0) {
                player_state.vertical_motion_pitch_offset = bob + 10;
            } else {
                player_state.vertical_motion_pitch_offset = bob - 30;
            }
        }
        if (player_state.landing_vertical_offset <= 0
            && player_state.vertical_motion_pitch_offset <= 0) {
            player_state.vertical_motion_state = KF_PLAYER_VERTICAL_GROUNDED;
            player_state.vertical_velocity = 0;
            player_state.landing_vertical_offset = 0;
            player_state.vertical_motion_pitch_offset = 0;
        }
        break;

    default:
        goto finish;
    }

    floor_result = &KF_COLLISION_CACHE_RESULT;
    height_difference = *floor_result - player_state.camera_position.vy;
    if (height_difference < 0) {
        if (height_difference >= -256) {
            if (height_difference < -128) {
                player_state.camera_position.vy -= 128;
            } else {
                player_state.camera_position.vy = *floor_result;
            }
            goto finish;
        }
        if (height_difference >= -512) {
            player_state.camera_position.vy -= 256;
            goto finish;
        }
        movement_speed = player_state.movement_speed.signed_value;
        player_state.vertical_motion_state = KF_PLAYER_VERTICAL_STEP_UP;
        player_state.vertical_velocity = movement_speed > 200 ? -300 : -150;
    } else {
        if (height_difference <= 0) {
            goto finish;
        }
        if (collision_query_world(player_state.camera_position.vx,
                           player_state.camera_position.vy + 1,
                           player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS,
                           KF_PLAYER_HEIGHT, PLAYER_MOTION_COLLISION_MASK) != 0) {
            goto finish;
        }
        if (height_difference <= 256) {
            if (height_difference >= 129) {
                player_state.camera_position.vy += 128;
            } else {
                player_state.camera_position.vy = *floor_result;
            }
            goto finish;
        }
        if (height_difference <= 512) {
            player_state.camera_position.vy += 256;
            goto finish;
        }
        player_state.vertical_motion_state = height_difference > 1024
            ? KF_PLAYER_VERTICAL_DEEP_FALL : KF_PLAYER_VERTICAL_FALLING;
        player_state.vertical_velocity = 40;
    }
    player_state.landing_vertical_offset = 0;
    player_state.vertical_motion_pitch_offset = 0;

finish:
    if (player_state.vertical_motion_state == KF_PLAYER_VERTICAL_GROUNDED) {
        if (player_state.walking_bob_enabled != 0) {
            player_state.walking_bob_phase =
                (player_state.walking_bob_phase + player_state.movement_speed.unsigned_value)
                & KF_ANGLE_WRAP_MASK;
            bob = rsin(player_state.walking_bob_phase) >> 5;
            if (bob < 0) {
                bob = -bob;
            }
            player_state.camera_vertical_offset = bob - (bob >> 2);
        } else {
            player_state.camera_vertical_offset = 0;
        }
    }
    player_update_collision_bounds();
}

ADDRESS(0x80027f78, 0x2ac)
s32 player_move_reaction_with_collision(void)
{
    VECTOR next;
    s32 flags;
    s32 length;
    s32 remaining;
    s32 minimum_length;
    SVECTOR *motion;

    next.vx = player_state.camera_position.vx + player_state.reaction.damage.rotation.vx;
    next.vy = player_state.camera_position.vy + player_state.reaction.damage.rotation.vy;
    next.vz = player_state.camera_position.vz + player_state.reaction.damage.rotation.vz;

    flags = collision_query_world(next.vx, next.vy, next.vz,
                                  KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                                  PLAYER_MOTION_COLLISION_MASK);
    if (flags == 0) {
    accept:
        if (player_state.reaction.damage.rotation.vy >= 160
            && KF_COLLISION_CACHE_RESULT - player_state.camera_position.vy
                   > COLLISION_DEPTH_DEATH_LIMIT) {
            player_death_begin(NULL);
            player_state.fatal_fall_latch = 1;
        }
        player_state.camera_position.vx = next.vx;
        player_state.camera_position.vy = next.vy;
        player_state.camera_position.vz = next.vz;
        player_state.reaction.damage.rotation.vy += 32;
        goto accepted;
    }

    next.vy = player_state.camera_position.vy;
    flags = collision_query_world(next.vx, next.vy, next.vz,
                                  KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                                  PLAYER_MOTION_COLLISION_MASK);
    if (flags == 0) {
        player_state.reaction.damage.rotation.vy = 1;
        flags = collision_query_world(next.vx, next.vy, next.vz,
                                      KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                                      PLAYER_MOTION_COLLISION_MASK);
        if (flags == 0) {
            minimum_length = 32;
        scale_motion:
            motion = &player_state.reaction.damage.rotation;
            length = fixed_vector2_length(motion->vx, motion->vz);
            remaining = length - minimum_length;
            if (length <= minimum_length) {
                motion->vz = 0;
                motion->vx = 0;
                goto exhausted;
            }
            motion->vx = (motion->vx * remaining) / length;
            motion->vz = (motion->vz * remaining) / length;
            goto accept;
        }
    }

    if (flags & ~(KF_COLLISION_HIT_AXIS | KF_COLLISION_HIT_FLOOR)) {
        return 1;
    }
    if (KF_COLLISION_CACHE_RESULT + 256 < player_state.camera_position.vy) {
        return 1;
    }
    next.vy = KF_COLLISION_CACHE_RESULT;
    minimum_length = 56;
    goto scale_motion;

exhausted:
    return 1;

accepted:
    player_update_collision_bounds();
    return 0;
}

enum {
    PLAYER_YAW_ACCEL_SHIFT = 2,
    PLAYER_PITCH_STEP = 3,
    PLAYER_PITCH_STEP_LIMIT = 32,
    PLAYER_CAMERA_PITCH_LIMIT = 700
};

ADDRESS(0x80028224, 0x2f8)
void player_update_camera_rotation(void)
{
    if (player_state.flags_140.low & PADLleft) {
        player_state.yaw_step += player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step > player_state.turn_step_limit) {
            player_state.yaw_step = player_state.turn_step_limit;
        }
    } else if (player_state.flags_140.low & PADLright) {
        player_state.yaw_step -= player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step < -player_state.turn_step_limit) {
            player_state.yaw_step = -player_state.turn_step_limit;
        }
    } else if (player_state.yaw_step > 0) {
        player_state.yaw_step -= player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step < 0) {
            player_state.yaw_step = 0;
        }
    } else if (player_state.yaw_step < 0) {
        player_state.yaw_step += player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step > 0) {
            player_state.yaw_step = 0;
        }
    }

    player_state.camera_rotation_target.angles[1] =
        (player_state.camera_rotation_target.angles[1] + player_state.yaw_step)
        & KF_ANGLE_WRAP_MASK;

    if (player_state.flags_140.low & PADR2) {
        player_state.pitch_step += PLAYER_PITCH_STEP;
        if (player_state.pitch_step > PLAYER_PITCH_STEP_LIMIT) {
            player_state.pitch_step = PLAYER_PITCH_STEP_LIMIT;
        }
    } else if (player_state.flags_140.low & PADL2) {
        player_state.pitch_step -= PLAYER_PITCH_STEP;
        if (player_state.pitch_step < -PLAYER_PITCH_STEP_LIMIT) {
            player_state.pitch_step = -PLAYER_PITCH_STEP_LIMIT;
        }
    } else if (player_state.pitch_step > 0) {
        player_state.pitch_step -= PLAYER_PITCH_STEP;
        if (player_state.pitch_step < 0) {
            player_state.pitch_step = 0;
        }
    } else if (player_state.pitch_step < 0) {
        player_state.pitch_step += PLAYER_PITCH_STEP;
        if (player_state.pitch_step > 0) {
            player_state.pitch_step = 0;
        }
    }

    if (player_state.pitch_step > 0) {
        if (angle_mod_delta_le_half_turn(
                (player_state.camera_rotation_target.angles[0] =
                    (player_state.camera_rotation_target.angles[0] + player_state.pitch_step)
                    & KF_ANGLE_WRAP_MASK),
                PLAYER_CAMERA_PITCH_LIMIT)) {
            player_state.camera_rotation_target.angles[0] = PLAYER_CAMERA_PITCH_LIMIT;
        }
    } else if (player_state.pitch_step < 0) {
        if (angle_mod_delta_le_half_turn(
                -PLAYER_CAMERA_PITCH_LIMIT,
                (player_state.camera_rotation_target.angles[0] =
                    (player_state.camera_rotation_target.angles[0] + player_state.pitch_step)
                    & KF_ANGLE_WRAP_MASK))) {
            player_state.camera_rotation_target.angles[0] = -PLAYER_CAMERA_PITCH_LIMIT;
        }
    }
}

ADDRESS(0x8002851c, 0x460)
void player_update_horizontal_motion(void)
{
    s16 forward;
    s16 strafe;
    s32 forward_square;
    s32 strafe_square;
    s16 magnitude;

    if (player_state.flags_140.low & PADLup) {
        forward = player_state.forward_velocity + (player_state.movement_step_limit >> 2);
        if (forward > player_state.movement_step_limit) {
            player_state.forward_velocity = player_state.movement_step_limit;
        } else {
            player_state.forward_velocity = forward;
        }
    } else if (player_state.flags_140.low & PADLdown) {
        forward = player_state.forward_velocity - (player_state.movement_step_limit >> 2);
        if (forward >= -player_state.movement_step_limit) {
            player_state.forward_velocity = forward;
        } else {
            player_state.forward_velocity = -player_state.movement_step_limit;
        }
    } else if (player_state.forward_velocity > 0) {
        player_state.forward_velocity -= player_state.movement_step_limit >> 3;
        if (player_state.forward_velocity < 0) {
            player_state.forward_velocity = 0;
        }
    } else if (player_state.forward_velocity < 0) {
        player_state.forward_velocity += player_state.movement_step_limit >> 3;
        if (player_state.forward_velocity > 0) {
            player_state.forward_velocity = 0;
        }
    }

    if (player_state.flags_140.low & PADR1) {
        strafe = player_state.strafe_velocity + (player_state.movement_step_limit >> 2);
        if (strafe > player_state.movement_step_limit) {
            player_state.strafe_velocity = player_state.movement_step_limit;
        } else {
            player_state.strafe_velocity = strafe;
        }
    } else if (player_state.flags_140.low & PADL1) {
        strafe = player_state.strafe_velocity - (player_state.movement_step_limit >> 2);
        if (strafe >= -player_state.movement_step_limit) {
            player_state.strafe_velocity = strafe;
        } else {
            player_state.strafe_velocity = -player_state.movement_step_limit;
        }
    } else if (player_state.strafe_velocity > 0) {
        player_state.strafe_velocity -= player_state.movement_step_limit >> 2;
        if (player_state.strafe_velocity < 0) {
            player_state.strafe_velocity = 0;
        }
    } else if (player_state.strafe_velocity < 0) {
        player_state.strafe_velocity += player_state.movement_step_limit >> 2;
        if (player_state.strafe_velocity > 0) {
            player_state.strafe_velocity = 0;
        }
    }

    strafe_square = player_state.strafe_velocity;
    strafe_square *= strafe_square;
    forward_square = player_state.forward_velocity;
    forward_square *= forward_square;
    magnitude = SquareRoot0(strafe_square + forward_square);
    if (magnitude == 0) {
        forward = 0;
        strafe = 0;
    } else {
        strafe = strafe_square / magnitude;
        if (player_state.strafe_velocity < 0) {
            strafe = -(strafe_square / magnitude);
        }
        forward = forward_square / magnitude;
        if (player_state.forward_velocity < 0) {
            forward = -(forward_square / magnitude);
        }
    }

    player_state.movement_speed.unsigned_value = SquareRoot0(strafe * strafe + forward * forward);
    if (forward >= 0) {
        player_move_horizontal((s16)player_state.camera_rotation_target.angles[1], forward);
    } else {
        player_move_horizontal(
            ((s16)player_state.camera_rotation_target.angles[1] + KF_ANGLE_HALF_TURN)
                & KF_ANGLE_WRAP_MASK,
            -forward);
    }
    if (strafe > 0) {
        player_move_horizontal(
            ((s16)player_state.camera_rotation_target.angles[1] - KF_ANGLE_QUARTER_TURN)
                & KF_ANGLE_WRAP_MASK,
            strafe);
    } else if (strafe < 0) {
        player_move_horizontal(
            ((s16)player_state.camera_rotation_target.angles[1] + KF_ANGLE_QUARTER_TURN)
                & KF_ANGLE_WRAP_MASK,
            -strafe);
    } else {
        player_state.frame_displacement.vz = 0;
        player_state.frame_displacement.vx = 0;
    }
}

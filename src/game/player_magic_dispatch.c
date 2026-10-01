#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/animation.h>
#include <kf/game/audio.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

extern void func_80026330(s32 mode, VECTOR *output);
extern s32 func_8003a9f4(s32 x, s32 y, s32 z, s32 radius, s32 height);
extern void func_80039c94(s32 actor_index, u16 power, u16 magic_06,
                           u16 magic_08, u16 magic_0a, u16 magic_0c,
                           u16 magic_0e, u16 magic_10, u16 magic_12,
                           u16 magic_14, s32 amount, s32 effect_flags,
                           const VECTOR *position);

RODATA(0x80011260, 0x34)

ADDRESS(0x80026498, 0x1c4)
void func_80026498(s32 magic_id, s32 consume_mp, s32 effect_parameter)
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
        func_80026330(DAT_800667e8.effect_ids[effect_parameter], &position);
        func_80025a18(magic_id, &position);
        break;
    case 11:
    case 12:
        func_80026330(0, &position);
        func_80025a18(magic_id, &position);
        break;
    case 2:
        effect_parameter <<= 9;
        player_state.unknown_118.vx = rcos(effect_parameter) >> 3;
        player_state.unknown_118.vy = rsin(effect_parameter) >> 3;
        player_state.unknown_118.vz = 600;
        func_80025a18(magic_id);
        break;
    case 0:
        for (effect_parameter = 0; effect_parameter < 4095; effect_parameter += 684) {
            player_state.unknown_118.vx = rcos(effect_parameter) >> 3;
            player_state.unknown_118.vy = rsin(effect_parameter) >> 3;
            player_state.unknown_118.vz = 400;
            func_80025a18(magic_id);
        }
        break;
    default:
        player_state.unknown_118.vx = 200;
        player_state.unknown_118.vy = 200;
        player_state.unknown_118.vz = 400;
        func_80025a18(magic_id);
        break;
    }
}

ADDRESS(0x8002665c, 0xbd0)
void func_8002665c(void)
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
    if (weapon_id == 0xff) {
        return;
    }
    goto regular_weapon;

special_weapon: {
        s32 mode;
        phase = player_state.weapon_attack_phase;
        if (phase == -1) {
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
                    player_state.weapon_effect = func_80040308(
                        10, 0x12, effect_kind, &player_state.camera_position,
                        0, &player_state.camera_rotation);
                    effect = player_state.weapon_effect;
                    if (effect != 0) {
                        effect->phase = 99;
                    }
                } else {
                    player_state.weapon_effect = 0;
                }
            }

            player_state.weapon_attack_phase += weapon->attack_phase_step;
            if (player_state.weapon_attack_phase >= 4095) {
                player_state.weapon_attack_phase = 4095;
            }
            if (player_state.weapon_attack_phase >= weapon->unknown_1e) {
                if (player_state.attack_charge_current == 0
                    && player_state.equipped_weapon_id == 16) {
                    audio_play_sound(3, 110);
                }
                player_state.attack_charge_current =
                    ((player_state.weapon_attack_phase - weapon->unknown_1e) * 5000)
                    / (4095 - weapon->unknown_1e);
            } else {
                player_state.attack_charge_current = 0;
            }

            effect = player_state.weapon_effect;
            if (effect != 0) {
                rotation.x = player_state.camera_rotation.angles[0] - weapon->rotation_offset_x;
                rotation.y = player_state.camera_rotation.angles[1] - weapon->rotation_offset_y;
                rotation.z = player_state.camera_rotation.angles[2] + weapon->rotation_offset_z;
                func_80034344(32, player_state.weapon_attack_mode,
                               player_state.weapon_attack_phase,
                               weapon->initial_vertex_index, &initial_vertex);
                initial_vertex.vx -= weapon->position_offset_x;
                initial_vertex.vy += weapon->position_offset_y;
                initial_vertex.vz -= weapon->position_offset_z;
                vector_rotate_yxz(&rotation, &initial_vertex, &world_position);
                effect->position.vx = player_state.camera_position.vx + world_position.vx;
                effect->position.vz = player_state.camera_position.vz + world_position.vz;
                effect->position.vy = player_state.camera_position.vy + world_position.vy
                                    + player_state.unknown_134 + player_state.unknown_138 - 1600;

                func_80034344(32, player_state.weapon_attack_mode,
                               player_state.weapon_attack_phase,
                               weapon->final_vertex_index, &initial_vertex);
                initial_vertex.vx -= weapon->position_offset_x;
                initial_vertex.vy += weapon->position_offset_y;
                initial_vertex.vz -= weapon->position_offset_z;
                vector_rotate_yxz(&rotation, &initial_vertex, &last_world);
                func_800154fc(world_position.vx - last_world.vx,
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
                vector3s_scale_shift12((player_state.attack_charge_current * 900) / 5000,
                                       &effect->direction);
                effect->updates_remaining = 50;
                *(u16 *)&effect->unknown_3c[4] = effect->position.vy;
            }
            player_state.weapon_attack_mode = 1;
            player_state.weapon_attack_phase = 0;
            return;
        }
special_mode_one: {
            phase += 400;
            player_state.weapon_attack_phase = phase;
            if (phase >= 4095) {
                player_state.weapon_attack_phase = -1;
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

    if (phase == -1) {
        goto regular_idle;
    }

    if (player_state.weapon_attack_mode == 0) {
        phase_step = weapon->attack_phase_step;
        phase_end = weapon->unknown_1e;
        sound_end = weapon->unknown_2c;
        sound_step = 0;
        hit_step = 0;
    } else {
        phase_step = weapon->unknown_24;
        phase_end = weapon->unknown_28;
        sound_end = weapon->unknown_30;
        sound_step = weapon->release_phase_step;
        hit_step = weapon->magic_phase_step;
    }
    player_state.weapon_attack_phase += phase_step;

    if (player_state.weapon_attack_mode == 0
        && weapon->initial_effect_id != 0xff
        && player_state.weapon_attack_fully_charged != 0
        && player_has_power_and_magic_60() != 0
        && (player_state.flags_140.low & 0x80) != 0) {
        if (player_state.weapon_attack_phase >= weapon->magic_window_start
            && player_state.weapon_attack_phase <= weapon->magic_window_end) {
            if (player_state.weapon_magic_shots_configured != 0) {
                func_80026498(weapon->initial_effect_id,
                               player_state.weapon_magic_shots_configured == weapon->magic_shots,
                               player_state.weapon_magic_shots_configured);
                player_state.weapon_magic_shots_configured--;
            }
        } else {
            player_state.weapon_magic_shots_configured = 0;
        }
    }

    if (player_state.weapon_attack_recovery <= player_state.weapon_attack_phase
        && player_state.weapon_attack_phase
             < player_state.weapon_attack_recovery + phase_step) {
        audio_play_sound(weapon->sound_id, 80);
        if (player_state.weapon_attack_recovery >= sound_end) {
            player_state.weapon_attack_recovery = 5000;
        } else {
            player_state.weapon_attack_recovery += sound_step;
        }
    }

    if (player_state.weapon_attack_phase >= player_state.weapon_attack_window
        && player_state.weapon_attack_phase
             < player_state.weapon_attack_window + phase_step) {
        player_state.unknown_a0 = 0;
        if (player_state.weapon_attack_mode == 1) {
            if (player_state.equipped_weapon_id == 13
                && (player_state.flags_140.low & 0x80) != 0) {
                player_state.weapon_attack_phase -= phase_step;
                player_state.unknown_a0 = 1;
                return;
            }
            if (weapon->release_effect_id != 0xff) {
                func_80026498(weapon->release_effect_id,
                               player_state.weapon_attack_phase >= phase_end,
                               (player_state.weapon_attack_phase - weapon->unknown_26)
                                   / hit_step);
            }
        }

        if (player_state.weapon_attack_phase >= phase_end) {
            player_state.weapon_attack_window = 5000;
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
            index = func_8003a9f4(world_position.vx, world_position.vy,
                                   world_position.vz, 400, 600);
            if (index != -1) {
                KfActor *actor = &actor_state.actors[index];
                KfTargetGroup *group = &actor_state.target_groups[actor->group_index];
                s32 bearing = vector_xz_to_angle(actor->position.vx - player_state.camera_position.vx,
                                                  actor->position.vz - player_state.camera_position.vz);
                if (angle_within_tolerance(player_state.camera_rotation.angles[1],
                                           bearing, group->unknown_16)
                    && angle_within_tolerance(actor->rotation.y,
                                              bearing + 0x800, group->unknown_18)) {
                    func_80039c94(index, player_state.physical_power,
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

    if (player_state.weapon_attack_phase > 4095) {
        player_state.weapon_attack_phase = -1;
        player_state.weapon_magic_shots_configured = 0;
    }
    return;

regular_idle:
    if ((player_state.flags_140.low & 0x10) == 0) {
        if (player_state.weapon_charge_delay == 0) {
            s32 gain = func_80023814(player_state.physical_power,
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
            if (player_state.attack_charge_current > 5000) {
                player_state.attack_charge_current = 5000;
            }
        } else {
            player_state.weapon_charge_delay--;
        }
    }
}

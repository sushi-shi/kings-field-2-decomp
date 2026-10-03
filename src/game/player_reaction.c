#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <psyq/libc.h>
#include <psyq/sdk.h>
#include <kf/lib/types.h>
#include <kf/game/notify.h>

DATA(0x8006d6b0, 0x20)
static RECT player_status_texture_rows[4] = {
    {0x240, 0x119, 16, 1},
    {0x240, 0x117, 16, 1},
    {0x240, 0x11a, 16, 1},
    {0x240, 0x118, 16, 1}
};

RODATA(0x80011300, 0x4c)

ADDRESS(0x8002897c, 0x1c)
s32 item_id_is_71_to_80(s32 value)
{
    s32 result = 0;
    if (value < 81) {
        result = value >= 71;
    }
    return result;
}

ADDRESS(0x80028998, 0x528)
void player_update_actions_and_charge(void)
{
    const u16 *attack_mask;
    s32 charge_gain;
    u8 timer;

    if (player_state.weapon_magic_shots_configured == 0
        && (player_state.flags_140.word & 0x00800080) == 0x80
        && player_state.weapon_magic_shots_remaining == 0) {
        player_select_magic_action(player_state.primary_magic_shortcut_id);
    }

    if ((player_state.flags_140.word & 0x08000800) == 0x800) {
        if (player_state.secondary_magic_shortcut_id != 0xff) {
            player_select_magic_action(player_state.secondary_magic_shortcut_id);
        }
        if (player_state.secondary_item_shortcut_id != 0xff) {
            if (game_counter_bytes[player_state.secondary_item_shortcut_id] != 0) {
                if (item_id_is_71_to_80(player_state.secondary_item_shortcut_id) != 0) {
                    menu_apply_item_effect(player_state.secondary_item_shortcut_id);
                } else {
                    event_scene_command_dispatch(&player_state.camera_position,
                                  &player_state.camera_rotation_target,
                                  player_state.secondary_item_shortcut_id);
                }
            } else {
                notify_enqueue(20);
            }
        }
    }

    if (player_state.queued_magic_action.magic_id != 0xff) {
        timer = player_state.queued_magic_action.countdown - 1;
        player_state.queued_magic_action.countdown = timer;
        if (timer == 0) {
            player_dispatch_magic_effect(player_state.queued_magic_action.magic_id);
            timer = player_state.queued_magic_action.casts_remaining - 1;
            player_state.queued_magic_action.casts_remaining = timer;
            if (timer == 0) {
                player_state.queued_magic_action.magic_id = 0xff;
            } else {
                player_state.queued_magic_action.countdown = player_state.queued_magic_action.repeat_interval;
            }
        }
    }

    if ((player_state.flags_140.word & 0x00200020) == 0x00200020
        && player_state.equipped_shield_id != 50) {
        if (player_state.movement_speed_adjustment_decay_latch != 0) {
            player_state.movement_speed_adjustment_decay_latch--;
        }
        player_state.weapon_charge_delay = 1;
        player_state.attack_charge_current -= 500;
        if ((s16)player_state.attack_charge_current <= 0) {
            player_state.attack_charge_current = 0;
            player_state.weapon_charge_delay = 40;
        }
        player_state.magic_charge -= 500;
        if ((s16)player_state.magic_charge <= 0) {
            player_state.magic_charge = 0;
        }
        player_state.damage_scale -= 128;
        if (player_state.damage_scale < 1024) {
            player_state.damage_scale = 1024;
        }
        return;
    }

    if (player_state.selected_magic_record == 0) {
        player_state.magic_charge += player_charge_gain_for_rank(player_state.magic, 0);
    } else {
        charge_gain = player_charge_gain_for_rank(player_state.magic,
                                    player_state.selected_magic_record->charge_rate);
        if (player_state.equipped_head_id == 24) {
            charge_gain >>= 1;
        }
        player_state.magic_charge += charge_gain;
    }
    if (player_state.magic_charge > KF_PLAYER_CHARGE_FULL) {
        player_state.magic_charge = KF_PLAYER_CHARGE_FULL;
    }
    player_state.damage_scale += 128;
    if (player_state.damage_scale > 4096) {
        player_state.damage_scale = 4096;
    }
    player_state.movement_speed_adjustment_decay_latch = 1;

    if (player_state.weapon_magic_shots_remaining != 0) {
        player_state.weapon_magic_shots_remaining--;
    } else {
        player_state.magic_attack_mask_cursor = player_magic_id_sequence.attack_masks;
    }

    if ((player_state.flags_140.low & 0xb0) != 0
        && (player_state.flags_140.halves.high & 0xb0) == 0
        && player_state.equipped_weapon_record->alternate_attack_phase_step != 0) {
        if (player_meets_weapon_magic_power_requirement() == 0) {
            goto cancel_weapon_attack;
        }
        attack_mask = player_state.magic_attack_mask_cursor;
        if ((player_state.flags_140.low & attack_mask[0]) == 0) {
            goto cancel_weapon_attack;
        }
        if (attack_mask == player_magic_id_sequence.attack_masks
            && (player_state.attack_charge_current != KF_PLAYER_CHARGE_FULL
                || player_state.magic_charge != KF_PLAYER_CHARGE_FULL)) {
            goto cancel_weapon_attack;
        }
        player_state.magic_attack_mask_cursor = attack_mask + 1;
        if (attack_mask[1] == 0xffff) {
            player_begin_weapon_attack(1);
            return;
        }
        player_state.weapon_magic_shots_remaining = 20;
        goto after_weapon_attack;
cancel_weapon_attack:
        player_state.weapon_magic_shots_remaining = 0;
    }

after_weapon_attack:
    if (player_state.weapon_charge_delay == 0
        && player_state.weapon_magic_shots_remaining == 0
        && (player_state.flags_140.word & 0x00100010) == 0x10) {
        player_begin_weapon_attack(0);
    }
}

ADDRESS(0x80028ec0, 0xe8)
void player_update_reaction_rotation_offsets(void)
{
    player_state.reaction.damage.motion.vx = angle_velocity_step(
        0, (s16)player_state.reaction_rotation_offset[0], player_state.reaction.damage.motion.vx, 8, 4);
    player_state.reaction.damage.motion.vy = angle_velocity_step(
        0, (s16)player_state.reaction_rotation_offset[1], player_state.reaction.damage.motion.vy, 8, 4);
    player_state.reaction.damage.motion.vz = angle_velocity_step(
        0, (s16)player_state.reaction_rotation_offset[2], player_state.reaction.damage.motion.vz, 8, 4);

    player_state.reaction_rotation_offset[0] += player_state.reaction.damage.motion.vx;
    player_state.reaction_rotation_offset[1] += player_state.reaction.damage.motion.vy;
    player_state.reaction_rotation_offset[2] += player_state.reaction.damage.motion.vz;
}

ADDRESS(0x80028fa8, 0x6c)
void player_render_frame_and_release_pool(void)
{
    u8 saved_hud_gauges = player_state.hud_gauges_enabled;
    u8 saved_compass = player_state.compass_enabled;

    player_state.hud_gauges_enabled = 0;
    player_state.compass_enabled = 0;
    render_game_frame(0, 0);
    player_state.hud_gauges_enabled = saved_hud_gauges;
    player_state.compass_enabled = saved_compass;
    pool_release_all();
}

ADDRESS(0x80029014, 0x154)
void player_handle_interaction_and_menu(void)
{
    s32 value;

    if ((player_state.flags_140.word & 0x00200020) == 0x20) {
        if (player_state.death_state == KF_PLAYER_REACTION_MAP_OBJECT_FOLLOW) {
            player_apply_map_object_reaction(
                &map_object_state.objects[player_state.reaction.view.map_object_index]);
        } else {
            event_world_dispatch_interaction(&player_state.camera_position,
                                             &player_state.camera_rotation_target);
        }
    }
    if ((player_state.flags_140.word & 0x00400040) != 0x40
        || player_state.weapon_attack_phase != -1) {
        return;
    }

    player_render_frame_and_release_pool();
    value = menu_run_root_controller();
    if (value >= 0) {
        if (item_id_is_71_to_80(value) == 0) {
            render_game_frame(0, 0);
            event_scene_command_dispatch(&player_state.camera_position,
                                         &player_state.camera_rotation_target,
                                         value);
        }
    } else if (value == -3) {
        s32 resource;
        player_restore_equipment_effects();
        resource = state_8017d118.active_resource_ids[0];
        player_reload_map_resources(resource, resource, resource, resource, resource, 255);
    }
    player_clear_motion();
}

ADDRESS(0x80029168, 0x68)
void player_reset_reaction_state(void)
{
    player_state.death_state = KF_PLAYER_REACTION_NORMAL;
    player_state.forward_velocity = 0;
    player_state.strafe_velocity = 0;
    player_state.reaction_rotation_offset[2] = 0;
    player_state.reaction_rotation_offset[1] = 0;
    player_state.reaction_rotation_offset[0] = 0;
    player_state.view_rotation_offset.components[2] = 0;
    player_state.view_rotation_offset.components[1] = 0;
    player_state.view_rotation_offset.components[0] = 0;
    player_state.camera_yaw_roll_offsets[1] = 0;
    player_state.camera_yaw_roll_offsets[0] = 0;
    player_state.vertical_motion_pitch_offset = 0;
}

ADDRESS(0x800291d0, 0x1c)
void player_begin_map_object_view_follow(u8 map_object_index)
{
    player_state.death_state = KF_PLAYER_REACTION_MAP_OBJECT_FOLLOW;
    player_state.reaction.view.map_object_index = map_object_index;
}

struct KfMapObjectRecord40 {
    u8 unknown_00;
    u8 reaction_mode;
    u8 unknown_02[0x0a];
    SVECTOR reaction_rotation_vector;
    u8 unknown_14[0x24];
    s16 reaction_rotation_scale_q15;
};

typedef char kf_player_map_object_vector_offset[
    (u32)&((KfMapObjectRecord40 *)0)->reaction_rotation_vector == 0x0c ? 1 : -1];
typedef char kf_player_map_object_reaction_mode_offset[
    (u32)&((KfMapObjectRecord40 *)0)->reaction_mode == 0x01 ? 1 : -1];
typedef char kf_player_map_object_halfword_offset[
    (u32)&((KfMapObjectRecord40 *)0)->reaction_rotation_scale_q15 == 0x38 ? 1 : -1];

ADDRESS(0x800291ec, 0x1e8)
void player_apply_map_object_reaction(KfMapObject *object)
{
    KfMapObjectRecord40 *record;

    if (player_state.death_state != KF_PLAYER_REACTION_MAP_OBJECT_FOLLOW) {
        return;
    }
    if (player_state.reaction.view.map_object_index != object - map_object_state.objects) {
        return;
    }

    record = object->extra_40.record;
    player_state.camera_rotation_target.angles[0] += player_state.view_rotation_offset.components[0];
    player_state.camera_rotation_target.angles[1] += player_state.view_rotation_offset.components[1];
    player_state.camera_rotation_target.angles[2] += player_state.view_rotation_offset.components[2];
    player_state.view_rotation_offset.components[0] = 0;
    player_state.view_rotation_offset.components[1] = 0;
    player_state.view_rotation_offset.components[2] = 0;

    if (record->reaction_mode == 0) {
        SVECTOR rotation;
        rotation.vx = (record->reaction_rotation_vector.vx * record->reaction_rotation_scale_q15) >> 15;
        rotation.vy = ((record->reaction_rotation_vector.vy * record->reaction_rotation_scale_q15) >> 15) + 512;
        rotation.vz = (record->reaction_rotation_vector.vz * record->reaction_rotation_scale_q15) >> 15;
        player_begin_rotation_reaction(&rotation);
    } else {
        struct KfVecXZi offset;
        angle_to_forward_xz(object->rotation.vy + 1024, &offset);
        vector2i_scale_shift11(900, &offset);
        player_state.death_state = KF_PLAYER_REACTION_POSITION_RECOVERY;
        player_state.reaction.position.recovery_step = 0;
        player_state.reaction.position.position.vx = object->position.vx + offset.x;
        player_state.reaction.position.position.vz = object->position.vz + offset.z;
        player_state.reaction.position.position.vy = object->position.vy;
    }
}

ADDRESS(0x800293d4, 0x54)
void player_begin_view_reaction(u8 map_object_index)
{
    player_state.death_state = KF_PLAYER_REACTION_MAP_OBJECT_APPROACH;
    player_state.reaction.view.map_object_index = map_object_index;
    player_state.reaction.view.approach_step = 0;
    player_state.reaction.view.rotation = player_state.camera_rotation_target;
}

ADDRESS(0x80029428, 0x3c)
void player_begin_rotation_reaction(const SVECTOR *rotation)
{
    player_state.death_state = KF_PLAYER_REACTION_ROTATION;
    player_state.reaction.damage.rotation = *rotation;
}

ADDRESS(0x80029464, 0x94)
void player_begin_moving_damage_reaction(const SVECTOR *rotation, const SVECTOR *motion, s16 duration)
{
    player_state.death_state = KF_PLAYER_REACTION_MOVING_DAMAGE;
    player_state.reaction.damage.rotation = *rotation;
    player_state.reaction.damage.motion = *motion;
    player_state.damage_red_overlay_scale = 3500;
    player_state.damage_red_overlay_decay = duration;
    player_state.vertical_motion_state = KF_PLAYER_VERTICAL_DEEP_FALL;
    player_state.vertical_velocity = player_state.reaction.damage.rotation.vy;
}

ADDRESS(0x800294f8, 0x78)
void player_begin_rotation_only_damage_reaction(const SVECTOR *rotation, const SVECTOR *motion, s16 duration)
{
    player_state.death_state = KF_PLAYER_REACTION_ROTATION_DAMAGE;
    player_state.reaction.damage.rotation = *rotation;
    player_state.reaction.damage.motion = *motion;
    player_state.damage_red_overlay_scale = 3500;
    player_state.damage_red_overlay_decay = duration;
}

enum {
    KF_PLAYER_DEATH_SOUND = 0xb,
    KF_PLAYER_DEATH_VOLUME = 110
};

ADDRESS(0x80029570, 0x88)
void player_death_begin(const SVECTOR *rotation)
{
    if (player_state.death_state != KF_PLAYER_REACTION_DEATH) {
        player_state.death_state = KF_PLAYER_REACTION_DEATH;
        audio_play_sound(KF_PLAYER_DEATH_SOUND, KF_PLAYER_DEATH_VOLUME);
        if (rotation != NULL) {
            player_state.reaction.damage.rotation = *rotation;
        }
        player_state.reaction.damage.motion.vx = 0;
        player_state.death_transition_frame = 0;
    }
}

ADDRESS(0x800295f8, 0x2c)
void player_begin_actor_overlap_bob(void)
{
    if (player_state.death_state == KF_PLAYER_REACTION_NORMAL) {
        player_state.death_state = KF_PLAYER_REACTION_OVERLAP_BOB;
        player_state.reaction.damage.rotation.vx = 0;
    }
}

ADDRESS(0x80029624, 0xc4)
s16 status_phase_step_scaled(u16 *phase, u16 *secondary, s32 duration, s32 scale)
{
    s16 current = *phase;
    s16 other = 0;
    s16 result;

    if (secondary != NULL) {
        other = *secondary;
    }
    if (current != 0 || other != 0) {
        if (current < other) {
            current++;
        } else {
            other = 0;
            current--;
        }
        if (current < duration) {
            result = current * scale / duration;
        } else {
            result = scale;
        }
        *phase = current;
        if (secondary != NULL) {
            *secondary = other;
        }
        return (s16)result;
    }
    return -1;
}

ADDRESS(0x800296e8, 0x74)
void player_adjust_hp(s32 delta)
{
    s32 value = player_state.vitals.current_hp;

    value += delta;

    if (value <= 0) {
        player_state.vitals.current_hp = 0;
        player_death_begin(NULL);
        return;
    }
    if (player_state.vitals.maximum_hp < value) {
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
    } else {
        player_state.vitals.current_hp = value;
    }
}

ADDRESS(0x8002975c, 0x58)
void player_adjust_mp(s32 delta)
{
    s32 value = player_state.vitals.current_mp;

    value += delta;

    if (value <= 0) {
        player_state.vitals.current_mp = 0;
        return;
    }
    if (player_state.vitals.maximum_mp < value) {
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;
    } else {
        player_state.vitals.current_mp = value;
    }
}

ADDRESS(0x800297b4, 0xa8)
void player_apply_equipment_hp_tick(const KfEquipmentRecord *equipment)
{
    if (equipment->hp_regen_interval != 0
        && player_state.equipment_effect_ticks % equipment->hp_regen_interval == 0) {
        player_adjust_hp(1);
    }
    if (equipment->hp_drain_interval != 0
        && player_state.equipment_effect_ticks % equipment->hp_drain_interval == 0) {
        player_adjust_hp(-1);
    }
}

ADDRESS(0x8002985c, 0x112c)
void player_update_frame(void)
{
    s16 value;
    s32 index;
    s32 fraction;
    u8 object_index;
    u8 step;
    KfMapObject *object;

    actor_state.actor_overlap_exclusion_flags = 4;
    map_cell_add_layer_occupancy(player_state.camera_position.vx,
                   player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS, -1);
    value = status_phase_step_scaled((u16 *)&player_state.darkness_phase,
                           &player_state.darkness_phase_limit, 64, 0xc00);
    if (value != -1) {
        interpolate_collision_filter_rows(10, 10, 10, 0xef9, value);
    }
    value = status_phase_step_scaled((u16 *)&player_state.magic_tint_phase,
                           (u16 *)&player_state.magic_tint_phase_limit, 64, 0xe10);
    if (value != -1) {
        interpolate_collision_filter_rows(220, 220, 160, 18000, value);
    }

    if (player_state.damage_red_overlay_scale != 0) {
        player_state.damage_red_overlay_scale -= player_state.damage_red_overlay_decay;
        if (player_state.damage_red_overlay_scale < 0) {
            player_state.damage_red_overlay_scale = 0;
        }
        value = player_state.damage_red_overlay_scale;
        if (value > 4095) {
            value = 4096;
        }
        accumulate_color_overlay(60, 0, 0, value);
    }

    player_state.flags_140.low = PadRead(1);
    if (player_state.flags_140.low & 0x800) {
        cd_report_error(3);
    }
    if (player_state.flags_140.low & 0x100) {
        player_state.flags_140.low = 0x40;
    }

    player_state.movement_step_limit = 200;
    player_state.turn_step_limit = 28;
    if ((player_state.flags_140.low & 0x5000) == 0) {
        player_state.turn_step_limit = 35;
    }
    if (player_state.slow_timer != 0) {
        if (player_state.slow_timer < 64) {
            player_state.movement_speed_adjustment_q12 += 100;
            if ((s16)player_state.movement_speed_adjustment_q12 > 0) {
                player_state.movement_speed_adjustment_q12 = 0;
            }
        } else {
            player_state.movement_speed_adjustment_q12 -= 100;
            if ((s16)player_state.movement_speed_adjustment_q12 < -3300) {
                player_state.movement_speed_adjustment_q12 = -3300;
            }
        }
        player_state.turn_step_limit >>= 1;
        player_state.slow_timer--;
    } else {
        if (player_state.movement_speed_adjustment_decay_latch == 0) {
            player_state.movement_speed_adjustment_q12 += 800;
            if ((s16)player_state.movement_speed_adjustment_q12 > 2800) {
                player_state.movement_speed_adjustment_q12 = 2800;
            }
        } else {
            player_state.movement_speed_adjustment_q12 -= 800;
            if ((s16)player_state.movement_speed_adjustment_q12 < 0) {
                player_state.movement_speed_adjustment_q12 = 0;
            }
        }
    }
    player_state.movement_step_limit +=
        ((s16)player_state.movement_speed_adjustment_q12 * player_state.movement_step_limit) >> 12;
    if (player_state.paralysis_timer != 0) {
        player_state.movement_step_limit = 0;
        player_state.turn_step_limit = 0;
        player_state.paralysis_timer--;
        if ((player_state.paralysis_timer & 3) == 0) {
            accumulate_color_overlay(60, 30, 0, 0xc00);
        }
    }
    if (player_state.weapon_guard_active != 0) {
        player_state.movement_step_limit >>= 1;
        player_state.turn_step_limit >>= 1;
    }

    switch (player_state.death_state) {
    case KF_PLAYER_REACTION_NORMAL:
        player_update_actions_and_charge();
        player_update_camera_rotation();
        player_update_horizontal_motion();
        goto update_reaction_pose;
    case KF_PLAYER_REACTION_MAP_OBJECT_FOLLOW:
        object_index = player_state.reaction.view.map_object_index;
        object = &map_object_state.objects[object_index];
        player_update_actions_and_charge();
        player_update_camera_rotation();
        player_state.frame_displacement.vx = (s16)object->position.vx
                                - (s16)player_state.camera_position.vx;
        player_state.frame_displacement.vy = (s16)object->position.vy
                                - (s16)player_state.camera_position.vy;
        player_state.frame_displacement.vz = (s16)object->position.vz
                                - (s16)player_state.camera_position.vz;
        player_state.camera_position = object->position;
        player_state.view_rotation_offset.vector = object->rotation;
update_reaction_view:
        player_handle_interaction_and_menu();
        goto after_reaction;
    case KF_PLAYER_REACTION_MAP_OBJECT_APPROACH:
        object_index = player_state.reaction.view.map_object_index;
        object = &map_object_state.objects[object_index];
        player_update_actions_and_charge();
        fraction = player_state.reaction.view.approach_step << 7;
        player_state.camera_position.vx = fixed_lerp_q12(
            player_state.camera_position.vx, object->position.vx, fraction);
        player_state.camera_position.vy = fixed_lerp_q12(
            player_state.camera_position.vy, object->position.vy, fraction);
        player_state.camera_position.vz = fixed_lerp_q12(
            player_state.camera_position.vz, object->position.vz, fraction);
        player_state.view_rotation_offset.components[0] = angle_lerp_shortest_q12(0, object->rotation.vx, fraction);
        player_state.view_rotation_offset.components[1] = angle_lerp_shortest_q12(0, object->rotation.vy, fraction);
        player_state.view_rotation_offset.components[2] = angle_lerp_shortest_q12(0, object->rotation.vz, fraction);
        player_state.camera_rotation_target.angles[0] = angle_lerp_shortest_q12(
            player_state.reaction.view.rotation.angles[0], 0, fraction);
        player_state.camera_rotation_target.angles[1] = angle_lerp_shortest_q12(
            player_state.reaction.view.rotation.angles[1], 0, fraction);
        player_state.camera_rotation_target.angles[2] = angle_lerp_shortest_q12(
            player_state.reaction.view.rotation.angles[2], 0, fraction);
        step = player_state.reaction.view.approach_step++;
        if (step > 31) {
            player_begin_map_object_view_follow(player_state.reaction.view.map_object_index);
        }
        goto after_reaction;
    case KF_PLAYER_REACTION_POSITION_RECOVERY:
        ++player_state.reaction.position.recovery_step;
        fraction = player_state.reaction.position.recovery_step << 8;
        player_state.camera_position.vx = fixed_lerp_q12(
            player_state.camera_position.vx,
            player_state.reaction.position.position.vx, fraction);
        player_state.camera_position.vy = fixed_lerp_q12(
            player_state.camera_position.vy,
            player_state.reaction.position.position.vy, fraction);
        player_state.camera_position.vz = fixed_lerp_q12(
            player_state.camera_position.vz,
            player_state.reaction.position.position.vz, fraction);
        if (player_state.reaction.position.recovery_step > 15) {
            player_reset_reaction_state();
        }
        goto after_reaction;
    case KF_PLAYER_REACTION_ROTATION:
        if (player_move_reaction_with_collision() != 0) {
            player_reset_reaction_state();
        }
        goto update_reaction_view;
    case KF_PLAYER_REACTION_OVERLAP_BOB: {
        u16 angle_phase;

        player_update_actions_and_charge();
        player_update_camera_rotation();
        player_update_horizontal_motion();
        player_state.camera_vertical_offset = 0;
        player_update_vertical_motion();
        player_state.camera_vertical_offset += -256
                                   + (s16)(rcos((s16)player_state.reaction.angle_phase) >> 4);
        player_state.view_rotation_offset.components[2] = rsin((s16)player_state.reaction.angle_phase) >> 6;
        angle_phase = (player_state.reaction.angle_phase + 128) & 0xfff;
        player_state.reaction.angle_phase = angle_phase;
        if (angle_phase == 0) {
            player_state.view_rotation_offset.components[2] = 0;
            player_reset_reaction_state();
        }
        goto update_reaction_view;
    }
    case KF_PLAYER_REACTION_MOVING_DAMAGE:
        player_update_actions_and_charge();
        player_update_camera_rotation();
        player_update_horizontal_motion();
        player_state.reaction.damage.rotation.vy = 1;
        player_move_reaction_with_collision();
        player_update_reaction_rotation_offsets();
        if (player_state.reaction_rotation_offset[0] == 0
            && player_state.reaction_rotation_offset[1] == 0
            && player_state.reaction_rotation_offset[2] == 0) {
            player_reset_reaction_state();
        }
update_reaction_pose:
        player_update_vertical_motion();
        goto update_reaction_view;
    case KF_PLAYER_REACTION_ROTATION_DAMAGE:
        player_update_reaction_rotation_offsets();
        if (player_state.reaction_rotation_offset[0] == 0
            && player_state.reaction_rotation_offset[1] == 0
            && player_state.reaction_rotation_offset[2] == 0) {
            player_reset_reaction_state();
        }
        goto update_reaction_view;
    case KF_PLAYER_REACTION_DEATH:
        player_state.vitals.current_hp = 0;
        value = angle_velocity_step(-1024, player_state.reaction_rotation_offset[0],
                                    player_state.reaction.damage.motion.vx, 8, 4);
        player_state.reaction.damage.motion.vx = value;
        player_state.reaction_rotation_offset[0] += (value * 3) >> 1;
        value = player_state.camera_vertical_offset;
        player_state.camera_vertical_offset = value < 1500 ? value + 500 : 1500;
        player_move_reaction_with_collision();
        player_state.death_transition_frame++;
        if (player_state.death_transition_frame == 31
            && player_state.collision_lower_clearance > -1001
            && player_state.collision_upper_clearance >= 0
            && player_state.fatal_fall_latch == 0
            && (player_state.equipped_accessory_id == 54
                || player_state.equipped_extra_id == 54)
            && game_counter_bytes[0x53] != 0) {
            game_counter_bytes[0x53]--;
            render_frames_with_color_overlay(1, 0, 4096, 512);
            player_reset_status();
            render_frames_with_color_overlay(1, 4096, 0, -512);
        }
        if (player_state.death_transition_frame > 31) {
            if (player_state.death_transition_frame < 65) {
                s32 shade = fixed_lerp_q12(0, 255,
                                            (player_state.death_transition_frame - 32) * 128);
                render_set_color_overlay(0x82, shade, shade, shade);
            } else {
                KfEffectRecord *effect = effect_state.records;
                for (index = KF_EFFECT_CAPACITY; index != 0; index--) {
                    effect->type = 0xff;
                    effect++;
                }
                if (game_counter_bytes[0x4c] != 0
                    && (event_state.control.fields.post_death_reload_flags &
                        KF_EVENT_POST_DEATH_RELOAD_ENABLED) != 0) {
                    actor_disable_type3_transition_actors();
                    player_state.camera_rotation_target.angles[0] = 0;
                    player_state.camera_rotation_target.angles[1] = 0xc00;
                    player_state.camera_rotation_target.angles[2] = 0;
                    player_state.camera_position.vy = -0x2480;
                    player_state.camera_position.vx = 0x1e000;
                    player_state.camera_position.vz = 0x22000;
                    player_state.map_layer_index = 5;
                    game_counter_bytes[0x4c]--;
                    event_world_state_save_slot(state_8017d118.active_resource_ids[0]);
                    player_reset_status();
                    player_reload_map_resources(1, 1, 1, 1, 1, 0x43);
                } else {
                    player_initialize_state();
                    event_state_initialize();
                    player_reload_map_resources(0, 0, 0, 0, 0, 255);
                }
            }
        }
        goto after_reaction;
    default:
        goto after_reaction;
    }

after_reaction:
    player_state.flags_140.halves.high = player_state.flags_140.low;
    if (player_state.poison_timer != 0) {
        if (player_state.poison_timer % 30 == 0) {
            player_state.damage_red_overlay_scale = 2400;
            player_state.damage_red_overlay_decay = 60;
            player_adjust_hp_unclamped(-1);
        }
        player_state.poison_timer--;
    }
    if (player_state.defense_boost_timer != 0 && --player_state.defense_boost_timer == 0) {
        player_recalculate_combat_stats();
    }
    if (player_state.attack_boost_timer != 0 && --player_state.attack_boost_timer == 0) {
        player_recalculate_combat_stats();
    }
    if (player_state.full_mp_timer != 0) {
        player_state.full_mp_timer--;
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;
        if (player_state.full_mp_timer == 0) {
            notify_enqueue(34);
        }
    }
    if (player_state.magic_boost_timer != 0 && --player_state.magic_boost_timer == 0) {
        player_recalculate_combat_stats();
        notify_enqueue(34);
    }
    if (player_state.map_marker_visual_effect_timer != 0) {
        if (player_state.map_marker_visual_effect_timer == 1) {
            MoveImage(&player_status_texture_rows[0], 0x240, 0x103);
            MoveImage(&player_status_texture_rows[2], 0x240, 0x106);
            notify_enqueue(34);
            player_state.map_marker_visual_effect_timer = 0;
            map_object_refresh_cell_markers(0);
        } else {
            if ((player_state.map_marker_visual_effect_timer & 7) == 0) {
                MoveImage(&player_status_texture_rows[1], 0x240, 0x103);
            }
            if ((player_state.map_marker_visual_effect_timer & 7) == 4) {
                MoveImage(&player_status_texture_rows[3], 0x240, 0x106);
            }
            if ((player_state.map_marker_visual_effect_timer & 7) == 1) {
                map_object_refresh_cell_markers(1);
            }
            player_state.map_marker_visual_effect_timer--;
        }
    }
    value = status_phase_step_scaled((u16 *)&player_state.curse_strength,
                           &player_state.curse_phase_limit, 64, 0xc00);
    if (value != -1) {
        if (player_state.curse_strength == 0) {
            player_recalculate_combat_stats();
        }
        accumulate_color_overlay(0x46, 0x46, 30, value);
    }

    player_state.camera_rotation.angles[0] =
        player_state.camera_rotation_target.angles[0]
        + player_state.reaction_rotation_offset[0] + player_state.view_rotation_offset.components[0]
        + player_state.vertical_motion_pitch_offset;
    player_state.camera_rotation.angles[1] =
        player_state.camera_rotation_target.angles[1]
        + player_state.reaction_rotation_offset[1] + player_state.view_rotation_offset.components[1]
        + player_state.camera_yaw_roll_offsets[0];
    player_state.camera_rotation.angles[2] =
        player_state.camera_rotation_target.angles[2]
        + player_state.reaction_rotation_offset[2] + player_state.view_rotation_offset.components[2]
        + player_state.camera_yaw_roll_offsets[1];
    map_cell_add_layer_occupancy(player_state.camera_position.vx,
                   player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS, 1);
    actor_state.actor_overlap_exclusion_flags = 0;
    index = actor_find_overlap_excluding_target_type3(player_state.camera_position.vx,
                           player_state.camera_position.vy,
                           player_state.camera_position.vz, 1, KF_PLAYER_HEIGHT);
    if (index != -1 && (actor_state.actors[index].unknown_28 & 8) != 0) {
        player_begin_actor_overlap_bob();
    }
    player_update_weapon_attack();
    if (player_state.equipped_weapon_id != KF_EQUIPMENT_NONE) {
        KfWeaponRecordGame *weapon = player_state.equipped_weapon_record;
        if (weapon->hp_regen_interval != 0
            && player_state.equipment_effect_ticks % weapon->hp_regen_interval == 0) {
            player_adjust_hp(1);
        }
        weapon = player_state.equipped_weapon_record;
        if (weapon->mp_regen_interval != 0
            && player_state.equipment_effect_ticks % weapon->mp_regen_interval == 0) {
            player_adjust_mp(1);
        }
    }
    if (player_state.equipped_head_id != KF_EQUIPMENT_NONE) {
        player_apply_equipment_hp_tick(player_state.equipped_head_record);
    }
    if (player_state.equipped_body_id != KF_EQUIPMENT_NONE) {
        player_apply_equipment_hp_tick(player_state.equipped_body_record);
    }
    if (player_state.equipped_arm_id != KF_EQUIPMENT_NONE) {
        player_apply_equipment_hp_tick(player_state.equipped_arm_record);
    }
    if (player_state.equipped_leg_id != KF_EQUIPMENT_NONE) {
        player_apply_equipment_hp_tick(player_state.equipped_leg_record);
    }
    if (player_state.equipped_shield_id != KF_EQUIPMENT_NONE) {
        player_apply_equipment_hp_tick(player_state.equipped_shield_record);
    }
    if (player_state.equipped_accessory_id != KF_EQUIPMENT_NONE) {
        player_apply_equipment_hp_tick(player_state.equipped_accessory_record);
    }
    if (player_state.equipped_extra_id != KF_EQUIPMENT_NONE) {
        player_apply_equipment_hp_tick(player_state.equipped_extra_record);
    }
    if (player_state.equipped_head_id == 25 && rand() < 36) {
        player_state.magic_origin_offset.vx = 0;
        player_state.magic_origin_offset.vy = -300;
        player_state.magic_origin_offset.vz = 400;
        player_dispatch_magic_effect(11);
    }
    if (player_state.equipped_body_id == 31) {
        interpolate_collision_filter_rows(20, 20, 20, 5000, 0x800);
    }
    player_state.equipment_effect_ticks++;
}

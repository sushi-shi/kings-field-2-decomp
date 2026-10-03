#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/event_stream.h>
#include <kf/game/graphics.h>
#include <kf/game/map_object.h>
#include <kf/game/menu.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>
#include <kf/lib/math.h>


#include <stdarg.h>
#include <psyq/sdk.h>

DATA(0x800679a0, 0x8)
u8 event_magic_unlock_ids_5a[8] = {7, 8, 9, 10, 0xff, 0, 0, 0};

DATA(0x800679a8, 0x8)
u8 event_magic_unlock_ids_5b[8] = {14, 15, 0, 13, 0xff, 0, 0, 0};

DATA(0x800679b0, 0x8)
u8 event_magic_unlock_ids_5c[8] = {16, 1, 2, 3, 0xff, 0, 0, 0};

DATA(0x800679b8, 0x8)
u8 event_magic_unlock_ids_5d[8] = {4, 17, 5, 6, 0xff, 0, 0, 0};

DATA(0x800679c0, 0x8)
u8 event_magic_unlock_ids_5e[8] = {18, 19, 11, 12, 0xff, 0xff, 0xff, 0xff};

DATA(0x8009a5e8, 0x78)
u8 game_counter_bytes[0x78];

typedef void (*KfEventCommandCallback)(const VECTOR *position,
                                       const KfPlayerViewRotation *rotation,
                                       s32 command);

RODATA(0x800128d0, 0x8c)

ADDRESS(0x80046700, 0x8c)
void event_spawn_effect_object(KfEventObjectView *event, s32 object_id)
{
    KfMapObject *object = map_object_effect_pool_acquire(0x17c, 0x10, -1);

    event->effect_object_index = (object - map_object_state.objects) - 0x7c;
    object->object_id = object_id;
    object->tail.fields.unknown_38 = 0;
}

ADDRESS(0x8004678c, 0xc54)
void event_scene_command_dispatch(const VECTOR *position,
                                  const KfPlayerViewRotation *rotation,
                                  s32 command)
{
    s32 index;
    s32 object_control_offset;
    const u8 *magic_ids;

    event_state.state_word = 0;
    switch (command) {
    case 0x63:
    case 0x64:
    case 0x65:
    case 0x66:
    case 0x68:
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 0x6d:
        index = 0;
        for (;;) {
            KfMapObject *object;
            s32 status;

            index = map_object_find_interaction_target(index, position, 800, 1700,
                                   rotation->angles[1], 512);
            if (index == -1) {
                break;
            }
            object = &map_object_state.objects[index];
            status = map_object_check_and_consume_marker(object, command);
            switch (status) {
            case 1:
                audio_play_sound_64();
                event_state.state_word = 1;
                goto invoke_callback;
            case 3:
                notify_enqueue(object->tail.notification.linked_notification);
                event_state.state_word = 1;
                goto invoke_callback;
            case 4:
                notify_enqueue(4);
                event_state.state_word = 1;
                goto invoke_callback;
            case 0:
            default:
                index++;
                continue;
            }
        }
        break;
    case 0x72:
        object_control_offset = 0x28;
        goto object_control_action;
    case 0x73:
        object_control_offset = 0x2c;
        goto object_control_action;
    case 0x74:
        object_control_offset = 0x30;
object_control_action:
        index = map_object_find_interaction_target(0, position, 800, 1700,
                               rotation->angles[1], 512);
        if (index != -1) {
            KfMapObject *object = &map_object_state.objects[index];

            if (object->object_id == 0xbd) {
                if (object->tail.fields.unknown_38 != 0xff) {
                    break;
                }
                object->tail.fields.unknown_38 = command;
                object->action_timer = 0;
                *(u16 *)&event_state.control.bytes[object_control_offset] =
                    object - map_object_state.objects;
                event_state.control.bytes[object_control_offset + 2] =
                    state_8017d118.values_04[0];
                event_state.state_word = 1;
                game_counter_decrement(command);
                object->extra_40.bytes[0] = 0;
                event_spawn_effect_object((KfEventObjectView *)object, command);
            } else if (map_object_check_and_consume_marker(object, command) == 3) {
                notify_enqueue(object->tail.notification.linked_notification);
                event_state.state_word = 1;
            }
        }
        break;
    case 0x6f:
        object_control_offset = 0x28;
        goto transition_action;
    case 0x70:
        object_control_offset = 0x2c;
        goto transition_action;
    case 0x71:
        object_control_offset = 0x30;
transition_action: {
        u8 previous_value;
        KfMapObject *object;
        struct KfVecXZi forward;
        u16 object_index;
        s16 yaw;

        if (state_8017d118.values_04[0] == 7 ||
            event_state.control.bytes[object_control_offset] == 0xff ||
            player_state.vitals.current_mp < 10) {
            break;
        }
        player_state.vitals.current_mp -= 10;
        render_frames_with_color_overlay(1, 0, 4096, 256);
        actor_disable_type3_transition_actors();
        previous_value = event_state.control.bytes[object_control_offset + 2];
        do {
            cd_request_yield();
            resource_advance_transition();
        } while (state_8017d118.transition_active != 0);
        if (previous_value != state_8017d118.values_04[0]) {
            resource_request_transition(previous_value, previous_value, previous_value,
                          0xff, 0xff, 0x7f, 0x7f, 0x7f);
        } else {
            resource_request_transition(0xff, 0xff, previous_value, 0xff, 0xff,
                          0x7f, 0x7f, 0x7f);
        }
        do {
            cd_request_yield();
            resource_advance_transition();
        } while (state_8017d118.transition_active != 0);
        cd_request_wait_idle();

        object_index = *(u16 *)&event_state.control.bytes[object_control_offset];
        object = &map_object_state.objects[object_index];
        angle_to_forward_xz(object->rotation.vy, &forward);
        vector2i_scale_shift11(1024, &forward);
        player_state.death_state = 0;
        player_state.view_rotation_offset.components[2] = 0;
        player_state.view_rotation_offset.components[1] = 0;
        player_state.view_rotation_offset.components[0] = 0;
        player_state.camera_rotation = player_state.camera_rotation_target;
        player_state.camera_position.vx = object->position.vx + forward.x;
        player_state.camera_position.vz = object->position.vz + forward.z;
        player_state.camera_position.vy = object->position.vy;
        yaw = object->rotation.vy + 2048;
        event_state.state_word = 1;
        player_state.camera_rotation_target.angles[1] = yaw;
        player_state.camera_rotation.angles[1] = yaw;
        render_frames_with_color_overlay(1, 4096, 4096, 0);
        if (game_graphics_runtime.asset_registry_entries[0x181] == 0) {
            resource_tmd_queue_read(0, 0x101, 0x181);
        }
        render_frames_with_color_overlay(1, 4096, 0, -256);
        render_set_color_overlay(0xff, 0, 0, 0);
        resource_request_transition(0xff, 0xff, 0xff, previous_value, previous_value,
                      0x7f, 0x7f, 0x7f);
        break;
    }
    case 0x67:
        index = map_object_find_interaction_target(0, position, 800, 1700,
                               rotation->angles[1], 512);
        if (index != -1) {
            KfMapObject *object = &map_object_state.objects[index];

            if (object->object_id == 0xb8) {
                if (object->tail.fields.unknown_38 == 0xff) {
                    object->tail.fields.unknown_38 = command;
                    object->action_timer = 0;
                    event_state.state_word = 1;
                    game_counter_decrement(command);
                    event_spawn_effect_object((KfEventObjectView *)object, command);
                }
            } else if (map_object_check_and_consume_marker(object, command) == 3) {
                notify_enqueue(object->tail.notification.linked_notification);
                event_state.state_word = 1;
            }
        }
        break;
    case 0x54:
        player_state.map_marker_visual_effect_timer = 1200;
        game_counter_decrement(0x54);
        event_state.state_word = 1;
        break;
    case 0x58:
        index = map_object_find_interaction_target(0, position, 800, 1700,
                               rotation->angles[1], 512);
        if (index != -1) {
            KfMapObject *object = &map_object_state.objects[index];

            if (object->object_id == 0x9d) {
                map_object_apply_marker_signal(object->tail.fields.unknown_38);
            }
            event_state.state_word = 1;
        }
        audio_play_sound_at_volume_100(8);
        break;
    case 0x59: {
        KfMapObject *scan = map_object_state.objects;
        KfMapObject *nearest = 0;
        s32 nearest_distance = 999999;
        s32 remaining = KF_MAP_OBJECT_CAPACITY - 1;

        for (; remaining != -1; scan++, remaining--) {
            KfMapObject *object = scan;
            s32 object_id = object->object_id;
            s32 distance;

            if (object_id < 82) {
                continue;
            }
            if (object_id >= 84) {
                if (object_id >= 97) {
                    continue;
                }
                if (object_id < 90) {
                    continue;
                }
            }
            distance = vector_distance_between_with_reach(&object->position, 25000,
                                     &player_state.camera_position, 0, 0);
            if (distance >= 0 && distance < nearest_distance) {
                nearest = object;
                nearest_distance = distance;
            }
        }
        if (nearest != 0) {
            VECTOR sound_position;

            /* Retail writes this adjusted vector, then passes the object position. */
            sound_position.vx = nearest->position.vx;
            sound_position.vz = nearest->position.vz;
            sound_position.vy = nearest->position.vy +
                4 * (nearest->position.vy - audio_state.listener_position.vy);
            audio_play_spatial_range(0x8009, &nearest->position, 0x6e,
                                     25000, 29000, 0);
            event_state.state_word = 1;
        }
        break;
    }
    case 0x5a:
        magic_ids = event_magic_unlock_ids_5a;
        goto magic_action;
    case 0x5b:
        magic_ids = event_magic_unlock_ids_5b;
        goto magic_action;
    case 0x5c:
        magic_ids = event_magic_unlock_ids_5c;
        goto magic_action;
    case 0x5d:
        magic_ids = event_magic_unlock_ids_5d;
        goto magic_action;
    case 0x5e:
        magic_ids = event_magic_unlock_ids_5e;
magic_action: {
        KfMapObject *object;
        VECTOR near_position;
        VECTOR far_position;
        s32 fraction;
        s32 spin;
        s32 magic_id;
        KfMagicRecord *magic_record;

        for (;;) {
            magic_id = *magic_ids++;
            if (magic_id == 0xff) {
                goto invoke_callback;
            }
            magic_record = &effect_state.magic_records[magic_id];
            if (magic_record->menu_available == 0) {
                magic_record->menu_available = 1;
                break;
            }
        }
        object = map_object_effect_pool_acquire(
            0x15e, 10, map_object_state.spawn_sequence_pool_15e);
        map_object_reset(object);
        game_counter_decrement(command);
        object->object_id = command;
        object->render_queue_mode = 1;
        object->layer_mask = 3;
        object->action = 0xff;
        object->lighting_override_index = 0x42;
        object->rotation.vz = 0;
        object->rotation.vy = 0;
        object->rotation.vx = 0;

        scene_position_from_camera_offset(0, 200, 1500, player_state.camera_rotation.angles[0],
                      player_state.camera_rotation.angles[1], 0, 0,
                      &far_position);
        scene_position_from_camera_offset(0, 200, 1000, player_state.camera_rotation.angles[0],
                      player_state.camera_rotation.angles[1], 0, 0,
                      &near_position);
        spin = 0;
        for (fraction = 0; fraction < 4096; fraction += 64) {
            scene_pose_interpolate((KfScenePoseView *)object, &near_position,
                          &far_position, 0, 0, fraction);
            object->rotation.vy += spin;
            spin += 4;
            object->lighting_blend_q12 = (rsin(fraction << 6) >> 2) + 1024;
            cd_request_service_vab();
            cd_request_service_stream();
            render_game_frame(0, 0);
        }
        /* The first decay step precedes service; later steps follow it. */
        goto decay_update;
        for (;;) {
            cd_request_service_vab();
            cd_request_service_stream();
            render_game_frame(0, 0);
decay_update:
            object->lighting_blend_q12 -= 64;
            object->rotation.vy += spin;
            spin += 8;
            if ((s16)object->lighting_blend_q12 <= 0) {
                break;
            }
        }
        object->lighting_blend_q12 = 0;
        do {
            object->rotation.vy += spin;
            object->lighting_blend_q12 += 128;
            spin += 8;
            if (object->lighting_blend_q12 >= 4096) {
                break;
            }
            cd_request_service_vab();
            cd_request_service_stream();
            render_game_frame(0, 0);
        } while (1);
        object->object_id = 0xff;
        notify_enqueue(1);
        event_state.state_word = 1;
        break;
    }
    case 0x52:
        if (collision_probe_forward_shape_0x20(position,
                           (const struct KfEulerAngles *)rotation) == 0) {
            index = map_object_find_interaction_target(0, position, 800, 1700,
                                   rotation->angles[1], 512);
            if (index == -1) {
                break;
            }
            if (map_object_state.objects[index].object_id != 0xc3) {
                goto invoke_callback;
            }
        }
        if (game_counter_increment(0x4d) == 0) {
            notify_enqueue(0x16);
            game_counter_decrement(0x52);
        }
        event_state.state_word = 1;
        break;
    case 0x55: {
        s32 side = player_state.map_layer_index == 0 ? 1 : 2;
        s32 actor_distance;
        KfActor *actor = actor_find_best_in_cone(position, rotation->angles[1],
                                       rotation->angles[0], 8000, 500, 500,
                                       &actor_distance, -1);

        if (actor != 0 && actor->current_map_layer == side) {
            menu_show_transition_image(6, actor->definition_id + 240);
            event_state.state_word = 1;
            break;
        }
        {
            s32 remaining;
            KfMapObject *scan;

            for (scan = map_object_state.objects,
                 remaining = KF_MAP_OBJECT_CAPACITY - 1;
                 remaining != -1; remaining--, scan++) {
                KfMapObject *object = scan;

                if (map_object_state.templates[object->object_id].collision_kind != 0xe2 ||
                    object->extra_40.bytes[0] != side) {
                    continue;
                }
                if (player_camera_within_map_region(object->position.vx >> 11,
                                  object->position.vz >> 11,
                                  object->tail.fields.unknown_38,
                                  object->tail.fields.unknown_39, 0x8000)) {
                    menu_show_transition_image(6, object->tail.fields.unknown_3a.value + 510);
                    event_state.state_word = 1;
                    break;
                }
            }
        }
        break;
    }
    case 0x56:
        game_counter_decrement(0x56);
        player_state.full_mp_timer = 900;
        event_state.state_word = 1;
        break;
    case 0x57:
        game_counter_decrement(0x57);
        player_state.magic_boost_timer = 900;
        event_state.state_word = 1;
        player_recalculate_combat_stats();
        break;
    /* Commands 0x53, 0x5f..0x62, 0x69, and 0x6e have no action. */
    }

invoke_callback:
    ((KfEventCommandCallback)state_8017d118.active_table[2])(
        position, rotation, command);
    if (event_state.state_word == 0) {
        notify_enqueue(0x14);
    }
    player_clear_motion();
}
ADDRESS(0x800473e0, 0x54)
s32 game_counter_decrement(s32 index)
{
    if (game_counter_bytes[index] != 0) {
        game_counter_bytes[index]--;
        return 0;
    }

    return 1;
}

ADDRESS(0x80047434, 0x90)
s32 game_counter_increment(s32 index)
{
    if (game_counter_bytes[index] < 99) {
        game_counter_bytes[index]++;
        state_8017d118.active_table[6]();
        return 0;
    }

    notify_enqueue(0x12);
    return 1;
}

ADDRESS(0x800474c4, 0x114)
void color_overlay_transition(s32 step, s32 first, s32 second, s32 third,
                              s32 target_first, s32 target_second,
                              s32 target_third)
{
    s32 fraction = 0;

    do {
        s32 current_first;
        s32 current_second;
        s32 current_third;

        reset_collision_rows_and_overlay();
        current_first = fixed_lerp_q12(first, target_first, fraction);
        current_second = fixed_lerp_q12(second, target_second, fraction);
        current_third = fixed_lerp_q12(third, target_third, fraction);
        accumulate_color_overlay(current_first, current_second, current_third, 0x800);
        render_game_frame(0, 0);
        fraction += step;
    } while (fraction < 4096);

    reset_collision_rows_and_overlay();
    accumulate_color_overlay(target_first, target_second, target_third, 0x800);
    render_game_frame(0, 0);
}
ADDRESS(0x800475d8, 0x6c0)
void event_map_object_interact(KfMapObject *object, ...)
{
    va_list arguments;
    s32 spawn_object_id;
    s32 spawned_id = -1;
    const KfMapObjectTemplate *template;
    const KfMapObjectTemplatePoseView *pose;
    SVECTOR next_angles;
    SVECTOR first_angles;
    VECTOR next_position;
    VECTOR first_position;
    s32 first_yaw;
    s32 current_yaw;
    s32 target_yaw;
    u16 first_pitch;
    s16 target_pitch;
    s32 fraction;
    u32 previous_buttons;
    u32 buttons;
    s32 remove_object;

    if (object == 0) {
        va_start(arguments, object);
        spawn_object_id = va_arg(arguments, s32);
        va_end(arguments);
        spawned_id = spawn_object_id;
        object = map_object_effect_pool_acquire(
            0x15e, 10, map_object_state.spawn_sequence_pool_15e);
        map_object_reset(object);
        object->object_id = spawn_object_id;
        object->layer_mask = 3;
        object->action = 0xff;
        object->rotation.vz = 0;
        object->rotation.vy = 0;
        object->rotation.vx = 0;
        object->tail.fields.unknown_38 = 0xff;
    }

    template = &map_object_state.templates[object->object_id];
    pose = (const KfMapObjectTemplatePoseView *)template;
    first_pitch = object->unknown_0e;
    if (object->tail.fields.unknown_38 != 0xff) {
        return;
    }
    if (template->kind == 0x20) {
        notify_enqueue(0x15, object->tail.fields.unknown_3a.value);
        object->object_id = 0xff;
        player_state.gold += object->tail.fields.unknown_3a.value;
        return;
    }

    if (object->action == 0x70 && object->object_id == 0x67) {
        KfMapObject *child = &map_object_state.objects[object->extra_40.object_index];
        KfMapObject *next = &map_object_state.objects[
            child->tail.fields.unknown_3a.bytes.high];
        if (next->action == 3 && next->action_timer != 0) {
            return;
        }
    }
    if (object->object_id == 0x12) {
        object->object_id = 0xd;
    }

    first_yaw = player_state.camera_rotation.angles[0] & 0xfff;
    target_yaw = first_yaw;
    if (first_yaw < 0x800) {
        if (first_yaw > 0x100) {
            target_yaw = 0x100;
        }
    } else if (first_yaw < 0xf00) {
        target_yaw = 0xf00;
    }
    previous_buttons = PadRead(1);
    target_pitch = (-pose->depth_offset) / 4 - 200;

    if (spawned_id != -1) {
        scene_position_from_camera_offset(0, 500, 1500, target_yaw,
                      player_state.camera_rotation.angles[1],
                      pose->height_offset, pose->depth_offset,
                      &object->position);
    } else {
        first_angles = object->rotation;
        first_position = object->position;
        scene_position_from_camera_offset(0, 500, 1500, target_yaw,
                      player_state.camera_rotation.angles[1],
                      pose->height_offset, pose->depth_offset,
                      &next_position);
        next_angles.vz = 0;
        next_angles.vx = 0;
        for (fraction = 0; fraction <= 0x1000; fraction += 0x200) {
            buttons = PadRead(1);
            if (previous_buttons == 0 && buttons != 0) {
                goto button_pressed;
            }
            previous_buttons = buttons;
            scene_pose_interpolate((KfScenePoseView *)object, &first_position,
                          &next_position, &first_angles, &next_angles,
                          fraction);
            object->unknown_0e = value_approach(
                (s16)first_pitch, target_pitch, fraction);
            player_state.camera_rotation.angles[0] = angle_lerp_shortest_q12(
                first_yaw, target_yaw, fraction);
            cd_request_service_vab();
            cd_request_service_stream();
            render_game_frame(0, (const SVECTOR *)&player_state.camera_rotation);
        }
    }
    object->unknown_0e = target_pitch;

    for (;;) {
        buttons = PadRead(1);
        if (previous_buttons == 0 && buttons != 0) {
            break;
        }
        object->rotation.vy += 0x40;
        cd_request_service_vab();
        cd_request_service_stream();
        render_game_frame(0, (const SVECTOR *)&player_state.camera_rotation);
        previous_buttons = buttons;
    }

button_pressed:
    if ((buttons & 0x20) == 0 && spawned_id == -1) {
        goto return_pose;
    }
    if (game_counter_increment(object->object_id) == 0) {
        switch (object->object_id) {
        case 0x75: {
            s32 amount = ((rand() * 6) >> 15) + 4;
            if (game_counter_bytes[0x75] < 99 - amount) {
                game_counter_bytes[0x75] += amount;
            } else {
                game_counter_bytes[0x75] = 99;
            }
            break;
        }
        case 0x76:
            if (game_counter_bytes[0x76] < 95) {
                game_counter_bytes[0x76] += 4;
            } else {
                game_counter_bytes[0x76] = 99;
            }
            break;
        }
        remove_object = 1;
        scene_position_from_camera_offset(-500, 500, 0, target_yaw,
                      player_state.camera_rotation.angles[1], 0, 0,
                      &first_position);
        first_angles = object->rotation;
        first_pitch = 0;
        goto interpolate_back;
    }
    if (spawned_id != -1) {
        object->object_id = 0xff;
        goto finish;
    }

return_pose:
    remove_object = 0;
    while (!angle_within_tolerance(object->rotation.vy,
                                   first_angles.vy, 0x80)) {
        object->rotation.vy = (object->rotation.vy + 0x100) & 0xfff;
        render_game_frame(0, (const SVECTOR *)&player_state.camera_rotation);
    }
    object->rotation.vy = first_angles.vy;

interpolate_back:
    next_angles = object->rotation;
    next_position = object->position;
    current_yaw = player_state.camera_rotation.angles[0];
    for (fraction = 0; fraction <= 0x1000; fraction += 0x200) {
        scene_pose_interpolate((KfScenePoseView *)object, &next_position,
                      &first_position, &next_angles, &first_angles,
                      fraction);
        player_state.camera_rotation.angles[0] = angle_lerp_shortest_q12(
            current_yaw, first_yaw, fraction);
        object->unknown_0e = value_approach(
            target_pitch, (s16)first_pitch, fraction);
        render_game_frame(0, (const SVECTOR *)&player_state.camera_rotation);
    }
    if (remove_object) {
        object->object_id = 0xff;
    } else if (object->object_id == 0xd) {
        object->object_id = 0x12;
    }
finish:
    player_clear_motion();
}

ADDRESS(0x80047c98, 0x660)
void event_world_dispatch_interaction(const VECTOR *position,
                                      const KfPlayerViewRotation *rotation)
{
    VECTOR probe;
    s32 object_index;
    KfMapObject *objects;
    KfMapObjectTemplate *templates;

    probe.vx = position->vx;
    probe.vy = position->vy + 500;
    probe.vz = position->vz;
    event_state.state_word = 0;
    if (collision_probe_forward_shape_0x20(&probe, (const struct KfEulerAngles *)rotation) != 0) {
        color_overlay_transition(0x400, 0, 0, 0, 0x80, 0xa0, 0xff);
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
        color_overlay_transition(0x400, 0x80, 0xa0, 0xff, 0, 0, 0);
    }

    object_index = actor_find_overlap_excluding_target_type3(probe.vx, probe.vy, probe.vz, 0x578, 0xc80);
    if (object_index != -1) {
        KfActor *actor = &actor_state.actors[object_index];
        s32 angle = vector_xz_to_angle(actor->position.vx - probe.vx,
                                       actor->position.vz - probe.vz);
        if (angle_within_tolerance(rotation->angles[1], angle, 300)) {
            event_target_stream_execute(actor);
        }
    }

    object_index = 0;
    objects = map_object_state.objects;
    templates = map_object_state.templates;
    for (;; object_index++) {
        KfMapObject *object;
        u16 object_id;
        s32 kind;

        object_index = map_object_find_interaction_target(object_index, &probe, 800, 2500,
                                      rotation->angles[1], 512);
        if (object_index == -1) {
            break;
        }
        object = &objects[object_index];
        object_id = object->object_id;
        event_state.state_word = 1;
        kind = templates[object_id].collision_kind;
        switch (kind) {
        case 0xa5:
        case 0xff:
            if (object->tail.notification.default_notification != 0xff) {
                notify_enqueue(object->tail.notification.default_notification);
            }
            break;
        case 0x40:
            event_map_object_interact(object);
            if (object->object_id == 0xff) {
                goto invoke_callback;
            }
            break;
        case 9:
        case 0x15: {
            u16 linked_index = object->tail.fields.unknown_3a.value;
            if (linked_index == 0xffff ||
                objects[linked_index].object_id == 0xff) {
                notify_enqueue(object->tail.notification.default_notification);
                break;
            }
            {
                KfMapObject *linked = &objects[linked_index];
                u8 linked_state = object->extra_40.bytes[0];
                u16 result_id;
                linked->tail.fields.unknown_38 = 0xff;
                linked->layer_mask = linked_state;
                event_map_object_interact(linked);
                result_id = linked->object_id;
                linked->layer_mask = 0;
                linked->tail.fields.unknown_38 = 0;
                if (result_id == 0xff) {
                    object->tail.fields.unknown_3a.value = 0xffff;
                }
            }
            break;
        }
        case 0x53:
            if (object->action_timer == 0) {
                object->action_timer = 1;
            }
            break;
        case 2:
            if (object->action_timer == 0) {
                if (object->tail.fields.unknown_38 == 0xff) {
                    object->action_timer = 1;
                } else {
                    notify_enqueue(object->tail.notification.default_notification);
                }
            }
            break;
        case 3:
        case 4:
            if (object->action_timer == 0) {
                if (object->tail.fields.unknown_38 >= 0xfc) {
                    if ((object->tail.fields.unknown_38 & 1) &&
                        angle_within_tolerance(rotation->angles[1],
                                               object->rotation.vy, 900)) {
                        object->action_timer = 1;
                        break;
                    }
                    if ((object->tail.fields.unknown_38 & 2) &&
                        angle_within_tolerance(rotation->angles[1],
                                               object->rotation.vy + 0x800,
                                               900)) {
                        object->action_timer = 1;
                        break;
                    }
                }
                if (object->tail.fields.unknown_38 == 0x0f && game_counter_bytes[0x0f] != 0) {
                    object->action_timer = 1;
                    break;
                }
                notify_enqueue(object->tail.notification.default_notification);
            }
            break;
        case 0x51:
            if (object->action_timer == 0 &&
                object->tail.fields.unknown_38 == 0xff) {
                object->action_timer = 1;
            }
            break;
        case 8:
        case 0x16:
            if (!angle_within_tolerance(rotation->angles[1],
                                        object->rotation.vy + 0x800, 0x155)) {
                break;
            }
            /* Kind five enters the same state handler without the angle gate. */
        case 5:
            switch (object->tail.fields.unknown_38) {
            case 0xfe: {
                u16 linked_index = object->tail.fields.unknown_3a.value;
                if (linked_index != 0xffff) {
                    goto check_linked_object;
                }
            notify_six:
                notify_enqueue(6);
                break;
            check_linked_object:
                if (objects[linked_index].object_id == 0xff) {
                    goto notify_six;
                }
                break;
            }
            case 0xff:
                object->tail.fields.unknown_38 = 0xfe;
                break;
            default:
                notify_enqueue(object->tail.notification.default_notification);
                break;
            }
            break;
        case 0x0f:
            if (object->tail.fields.unknown_38 == 0xff) {
                notify_enqueue(0x10);
            }
            break;
        case 0x20:
            if (player_state.death_state == 0) {
                u8 *linked_state;
                player_begin_view_reaction(object_index);
                linked_state = (u8 *)object->extra_40.record;
                if (linked_state[1] == 1) {
                    linked_state[1] = 5;
                }
            }
            break;
        case 0x0d:
        case 0x14:
            menu_show_transition_image(6, object->tail.pair_38.value_38 + 0x78);
            break;
        case 0x12:
            color_overlay_transition(0x200, 0, 0, 0, 0x80, 0xc8, 0xff);
            player_state.vitals.current_hp = player_state.vitals.maximum_hp;
            color_overlay_transition(0x200, 0x80, 0xc8, 0xff, 0, 0, 0);
            break;
        case 0x0e:
            event_world_state_save_slot(state_8017d118.values_04[0]);
            player_render_frame_and_release_pool();
            menu_card_save_browser();
            break;
        }
    }

invoke_callback:
    ((void (*)(const VECTOR *, const KfPlayerViewRotation *))
        state_8017d118.active_table[0])(&probe, rotation);
}

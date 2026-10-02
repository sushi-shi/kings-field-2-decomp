#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/event_stream.h>
#include <kf/game/graphics.h>
#include <kf/game/map_object.h>
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

            index = func_80036190(index, position, 800, 1700,
                                   rotation->angles[1], 512);
            if (index == -1) {
                break;
            }
            object = &map_object_state.objects[index];
            status = func_800368b4(object, command);
            switch (status) {
            case 1:
                audio_play_sound_64();
                event_state.state_word = 1;
                goto invoke_callback;
            case 3:
                notify_enqueue(object->tail.fields.unknown_3e.bytes.low);
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
        index = func_80036190(0, position, 800, 1700,
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
            } else if (func_800368b4(object, command) == 3) {
                notify_enqueue(object->tail.fields.unknown_3e.bytes.low);
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
        func_80036e24(1, 0, 4096, 256);
        func_80038f20();
        previous_value = event_state.control.bytes[object_control_offset + 2];
        do {
            cd_request_yield();
            func_80016820();
        } while (state_8017d118.transition_active != 0);
        if (previous_value != state_8017d118.values_04[0]) {
            func_80016260(previous_value, previous_value, previous_value,
                          0xff, 0xff, 0x7f, 0x7f, 0x7f);
        } else {
            func_80016260(0xff, 0xff, previous_value, 0xff, 0xff,
                          0x7f, 0x7f, 0x7f);
        }
        do {
            cd_request_yield();
            func_80016820();
        } while (state_8017d118.transition_active != 0);
        cd_request_wait_idle();

        object_index = *(u16 *)&event_state.control.bytes[object_control_offset];
        object = &map_object_state.objects[object_index];
        angle_to_forward_xz(object->rotation.vy, &forward);
        vector2i_scale_shift11(1024, &forward);
        player_state.death_state = 0;
        player_state.unknown_108.components[2] = 0;
        player_state.unknown_108.components[1] = 0;
        player_state.unknown_108.components[0] = 0;
        player_state.camera_rotation = player_state.camera_rotation_target;
        player_state.camera_position.vx = object->position.vx + forward.x;
        player_state.camera_position.vz = object->position.vz + forward.z;
        player_state.camera_position.vy = object->position.vy;
        yaw = object->rotation.vy + 2048;
        event_state.state_word = 1;
        player_state.camera_rotation_target.angles[1] = yaw;
        player_state.camera_rotation.angles[1] = yaw;
        func_80036e24(1, 4096, 4096, 0);
        if (game_graphics_runtime.asset_registry_entries[0x181] == 0) {
            resource_tmd_queue_read(0, 0x101, 0x181);
        }
        func_80036e24(1, 4096, 0, -256);
        render_set_color_overlay(0xff, 0, 0, 0);
        func_80016260(0xff, 0xff, 0xff, previous_value, previous_value,
                      0x7f, 0x7f, 0x7f);
        break;
    }
    case 0x67:
        index = func_80036190(0, position, 800, 1700,
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
            } else if (func_800368b4(object, command) == 3) {
                notify_enqueue(object->tail.fields.unknown_3e.bytes.low);
                event_state.state_word = 1;
            }
        }
        break;
    case 0x54:
        player_state.unknown_6a = 1200;
        game_counter_decrement(0x54);
        event_state.state_word = 1;
        break;
    case 0x58:
        index = func_80036190(0, position, 800, 1700,
                               rotation->angles[1], 512);
        if (index != -1) {
            KfMapObject *object = &map_object_state.objects[index];

            if (object->object_id == 0x9d) {
                func_800366fc(object->tail.fields.unknown_38);
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
            0x15e, 10, map_object_state.unknown_873e);
        map_object_reset(object);
        game_counter_decrement(command);
        object->object_id = command;
        object->unknown_02 = 1;
        object->unknown_00 = 3;
        object->action = 0xff;
        object->unknown_05 = 0x42;
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
            object->unknown_10 = (rsin(fraction << 6) >> 2) + 1024;
            cd_request_service_vab();
            cd_request_service_stream();
            func_800335a0(0, 0);
        }
        /* The first decay step precedes service; later steps follow it. */
        goto decay_update;
        for (;;) {
            cd_request_service_vab();
            cd_request_service_stream();
            func_800335a0(0, 0);
decay_update:
            object->unknown_10 -= 64;
            object->rotation.vy += spin;
            spin += 8;
            if ((s16)object->unknown_10 <= 0) {
                break;
            }
        }
        object->unknown_10 = 0;
        do {
            object->rotation.vy += spin;
            object->unknown_10 += 128;
            spin += 8;
            if (object->unknown_10 >= 4096) {
                break;
            }
            cd_request_service_vab();
            cd_request_service_stream();
            func_800335a0(0, 0);
        } while (1);
        object->object_id = 0xff;
        notify_enqueue(1);
        event_state.state_word = 1;
        break;
    }
    case 0x52:
        if (func_80045e5c(position,
                           (const struct KfEulerAngles *)rotation) == 0) {
            index = func_80036190(0, position, 800, 1700,
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
        s32 side = player_state.unknown_128 == 0 ? 1 : 2;
        s32 actor_distance;
        KfActor *actor = func_8003a778(position, rotation->angles[1],
                                       rotation->angles[0], 8000, 500, 500,
                                       &actor_distance, -1);

        if (actor != 0 && actor->unknown_03 == side) {
            func_80034e10(6, actor->unknown_01 + 240);
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
                if (func_80036ad8(object->position.vx >> 11,
                                  object->position.vz >> 11,
                                  object->tail.fields.unknown_38,
                                  object->tail.fields.unknown_39, 0x8000)) {
                    func_80034e10(6, object->tail.fields.unknown_3a.value + 510);
                    event_state.state_word = 1;
                    break;
                }
            }
        }
        break;
    }
    case 0x56:
        game_counter_decrement(0x56);
        player_state.unknown_6c = 900;
        event_state.state_word = 1;
        break;
    case 0x57:
        game_counter_decrement(0x57);
        player_state.unknown_6e = 900;
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

        func_8002bc18();
        current_first = func_8001584c(first, target_first, fraction);
        current_second = func_8001584c(second, target_second, fraction);
        current_third = func_8001584c(third, target_third, fraction);
        func_80031634(current_first, current_second, current_third, 0x800);
        func_800335a0(0, 0);
        fraction += step;
    } while (fraction < 4096);

    func_8002bc18();
    func_80031634(target_first, target_second, target_third, 0x800);
    func_800335a0(0, 0);
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
            0x15e, 10, map_object_state.unknown_873e);
        map_object_reset(object);
        object->object_id = spawn_object_id;
        object->unknown_00 = 3;
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
            player_state.camera_rotation.angles[0] = func_8001586c(
                first_yaw, target_yaw, fraction);
            cd_request_service_vab();
            cd_request_service_stream();
            func_800335a0(0, (const SVECTOR *)&player_state.camera_rotation);
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
        func_800335a0(0, (const SVECTOR *)&player_state.camera_rotation);
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
        func_800335a0(0, (const SVECTOR *)&player_state.camera_rotation);
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
        player_state.camera_rotation.angles[0] = func_8001586c(
            current_yaw, first_yaw, fraction);
        object->unknown_0e = value_approach(
            target_pitch, (s16)first_pitch, fraction);
        func_800335a0(0, (const SVECTOR *)&player_state.camera_rotation);
    }
    if (remove_object) {
        object->object_id = 0xff;
    } else if (object->object_id == 0xd) {
        object->object_id = 0x12;
    }
finish:
    player_clear_motion();
}

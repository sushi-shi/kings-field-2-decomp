#include <kf/game/cd.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_stream.h>
#include <kf/game/map_object.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <psyq/sdk.h>

extern void func_80045f20(s32 x, s32 y, s32 z, s32 pitch, s32 yaw,
                          s32 height_offset, s32 z_offset, VECTOR *output);
extern void func_80045fd4(KfScenePoseView *destination,
                          const VECTOR *start_position,
                          const VECTOR *end_position,
                          const SVECTOR *start_angles,
                          const SVECTOR *end_angles, s32 fraction);
extern void func_800335a0(const VECTOR *position, const SVECTOR *rotation);

ADDRESS(0x800475d8, 0x6c0)
void func_800475d8(KfMapObject *object, s32 spawn_object_id)
{
    s32 spawned_id = -1;
    const KfMapObjectTemplate *template;
    const KfMapObjectTemplatePoseView *pose;
    VECTOR first_position;
    VECTOR next_position;
    SVECTOR first_angles;
    SVECTOR next_angles;
    s32 first_yaw;
    s32 current_yaw;
    s32 target_yaw;
    u16 first_pitch;
    s32 target_pitch;
    s32 end_pitch;
    s32 negative_depth;
    s32 fraction;
    u32 previous_buttons;
    u32 buttons;
    s32 remove_object = 0;

    if (object == 0) {
        spawned_id = spawn_object_id;
        object = map_object_effect_pool_acquire(
            0x15e, 10, map_object_state.unknown_873e);
        map_object_reset(object);
        object->object_id = spawn_object_id;
        object->unknown_00 = 3;
        object->action = 0xff;
        object->rotation.vx = 0;
        object->rotation.vy = 0;
        object->rotation.vz = 0;
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
    negative_depth = -pose->depth_offset;
    if (negative_depth < 0) {
        negative_depth += 3;
    }
    target_pitch = (negative_depth >> 2) - 200;

    if (spawned_id == -1) {
        first_position = object->position;
        first_angles = object->rotation;
        func_80045f20(0, 500, 1500, target_yaw,
                      player_state.camera_rotation.angles[1],
                      pose->height_offset, pose->depth_offset,
                      &next_position);
        next_angles.vx = 0;
        next_angles.vz = 0;
        for (fraction = 0; fraction <= 0x1000; fraction += 0x200) {
            buttons = PadRead(1);
            if (previous_buttons == 0 && buttons != 0) {
                goto button_pressed;
            }
            func_80045fd4((KfScenePoseView *)object, &first_position,
                          &next_position, &first_angles, &next_angles,
                          fraction);
            object->unknown_0e = value_approach(
                (s16)first_pitch, target_pitch, fraction);
            player_state.camera_rotation.angles[0] = func_8001586c(
                first_yaw, target_yaw, fraction);
            cd_request_service_vab();
            cd_request_service_stream();
            func_800335a0(0, (const SVECTOR *)&player_state.camera_rotation);
            previous_buttons = buttons;
        }
    } else {
        func_80045f20(0, 500, 1500, target_yaw,
                      player_state.camera_rotation.angles[1],
                      pose->height_offset, pose->depth_offset,
                      &object->position);
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
    if (func_80047434(object->object_id) == 0) {
        if (object->object_id == 0x75) {
            s32 amount = ((rand() * 6) >> 15) + 4;
            if (game_counter_bytes[0x75] < 99 - amount) {
                game_counter_bytes[0x75] += amount;
            } else {
                game_counter_bytes[0x75] = 99;
            }
        } else if (object->object_id == 0x76) {
            if (game_counter_bytes[0x76] < 95) {
                game_counter_bytes[0x76] += 4;
            } else {
                game_counter_bytes[0x76] = 99;
            }
        }
        remove_object = 1;
        func_80045f20(-500, 500, 0, target_yaw,
                      player_state.camera_rotation.angles[1], 0, 0,
                      &next_position);
        next_angles = object->rotation;
        end_pitch = 0;
        goto interpolate_back;
    }
    if (spawned_id != -1) {
        object->object_id = 0xff;
        goto finish;
    }

return_pose:
    while (!angle_within_tolerance(object->rotation.vy,
                                   first_angles.vy, 0x80)) {
        object->rotation.vy = (object->rotation.vy + 0x100) & 0xfff;
        func_800335a0(0, (const SVECTOR *)&player_state.camera_rotation);
    }
    object->rotation.vy = first_angles.vy;
    next_position = first_position;
    next_angles = first_angles;
    end_pitch = (s16)first_pitch;

interpolate_back:
    first_position = object->position;
    first_angles = object->rotation;
    current_yaw = player_state.camera_rotation.angles[0];
    for (fraction = 0; fraction <= 0x1000; fraction += 0x200) {
        func_80045fd4((KfScenePoseView *)object, &first_position,
                      &next_position, &first_angles, &next_angles,
                      fraction);
        player_state.camera_rotation.angles[0] = func_8001586c(
            current_yaw, first_yaw, fraction);
        object->unknown_0e = value_approach(
            target_pitch, end_pitch, fraction);
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

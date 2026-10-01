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

extern s32 func_80045e5c(const VECTOR *position,
                         const struct KfEulerAngles *rotation);
extern void func_800366fc(u8 identifier);
extern s32 func_800368b4(KfMapObject *object, s32 command);
extern void func_80046700(KfEventObjectView *event, s32 object_id);
extern KfActor *func_8003a778(const VECTOR *position, s16 yaw, s16 pitch,
                              s32 max_distance, s32 yaw_limit, s32 pitch_limit,
                              s32 *distance, s32 variation);
extern void func_80034e10(u16 archive_slot, u16 archive_entry);
extern s32 func_80036ad8(s32 x, s32 z, s32 width, s32 depth, s32 height);
extern void func_80045f20(s32 x, s32 y, s32 z, s32 pitch, s32 yaw,
                          s32 height_offset, s32 z_offset, VECTOR *output);
extern void func_80045fd4(KfScenePoseView *destination,
                          const VECTOR *start_position,
                          const VECTOR *end_position,
                          const SVECTOR *start_angles,
                          const SVECTOR *end_angles, s32 fraction);
extern void func_800335a0(const VECTOR *position, const SVECTOR *rotation);
extern void func_80036e24(s32 mode, s32 phase, s32 last_phase, s32 step);
extern void func_80038f20(void);
extern void func_80016820(void);
extern void func_800314d4(u8 control, u8 red, u8 green, u8 blue);

DATA(0x800679a0, 0x8)
u8 DAT_800679a0[8] = {7, 8, 9, 10, 0xff, 0, 0, 0};

DATA(0x800679a8, 0x8)
u8 DAT_800679a8[8] = {14, 15, 0, 13, 0xff, 0, 0, 0};

DATA(0x800679b0, 0x8)
u8 DAT_800679b0[8] = {16, 1, 2, 3, 0xff, 0, 0, 0};

DATA(0x800679b8, 0x8)
u8 DAT_800679b8[8] = {4, 17, 5, 6, 0xff, 0, 0, 0};

DATA(0x800679c0, 0x8)
u8 DAT_800679c0[8] = {18, 19, 11, 12, 0xff, 0xff, 0xff, 0xff};

typedef void (*KfEventCommandCallback)(const VECTOR *position,
                                       const KfPlayerViewRotation *rotation,
                                       s32 command);

RODATA(0x800128d0, 0x8c)

ADDRESS(0x8004678c, 0xc54)
void func_8004678c(const VECTOR *position,
                   const KfPlayerViewRotation *rotation, s32 command)
{
    s32 index;

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
                break;
            case 3:
                notify_enqueue(object->tail.fields.unknown_3e.bytes.low);
                break;
            case 4:
                notify_enqueue(4);
                break;
            default:
                index++;
                continue;
            }
            event_state.state_word = 1;
            break;
        }
        break;
    case 0x72:
    case 0x73:
    case 0x74:
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
                *(u16 *)&event_state.control.bytes[0x28 +
                    4 * (command - 0x72)] = index;
                event_state.control.bytes[0x2a +
                    4 * (command - 0x72)] = state_8017d118.values_04[0];
                event_state.state_word = 1;
                func_800473e0(command);
                object->extra_40.bytes[0] = 0;
                func_80046700((KfEventObjectView *)object, command);
            } else if (func_800368b4(object, command) == 3) {
                notify_enqueue(object->tail.fields.unknown_3e.bytes.low);
                event_state.state_word = 1;
            }
        }
        break;
    case 0x6f:
    case 0x70:
    case 0x71: {
        s32 control_offset = 0x28 + 4 * (command - 0x6f);
        u8 previous_value;
        KfMapObject *object;
        struct KfVecXZi forward;
        u16 object_index;
        s16 yaw;

        if (state_8017d118.values_04[0] == 7 ||
            event_state.control.bytes[control_offset] == 0xff ||
            player_state.attack_charge_current < 10) {
            break;
        }
        player_state.attack_charge_current -= 10;
        func_80036e24(1, 0, 4096, 256);
        func_80038f20();
        previous_value = event_state.control.bytes[control_offset + 2];
        do {
            cd_request_yield();
            func_80016820();
        } while (state_8017d118.transition_active != 0);
        if (previous_value == state_8017d118.values_04[0]) {
            func_80016260(0xff, 0xff, 0xff, 0xff, 0xff,
                          0x7f, 0x7f, 0x7f);
        } else {
            func_80016260(previous_value, previous_value, previous_value,
                          0xff, 0xff, 0x7f, 0x7f, 0x7f);
        }
        do {
            cd_request_yield();
            func_80016820();
        } while (state_8017d118.transition_active != 0);
        cd_request_wait_idle();

        object_index = *(u16 *)&event_state.control.bytes[control_offset];
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
        event_state.state_word = 1;
        yaw = object->rotation.vy + 2048;
        player_state.camera_rotation_target.angles[1] = yaw;
        player_state.camera_rotation.angles[1] = yaw;
        func_80036e24(1, 4096, 4096, 0);
        if (game_graphics_runtime.asset_registry_entries[0x181] == 0) {
            resource_tmd_queue_read(0, 0x101, 0x181);
        }
        func_80036e24(1, 4096, 0, -256);
        func_800314d4(0xff, 0, 0, 0);
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
                    func_800473e0(command);
                    func_80046700((KfEventObjectView *)object, command);
                }
            } else if (func_800368b4(object, command) == 3) {
                notify_enqueue(object->tail.fields.unknown_3e.bytes.low);
                event_state.state_word = 1;
            }
        }
        break;
    case 0x54:
        player_state.unknown_6a = 1200;
        func_800473e0(0x54);
        event_state.state_word = 1;
        break;
    case 0x58:
        index = func_80036190(0, position, 800, 1700,
                               rotation->angles[1], 512);
        if (index != -1) {
            if (map_object_state.objects[index].object_id == 0x9d) {
                func_800366fc(map_object_state.objects[index].tail.fields.unknown_38);
            }
            event_state.state_word = 1;
        }
        audio_play_sound_at_volume_100(8);
        break;
    case 0x59: {
        KfMapObject *nearest = 0;
        s32 nearest_distance = 999999;
        s32 object_index;

        for (object_index = 0; object_index < KF_MAP_OBJECT_CAPACITY;
             object_index++) {
            KfMapObject *object = &map_object_state.objects[object_index];
            s32 object_id = object->object_id;
            s32 distance;

            if (object_id != 82 && object_id != 83 &&
                (object_id < 90 || object_id >= 97)) {
                continue;
            }
            distance = func_80015698(&object->position, 25000,
                                     &player_state.camera_position, 0, 0);
            if (distance >= 0 && distance < nearest_distance) {
                nearest = object;
                nearest_distance = distance;
            }
        }
        if (nearest != 0) {
            VECTOR sound_position;

            sound_position.vx = nearest->position.vx;
            sound_position.vy = nearest->position.vy +
                4 * (nearest->position.vy - audio_state.listener_position.vy);
            sound_position.vz = nearest->position.vz;
            audio_play_spatial_range(0x8009, &sound_position, 0x6e,
                                     25000, 29000, 0);
            event_state.state_word = 1;
        }
        break;
    }
    case 0x5a:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x5e: {
        const u8 *magic_ids;
        KfMapObject *object;
        VECTOR near_position;
        VECTOR far_position;
        s32 fraction;
        s32 spin;
        u8 magic_id;

        switch (command) {
        case 0x5a: magic_ids = DAT_800679a0; break;
        case 0x5b: magic_ids = DAT_800679a8; break;
        case 0x5c: magic_ids = DAT_800679b0; break;
        case 0x5d: magic_ids = DAT_800679b8; break;
        default: magic_ids = DAT_800679c0; break;
        }
        for (;;) {
            magic_id = *magic_ids++;
            if (magic_id == 0xff) {
                goto invoke_callback;
            }
            if (effect_state.magic_records[magic_id].menu_available == 0) {
                break;
            }
        }
        effect_state.magic_records[magic_id].menu_available = 1;
        object = map_object_effect_pool_acquire(
            0x15e, 10, map_object_state.unknown_873e);
        map_object_reset(object);
        func_800473e0(command);
        object->object_id = command;
        object->unknown_02 = 1;
        object->unknown_00 = 3;
        object->action = 0xff;
        object->unknown_05 = 0x42;
        object->rotation.vz = 0;
        object->rotation.vy = 0;
        object->rotation.vx = 0;

        func_80045f20(0, 200, 1500, player_state.camera_rotation.angles[0],
                      player_state.camera_rotation.angles[1], 0, 0,
                      &far_position);
        func_80045f20(0, 200, 1000, player_state.camera_rotation.angles[0],
                      player_state.camera_rotation.angles[1], 0, 0,
                      &near_position);
        spin = 0;
        for (fraction = 0; fraction < 4096; fraction += 64) {
            func_80045fd4((KfScenePoseView *)object, &near_position,
                          &far_position, 0, 0, fraction);
            object->rotation.vy += spin;
            spin += 4;
            object->unknown_10 = (rsin(fraction << 6) >> 2) + 1024;
            cd_request_service_vab();
            cd_request_service_stream();
            func_800335a0(0, 0);
        }
        do {
            object->unknown_10 -= 64;
            object->rotation.vy += spin;
            spin += 8;
            if ((s16)object->unknown_10 <= 0) {
                break;
            }
            cd_request_service_vab();
            cd_request_service_stream();
            func_800335a0(0, 0);
        } while (1);
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
        index = func_80047434(0x4d);
        if (index == 0) {
            notify_enqueue(0x16);
            func_800473e0(0x52);
            index = 1;
        }
        event_state.state_word = index;
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
        for (index = 0; index < KF_MAP_OBJECT_CAPACITY; index++) {
            KfMapObject *object = &map_object_state.objects[index];

            if (object->object_id != 0xe2 ||
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
        break;
    }
    case 0x56:
        func_800473e0(0x56);
        player_state.unknown_6c = 900;
        event_state.state_word = 1;
        break;
    case 0x57:
        func_800473e0(0x57);
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

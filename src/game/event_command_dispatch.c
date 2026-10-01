#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/event_stream.h>
#include <kf/game/map_object.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>
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
    case 0x54:
        player_state.unknown_6a = 1200;
        func_800473e0(0x54);
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
            if (status == 1) {
                audio_play_sound_64();
            } else if (status == 3) {
                notify_enqueue(object->tail.fields.unknown_3e.bytes.low);
            } else if (status == 4) {
                notify_enqueue(4);
            } else {
                index++;
                continue;
            }
            event_state.state_word = 1;
            break;
        }
        break;
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
    /* Remaining command arms are source WIP. The retail table has 35
     * entries; its indirect jump is not a proven direct-call edge. */
    }

invoke_callback:
    ((KfEventCommandCallback)state_8017d118.active_table[2])(
        position, rotation, command);
    if (event_state.state_word == 0) {
        notify_enqueue(0x14);
    }
    player_clear_motion();
}

#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/event_stream.h>
#include <kf/game/map_object.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>

extern s32 func_80045e5c(const VECTOR *position,
                         const struct KfEulerAngles *rotation);
extern void func_800366fc(u8 identifier);
extern s32 func_800368b4(KfMapObject *object, s32 command);
extern void func_80046700(KfEventObjectView *event, s32 object_id);

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
        if (func_80047434(0x4d) == 0) {
            notify_enqueue(0x16);
            func_800473e0(0x52);
        }
        break;
    case 0x54:
        player_state.unknown_6a = 1200;
        func_800473e0(0x54);
        break;
    case 0x56:
        func_800473e0(0x56);
        player_state.unknown_6c = 900;
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

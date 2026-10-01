#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/event_counter.h>
#include <kf/game/event_state.h>
#include <kf/game/map_object.h>
#include <kf/game/notify.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <kf/lib/math.h>

extern s32 func_80045e5c(const VECTOR *position,
                         const struct KfEulerAngles *angles);
extern s32 func_8003a9f4(s32 x, s32 y, s32 z, s32 radius, s32 height);
extern void func_800462bc(KfActor *actor);
extern void func_800475d8(KfMapObject *object, ...);
extern void func_80034e10(u16 archive_slot, u16 archive_entry);
extern void func_80028fa8(void);
extern void func_8001bcfc(void);
extern void func_800293d4(u8 mode);

ADDRESS(0x80047c98, 0x660)
void func_80047c98(const VECTOR *position, const KfPlayerViewRotation *rotation)
{
    VECTOR probe;
    s32 object_index;
    KfMapObject *objects;
    KfMapObjectTemplate *templates;

    probe.vx = position->vx;
    probe.vy = position->vy + 500;
    probe.vz = position->vz;
    event_state.state_word = 0;
    if (func_80045e5c(&probe, (const struct KfEulerAngles *)rotation) != 0) {
        func_800474c4(0x400, 0, 0, 0, 0x80, 0xa0, 0xff);
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
        func_800474c4(0x400, 0x80, 0xa0, 0xff, 0, 0, 0);
    }

    object_index = func_8003a9f4(probe.vx, probe.vy, probe.vz, 0x578, 0xc80);
    if (object_index != -1) {
        KfActor *actor = &actor_state.actors[object_index];
        s32 angle = vector_xz_to_angle(actor->position.vx - probe.vx,
                                       actor->position.vz - probe.vz);
        if (angle_within_tolerance(rotation->angles[1], angle, 300)) {
            func_800462bc(actor);
        }
    }

    objects = map_object_state.objects;
    templates = map_object_state.templates;
    object_index = 0;
    for (;;) {
        KfMapObject *object;
        u16 object_id;
        s32 kind;

        object_index = func_80036190(object_index, &probe, 800, 2500,
                                      rotation->angles[1], 512);
        if (object_index == -1) {
            break;
        }
        object = &objects[object_index];
        object_id = object->object_id;
        event_state.state_word = 1;
        kind = templates[object_id].collision_kind;
        switch (kind) {
        case 2:
            if (object->action_timer == 0) {
                if (object->tail.fields.unknown_38 == 0xff) {
                    object->action_timer = 1;
                } else {
                    notify_enqueue(object->tail.fields.unknown_3e.bytes.high);
                }
            }
            break;
        case 3:
            if (object->action_timer == 0) {
                u8 state = object->tail.fields.unknown_38;
                if (state >= 0xfc) {
                    if ((state & 1) &&
                        angle_within_tolerance(rotation->angles[1],
                                               object->rotation.vy, 900)) {
                        object->action_timer = 1;
                        break;
                    }
                    if ((state & 2) &&
                        angle_within_tolerance(rotation->angles[1],
                                               object->rotation.vy + 0x800,
                                               900)) {
                        object->action_timer = 1;
                        break;
                    }
                }
                if (state == 0x0f && game_counter_bytes[0x0f] != 0) {
                    object->action_timer = 1;
                    break;
                }
                notify_enqueue(object->tail.fields.unknown_3e.bytes.high);
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
            {
                u8 state = object->tail.fields.unknown_38;
                s32 linked_index = object->tail.fields.unknown_3a.value;
                if (state == 0xfe) {
                    if (linked_index == 0xffff ||
                        objects[linked_index].object_id == 0xff) {
                        notify_enqueue(6);
                    }
                } else if (state == 0xff) {
                    object->tail.fields.unknown_38 = 0xfe;
                } else {
                    notify_enqueue(object->tail.fields.unknown_3e.bytes.high);
                }
            }
            break;
        case 9:
        case 0x15: {
            u16 linked_index = object->tail.fields.unknown_3a.value;
            if (linked_index == 0xffff ||
                objects[linked_index].object_id == 0xff) {
                notify_enqueue(object->tail.fields.unknown_3e.bytes.high);
                break;
            }
            {
                KfMapObject *linked = &objects[linked_index];
                linked->tail.fields.unknown_38 = 0xff;
                linked->unknown_00 = object->extra_40.bytes[0];
                func_800475d8(linked);
                if (linked->object_id == 0xff) {
                    object->tail.fields.unknown_3a.value = 0xffff;
                }
                linked->unknown_00 = 0;
                linked->tail.fields.unknown_38 = 0;
            }
            break;
        }
        case 0x0d:
        case 0x14:
            func_80034e10(6,
                *(u16 *)&object->tail.fields.unknown_38 + 0x78);
            break;
        case 0x0e:
            func_80048554(state_8017d118.values_04[0]);
            func_80028fa8();
            func_8001bcfc();
            break;
        case 0x0f:
            if (object->tail.fields.unknown_38 == 0xff) {
                notify_enqueue(0x10);
            }
            break;
        case 0x20:
            if (player_state.death_state == 0) {
                u8 *linked_state;
                func_800293d4(object_index);
                linked_state = (u8 *)object->extra_40.record;
                if (linked_state[1] == 1) {
                    linked_state[1] = 5;
                }
            }
            break;
        case 0x40:
            func_800475d8(object);
            if (object->object_id == 0xff) {
                goto invoke_callback;
            }
            break;
        case 0x51:
            if (object->action_timer == 0 &&
                object->tail.fields.unknown_38 == 0xff) {
                object->action_timer = 1;
            }
            break;
        case 0x53:
            if (object->action_timer == 0) {
                object->action_timer = 1;
            }
            break;
        case 0xa5:
        case 0xff:
            if (object->tail.fields.unknown_3e.bytes.high != 0xff) {
                notify_enqueue(object->tail.fields.unknown_3e.bytes.high);
            }
            break;
        case 0x12:
            func_800474c4(0x200, 0, 0, 0, 0x80, 0xc8, 0xff);
            player_state.vitals.current_hp = player_state.vitals.maximum_hp;
            func_800474c4(0x200, 0x80, 0xc8, 0xff, 0, 0, 0);
            break;
        }
        object_index++;
    }

invoke_callback:
    ((void (*)(const VECTOR *, const KfPlayerViewRotation *))
        state_8017d118.active_table[0])(&probe, rotation);
}

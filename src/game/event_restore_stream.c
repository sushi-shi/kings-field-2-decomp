#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/event_state.h>
#include <kf/game/map_object.h>
#include <kf/lib/address.h>
#include <psyq/sdk.h>

RODATA(0x80012bf8, 0x40)

ADDRESS(0x800489ac, 0x378)
void func_800489ac(s32 save_slot)
{
    u8 *saved[10];
    u8 *stream;
    s32 index;
    KfActor *actors;
    KfTargetGroup *groups;
    KfMapObject *object;

    func_800483d8(saved);
    stream = saved[save_slot];
    if (stream == 0) {
        return;
    }

    actors = actor_state.actors;
    for (;;) {
        s32 actor_index = *stream++;
        if (actor_index == 0xff) {
            break;
        }
        actors[actor_index].lifecycle = *stream++;
    }

    groups = actor_state.target_groups;
    for (;;) {
        s32 group_index = *stream++;
        KfTargetCandidate *candidate;
        if (group_index == 0xff) {
            break;
        }
        candidate = groups[group_index].targets[0].pointer;
        candidate->fallback_offset = *stream++;
        candidate->marker_state = *stream++;
    }

    object = map_object_state.objects;
    for (index = 0; index < KF_MAP_OBJECT_CAPACITY; object++, index++) {
        u8 opcode = *stream++;
        u16 x;
        u16 y;
        u16 z;

        switch (opcode - 0xf0) {
        case 15:
            object->object_id = 0xff;
            break;
        case 4:
            object->tail.fields.unknown_38 = *stream++;
            object->tail.fields.unknown_39 = *stream++;
            break;
        case 0: {
            s32 x_high;
            s32 z_high;
            s32 y_high;
            s32 angle;

            map_object_reset(object);
            object->action = 0x60;
            object->object_id = *stream++;
            x = *stream++;
            x_high = *stream++;
            z = *stream++;
            z_high = *stream++;
            y = *stream++;
            y_high = *stream++;
            angle = *stream++;
            x |= x_high << 8;
            z |= z_high << 8;
            y |= y_high << 8;
            object->rotation.vz = 0x400;
            object->rotation.vy = angle << 4;
apply_position:
            object->position.vx = x << 2;
            object->position.vz = z << 2;
            object->position.vy = (s16)y;
            object->action_timer = 0x63;
            func_8002a988(object->position.vx, object->position.vy,
                           object->position.vz);
            object->unknown_00 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
            object->tail.fields.unknown_38 = 0xff;
            continue;
        }
        case 1:
            map_object_reset(object);
            object->action = 0x61;
            object->object_id = *stream++;
            x = *stream++;
            x |= *stream++ << 8;
            z = *stream++;
            z |= *stream++ << 8;
            y = *stream++;
            y |= *stream++ << 8;
            goto apply_position;
        case 2:
            map_object_reset(object);
            object->action = 0x62;
            object->object_id = *stream++;
            x = *stream++;
            x |= *stream++ << 8;
            z = *stream++;
            z |= *stream++ << 8;
            y = *stream++;
            y |= *stream++ << 8;
            object->tail.fields.unknown_3a.value = (*stream << 2) + (rand() >> 13);
            stream++;
            if (object->object_id == 0x67) {
                object->rotation.vx = 0x400;
            }
            goto apply_position;
        case 3:
            map_object_reset(object);
            object->action = 0x70;
            object->object_id = *stream++;
            /* The next byte is also the body of opcode 0xfd. */
        case 13:
            object->tail.fields.unknown_38 = *stream++;
            break;
        default:
            break;
        }
    }
}

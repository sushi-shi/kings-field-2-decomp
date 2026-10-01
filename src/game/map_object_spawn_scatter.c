#include <kf/lib/address.h>
#include <kf/game/map_object.h>
#include <psyq/sdk.h>
#include <psyq/libc.h>

enum { KF_MAP_OBJECT_SCATTER_RADIUS = 600 };

RODATA(0x800118c4, 0x54)

ADDRESS(0x800365d8, 0x124)
void func_800365d8(u16 parameter, const VECTOR *origin, s32 height_offset)
{
    KfMapObject *object;
    u16 sequence;
    u16 angle;

    object = map_object_effect_pool_acquire(0x15e, 10,
                                            map_object_state.unknown_873e);
    map_object_reset(object);
    sequence = map_object_state.unknown_873e;
    map_object_state.unknown_873e = sequence + 1;
    object->tail.fields.spawn_sequence = sequence;
    object->object_id = 0x46;
    object->tail.fields.unknown_3a.value = parameter;
    angle = (u16)(rand() >> 3);
    object->position.vx = origin->vx +
        ((rsin(angle) * KF_MAP_OBJECT_SCATTER_RADIUS) >> 12);
    object->position.vy = origin->vy + height_offset;
    object->position.vz = origin->vz +
        ((rcos(angle) * KF_MAP_OBJECT_SCATTER_RADIUS) >> 12);
    object->rotation.vy = rand() >> 3;
    object->tail.fields.unknown_38 = 0xff;
    map_object_start_action_if_idle(object, 0x62);
    object->tail.fields.unknown_3e.signed_value = -120;
}

ADDRESS(0x800366fc, 0x1b8)
void func_800366fc(u8 identifier)
{
    KfMapObject *object = map_object_state.objects;
    u16 remaining;

    if (identifier == 0xff) {
        return;
    }
    remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        switch (object->action) {
        case 0x50:
        case 0x54:
        case 0x5f:
        case 0xa2:
        case 0xa3:
            if (object->tail.fields.unknown_38 == identifier) {
                object->tail.fields.unknown_38 = 0xff;
            }
            break;
        case 0x58:
            if (object->tail.fields.unknown_39 == identifier) {
                object->action_timer = 1;
                object->tail.fields.unknown_38 =
                    object->tail.fields.unknown_38 == 0;
            }
            break;
        case 0x51:
            if (((u8 *)&object->tail.fields.spawn_sequence)[1] == identifier) {
                object->tail.fields.unknown_38 =
                    object->tail.fields.unknown_38 == 0 ? 0xff : 0;
            }
            break;
        case 0x59:
            if (object->tail.fields.unknown_38 == identifier) {
                object->action_timer = 1;
            }
            break;
        case 2:
        case 3:
        case 4:
            if ((u8)(identifier + 106) < 49) {
                if ((object->tail.fields.unknown_38 & 0xfe) == identifier) {
                    object->tail.fields.unknown_38 ^= 1;
                }
            } else {
                u8 marker = object->tail.fields.unknown_38;

                if (marker == identifier) {
                    if (marker >= 200) {
                        object->tail.fields.unknown_38 = 0xff;
                    } else if (object->action_timer == 0) {
                        object->action_timer = 1;
                        if (marker >= 100) {
                            object->tail.fields.unknown_38 = 0xff;
                        }
                    }
                }
            }
            break;
        }
        object++;
    } while (remaining-- != 0);
}

ADDRESS(0x800368b4, 0x90)
s32 func_800368b4(KfMapObject *object, s32 marker)
{
    switch (object->action) {
    case 2:
    case 3:
    case 4:
        if (object->action_timer != 0) {
            return 0;
        }
        /* Fall through to the active marker check. */
    case 5:
    case 8:
    case 22:
        if (object->tail.fields.unknown_38 >= 0xfe) {
            return 2;
        }
        if (object->tail.fields.unknown_38 == marker) {
            object->tail.fields.unknown_38 = 0xff;
            return 1;
        }
        return 3;
    case 15:
    case 17:
        return 4;
    default:
        return 0;
    }
}

#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/map_object.h>
#include <psyq/libc.h>

RODATA(0x8001187c, 0x44)

ADDRESS(0x80036190, 0x22c)
s32 func_80036190(s32 first_index, const VECTOR *position, s32 radius,
                   s32 point_height, s32 angle, s32 tolerance)
{
    KfMapObject *object;
    KfMapObjectTemplate *template;
    SVECTOR forward;
    VECTOR rotated;
    MATRIX rotation;
    s32 index;
    s32 direction;

    object = &map_object_state.objects[first_index];
    index = first_index;
    if ((s16)first_index >= KF_MAP_OBJECT_CAPACITY) {
        return -1;
    }

    for (; (s16)index < KF_MAP_OBJECT_CAPACITY; index++, object++) {
        if (object->object_id == KF_MAP_OBJECT_ID_NONE) {
            continue;
        }
        template = &map_object_state.templates[object->object_id];
        if (template->collision_kind == 4) {
            forward.vx = 0x700;
            forward.vy = 0;
            forward.vz = 0;
            matrix_set_rotation_y(object->rotation.vy, &rotation);
            ApplyMatrix(&rotation, &forward, &rotated);
            rotated.vx += position->vx;
            rotated.vz += position->vz;
            if (vector_distance_to_point(&object->position,
                                         rotated.vx, position->vy, rotated.vz,
                                         template->interaction_radius + radius,
                                         object->collision_height, point_height) == -1) {
                continue;
            }
            direction = vector_xz_to_angle(
                object->position.vx - rotated.vx,
                object->position.vz - rotated.vz);
        } else {
            if (vector_distance_to_point(&object->position,
                                         position->vx, position->vy, position->vz,
                                         template->interaction_radius + radius,
                                         template->interaction_height, point_height) == -1) {
                continue;
            }
            if (object->collision_flags & 4) {
                return (s16)index;
            }
            direction = vector_xz_to_angle(
                object->position.vx - position->vx,
                object->position.vz - position->vz);
        }
        if (angle_within_tolerance(angle, direction, tolerance)) {
            return (s16)index;
        }
    }
    return -1;
}

ADDRESS(0x800363bc, 0x20)
void map_object_start_action_if_idle(KfMapObject *object, u8 action)
{
    if (object->action == KF_MAP_OBJECT_ACTION_NONE) {
        object->action = action;
        object->action_timer = KF_MAP_OBJECT_ACTION_TIMER_INIT;
    }
}

ADDRESS(0x800363dc, 0x88)
KfMapObject *map_object_effect_pool_acquire(s32 first_index, s32 count, s32 sequence)
{
    KfMapObject *object = &map_object_state.objects[first_index];
    KfMapObject *oldest = 0;
    s32 oldest_age = 0;
    s32 age;

    do {
        if (object->object_id == KF_MAP_OBJECT_ID_NONE) {
            return object;
        }
        if (sequence != -1) {
            age = sequence - object->tail.fields.spawn_sequence;
            if (age < 0) {
                age += KF_MAP_OBJECT_SPAWN_SEQUENCE_MODULUS;
            }
            if (oldest_age < age) {
                oldest = object;
                oldest_age = age;
            }
        }
        object++;
    } while (--count != 0);
    return oldest;
}

ADDRESS(0x80036464, 0x174)
void map_object_spawn_effect(u8 source, u8 object_id, const VECTOR *position,
                             s32 height_offset)
{
    KfMapObject *object;
    KfMapObjectTemplate *template;
    u16 *sequence;
    s32 first_index;

    if (source == 0) {
        sequence = &map_object_state.unknown_8742;
        first_index = 0x172;
    } else {
        first_index = 0x168;
        sequence = &map_object_state.unknown_8740;
    }

    object = map_object_effect_pool_acquire(first_index, 10, *sequence);
    object->tail.fields.spawn_sequence = (*sequence)++;
    object->object_id = object_id;
    map_object_reset(object);
    template = &map_object_state.templates[object->object_id];
    object->position.vx = position->vx;
    object->position.vy = height_offset + position->vy;
    object->position.vz = position->vz;
    object->rotation.vy = rand() >> 3;

    switch (template->kind) {
    case 0x10:
    case 0x13:
    case 0x16:
        map_object_start_action_if_idle(object, 0x60);
        break;
    case 0x17:
        map_object_start_action_if_idle(object, 0x61);
        break;
    case 0x11:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x18:
    case 0x19:
    case 0x20:
        map_object_start_action_if_idle(object, 0x62);
        if (object_id == 0x67) {
            object->rotation.pad = 0x400;
        } else {
            object->rotation.pad = 0;
        }
        break;
    }
    object->tail.fields.unknown_3e = 0;
    object->tail.fields.unknown_38 = 0xff;
}

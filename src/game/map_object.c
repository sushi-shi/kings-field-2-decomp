#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/map_object.h>
#include <kf/game/animation.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

RODATA(0x8001187c, 0x9c)

ADDRESS(0x80036190, 0x22c)
s32 map_object_find_interaction_target(s32 first_index, const VECTOR *position, s32 radius,
                   s32 point_height, s32 angle, s32 tolerance)
{
    KfMapObject *object;
    KfMapObjectTemplate *template;
    SVECTOR forward;
    VECTOR rotated;
    MATRIX rotation;
    s16 index;
    s32 direction;

    object = &map_object_state.objects[first_index];
    index = first_index;
    for (; index < KF_MAP_OBJECT_CAPACITY; index++, object++) {
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
                return index;
            }
            direction = vector_xz_to_angle(
                object->position.vx - position->vx,
                object->position.vz - position->vz);
        }
        if (angle_within_tolerance(angle, direction, tolerance)) {
            return index;
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
    object->tail.fields.unknown_3e.value = 0;
    object->tail.fields.unknown_38 = 0xff;
}

enum { KF_MAP_OBJECT_SCATTER_RADIUS = 600 };



ADDRESS(0x800365d8, 0x124)
void map_object_spawn_scattered_effect(u16 effect_id, const VECTOR *origin,
                                       s32 height_offset)
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
    object->tail.fields.unknown_3a.value = effect_id;
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
void map_object_apply_marker_signal(u8 identifier)
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
            if (object->tail.spawn_bytes.spawn_sequence.high == identifier) {
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
s32 map_object_check_and_consume_marker(KfMapObject *object, s32 marker)
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

ADDRESS(0x80036944, 0x74)
void map_object_process_marker_all(u8 marker)
{
    KfMapObject *object = map_object_state.objects;
    u16 remaining;

    if (marker == 0xff) {
        return;
    }
    remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        map_object_check_and_consume_marker(object, marker);
        object++;
    } while (remaining-- != 0);
}

ADDRESS(0x800369b8, 0x120)
void map_object_sample_world_vertex(KfMapObject *object, s32 vertex_index, VECTOR *result)
{
    struct KfEulerAngles angles;
    SVECTOR vertex;

    animation_sample_vertex(
        object->object_id + 0x100, object->unknown_01, object->unknown_0a,
        vertex_index, &vertex);

    vertex.vx = (vertex.vx * object->scale.vx) >> 12;
    vertex.vy = (vertex.vy * object->scale.vy) >> 12;
    vertex.vz = (vertex.vz * object->scale.vz) >> 12;

    angles.x = (u16)object->rotation.vx;
    angles.y = (u16)object->rotation.vy + 0x800;
    angles.z = (u16)object->rotation.vz;
    vector_rotate_yxz(&angles, &vertex, result);

    addVector(result, &object->position);
}

ADDRESS(0x80036ad8, 0x90)
s32 player_camera_within_map_region(s32 x, s32 z, s32 width, s32 depth, s32 height)
{
    s32 camera_x = player_state.camera_position.vx >> 11;
    s32 camera_z = player_state.camera_position.vz >> 11;

    if (camera_x < x || camera_x >= x + width ||
        camera_z < z || camera_z >= z + depth) {
        goto outside;
    }
    if (height == 0x8000) {
        return 1;
    }
    if (height + 2048 < player_state.camera_position.vy ||
        player_state.camera_position.vy < height - 3200) {
        goto outside;
    }
    return 1;
outside:
    return 0;
}

enum {
    KF_MAP_OBJECT_MOTION_ACTION = 0x70,
    KF_MAP_OBJECT_MOTION_COMPLETE_ACTION = 0x63,
    KF_MAP_OBJECT_MOTION_STEP = 204,
    KF_MAP_OBJECT_MOTION_LIMIT = 0xfff
};

ADDRESS(0x80036b68, 0x2bc)
s32 map_object_step_offset_motion(KfMapObject *source, KfMapObject *target,
                   SVECTOR *start_offset, SVECTOR *end_offset,
                   s32 brighten, s32 duration)
{
    VECTOR start_position;
    VECTOR end_position;
    s32 fraction;

    switch (source->action_timer) {
    case 0:
        if (source->tail.fields.unknown_38 == 0xff) {
            return 0;
        }
        if (target->tail.fields.unknown_38 == 0) {
            source->extra_40.bytes[0] = 0;
        } else {
            source->extra_40.bytes[0] = duration - 1;
        }
        source->action_timer = 1;
        map_object_reset(target);
        target->action = KF_MAP_OBJECT_MOTION_ACTION;
        target->unknown_00 = source->unknown_00;
        if (brighten) {
            target->unknown_01 = 0;
        } else {
            target->unknown_01 = 0x80;
        }
        vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                          start_offset, &target->position);
        addVector(&target->position, &source->position);
        target->rotation.vy = source->rotation.vy;
        target->extra_40.object_index = source - map_object_state.objects;
        break;
    case 1:
        break;
    default:
        return 0;
    }

    if (brighten) {
        target->unknown_0a += KF_MAP_OBJECT_MOTION_STEP;
        if (target->unknown_0a >= 0x1000) {
            target->unknown_0a = KF_MAP_OBJECT_MOTION_LIMIT;
        }
    }
    vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                      start_offset, &start_position);
    vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                      end_offset, &end_position);
    source->extra_40.bytes[0]++;
    fraction = ((s32)source->extra_40.bytes[0] << 12) / duration;
    target->position.vx = source->position.vx +
        fixed_lerp_q12(start_position.vx, end_position.vx, fraction);
    target->position.vy = source->position.vy +
        fixed_lerp_q12(start_position.vy, end_position.vy, fraction);
    target->position.vz = source->position.vz +
        fixed_lerp_q12(start_position.vz, end_position.vz, fraction);
    if (source->extra_40.bytes[0] >= duration) {
        if (brighten) {
            target->unknown_0a = KF_MAP_OBJECT_MOTION_LIMIT;
        }
        map_object_play_spatial_sound(target, 0x42);
        target->tail.fields.unknown_38 = 0xff;
        source->action_timer = KF_MAP_OBJECT_MOTION_COMPLETE_ACTION;
        return 1;
    }
    return 0;
}

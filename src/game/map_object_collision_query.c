#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/map_object.h>

ADDRESS(0x80036078, 0x118)
s32 map_object_find_collision_at_point(s32 x, s32 y, s32 z, s32 radius, s32 point_height)
{
    KfMapObject *object = map_object_state.objects;
    s32 index;

    for (index = 0; index < KF_MAP_OBJECT_CAPACITY; index++, object++) {
        if (object->object_id == KF_MAP_OBJECT_ID_NONE) {
            continue;
        }
        if (object->unknown_00 == 0) {
            continue;
        }
        if (object == map_object_state.current_collision_object) {
            continue;
        }
        if (map_object_state.templates[object->object_id].collision_radius == 0) {
            continue;
        }
        if (vector_distance_to_point(
                &object->position, x, y, z,
                map_object_state.templates[object->object_id].collision_radius + radius,
                object->collision_height, point_height) != -1) {
            return index;
        }
    }
    return -1;
}

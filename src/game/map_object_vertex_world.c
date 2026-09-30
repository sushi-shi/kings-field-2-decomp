#include <kf/game/map_object.h>
#include <kf/game/animation.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <kf/lib/math.h>

ADDRESS(0x800369b8, 0x120)
void func_800369b8(KfMapObject *object, s32 vertex_index, VECTOR *result)
{
    struct KfEulerAngles angles;
    SVECTOR vertex;

    func_80034344(
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
s32 func_80036ad8(s32 x, s32 z, s32 width, s32 depth, s32 height)
{
    s32 camera_x = player_state.camera_position.vx >> 11;
    s32 camera_z = player_state.camera_position.vz >> 11;

    if (camera_x < x || camera_x >= x + width ||
        camera_z < z || camera_z >= z + depth) {
        goto outside;
    }
    if (height == 0x8000) {
        goto inside;
    }
    if (height + 2048 < player_state.camera_position.vy ||
        player_state.camera_position.vy < height - 3200) {
        goto outside;
    }
inside:
    return 1;
outside:
    return 0;
}

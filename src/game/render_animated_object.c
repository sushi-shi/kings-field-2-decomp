#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/graphics.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

ADDRESS(0x80031d8c, 0x214)
void func_80031d8c(s32 asset_index, const struct KfEulerAngles *rotation,
                   KfPoolRecord **cache, s32 clip, u16 phase,
                   s32 blend_mode, s32 lighting_flags, s16 depth)
{
    MATRIX model;
    MATRIX reversed_light;
    KfCollisionRow *lighting;
    KfTmdObject *object;
    s32 object_index;

    model.t[2] = 0;
    model.t[1] = 0;
    model.t[0] = 0;
    matrix_set_rotation_yxz(rotation, &model);
    MulMatrix2(&game_graphics_runtime.render_state.view_matrix, &model);
    SetRotMatrix(&model);
    SetTransMatrix(&model);

    lighting = &game_graphics_runtime.collision_rows[lighting_flags & 0x7f];
    SetColorMatrix((MATRIX *)&lighting->motion);
    SetBackColor(lighting->filter.kinds.types[0],
                 lighting->filter.kinds.types[1],
                 lighting->filter.kinds.types[2]);
    if (lighting_flags & 0x80) {
        reversed_light.m[0][0] = -lighting->rotations[0].m[0][0];
        reversed_light.m[0][1] = -lighting->rotations[0].m[0][1];
        reversed_light.m[0][2] = -lighting->rotations[0].m[0][2];
        reversed_light.m[1][0] = -lighting->rotations[0].m[1][0];
        reversed_light.m[1][1] = -lighting->rotations[0].m[1][1];
        reversed_light.m[1][2] = -lighting->rotations[0].m[1][2];
        reversed_light.m[2][0] = -lighting->rotations[0].m[2][0];
        reversed_light.m[2][1] = -lighting->rotations[0].m[2][1];
        reversed_light.m[2][2] = -lighting->rotations[0].m[2][2];
        SetLightMatrix(&reversed_light);
    } else {
        SetLightMatrix((MATRIX *)&lighting->rotations[0]);
    }

    object_index = asset_index & 0xffff;
    asset_registry_select(object_index);
    object = tmd_get_object(0);
    if (func_80034070(cache, object_index, clip & 0xffff, phase,
                      object->vertex_count) == 0) {
        tmd_select_object_vertices(0);
        object = tmd_get_object(0);
        tmd_project_vertices(object->vertex_count);
    } else {
        tmd_project_vertices(object->vertex_count);
    }
    func_8002ebe0(0, blend_mode, depth);
}

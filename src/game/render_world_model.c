#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/asset.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

extern void func_8002e4dc(s32 object_index, s32 depth_bias);
extern void func_8002ddb4(u16 object_index, s32 depth_bias, s32 render_mode);

ADDRESS(0x80031850, 0x53c)
void func_80031850(u8 map_layer, u16 asset_index, const VECTOR *position,
                   const struct KfEulerAngles *rotation, const SVECTOR *scale,
                   KfPoolRecord **cache, MATRIX *world_matrix, u16 clip,
                   u16 phase, u8 lighting_override, s16 lighting_blend,
                   u8 render_mode, s32 depth)
{
    SVECTOR relative;
    VECTOR scale_vector;
    MATRIX model;
    MATRIX light_matrix;
    MATRIX color_matrix;
    long gte_flags;
    KfCollisionRow *lighting;
    KfCollisionRow *override;
    KfCollisionRow *light_rotation;
    KfTmdObject *object;
    const u8 *cell_lighting;
    s32 light_index;
    s32 object_index;
    s32 red;
    s32 green;
    s32 blue;

    SetRotMatrix(&game_graphics_runtime.render_state.view_matrix);
    SetTransMatrix(&game_graphics_runtime.render_state.view_matrix);
    if (world_matrix != 0) {
        relative.vx = (s16)position->vx -
                      (s16)game_graphics_runtime.render_state.view_position.vx;
        relative.vy = (s16)position->vy -
                      (s16)game_graphics_runtime.render_state.view_position.vy;
        relative.vz = (s16)position->vz -
                      (s16)game_graphics_runtime.render_state.view_position.vz;
        RotTrans(&relative, (VECTOR *)&model.t, &gte_flags);
        cell_lighting = (const u8 *)&bss_801c7540.map_cells[0][0];
        cell_lighting += (position->vz >> 11) * sizeof(bss_801c7540.map_cells[0]);
        cell_lighting += (position->vx >> 11) * sizeof(KfMapOccupancyCell);
        if (map_layer != 1) {
            cell_lighting += sizeof(KfMapOccupancyLayer);
        }
        light_index = cell_lighting[4] & 0x3f;
    } else {
        model.t[0] = position->vx;
        model.t[1] = position->vy;
        model.t[2] = position->vz;
        cell_lighting = &bss_801c7540.map_cells[0][0].layer[0].lighting_index;
        cell_lighting += (game_graphics_runtime.render_state.view_position.vz >> 11) *
                         sizeof(bss_801c7540.map_cells[0]);
        cell_lighting += (game_graphics_runtime.render_state.view_position.vx >> 11) *
                         sizeof(KfMapOccupancyCell);
        cell_lighting += player_state.unknown_128;
        light_index = *cell_lighting & 0x3f;
    }
    lighting = &game_graphics_runtime.collision_rows[light_index];

    if (render_mode == 0x80) {
        render_mode = 0xff;
    } else if (relative.vy <= 0) {
        depth += 240;
    }

    matrix_set_rotation_yxz(rotation, &model);
    if (scale != 0) {
        scale_vector.vx = scale->vx;
        scale_vector.vy = scale->vy;
        scale_vector.vz = scale->vz;
        ScaleMatrix(&model, &scale_vector);
    }

    if (lighting_override != 0xff) {
        override = &game_graphics_runtime.collision_rows[lighting_override];
        if (override->motion.values[0] != -1) {
            func_800158b4((const u16 *)lighting->motion.values,
                          (const u16 *)override->motion.values,
                          (u16 *)&color_matrix, lighting_blend);
            SetColorMatrix(&color_matrix);
        } else {
            SetColorMatrix((MATRIX *)&lighting->motion);
        }
        light_rotation = lighting;
        if (override->rotations[0].m[0][0] != -1) {
            light_rotation = override;
        }
        MulMatrix0((MATRIX *)&light_rotation->rotations[0], &model,
                   &light_matrix);
        if (override->filter.angle != -1) {
            fog_set_near(func_8001584c(lighting->filter.angle,
                                       override->filter.angle,
                                       lighting_blend));
        } else {
            fog_set_near(lighting->filter.angle);
        }
        if (override->filter.kinds.types[0] != 0xff) {
            red = func_8001584c(lighting->filter.kinds.types[0],
                                override->filter.kinds.types[0], lighting_blend);
            green = func_8001584c(lighting->filter.kinds.types[1],
                                  override->filter.kinds.types[1], lighting_blend);
            blue = func_8001584c(lighting->filter.kinds.types[2],
                                 override->filter.kinds.types[2], lighting_blend);
            SetBackColor(red, green, blue);
        } else {
            SetBackColor(lighting->filter.kinds.types[0],
                         lighting->filter.kinds.types[1],
                         lighting->filter.kinds.types[2]);
        }
    } else {
        SetColorMatrix((MATRIX *)&lighting->motion);
        fog_set_near(lighting->filter.angle);
        SetBackColor(lighting->filter.kinds.types[0],
                     lighting->filter.kinds.types[1],
                     lighting->filter.kinds.types[2]);
        MulMatrix0((MATRIX *)&lighting->rotations[0], &model, &light_matrix);
    }
    SetLightMatrix(&light_matrix);
    if (world_matrix != 0) {
        MulMatrix2(world_matrix, &model);
    }
    SetRotMatrix(&model);
    SetTransMatrix(&model);

    asset_registry_select(asset_index);
    object_index = 0;
    if (clip < 0x80) {
        object = tmd_get_object(0);
        if (func_80034070(cache, asset_index, clip, phase,
                          object->vertex_count) == 0) {
            tmd_select_object_vertices(0);
        }
    } else {
        object_index = clip & 0x7f;
        tmd_select_object_vertices(object_index);
        object = tmd_get_object(object_index);
    }
    if (world_matrix == 0) {
        tmd_transform_vertices_depth(object->vertex_count, depth);
    } else {
        func_8002d918(object->vertex_count);
    }
    if (render_mode == 0xff) {
        func_8002e4dc(object_index, depth);
    } else if (render_mode == 0xfe) {
        func_8002f808(object_index, depth, 0);
    } else {
        func_8002ddb4(object_index, depth, render_mode);
    }
}

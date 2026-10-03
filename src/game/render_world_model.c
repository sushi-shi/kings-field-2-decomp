#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/asset.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

ADDRESS(0x80031850, 0x53c)
void render_world_model(u8 map_layer, u16 asset_index, const VECTOR *position,
                   const struct KfEulerAngles *rotation, const SVECTOR *scale,
                   KfPoolRecord **cache, MATRIX *world_matrix, u16 clip,
                   u16 phase, u8 lighting_override, s16 lighting_blend,
                   u8 render_mode, s32 depth)
{
    /* Retail reads vy after a null world_matrix path without initializing it. */
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
    u16 object_index;
    s32 red;
    s32 green;
    s32 blue;

    SetRotMatrix(&game_graphics_runtime.render_state.view_matrix);
    SetTransMatrix(&game_graphics_runtime.render_state.view_matrix);
    if (world_matrix != 0) {
        KfMapOccupancyCell *cell;
        KfMapOccupancyCell *row;
        KfMapOccupancyLayer *lighting_layer;

        relative.vx = (s16)position->vx -
                      (s16)game_graphics_runtime.render_state.view_position.vx;
        relative.vy = (s16)position->vy -
                      (s16)game_graphics_runtime.render_state.view_position.vy;
        relative.vz = (s16)position->vz -
                      (s16)game_graphics_runtime.render_state.view_position.vz;
        RotTrans(&relative, (VECTOR *)&model.t, &gte_flags);
        row = bss_801c7540.map_cells[position->vz >> 11];
        cell = &row[position->vx >> 11];
        if (map_layer != 1) {
            lighting_layer = &cell->layer[1];
        } else {
            lighting_layer = &cell->layer[0];
        }
        lighting = &game_graphics_runtime.collision_rows[
            lighting_layer->lighting_index & 0x3f];
    } else {
        model.t[0] = position->vx;
        model.t[1] = position->vy;
        model.t[2] = position->vz;
        {
            u16 layer_offset = player_state.map_layer_index;
            u8 lighting_index = *(
                &bss_801c7540.map_cells[
                    game_graphics_runtime.render_state.view_position.vz >> 11][
                    game_graphics_runtime.render_state.view_position.vx >> 11]
                    .layer[0].lighting_index + layer_offset);
            lighting = &game_graphics_runtime.collision_rows[lighting_index & 0x3f];
        }
    }

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
            fixed_lerp_nine_halfwords_q12((const u16 *)lighting->motion.values,
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
            fog_set_near(fixed_lerp_q12(lighting->filter.angle,
                                       override->filter.angle,
                                       lighting_blend));
        } else {
            fog_set_near(lighting->filter.angle);
        }
        if (override->filter.kinds.types[0] != 0xff) {
            red = fixed_lerp_q12(lighting->filter.kinds.types[0],
                                override->filter.kinds.types[0], lighting_blend);
            green = fixed_lerp_q12(lighting->filter.kinds.types[1],
                                  override->filter.kinds.types[1], lighting_blend);
            blue = fixed_lerp_q12(lighting->filter.kinds.types[2],
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
    if (clip < 0x80) {
        object_index = 0;
        object = tmd_get_object(0);
        if (animation_prepare_asset_vertices(cache, asset_index, clip, phase,
                          object->vertex_count) == 0) {
            tmd_select_object_vertices(0);
        }
    } else {
        object_index = clip & 0x7f;
        tmd_select_object_vertices(object_index);
        object = tmd_get_object(object_index);
    }
    if (world_matrix != 0) {
        tmd_project_vertices_with_fog(object->vertex_count);
    } else {
        tmd_transform_vertices_depth(object->vertex_count, depth);
    }
    if (render_mode == 0xff) {
        render_enqueue_textured_tmd(object_index, depth);
    } else if (render_mode == 0xfe) {
        render_enqueue_tmd_with_clipping(object_index, depth, 0);
    } else {
        render_enqueue_blended_tmd(object_index, depth, render_mode);
    }
}

ADDRESS(0x80031d8c, 0x214)
void render_animated_object(s32 asset_index, const struct KfEulerAngles *rotation,
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
    if (animation_prepare_asset_vertices(cache, object_index, clip & 0xffff, phase,
                      object->vertex_count) == 0) {
        tmd_select_object_vertices(0);
        object = tmd_get_object(0);
        tmd_project_vertices(object->vertex_count);
    } else {
        tmd_project_vertices(object->vertex_count);
    }
    render_enqueue_tmd_fixed_depth(0, blend_mode, depth);
}

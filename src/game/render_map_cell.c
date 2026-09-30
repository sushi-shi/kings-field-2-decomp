#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/callback.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/player.h>
#include <kf/game/asset.h>
#include <kf/game/render_model.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>

enum {
    KF_MAP_CELL_ORIENTATION_MASK = 3,
    KF_MAP_CELL_LIGHTING_MASK = 63,
    KF_MAP_CELL_OBJECT_SPECIAL = 0x80,
    KF_MAP_CELL_OBJECT_PREPARE = 0x40,
    KF_MAP_CELL_PREPARED_LIMIT = 16
};

extern void func_8002ff5c(KfTmdHeader *asset, u16 object_index,
                           KfTmdPreparedAsset *prepared_asset);

ADDRESS(0x80030c18, 0x1cc)
void render_map_cell_object(const KfMapCellShape *shape, SVECTOR *position,
                            u32 flags)
{
    MATRIX cell_matrix;
    long gte_flags;
    KfCollisionRow *lighting;
    u16 object_index;
    s32 orientation;

    orientation = shape->quarter_turns & KF_MAP_CELL_ORIENTATION_MASK;
    SetRotMatrix(&game_graphics_runtime.render_state.view_matrix);
    SetTransMatrix(&game_graphics_runtime.render_state.view_matrix);
    RotTrans(position, (VECTOR *)&cell_matrix.t, &gte_flags);
    matrix_rotate_quarter_turns(&game_graphics_runtime.render_state.view_matrix,
                                &cell_matrix, orientation);
    SetRotMatrix(&cell_matrix);
    SetTransMatrix(&cell_matrix);

    lighting = &game_graphics_runtime.collision_rows[
        shape->lighting_index & KF_MAP_CELL_LIGHTING_MASK];
    SetLightMatrix((MATRIX *)&lighting->rotations[orientation]);
    SetColorMatrix((MATRIX *)&lighting->motion);
    fog_set_near(lighting->filter.angle);
    SetBackColor(lighting->filter.kinds.types[0],
                 lighting->filter.kinds.types[1],
                 lighting->filter.kinds.types[2]);

    object_index = shape->object_index;
    if (state_8017d118.transition_active == 1 && state_8017d118.unknown_15[1] &&
        object_index >= game_graphics_runtime.tmd_state.current_asset->flags) {
        return;
    }
    tmd_select_object_vertices(object_index);
    if (flags & KF_MAP_CELL_OBJECT_SPECIAL) {
        if (flags & KF_MAP_CELL_OBJECT_PREPARE) {
            if (tmd_get_object(object_index)->primitive_count < KF_MAP_CELL_PREPARED_LIMIT) {
    KfTmdPreparedAsset prepared_asset;

                func_8002ff5c(game_graphics_runtime.tmd_state.current_asset,
                              object_index, &prepared_asset);
                func_8002f808(object_index, 240, &prepared_asset);
                return;
            }
        }
        func_8002f808(object_index, 240, 0);
    } else {
        render_enqueue_map(object_index);
    }
}

enum {
    KF_MAP_GRID_WIDTH = 80,
    KF_MAP_GRID_SCAN_WIDTH = 24,
    KF_MAP_GRID_EMPTY_OBJECT = 240,
    KF_MAP_GRID_CELL_LENGTH = 2048,
    KF_MAP_GRID_CELL_MIDPOINT = 1024,
    KF_MAP_GRID_ELEVATION_LENGTH = 128
};

ADDRESS(0x80030de4, 0x178)
void func_80030de4(s32 x, s32 z, u8 flags)
{
    KfMapOccupancyCell *cell = &bss_801c7540.map_cells[z][x];
    s32 object_index = cell->layer[0].object_index;
    SVECTOR position;

    if ((flags & 1) && object_index < (s32)KF_MAP_GRID_EMPTY_OBJECT) {
        position.vx = x * KF_MAP_GRID_CELL_LENGTH -
                      (u16)game_graphics_runtime.render_state.view_position.vx +
                      KF_MAP_GRID_CELL_MIDPOINT;
        position.vy = -cell->layer[0].elevation * KF_MAP_GRID_ELEVATION_LENGTH -
                      (u16)game_graphics_runtime.render_state.view_position.vy;
        position.vz = z * KF_MAP_GRID_CELL_LENGTH -
                      (u16)game_graphics_runtime.render_state.view_position.vz +
                      KF_MAP_GRID_CELL_MIDPOINT;
        render_map_cell_object(&cell->layer[0], &position, flags);

        object_index = cell->layer[1].object_index;
        if ((flags & 2) && object_index < (s32)KF_MAP_GRID_EMPTY_OBJECT) {
            position.vy = -cell->layer[1].elevation * KF_MAP_GRID_ELEVATION_LENGTH -
                          (u16)game_graphics_runtime.render_state.view_position.vy;
            render_map_cell_object(&cell->layer[1], &position, flags);
        }
    } else {
        object_index = cell->layer[1].object_index;
        if ((flags & 2) && object_index < (s32)KF_MAP_GRID_EMPTY_OBJECT) {
            position.vx = x * KF_MAP_GRID_CELL_LENGTH -
                          (u16)game_graphics_runtime.render_state.view_position.vx +
                          KF_MAP_GRID_CELL_MIDPOINT;
            position.vy = -cell->layer[1].elevation * KF_MAP_GRID_ELEVATION_LENGTH -
                          (u16)game_graphics_runtime.render_state.view_position.vy;
            position.vz = z * KF_MAP_GRID_CELL_LENGTH -
                          (u16)game_graphics_runtime.render_state.view_position.vz +
                          KF_MAP_GRID_CELL_MIDPOINT;
            render_map_cell_object(&cell->layer[1], &position, flags);
        }
    }
}

ADDRESS(0x80030f5c, 0xc8)
void func_80030f5c(void)
{
    s32 row;
    s32 remaining_rows;
    u8 *mask;
    s32 start_x;
    KfRenderGridState *render = &game_graphics_runtime.render_grid;

    tmd_select(0);
    mask = &render->map_cell_layer_masks[0][0];
    remaining_rows = KF_MAP_GRID_SCAN_WIDTH;
    start_x = render->map_scan_start_x;
    row = render->map_scan_start_z;
    while (remaining_rows != 0) {
        if ((u32)row < KF_MAP_GRID_WIDTH) {
            s32 x = start_x;
            s32 remaining_columns = KF_MAP_GRID_SCAN_WIDTH;
            do {
                s32 column = x & 0xff;
                x++;
                if ((u32)column < KF_MAP_GRID_WIDTH && *mask != 0) {
                    func_80030de4(column, row, *mask);
                }
                mask++;
                remaining_columns--;
            } while (remaining_columns != 0);
        } else {
            mask += KF_MAP_GRID_SCAN_WIDTH;
        }
        remaining_rows--;
        row++;
    }
}

enum {
    KF_RENDER_MODEL_END = 0xff,
    KF_RENDER_MODEL_ACTIVE = 1
};

extern void func_8002e4dc(s32 object_index, s32 depth_bias);

DATA(0x80066888, 0x21c)
KfRenderModelRow render_model_rows[KF_RENDER_MODEL_ROW_COUNT] = {
    {1, 0, 0x40, 0,  0, 0, { 85,  85, 85, 0}, {290, 32, 50, 0}, {0}, 0},
    {1, 0, 0x41, 0,  1, 0, {256, 256,256, 0}, { 28, 25, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0,  2, 0, {256, 256,256, 0}, { 28, 42, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0,  3, 0, {256, 256,256, 0}, { 52, 25, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0,  3, 0, {256, 256,256, 0}, { 64, 25, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0,  3, 0, {256, 256,256, 0}, { 76, 25, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0,  3, 0, {256, 256,256, 0}, { 52, 42, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0,  3, 0, {256, 256,256, 0}, { 64, 42, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0,  3, 0, {256, 256,256, 0}, { 76, 42, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0, 13, 0, { 64,   8,  2, 0}, { 16, 35, 24, 0}, {0}, 0},
    {1, 0, 0x41, 0, 14, 0, { 64,   8,  2, 0}, { 16, 52, 24, 0}, {0}, 0},
    {1, 0, 0x41, 0, 15, 0, {204,   8,  2, 0}, { 16, 35, 32, 0}, {0}, 0},
    {1, 0, 0x41, 0, 15, 0, {204,   8,  2, 0}, { 16, 52, 32, 0}, {0}, 0},
    {1, 0, 0x48, 0, 16, 0, {178, 200,  2, 0}, {  5, 12, 40, 0}, {0}, 0},
    {KF_RENDER_MODEL_END}
};

ADDRESS(0x80031024, 0x18c)
void func_80031024(void)
{
    KfRenderModelRow *entry;
    KfCollisionRow *lighting;
    KfTmdObject *object;
    MATRIX model;
    VECTOR scale;
    MATRIX light_matrix;

    entry = render_model_rows;
    if (entry->state == KF_RENDER_MODEL_END) {
        return;
    }
    do {
        if (entry->state == KF_RENDER_MODEL_ACTIVE) {
            lighting = &game_graphics_runtime.collision_rows[entry->lighting_index];
            model.t[0] = entry->translation.vx;
            model.t[1] = entry->translation.vy;
            model.t[2] = entry->translation.vz;
            RotMatrix(&entry->rotation, &model);
            fog_set_near(lighting->filter.angle);
            SetBackColor(lighting->filter.kinds.types[0],
                         lighting->filter.kinds.types[1],
                         lighting->filter.kinds.types[2]);
            SetColorMatrix((MATRIX *)&lighting->motion);
            MulMatrix0((MATRIX *)&lighting->rotations[0], &model, &light_matrix);
            SetLightMatrix(&light_matrix);
            copyVector(&scale, &entry->scale);
            ScaleMatrix(&model, &scale);
            SetRotMatrix(&model);
            SetTransMatrix(&model);
            asset_registry_select(entry->asset_id);
            object = tmd_get_object(0);
            if (func_80034070(&entry->animation_state, entry->asset_id,
                              entry->animation_clip, entry->animation_phase,
                              object->vertex_count)) {
                tmd_select_object_vertices(0);
            }
            tmd_transform_vertices(object->vertex_count);
            func_8002e4dc(0, 0);
        }
        entry++;
    } while (entry->state != KF_RENDER_MODEL_END);
}

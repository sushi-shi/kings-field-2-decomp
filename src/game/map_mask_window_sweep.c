#include <kf/lib/address.h>
#include <kf/game/collision_cache.h>
#include <kf/game/graphics.h>
#include <kf/game/render_mask.h>
#include <psyq/sdk.h>

ADDRESS(0x8002c670, 0x7bc)
void build_camera_map_cell_layer_masks(void)
{
    s16 shape[7];
    KfCollisionMaskPoint corners[4];
    KfMapMaskShapePair *pair;
    s16 *shape_cursor;
    s32 pitch_weight;
    s32 sine;
    s32 cosine;
    s32 center_x;
    s32 center_z;
    s32 x;
    s32 z;
    s32 shape_index;
    s32 index;
    u16 layer;
    u8 *first_lighting;
    u8 *mask;
    s32 lighting_offset;

    pitch_weight = 0x1000 - rcos(game_graphics_runtime.render_state.view_rotation.vx);
    pair = map_mask_pitch_shape_pairs;
    shape_cursor = shape;
    shape_index = 6;
    do {
        s16 near = pair->near;
        s16 far = pair->far;
        *shape_cursor = (((far - near) * pitch_weight) >> 12) + near;
        pair++;
        shape_cursor++;
    } while (--shape_index != -1);

    sine = rsin(game_graphics_runtime.render_state.view_rotation.vy);
    cosine = rcos(game_graphics_runtime.render_state.view_rotation.vy);
    clear_map_cell_layer_masks();

    render_mask_scan_state.map_x = game_graphics_runtime.render_state.view_cell_x;
    render_mask_scan_state.map_z = game_graphics_runtime.render_state.view_cell_z;
    x = (u8)(((sine * shape[0]) >> 20) + 12);
    game_graphics_runtime.render_state.cell_origin_x = x - render_mask_scan_state.map_x;
    game_graphics_runtime.render_grid.map_scan_start_x =
        -game_graphics_runtime.render_state.cell_origin_x;
    render_mask_scan_state.window_x = x;
    center_x = (s32)((u32)game_graphics_runtime.render_state.view_position.vx
                     << 1);
    z = (u8)((((-cosine) * shape[0]) >> 20) + 12);
    game_graphics_runtime.render_state.cell_origin_z = z - render_mask_scan_state.map_z;
    game_graphics_runtime.render_grid.map_scan_start_z =
        -game_graphics_runtime.render_state.cell_origin_z;
    render_mask_scan_state.window_z = z;
    mask = &game_graphics_runtime.render_grid.map_cell_layer_masks[z][x];
    render_mask_scan_state.mask_cursor = mask;

    center_z = (s32)((u32)game_graphics_runtime.render_state.view_position.vz
                     << 1);
    collision_sample_map_cell_layer(game_graphics_runtime.render_state.view_position.vx, game_graphics_runtime.render_state.view_position.vy,
                  game_graphics_runtime.render_state.view_position.vz);
    layer = KF_COLLISION_CACHE_LAYER;
    render_mask_scan_state.first_layer_byte_offset = layer;
    render_mask_scan_state.second_layer_byte_offset = 5 - layer;
    if (layer == 0) {
        render_mask_scan_state.first_layer_mask = 1;
        render_mask_scan_state.second_layer_mask = 2;
    } else {
        render_mask_scan_state.first_layer_mask = 2;
        render_mask_scan_state.second_layer_mask = 1;
    }
    corners[0].x = ((shape[1] * cosine - shape[3] * sine) >> 8) + center_x;
    corners[0].z = ((shape[1] * sine + shape[3] * cosine) >> 8) + center_z;
    corners[1].x = ((shape[2] * cosine - shape[3] * sine) >> 8) + center_x;
    corners[1].z = ((shape[2] * sine + shape[3] * cosine) >> 8) + center_z;
    corners[2].x = ((shape[4] * cosine - shape[6] * sine) >> 8) + center_x;
    corners[2].z = ((shape[4] * sine + shape[6] * cosine) >> 8) + center_z;
    corners[3].x = ((shape[5] * cosine - shape[6] * sine) >> 8) + center_x;
    corners[3].z = ((shape[5] * sine + shape[6] * cosine) >> 8) + center_z;

    rasterize_map_cell_layer_mask_line(&corners[0], &corners[1],
                  render_mask_scan_state.first_layer_mask | 0x20);
    rasterize_map_cell_layer_mask_line(&corners[1], &corners[3],
                  render_mask_scan_state.first_layer_mask | 0x20);
    rasterize_map_cell_layer_mask_line(&corners[3], &corners[2],
                  render_mask_scan_state.first_layer_mask | 0x20);
    rasterize_map_cell_layer_mask_line(&corners[2], &corners[0],
                  render_mask_scan_state.first_layer_mask | 0x20);
    fill_map_cell_layer_mask_interior(render_mask_scan_state.first_layer_mask | 0x20);

    lighting_offset = render_mask_scan_state.map_z * sizeof(bss_801c7540.map_cells[0]) +
        render_mask_scan_state.map_x * sizeof(bss_801c7540.map_cells[0][0]) +
        render_mask_scan_state.first_layer_byte_offset;
    /* The cache stores a byte offset (0 or 5) into the two-layer cell;
     * address it from the complete grid using the typed field offset. */
    first_lighting = (u8 *)&bss_801c7540.map_cells + lighting_offset +
        (u32)&((KfMapOccupancyCell *)0)->layer[0].lighting_index;
    if (*first_lighting & 0x80) {
        *render_mask_scan_state.mask_cursor = 3;
    } else {
        *render_mask_scan_state.mask_cursor = render_mask_scan_state.first_layer_mask;
    }

    for (index = 0; index < 14; index++) {
        render_mask_scan_state.map_z++;
        render_mask_scan_state.mask_cursor += 24;
        render_mask_scan_state.window_z++;
        update_current_map_cell_layer_mask(-24);
        render_mask_scan_state.map_x--;
        render_mask_scan_state.window_x--;
        render_mask_scan_state.mask_cursor--;
        sweep_map_cell_layer_mask_line(-24, -23, -1, 0, -1, index);
        update_current_map_cell_layer_mask(-23);

        render_mask_scan_state.map_z--;
        render_mask_scan_state.mask_cursor -= 24;
        render_mask_scan_state.window_z--;
        sweep_map_cell_layer_mask_line(1, -23, 0, -1, -24, index);
        update_current_map_cell_layer_mask(1);
        render_mask_scan_state.map_z--;
        render_mask_scan_state.mask_cursor -= 24;
        render_mask_scan_state.window_z--;
        sweep_map_cell_layer_mask_line(1, 25, 0, -1, -24, index);
        update_current_map_cell_layer_mask(25);

        render_mask_scan_state.map_x++;
        render_mask_scan_state.window_x++;
        render_mask_scan_state.mask_cursor++;
        sweep_map_cell_layer_mask_line(24, 25, 1, 0, 1, index);
        update_current_map_cell_layer_mask(24);
        render_mask_scan_state.map_x++;
        render_mask_scan_state.window_x++;
        render_mask_scan_state.mask_cursor++;
        sweep_map_cell_layer_mask_line(24, 23, 1, 0, 1, index);
        update_current_map_cell_layer_mask(23);

        render_mask_scan_state.map_z++;
        render_mask_scan_state.mask_cursor += 24;
        render_mask_scan_state.window_z++;
        sweep_map_cell_layer_mask_line(-1, 23, 0, 1, 24, index);
        update_current_map_cell_layer_mask(-1);
        render_mask_scan_state.map_z++;
        render_mask_scan_state.mask_cursor += 24;
        render_mask_scan_state.window_z++;
        sweep_map_cell_layer_mask_line(-1, -25, 0, 1, 24, index);
        update_current_map_cell_layer_mask(-25);

        render_mask_scan_state.map_x--;
        render_mask_scan_state.window_x--;
        render_mask_scan_state.mask_cursor--;
        sweep_map_cell_layer_mask_line(-24, -25, -1, 0, -1, index);
    }

    mask[-25] |= 0x80;
    mask[-23] |= 0x80;
    mask[-24] |= 0xc0;
    mask[0] |= 0xc0;
    mask[-1] |= 0xc0;
    mask[23] |= 0x80;
    mask[1] |= 0xc0;
    mask[25] |= 0x80;
    mask[24] |= 0xc0;
}

DATA(0x80067874, 0x1c)
KfMapMaskShapePair map_mask_pitch_shape_pairs[7] = {
    {0x0500, 0x0000},
    {(s16)0xf720, (s16)0xfb20},
    {0x0920, 0x0520},
    {0x0a80, 0x0480},
    {(s16)0xff20, (s16)0xfb20},
    {0x0120, 0x0520},
    {(s16)0xff80, (s16)0xfb80},
};

DATA(0x801b5a70, 0x20)
KfRenderMaskScanState render_mask_scan_state;

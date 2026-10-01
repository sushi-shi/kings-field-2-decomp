#include <kf/lib/address.h>
#include <kf/game/collision_cache.h>
#include <kf/game/graphics.h>
#include <kf/game/render_mask.h>
#include <psyq/sdk.h>

enum { KF_MAP_MASK_LIGHTING_OFFSET = 4 };
typedef char kf_map_mask_lighting_offset[
    (u32)&((KfMapOccupancyCell *)0)->layer[0].lighting_index ==
    KF_MAP_MASK_LIGHTING_OFFSET ? 1 : -1];

typedef struct KfMapMaskShapePair {
    s16 near;
    s16 far;
} KfMapMaskShapePair;

extern KfMapMaskShapePair DAT_80067874[7];

extern void func_8002bfac(void);
extern void func_8002bfd4(const KfCollisionMaskPoint *start,
                          const KfCollisionMaskPoint *end, u8 value);
extern void func_8002c1d4(u8 value);
extern void func_8002c290(s32 cursor_offset);
extern void func_8002c424(s32 first_offset, s32 second_offset, s32 map_step,
                          s8 window_step, s32 mask_stride, s32 count);

ADDRESS(0x8002c670, 0x7bc)
void func_8002c670(void)
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
    KfMapOccupancyCell *center_cell;

    pitch_weight = 0x1000 - rcos(game_graphics_runtime.render_state.view_rotation.vx);
    pair = DAT_80067874;
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
    func_8002bfac();

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
    func_8002a988(game_graphics_runtime.render_state.view_position.vx, game_graphics_runtime.render_state.view_position.vy,
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

    func_8002bfd4(&corners[0], &corners[1],
                  render_mask_scan_state.first_layer_mask | 0x20);
    func_8002bfd4(&corners[1], &corners[3],
                  render_mask_scan_state.first_layer_mask | 0x20);
    func_8002bfd4(&corners[3], &corners[2],
                  render_mask_scan_state.first_layer_mask | 0x20);
    func_8002bfd4(&corners[2], &corners[0],
                  render_mask_scan_state.first_layer_mask | 0x20);
    func_8002c1d4(render_mask_scan_state.first_layer_mask | 0x20);

    center_cell = &bss_801c7540.map_cells[render_mask_scan_state.map_z]
        [render_mask_scan_state.map_x];
    /* The cached offset selects either layer's lighting byte (0 or 5). */
    first_lighting = (u8 *)center_cell + KF_MAP_MASK_LIGHTING_OFFSET +
        render_mask_scan_state.first_layer_byte_offset;
    if (*first_lighting & 0x80) {
        *render_mask_scan_state.mask_cursor = 3;
    } else {
        *render_mask_scan_state.mask_cursor = render_mask_scan_state.first_layer_mask;
    }

    for (index = 0; index < 14; index++) {
        render_mask_scan_state.map_z++;
        render_mask_scan_state.mask_cursor += 24;
        render_mask_scan_state.window_z++;
        func_8002c290(-24);
        render_mask_scan_state.map_x--;
        render_mask_scan_state.window_x--;
        render_mask_scan_state.mask_cursor--;
        func_8002c424(-24, -23, -1, 0, -1, index);
        func_8002c290(-23);

        render_mask_scan_state.map_z--;
        render_mask_scan_state.mask_cursor -= 24;
        render_mask_scan_state.window_z--;
        func_8002c424(1, -23, 0, -1, -24, index);
        func_8002c290(1);
        render_mask_scan_state.map_z--;
        render_mask_scan_state.mask_cursor -= 24;
        render_mask_scan_state.window_z--;
        func_8002c424(1, 25, 0, -1, -24, index);
        func_8002c290(25);

        render_mask_scan_state.map_x++;
        render_mask_scan_state.window_x++;
        render_mask_scan_state.mask_cursor++;
        func_8002c424(24, 25, 1, 0, 1, index);
        func_8002c290(24);
        render_mask_scan_state.map_x++;
        render_mask_scan_state.window_x++;
        render_mask_scan_state.mask_cursor++;
        func_8002c424(24, 23, 1, 0, 1, index);
        func_8002c290(23);

        render_mask_scan_state.map_z++;
        render_mask_scan_state.mask_cursor += 24;
        render_mask_scan_state.window_z++;
        func_8002c424(-1, 23, 0, 1, 24, index);
        func_8002c290(-1);
        render_mask_scan_state.map_z++;
        render_mask_scan_state.mask_cursor += 24;
        render_mask_scan_state.window_z++;
        func_8002c424(-1, -25, 0, 1, 24, index);
        func_8002c290(-25);

        render_mask_scan_state.map_x--;
        render_mask_scan_state.window_x--;
        render_mask_scan_state.mask_cursor--;
        func_8002c424(-24, -25, -1, 0, -1, index);
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
KfMapMaskShapePair DAT_80067874[7] = {
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

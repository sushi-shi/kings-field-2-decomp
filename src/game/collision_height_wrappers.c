#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/collision_cache.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/render_mask.h>

#define COLLISION_CACHE_CELL KF_COLLISION_CACHE_CELL
#define COLLISION_CACHE_SHAPE KF_COLLISION_CACHE_SHAPE
#define COLLISION_CACHE_LAYER KF_COLLISION_CACHE_LAYER
#define COLLISION_CACHE_HEIGHT KF_COLLISION_CACHE_HEIGHT
#define COLLISION_CACHE_RESULT KF_COLLISION_CACHE_RESULT
#define COLLISION_CACHE_FLAGS KF_COLLISION_CACHE_FLAGS
#define COLLISION_CACHE_ACTOR_INDEX KF_COLLISION_CACHE_ACTOR_INDEX
#define COLLISION_CACHE_OBJECT_INDEX KF_COLLISION_CACHE_OBJECT_INDEX
#define COLLISION_CACHE_POSITION KF_COLLISION_CACHE_POSITION
#define COLLISION_CACHE_RADIUS KF_COLLISION_CACHE_RADIUS
#define COLLISION_CACHE_INTERACTION_HEIGHT KF_COLLISION_CACHE_INTERACTION_HEIGHT

ADDRESS(0x8002b604, 0x78)
s32 func_8002b604(s32 x, s32 y, s32 z, s32 radius, s32 height)
{
    func_8002a988(x, y - 1280, z);
    func_8002aaa4(x, y, z, radius, height);
    return COLLISION_CACHE_RESULT;
}

ADDRESS(0x8002b67c, 0xc0)
s32 func_8002b67c(u8 kind, s32 x, s32 z, s32 radius, s32 height)
{
    KfMapOccupancyCell *cell = &bss_801c7540.map_cells[z >> 11][x >> 11];
    s32 elevation;

    if (kind == 2) {
        COLLISION_CACHE_LAYER = 5;
        elevation = -(s32)cell->layer[1].elevation;
    } else {
        COLLISION_CACHE_LAYER = 0;
        elevation = -(s32)cell->layer[0].elevation;
    }
    COLLISION_CACHE_HEIGHT = elevation * 128;
    COLLISION_CACHE_CELL = cell;
    func_8002aaa4(x, COLLISION_CACHE_HEIGHT, z, radius, height);
    return COLLISION_CACHE_RESULT;
}

ADDRESS(0x8002b73c, 0xbc)
void func_8002b73c(s32 x, s32 z, s32 radius, s32 amount)
{
    s32 expanded = radius + 2048;
    s32 first_x = (x - expanded) >> 11;
    s32 width = ((x + expanded) >> 11) - first_x;
    s32 first_z = (z - expanded) >> 11;
    s32 height = ((z + expanded) >> 11) - first_z;
    KfMapOccupancyCell *row = &bss_801c7540.map_cells[first_z][first_x];
    u32 value = (u32)amount << 2;

    do {
        KfMapOccupancyCell *current_row = row;
        row += 80;
        if ((u32)first_z < 80) {
            KfMapOccupancyCell *cell = current_row;
            s32 col = first_x;
            s32 remaining = width;
            do {
                if ((u32)col < 80) {
                    cell->layer[0].quarter_turns =
                        value + cell->layer[0].quarter_turns;
                }
                col++;
                cell++;
                remaining--;
            } while (remaining != -1);
        }
        first_z++;
        height--;
    } while (height != -1);
}

ADDRESS(0x8002b7f8, 0x7c)
s32 func_8002b7f8(s32 x, s32 y, s32 z, s32 radius, s32 height)
{
    func_8002a988(x, y - (((u32)height << 4) >> 5), z);
    return func_8002aaa4(x, y, z, radius, height);
}

ADDRESS(0x8002b874, 0x160)
void func_8002b874(void)
{
    u16 interaction_height;

    if (COLLISION_CACHE_FLAGS & 0x80) {
        COLLISION_CACHE_POSITION = player_state.camera_position;
        COLLISION_CACHE_RADIUS = 800;
        interaction_height = 1700;
    } else if (COLLISION_CACHE_ACTOR_INDEX != -1) {
        KfActor *actor = &actor_state.actors[COLLISION_CACHE_ACTOR_INDEX];
        COLLISION_CACHE_POSITION = actor->position;
        COLLISION_CACHE_RADIUS = actor->unknown_1c;
        interaction_height = actor->unknown_1e;
    } else {
        KfMapObject *object;
        KfMapObjectTemplate *object_template;

        if (COLLISION_CACHE_OBJECT_INDEX == -1) {
            return;
        }
        object = &map_object_state.objects[COLLISION_CACHE_OBJECT_INDEX];
        object_template = &map_object_state.templates[object->object_id];
        COLLISION_CACHE_POSITION = object->position;
        COLLISION_CACHE_RADIUS = object_template->collision_radius;
        interaction_height = object_template->interaction_height;
    }
    COLLISION_CACHE_INTERACTION_HEIGHT = interaction_height;
}

ADDRESS(0x8002b9d4, 0x244)
s32 collision_query_world(s32 x, s32 y, s32 z, s32 radius, s32 height, s32 mode)
{
    s32 result = 0;

    if (mode & 1) {
        result = func_8002b7f8(x, y, z, radius, height);
        if ((mode & 2) && (COLLISION_CACHE_SHAPE[4] & 0x40)) {
            COLLISION_CACHE_RESULT = -100000;
            result |= 1;
        }
    } else {
        COLLISION_CACHE_CELL = &bss_801c7540.map_cells[z >> 11][x >> 11];
    }

    height &= 0x0fffffff;
    if (COLLISION_CACHE_CELL->layer[0].quarter_turns & 0xfc) {
        if (mode & 0x10) {
            COLLISION_CACHE_ACTOR_INDEX = func_8003a9f4(x, y, z, radius, height);
            if (COLLISION_CACHE_ACTOR_INDEX != -1) {
                result |= 0x10;
            }
        } else {
            if (mode & 0x40) {
                COLLISION_CACHE_ACTOR_INDEX = func_8003ab5c(x, y, z, radius, height);
                if (COLLISION_CACHE_ACTOR_INDEX != -1) {
                    result |= 0x10;
                }
            }
            COLLISION_CACHE_ACTOR_INDEX = -1;
        }

        if (mode & 0x20) {
            COLLISION_CACHE_OBJECT_INDEX = func_80036078(x, y, z, radius, height);
            if (COLLISION_CACHE_OBJECT_INDEX != -1) {
                result |= 0x20;
            }
        } else {
            COLLISION_CACHE_OBJECT_INDEX = -1;
        }

        if ((mode & 0x80) &&
            player_distance_to_point_with_margin(x, y, z, radius, height) != -1) {
            result |= 0x80;
        }
    } else {
        COLLISION_CACHE_ACTOR_INDEX = -1;
        COLLISION_CACHE_OBJECT_INDEX = -1;
    }

    COLLISION_CACHE_FLAGS = result;
    return result;
}

/* Startup may replace this load-image table from the archive. */
DATA(0x80066ab4, 0xdc0)
KfCollisionDefaultRow collision_default_rows[KF_COLLISION_ROW_COUNT] = {
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {{{{0, 0, -4096}, {0, 0, -3800}, {0, 0, -3800}}, 0}, {{900, 900, 900, 900, 900, 900, 900, 900, 900}}, {{{60, 60, 60}, 0}, 32000}},
    {{{{0, 0, -4096}, {0, 0, -3800}, {0, 0, -3800}}, 0}, {{500, 500, 500, 500, 500, 500, 500, 500, 500}}, {{{60, 60, 60}, 0}, 32000}},
    {{{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0}, {{0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{{60, 60, 60}, 0}, 0}},
    {{{{0, 0, -4096}, {0, 0, -3800}, {0, 0, -3800}}, 0}, {{900, 900, 900, 900, 900, 900, 900, 900, 900}}, {{{90, 90, 90}, 0}, 32000}},
    {{{{3800, -2800, 1024}, {-3000, -3600, -3400}, {-1300, 2700, 1024}}, 0}, {{1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}}, {{{90, 90, 90}, 0}, 32000}},
    {{{{0, 3800, 0}, {-500, 3600, 0}, {0, 3700, 500}}, 0}, {{1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200}}, {{{10, 10, 10}, 0}, 32000}},
    {{{{0, 0, -4096}, {0, 0, -3800}, {0, 0, -3800}}, 0}, {{800, 800, 800, 600, 600, 600, 500, 500, 500}}, {{{30, 25, 20}, 0}, 64268}},
    {{{{3000, 3700, -1500}, {-2000, 3700, -3000}, {-1300, -1000, 1024}}, 0}, {{4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095}}, {{{100, 100, 100}, 0}, 65535}},
    {{{{0, 0, -4096}, {0, 0, -3800}, {0, 0, -3800}}, 0}, {{3095, 3095, 3095, 3095, 3095, 3095, 3095, 3095, 3095}}, {{{60, 60, 60}, 0}, 32000}},
    {{{{3800, -2800, 1024}, {-3000, -3600, -3400}, {-1300, 2700, 1024}}, 0}, {{3072, 3072, 3072, 3072, 3072, 3072, 3072, 3072, 3072}}, {{{100, 100, 100}, 0}, 32000}},
    {{{{0, 3800, 0}, {0, -3600, 0}, {0, 3700, 0}}, 0}, {{3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000}}, {{{0, 0, 0}, 0}, 32000}},
    {0},
    {0},
    {0},
    {0},
    {0},
};

ADDRESS(0x8002bc18, 0x124)
void func_8002bc18(void)
{
    KfCollisionDefaultRow *defaults = collision_default_rows;
    KfCollisionRow *row = game_graphics_runtime.collision_rows;
    s32 index = KF_COLLISION_ROW_COUNT - 1;

    do {
        row->motion = defaults->motion;
        row->rotations[0] = defaults->rotation;
        row->filter.kinds = defaults->filter.kinds;
        row->filter.angle = defaults->filter.angle;
        defaults++;
        row++;
        index--;
    } while (index != -1);

    game_graphics_runtime.collision_rotation_dirty = 0;
    game_graphics_runtime.unknown_14cc5 = 0;
    game_graphics_runtime.unknown_14cca = 0;
    game_graphics_runtime.unknown_14cc8 = 0;
    game_graphics_runtime.unknown_14cc6 = 0;
}

ADDRESS(0x8002bd3c, 0x80)
void func_8002bd3c(void)
{
    KfCollisionRow *row;
    s32 index;

    if (game_graphics_runtime.collision_rotation_dirty) {
        row = game_graphics_runtime.collision_rows;
        index = KF_COLLISION_ROW_COUNT - 1;
        do {
            matrix_rotate_quarter_turns((MATRIX *)&row->rotations[0],
                                        (MATRIX *)&row->rotations[1], 1);
            matrix_rotate_quarter_turns((MATRIX *)&row->rotations[0],
                                        (MATRIX *)&row->rotations[2], 2);
            matrix_rotate_quarter_turns((MATRIX *)&row->rotations[0],
                                        (MATRIX *)&row->rotations[3], 3);
            index--;
            row++;
        } while (index != -1);
    }
}

ADDRESS(0x8002bdbc, 0xe0)
void func_8002bdbc(s32 flags, const KfCollisionFilterPayload *payload,
                   KfCollisionRow *row, s32 amount)
{
    if (flags & 2) {
        func_800158b4((const u16 *)row->motion.values,
                      (const u16 *)&payload->unknown_00[20],
                      (u16 *)row->motion.values, amount);
    }
    if (flags & 1) {
        func_800158b4((const u16 *)&row->rotations[0],
                      (const u16 *)payload->unknown_00,
                      (u16 *)&row->rotations[0], amount);
    }
    if (flags & 4) {
        row->filter.kinds.types[0] = func_8001584c(
            row->filter.kinds.types[0], payload->filter.kinds.types[0], amount);
        row->filter.kinds.types[1] = func_8001584c(
            row->filter.kinds.types[1], payload->filter.kinds.types[1], amount);
        row->filter.kinds.types[2] = func_8001584c(
            row->filter.kinds.types[2], payload->filter.kinds.types[2], amount);
    }
    if (flags & 8) {
        row->filter.angle = func_8001584c(row->filter.angle, payload->filter.angle, amount);
    }
}

ADDRESS(0x8002be9c, 0x9c)
void func_8002be9c(s32 flags, const KfCollisionFilterPayload *payload, s32 amount)
{
    KfCollisionRow *row = game_graphics_runtime.collision_rows;
    s32 index;

    for (index = 0; index < 62; index++, row++) {
        if (index != 38) {
            func_8002bdbc(flags, payload, row, amount);
        }
    }
    if (flags & 1) {
        game_graphics_runtime.collision_rotation_dirty = 1;
    }
}

ADDRESS(0x8002bf38, 0x74)
void func_8002bf38(u8 arg0, u8 arg1, u8 arg2, s32 angle, u16 value)
{
    KfCollisionFilterPayload payload;
    s32 flags = 0;

    if (arg0 != 0xff || arg1 != arg0 || arg2 != arg1) {
        payload.filter.kinds.types[0] = arg0;
        payload.filter.kinds.types[1] = arg1;
        payload.filter.kinds.types[2] = arg2;
        flags |= 4;
    }
    if (angle != -1) {
        payload.filter.angle = angle;
        flags |= 8;
    }
    func_8002be9c(flags, &payload, (s16)value);
}

ADDRESS(0x8002bfac, 0x28)
void func_8002bfac(void)
{
    u32 *mask = (u32 *)game_graphics_runtime.render_grid.map_cell_layer_masks;
    s32 index = 143;

    do {
        *mask = 0;
        index--;
        mask++;
    } while (index != -1);
}

ADDRESS(0x8002bfd4, 0x19c)
void func_8002bfd4(const KfCollisionMaskPoint *start,
                   const KfCollisionMaskPoint *end, u8 value)
{
    /* The rasterizer uses 16-bit origins and truncates grid coordinates. */
    u16 origin_x = (u16)game_graphics_runtime.render_state.cell_origin_x;
    u16 origin_z = (u16)game_graphics_runtime.render_state.cell_origin_z;
    s32 x = ((u32)start->x >> 12) + origin_x;
    s32 z = ((u32)start->z >> 12) + origin_z;
    s32 dx = (((u32)end->x >> 12) + origin_x) - x;
    s32 dz = (((u32)end->z >> 12) + origin_z) - z;
    s32 step_x;
    s32 step_z;
    s32 count;
    s32 error;

    if ((s16)dx < 0) {
        dx = -dx;
        step_x = -1;
    } else {
        step_x = 1;
    }
    step_z = 1;
    if ((s16)dz < 0) {
        dz = -dz;
        step_z = -1;
    }

    if ((s16)dx >= (s16)dz) {
        error = (s16)dx >> 1;
        count = dx;
        do {
            if ((u16)x < KF_MAP_CELL_GRID_SIDE &&
                (u16)z < KF_MAP_CELL_GRID_SIDE) {
                game_graphics_runtime.render_grid
                    .map_cell_layer_masks[(u16)z][(u16)x] = value;
            }
            error -= dz;
            if ((s16)error <= 0) {
                z += step_z;
                error += dx;
            }
            x += step_x;
            count--;
        } while ((s16)count >= 0);
    } else {
        error = (s16)dz >> 1;
        count = dz;
        do {
            if ((u16)x < KF_MAP_CELL_GRID_SIDE &&
                (u16)z < KF_MAP_CELL_GRID_SIDE) {
                game_graphics_runtime.render_grid
                    .map_cell_layer_masks[(u16)z][(u16)x] = value;
            }
            error -= dx;
            if ((s16)error <= 0) {
                x += step_x;
                error += dz;
            }
            z += step_z;
            count--;
        } while ((s16)count >= 0);
    }
}

ADDRESS(0x8002c170, 0x64)
s32 func_8002c170(const u8 *row, s32 index, s32 step, u8 value)
{
    s32 state = 0;

    for (;;) {
        u8 current;
        if ((u32)index >= KF_MAP_CELL_GRID_SIDE) {
            return index;
        }
        current = row[index];
        switch (state) {
        case 0:
            if (current == value) {
                state = 1;
            }
            break;
        case 1:
            if (current != value) {
                return index;
            }
            break;
        }
        index += step;
    }
}

ADDRESS(0x8002c1d4, 0xbc)
void func_8002c1d4(u8 value)
{
    u8 *row = &game_graphics_runtime.render_grid.map_cell_layer_masks[0][0];
    s32 row_index = KF_MAP_CELL_GRID_SIDE - 1;

    do {
        s32 first = func_8002c170(row, 0, 1, value);
        s32 last = func_8002c170(row, KF_MAP_CELL_GRID_SIDE - 1, -1, value);

        if (last >= first) {
            u8 *cell = row + first;
            s32 count = last - first;
            do {
                *cell = value;
                count--;
                cell++;
            } while (count != -1);
        }
        row_index--;
        row += KF_MAP_CELL_GRID_SIDE;
    } while (row_index != -1);
}

ADDRESS(0x8002c290, 0x194)
void func_8002c290(s32 cursor_offset)
{
    KfMapOccupancyCell *cell;
    KfMapOccupancyLayer *first_layer;
    KfMapOccupancyLayer *second_layer;
    u8 *mask;
    u8 value;

    if ((u32)render_mask_scan_state.window_x >= KF_MAP_CELL_GRID_SIDE ||
        (u32)render_mask_scan_state.window_z >= KF_MAP_CELL_GRID_SIDE) {
        return;
    }
    mask = render_mask_scan_state.mask_cursor;
    if (*mask == 0) {
        return;
    }
    if ((u32)render_mask_scan_state.map_x < 80 &&
        (u32)render_mask_scan_state.map_z < 80) {
        value = mask[cursor_offset];
        cell = &bss_801c7540.map_cells[render_mask_scan_state.map_z]
                                            [render_mask_scan_state.map_x];
        first_layer = (KfMapOccupancyLayer *)((u8 *)cell +
                          render_mask_scan_state.first_layer_byte_offset);
        if (value & render_mask_scan_state.first_layer_mask) {
            goto check_first_layer;
        }
clear_first_layer:
        *render_mask_scan_state.mask_cursor &=
            ~render_mask_scan_state.first_layer_mask;
        goto check_second_layer;
check_first_layer:
        if (first_layer->object_index == 0xff) {
            goto clear_first_layer;
        }
        if (!(first_layer->lighting_index & 0x80)) {
            goto check_second_layer;
        }
set_second_layer:
        *render_mask_scan_state.mask_cursor |=
            render_mask_scan_state.second_layer_mask;
        return;
check_second_layer:
        second_layer = (KfMapOccupancyLayer *)((u8 *)cell +
                           render_mask_scan_state.second_layer_byte_offset);
        if (second_layer->object_index != 0xff &&
            (value & render_mask_scan_state.second_layer_mask)) {
            goto set_second_layer;
        }
        return;
    }
    *render_mask_scan_state.mask_cursor = 0;
}

ADDRESS(0x8002c424, 0x24c)
void func_8002c424(s32 first_offset, s32 second_offset, s32 map_step,
                   s8 window_step, s32 mask_stride, s32 count)
{
    s32 window_x = render_mask_scan_state.window_x;
    s32 window_z = render_mask_scan_state.window_z;
    s32 map_x = render_mask_scan_state.map_x;
    s32 map_z = render_mask_scan_state.map_z;
    u8 *cursor = render_mask_scan_state.mask_cursor;

    count--;
    if (count != -1) {
        const u8 *first_layer_mask = &render_mask_scan_state.first_layer_mask;
        const u8 *second_layer_mask = &render_mask_scan_state.second_layer_mask;

        do {
            if ((u32)window_x < 24 && (u32)window_z < 24 && *cursor != 0) {
                if ((u32)map_x < 80 && (u32)map_z < 80) {
                    u8 first = cursor[first_offset];
                    u8 second = cursor[second_offset];
                    KfMapOccupancyCell *cell;
                    KfMapOccupancyLayer *first_layer;
                    KfMapOccupancyLayer *second_layer;

                    if (first & *first_layer_mask) {
                        goto check_first_layer;
                    }
                    if (second & *first_layer_mask) {
                        goto check_first_layer;
                    }
                    cell = &bss_801c7540.map_cells[map_z][map_x];
    clear_first_layer:
                    *cursor &= ~*first_layer_mask;
                    goto check_second_layer;
    check_first_layer:
                    cell = &bss_801c7540.map_cells[map_z][map_x];
                    first_layer = (KfMapOccupancyLayer *)((u8 *)cell +
                                  render_mask_scan_state.first_layer_byte_offset);
                    if (first_layer->object_index == 0xff) {
                        goto clear_first_layer;
                    }
                    if (!(first_layer->lighting_index & 0x80)) {
                        goto check_second_layer;
                    }
    set_second_layer:
                    *cursor |= *second_layer_mask;
                    goto advance_iteration;
    check_second_layer:
                    second_layer = (KfMapOccupancyLayer *)((u8 *)cell +
                                   render_mask_scan_state.second_layer_byte_offset);
                    if (second_layer->object_index != 0xff) {
                        if ((first & *second_layer_mask) ||
                            (second & *second_layer_mask)) {
                            goto set_second_layer;
                        }
                    }
                } else {
                    *cursor = 0;
                }
            }
    advance_iteration:
            cursor += mask_stride;
            window_x += map_step;
            window_z += window_step;
            map_x += map_step;
            map_z += window_step;
            count--;
        } while (count != -1);
    }

    render_mask_scan_state.window_x = window_x;
    render_mask_scan_state.window_z = window_z;
    render_mask_scan_state.map_x = map_x;
    render_mask_scan_state.map_z = map_z;
    render_mask_scan_state.mask_cursor = cursor;
}

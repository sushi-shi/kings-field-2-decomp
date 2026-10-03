#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/callback.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/memory.h>
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

typedef struct KfTmdUvBytes {
    u8 u, v;
} KfTmdUvBytes;
typedef char kf_tmd_uv_bytes_size[sizeof(KfTmdUvBytes) == 2 ? 1 : -1];

typedef struct KfTmdFt4TextureWords {
    KfTmdUvBytes uv0; u16 clut; KfTmdUvBytes uv1; u16 tpage;
    KfTmdUvBytes uv2; u16 pad0; KfTmdUvBytes uv3; u16 pad1;
} KfTmdFt4TextureWords;
typedef char kf_tmd_ft4_texture_words_size[
    sizeof(KfTmdFt4TextureWords) == 16 ? 1 : -1];
typedef char kf_tmd_ft4_texture_uv1_offset[
    (u32)&((KfTmdFt4TextureWords *)0)->uv1 == 4 ? 1 : -1];
typedef char kf_tmd_ft4_texture_uv2_offset[
    (u32)&((KfTmdFt4TextureWords *)0)->uv2 == 8 ? 1 : -1];
typedef char kf_tmd_ft4_texture_uv3_offset[
    (u32)&((KfTmdFt4TextureWords *)0)->uv3 == 12 ? 1 : -1];
typedef KfTmdUvBytes KfUvScratch;
typedef char kf_tmd_uv_scratch_size[sizeof(KfUvScratch) == 2 ? 1 : -1];
#define WRITE_UV_CACHED(field, value) do { \
    ((u8 *)&(field))[0] = (value).u; \
    ((u8 *)&(field))[1] = (value).v; \
} while (0)
#define WRITE_INDEX(field, value) do { \
    ((u8 *)&(field))[0] = (u8)(value); \
    ((u8 *)&(field))[1] = (u8)((value) >> 8); \
} while (0)
#define MID_INDEX(src, n) ((u16)(((src)->vertex_count + (n)) << 3))
#define MID_VECTOR(dst, lhs, rhs) do { \
    (dst)->vx = ((s32)(lhs)->vx + (s32)(rhs)->vx) >> 1; \
    (dst)->vy = ((s32)(lhs)->vy + (s32)(rhs)->vy) >> 1; \
    (dst)->vz = ((s32)(lhs)->vz + (s32)(rhs)->vz) >> 1; \
} while (0)
#define MID_UV_INTO(dst, lhs, rhs) do { \
    (dst).v = ((u32)(lhs).v + (u32)(rhs).v) >> 1; \
    (dst).u = ((u32)(lhs).u + (u32)(rhs).u) >> 1; \
} while (0)

ADDRESS(0x8002ff5c, 0xcbc)
void tmd_prepare_subdivided_object(KfTmdHeader *asset, s32 object_index,
                   KfTmdPreparedAsset *prepared_asset)
{
    u8 *base;
    KfTmdObject *source;
    KfTmdObject *target;
    u8 *source_packet;
    u8 *output_packet;
    union { SVECTOR vector; u32 words[2]; } corners[4];
    KfTmdFt4TextureWords tex;
    SVECTOR midpoints[128];
    SVECTOR *midpoint_end;
    u32 midpoint_count;
    u32 source_vertex_count;
    u32 output_packet_bytes;
    u32 remaining;

    midpoint_count = 0;
    output_packet_bytes = 0;
    midpoint_end = midpoints;
    target = &prepared_asset->object;
    output_packet = (u8 *)target;
    target->primitive_offset = sizeof(KfTmdObject);
    output_packet += target->primitive_offset;
    base = (u8 *)asset + KF_TMD_HEADER_BYTES;
    source = &TMD_OBJECTS(asset)[object_index];
    target->primitive_count = source->primitive_count;
    remaining = source->primitive_count;
    source_packet = base + source->primitive_offset;
    while (--remaining != (u32)-1) {
        KfTmdPacketHeader header;
        KfUvScratch uv_ab, uv_ac, uv_ad, uv_cd, uv_bd;
        header.word = *(u32 *)source_packet;
        if (header.bytes.mode == 0x2c || header.bytes.mode == 0x2e) {
            KfTmdFt4 *face = (KfTmdFt4 *)(source_packet + 4);
            KfTmdFt4PackedIndices *first =
                (KfTmdFt4PackedIndices *)(output_packet + 4);
            KfTmdFt4 *second;
            KfTmdFt4 *third;
            KfTmdFt4 *fourth;
            u16 ab, ac, ad, cd, bd;

            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex0);
                corners[0].words[0] = vertex[0];
                corners[0].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex1);
                corners[1].words[0] = vertex[0];
                corners[1].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex2);
                corners[2].words[0] = vertex[0];
                corners[2].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex3);
                corners[3].words[0] = vertex[0];
                corners[3].words[1] = vertex[1];
            }
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[1].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[2].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[3].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[2].vector, &corners[3].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[1].vector, &corners[3].vector);
            midpoint_end++;
            resource_copy_words((u32 *)&tex, (u32 *)(source_packet + 4), 4);
            MID_UV_INTO(uv_ab, tex.uv0, tex.uv1);
            MID_UV_INTO(uv_ac, tex.uv0, tex.uv2);
            MID_UV_INTO(uv_ad, tex.uv0, tex.uv3);
            MID_UV_INTO(uv_cd, tex.uv2, tex.uv3);
            MID_UV_INTO(uv_bd, tex.uv1, tex.uv3);
            ab = MID_INDEX(source, midpoint_count);
            ac = MID_INDEX(source, midpoint_count + 1);
            ad = MID_INDEX(source, midpoint_count + 2);
            cd = MID_INDEX(source, midpoint_count + 3);
            bd = MID_INDEX(source, midpoint_count + 4);
            WRITE_UV_CACHED(first->uv1, uv_ab);
            WRITE_UV_CACHED(first->uv2, uv_ac);
            WRITE_UV_CACHED(first->uv3, uv_ad);
            first->vertex1_vertex2 = (u32)ab | ((u32)ac << 16);
            first->vertex3_pad2 = (u32)ad | ((u32)ac << 16);
            output_packet += 32;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            second = (KfTmdFt4 *)(output_packet + 4);
            WRITE_UV_CACHED(second->uv0, uv_ab);
            WRITE_UV_CACHED(second->uv2, uv_ad);
            WRITE_UV_CACHED(second->uv3, uv_bd);
            WRITE_INDEX(second->vertex0, ab);
            WRITE_INDEX(second->vertex2, ad);
            WRITE_INDEX(second->vertex3, bd);
            output_packet += 32;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            third = (KfTmdFt4 *)(output_packet + 4);
            WRITE_UV_CACHED(third->uv0, uv_ac);
            WRITE_UV_CACHED(third->uv1, uv_ad);
            WRITE_UV_CACHED(third->uv3, uv_cd);
            WRITE_INDEX(third->vertex0, ac);
            WRITE_INDEX(third->vertex1, ad);
            WRITE_INDEX(third->vertex3, cd);
            output_packet += 32;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 8);
            fourth = (KfTmdFt4 *)(output_packet + 4);
            WRITE_UV_CACHED(fourth->uv0, uv_ad);
            WRITE_UV_CACHED(fourth->uv1, uv_bd);
            WRITE_UV_CACHED(fourth->uv2, uv_cd);
            WRITE_INDEX(fourth->vertex0, ad);
            WRITE_INDEX(fourth->vertex1, bd);
            WRITE_INDEX(fourth->vertex2, cd);
            output_packet += 32;
            output_packet_bytes += 128;
            midpoint_count += 5;
            target->primitive_count += 3;
        } else if (header.bytes.mode == 0x24 || header.bytes.mode == 0x26) {
            KfTmdFt3 *face = (KfTmdFt3 *)(source_packet + 4);
            KfTmdFt3 *first = (KfTmdFt3 *)(output_packet + 4);
            KfTmdFt3 *second;
            KfTmdFt3 *third;
            KfTmdFt3 *fourth;
            u16 ab, ac, bc;

            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex0);
                corners[0].words[0] = vertex[0];
                corners[0].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex1);
                corners[1].words[0] = vertex[0];
                corners[1].words[1] = vertex[1];
            }
            {
                const u32 *vertex = (const u32 *)(base + source->vertex_offset + (s16)face->vertex2);
                corners[2].words[0] = vertex[0];
                corners[2].words[1] = vertex[1];
            }
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[1].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[0].vector, &corners[2].vector);
            midpoint_end++;
            MID_VECTOR(midpoint_end, &corners[1].vector, &corners[2].vector);
            midpoint_end++;
            resource_copy_words((u32 *)&tex, (u32 *)(source_packet + 4), 3);
            MID_UV_INTO(uv_ab, tex.uv0, tex.uv1);
            MID_UV_INTO(uv_ac, tex.uv0, tex.uv2);
            MID_UV_INTO(uv_bd, tex.uv1, tex.uv2);
            ab = MID_INDEX(source, midpoint_count);
            ac = MID_INDEX(source, midpoint_count + 1);
            bc = MID_INDEX(source, midpoint_count + 2);
            WRITE_UV_CACHED(first->uv1, uv_ab);
            WRITE_UV_CACHED(first->uv2, uv_ac);
            WRITE_INDEX(first->vertex1, ab);
            WRITE_INDEX(first->vertex2, ac);
            output_packet += 24;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            second = (KfTmdFt3 *)(output_packet + 4);
            WRITE_UV_CACHED(second->uv0, uv_ab);
            WRITE_UV_CACHED(second->uv2, uv_bd);
            WRITE_INDEX(second->vertex0, ab);
            WRITE_INDEX(second->vertex2, bc);
            output_packet += 24;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            third = (KfTmdFt3 *)(output_packet + 4);
            WRITE_UV_CACHED(third->uv0, uv_ac);
            WRITE_UV_CACHED(third->uv1, uv_bd);
            WRITE_INDEX(third->vertex0, ac);
            WRITE_INDEX(third->vertex1, bc);
            output_packet += 24;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, 6);
            fourth = (KfTmdFt3 *)(output_packet + 4);
            WRITE_UV_CACHED(fourth->uv0, uv_ab);
            WRITE_UV_CACHED(fourth->uv1, uv_bd);
            WRITE_UV_CACHED(fourth->uv2, uv_ac);
            WRITE_INDEX(fourth->vertex0, ab);
            WRITE_INDEX(fourth->vertex1, bc);
            WRITE_INDEX(fourth->vertex2, ac);
            output_packet += 24;
            output_packet_bytes += 96;
            midpoint_count += 3;
            target->primitive_count += 3;
        } else {
            u32 input_words = header.bytes.input_length + 1;
            resource_copy_words((u32 *)output_packet, (u32 *)source_packet, input_words);
            output_packet += input_words * KF_TMD_WORD_BYTES;
            output_packet_bytes += input_words * KF_TMD_WORD_BYTES;
        }
        header.word = *(u32 *)source_packet;
        source_packet += (header.bytes.input_length + 1) * KF_TMD_WORD_BYTES;
    }
    target->vertex_offset = target->primitive_offset + output_packet_bytes;
    target->vertex_count = source->vertex_count + midpoint_count;
    source_vertex_count = source->vertex_count;
    resource_copy_words((u32 *)output_packet, (u32 *)(base + source->vertex_offset),
                        source_vertex_count * 2);
    output_packet += source_vertex_count * sizeof(SVECTOR);
    resource_copy_words((u32 *)output_packet, (u32 *)midpoints, midpoint_count * 2);
    target->normal_offset = target->vertex_offset +
        source_vertex_count * sizeof(SVECTOR) + midpoint_count * sizeof(SVECTOR);
    target->normal_count = source->normal_count;
    resource_copy_words((u32 *)(output_packet + midpoint_count * sizeof(SVECTOR)),
                        (u32 *)(base + source->normal_offset), source->normal_count * 2);
}
#undef MID_INDEX
#undef MID_VECTOR
#undef MID_UV_INTO
#undef WRITE_UV_CACHED
#undef WRITE_INDEX

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
    if (state_8017d118.transition_active == 1 && state_8017d118.flag_16 &&
        object_index >= game_graphics_runtime.tmd_state.current_asset->flags) {
        return;
    }
    tmd_select_object_vertices(object_index);
    if (flags & KF_MAP_CELL_OBJECT_SPECIAL) {
        if (flags & KF_MAP_CELL_OBJECT_PREPARE) {
            if (tmd_get_object(object_index)->primitive_count < KF_MAP_CELL_PREPARED_LIMIT) {
                KfTmdPreparedAsset prepared_asset;

                tmd_prepare_subdivided_object(game_graphics_runtime.tmd_state.current_asset,
                              object_index, &prepared_asset);
                render_enqueue_tmd_with_clipping(object_index, 240, &prepared_asset);
                return;
            }
        }
        render_enqueue_tmd_with_clipping(object_index, 240, 0);
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
void render_map_cell_layers(s32 x, s32 z, u8 flags)
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
void render_map_cell_window(void)
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
                    render_map_cell_layers(column, row, *mask);
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
void render_active_model_rows(void)
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
            if (animation_prepare_asset_vertices(&entry->animation_state, entry->asset_id,
                              entry->animation_clip, entry->animation_phase,
                              object->vertex_count)) {
                tmd_select_object_vertices(0);
            }
            tmd_transform_vertices(object->vertex_count);
            render_enqueue_textured_tmd(0, 0);
        }
        entry++;
    } while (entry->state != KF_RENDER_MODEL_END);
}

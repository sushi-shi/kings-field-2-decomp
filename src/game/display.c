#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>
#include <kf/game/tmd_packets.h>
#include <kf/lib/math.h>
#include <kf/game/callback.h>
#include <kf/game/map_cell.h>
#include <kf/game/memory.h>
#include <kf/game/player.h>
#include <kf/game/asset.h>
#include <kf/game/render_model.h>

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

enum {
    KF_GPU_RESET_KEEP_DISPLAY = 3,
    KF_DISPLAY_WIDTH = 320,
    KF_DISPLAY_HEIGHT = 240,
    KF_PROJECTION_DISTANCE = 200,
    KF_CLIP_DEPTH = 100,
    KF_CLIP_FAR = 0x10000,
    KF_INITIAL_FOG_NEAR_DISTANCE = 0x59d8,
    /* SetFogNear takes a 15-bit distance at half scale. */
    KF_FOG_DISTANCE_MASK = 0x7fff
};

#define GRAPHICS game_graphics_runtime
#define DISPLAY game_graphics_runtime.display_state

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

DATA(0x8006d6d0, 0x4)
CVECTOR map_textured_primitive_color = {128, 128, 128, 0};

DATA(0x800fba58, 0x32000)
u8 display_primitive_memory[KF_DISPLAY_BUFFER_COUNT * KF_GAME_PRIMITIVE_BUFFER_BYTES];

DATA(0x8017d140, 0x17cf0)
KfGraphicsRuntimeGame game_graphics_runtime;

DATA(0x801d9610, 0x4)
s32 display_frame_cleared_word;

enum { KF_TMD_DEPTH_SHIFT = 2 };

RODATA(0x80011410, 0x74)

ADDRESS(0x8002d0a4, 0x30)
void fog_set_near(s32 distance)
{
    GRAPHICS.render_grid.fog_near_distance = distance;
    SetFogNear((distance & KF_FOG_DISTANCE_MASK) >> 1, KF_PROJECTION_DISTANCE);
}

ADDRESS(0x8002d0d4, 0x174)
void display_initialize(void)
{
    ResetGraph(KF_GPU_RESET_KEEP_DISPLAY);
    SetDispMask(1);
    InitGeom();
    SetGeomOffset(KF_DISPLAY_WIDTH / 2, KF_DISPLAY_HEIGHT / 2);
    SetGeomScreen(KF_PROJECTION_DISTANCE);
    InitClip(GRAPHICS.clip_edges, KF_DISPLAY_WIDTH, KF_DISPLAY_HEIGHT, KF_CLIP_DEPTH, 0, KF_CLIP_FAR);
    SetDefDrawEnv(&GRAPHICS.display_draw_environments[0], 0, 0, KF_DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&GRAPHICS.display_disp_environments[0], 0, KF_DISPLAY_HEIGHT, KF_DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDrawEnv(&GRAPHICS.display_draw_environments[1], 0, KF_DISPLAY_HEIGHT, KF_DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    SetDefDispEnv(&GRAPHICS.display_disp_environments[1], 0, 0, KF_DISPLAY_WIDTH, KF_DISPLAY_HEIGHT);
    GRAPHICS.display_draw_environments[0].dtd = GRAPHICS.display_draw_environments[1].dtd = 1;
    GRAPHICS.display_draw_environments[0].isbg = 1;
    GRAPHICS.display_draw_environments[1].isbg = 1;
    setRGB0(&GRAPHICS.display_draw_environments[0], 0, 0, 0);
    setRGB0(&GRAPHICS.display_draw_environments[1], 0, 0, 0);
    PutDispEnv(&GRAPHICS.display_disp_environments[0]);
    SetGeomScreen(KF_PROJECTION_DISTANCE);
    SetBackColor(0, 0, 0);
    SetFarColor(0, 0, 0);
    fog_set_near(KF_INITIAL_FOG_NEAR_DISTANCE);
    display_reset();
}

ADDRESS(0x8002d248, 0xe4)
void display_reset(void)
{
    u8 *memory;
    u8 *message_id;
    u8 messages_left;
    KfFloorItem *item;
    s32 items_left;

    DISPLAY.buffer_index = KF_DISPLAY_BUFFER_NONE;
    memory = display_primitive_memory;
    DISPLAY.primitive_buffers[0].start = DISPLAY.asset_load_buffer = memory;
    memory += KF_GAME_PRIMITIVE_BUFFER_BYTES;
    DISPLAY.primitive_buffers[1].start = DISPLAY.primitive_buffers[0].end = memory;
    memory += KF_GAME_PRIMITIVE_BUFFER_BYTES;
    DISPLAY.primitive_buffers[1].end = memory;
    GRAPHICS.notification_control.effect_phase = 0;
    GRAPHICS.notification_control.queue_tail = 0;
    GRAPHICS.notification_control.queue_head = 0;
    message_id = GRAPHICS.notification_message_ids;
    messages_left = KF_NOTIFICATION_CAPACITY - 1;
    do {
        *message_id++ = KF_NOTIFICATION_NONE;
    } while (messages_left-- != 0);
    item = GRAPHICS.floor_items;
    for (items_left = KF_FLOOR_ITEM_CAPACITY - 1; items_left != -1; items_left--) {
        item->kind = KF_FLOOR_ITEM_NONE;
        item++;
    }
    GRAPHICS.color_overlay_control = 0xff;
    GRAPHICS.map_placed_frame_counter = 0;
    pool_reset();
}

ADDRESS(0x8002d32c, 0x98)
void display_begin_frame(void)
{
    DISPLAY.buffer_index = DISPLAY.buffer_index == 0;
    DISPLAY.primitive_buffer = &DISPLAY.primitive_buffers[DISPLAY.buffer_index];
    DISPLAY.ordering_table = DISPLAY.ordering_tables[DISPLAY.buffer_index].entries;
    ClearOTagR(DISPLAY.ordering_table, KF_GAME_ORDERING_TABLE_LENGTH);
    DISPLAY.primitive_buffer->cursor = DISPLAY.primitive_buffer->start;
    display_frame_cleared_word = 0;
    GRAPHICS.frame_counter_b = 0;
    GRAPHICS.frame_counter_a = 0;
}

ADDRESS(0x8002d3c4, 0x94)
void display_present_frame(void)
{
    DrawSync(0);
    VSync(0);
    PutDrawEnv(&GRAPHICS.display_draw_environments[DISPLAY.buffer_index]);
    PutDispEnv(&GRAPHICS.display_disp_environments[DISPLAY.buffer_index]);
    DrawOTag(DISPLAY.ordering_table + (KF_GAME_ORDERING_TABLE_LENGTH - 1));
}

ADDRESS(0x8002d458, 0x2c)
void tmd_select(u16 slot)
{
    GRAPHICS.tmd_state.current_asset = GRAPHICS.tmd_state.slots[slot];
}

ADDRESS(0x8002d484, 0x24)
KfTmdObject *tmd_get_object(u16 index)
{
    return TMD_OBJECTS(GRAPHICS.tmd_state.current_asset) + index;
}

ADDRESS(0x8002d4a8, 0x10)
void tmd_set_current_vertices(SVECTOR *vertices)
{
    GRAPHICS.current_tmd_vertices = vertices;
}

ADDRESS(0x8002d4b8, 0x3c)
void tmd_select_object_vertices(u16 index)
{
    GRAPHICS.current_tmd_vertices =
        TMD_OBJECT_VERTICES(GRAPHICS.tmd_state.current_asset, tmd_get_object(index));
}

ADDRESS(0x8002d4f4, 0xe8)
void display_set_view_transform(const VECTOR *position, const SVECTOR *rotation)
{
    struct KfEulerAngles angles;

    if (position != 0) {
        GRAPHICS.render_state.view_position = *position;
        GRAPHICS.render_state.view_cell_x =
            GRAPHICS.render_state.view_position.vx >> KF_FIXED11_BITS;
        GRAPHICS.render_state.view_cell_z =
            GRAPHICS.render_state.view_position.vz >> KF_FIXED11_BITS;
    }
    if (rotation != 0)
        GRAPHICS.render_state.view_rotation = *rotation;

    angles.x = (u16)GRAPHICS.render_state.view_rotation.vx;
    angles.y = -(u16)GRAPHICS.render_state.view_rotation.vy;
    angles.z = (u16)GRAPHICS.render_state.view_rotation.vz;
    matrix_set_rotation_xzy(&angles, &GRAPHICS.render_state.view_matrix);
    matrix_set_rotation_x(angles.x, &GRAPHICS.render_state.pitch_matrix);
}

ADDRESS(0x8002d5dc, 0x2d4)
void tmd_prepare_primitive_indices(KfTmdHeader *tmd)
{
    KfTmdObject *object;
    u8 *packet;
    KfTmdPrimitive *primitive;
    u32 objects_left;
    u32 primitives_left;
    KfTmdPacketHeader header;

    objects_left = tmd->object_count;
    object = TMD_OBJECTS(tmd);
    while (--objects_left != (u32)-1) {
        primitives_left = object->primitive_count;
        packet = (u8 *)tmd + (object->primitive_offset + KF_TMD_HEADER_BYTES);
        while (--primitives_left != (u32)-1) {
            primitive = (KfTmdPrimitive *)TMD_PACKET_BODY(packet);
            header.word = *(u32 *)packet;
            packet = (u8 *)primitive + header.bytes.input_length * KF_TMD_WORD_BYTES;
            switch (tmd_packet_kind(header.word)) {
            case KF_TMD_MODE_F3: {
                primitive->f3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f3.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_G3: {
                primitive->g3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g3.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_FT3: {
                primitive->ft3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft3.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_GT3: {
                primitive->gt3.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt3.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_F4: {
                primitive->f4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->f4.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_G4: {
                primitive->g4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->g4.normal3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_FT4: {
                primitive->ft4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->ft4.normal <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            case KF_TMD_MODE_GT4: {
                primitive->gt4.vertex0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.vertex1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.vertex2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.vertex3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal0 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal1 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal2 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                primitive->gt4.normal3 <<= KF_TMD_VECTOR_OFFSET_SHIFT;
                break;
            }
            }
        }
        object++;
    }
}

ADDRESS(0x8002d8b0, 0x40)
void tmd_register(u16 slot, KfTmdHeader *tmd)
{
    game_graphics_runtime.tmd_state.current_asset = game_graphics_runtime.tmd_state.slots[slot] = tmd;
    tmd_prepare_primitive_indices(tmd);
}

ADDRESS(0x8002d8f0, 0x20)
void tmd_set_slot(u16 slot, KfTmdHeader *tmd)
{
    game_graphics_runtime.tmd_state.slots[slot] = tmd;
}

/* Left empty in retail. */
ADDRESS(0x8002d910, 0x8)
void tmd_release_slot(void)
{
}

ADDRESS(0x8002d918, 0x17c)
void tmd_project_vertices_with_fog(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    long perspective;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    remaining = count;
    if (game_graphics_runtime.render_grid.fog_near_distance >= 32000) {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            projected->depth_cue = 0;
            projected++;
            vertex++;
        }
    } else if (game_graphics_runtime.render_grid.fog_near_distance & 0x8000) {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            perspective -= 800;
            if (perspective < 0) {
                perspective = 0;
            }
            projected->depth_cue = (u16)perspective << 1;
            projected++;
            vertex++;
        }
    } else {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            if (perspective >= 2800) {
                perspective += (perspective - 2800) * 2;
            }
            projected->depth_cue = perspective;
            projected++;
            vertex++;
        }
    }
}

ADDRESS(0x8002da94, 0x144)
void tmd_project_vertices_mark_clipped(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    long perspective;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    remaining = count;
    if (game_graphics_runtime.render_grid.fog_near_distance >= 32000) {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            if (gte_flags != 0x1000) {
                projected->sz = -1;
            }
            projected->depth_cue = 0;
            projected++;
            vertex++;
        }
    } else {
        for (remaining--; remaining != -1; remaining--) {
            projected->sz = RotTransPers(vertex, (long *)projected,
                                         &perspective, &gte_flags);
            if (gte_flags != 0x1000) {
                projected->sz = -1;
            }
            if (perspective >= 2800) {
                perspective += (perspective - 2800) * 2;
            }
            projected->depth_cue = perspective;
            projected++;
            vertex++;
        }
    }
}

ADDRESS(0x8002dbd8, 0xa8)
void tmd_transform_vertices(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    VECTOR transformed;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    for (remaining = count - 1; remaining != -1; remaining--) {
        RotTrans(vertex, &transformed, &gte_flags);
        projected->x = transformed.vx;
        projected->y = transformed.vy;
        projected->depth_cue = transformed.vz;
        projected->sz = transformed.vz >> KF_TMD_DEPTH_SHIFT;
        projected++;
        vertex++;
    }
}

ADDRESS(0x8002dc80, 0xa8)
void tmd_transform_vertices_depth(s32 count, s16 depth)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    VECTOR transformed;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    for (remaining = count - 1; remaining != -1; remaining--) {
        RotTrans(vertex, &transformed, &gte_flags);
        projected->x = transformed.vx;
        projected->y = transformed.vy;
        projected->depth_cue = transformed.vz;
        projected->sz = depth;
        projected++;
        vertex++;
    }
}

ADDRESS(0x8002dd28, 0x8c)
void tmd_project_vertices(s32 count)
{
    KfScreenVertex *projected;
    SVECTOR *vertex;
    long perspective;
    long gte_flags;
    s32 remaining;

    projected = game_graphics_runtime.tmd_projected_vertices;
    vertex = game_graphics_runtime.current_tmd_vertices;
    for (remaining = count - 1; remaining != -1; remaining--) {
        projected->sz = RotTransPers(vertex, (long *)projected, &perspective, &gte_flags);
        projected->depth_cue = 0;
        projected++;
        vertex++;
    }
}

#define TMD_VERTEX(base, offset) ((KfScreenVertex *)((base) + (offset)))
#define TMD_XY(vertex) (*(long *)(vertex))

ADDRESS(0x8002ddb4, 0x728)
void render_enqueue_blended_tmd(u16 object_index, s32 depth_bias, s32 render_mode)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    u32 remaining;
    u16 blend_bits = (u16)render_mode << 5;

    object = tmd_get_object(object_index);
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
             (object->primitive_offset + KF_TMD_HEADER_BYTES);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
              (object->normal_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
        KfTmdPacketHeader header;
        u8 mode;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;
        s32 depth;

        header.word = *(u32 *)packet;
        mode = header.word >> 24;
        packet += KF_TMD_PACKET_HEADER_BYTES;
        face = (KfTmdPrimitive *)packet;
        switch (mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_FT3: {
            POLY_FT3 *prim;

            va = TMD_VERTEX(vertices, face->ft3.vertex0);
            vb = TMD_VERTEX(vertices, face->ft3.vertex1);
            vc = TMD_VERTEX(vertices, face->ft3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (POLY_FT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft3.clut;
            prim->tpage = (face->ft3.tpage & 0xff9f) | blend_bits;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(u16 *)&prim->u0 = face->ft3.uv0;
            *(u16 *)&prim->u1 = face->ft3.uv1;
            *(u16 *)&prim->u2 = face->ft3.uv2;
            NormalColorDpq((SVECTOR *)(normals + face->ft3.normal),
                           &map_textured_primitive_color,
                           (va->depth_cue + vb->depth_cue + vc->depth_cue) / 3,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 7;
            prim->code = 0x26;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth >= KF_MAP_OT_DEPTH_LIMIT)
                break;
            AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        case KF_TMD_MODE_GT3: {
            KfGpuGT3 *prim;

            va = TMD_VERTEX(vertices, face->gt3.vertex0);
            vb = TMD_VERTEX(vertices, face->gt3.vertex1);
            vc = TMD_VERTEX(vertices, face->gt3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt3.clut;
            prim->packed.tpage = (face->gt3.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.uv0 = face->gt3.uv0;
            prim->packed.uv1 = face->gt3.uv1;
            prim->packed.uv2 = face->gt3.uv2;
            NormalColorDpq3((SVECTOR *)(normals + face->gt3.normal0),
                            (SVECTOR *)(normals + face->gt3.normal1),
                            (SVECTOR *)(normals + face->gt3.normal2),
                            &map_textured_primitive_color, va->depth_cue,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 9;
            prim->sdk.code = 0x36;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_GT4: {
            KfGpuGT4 *prim;

            va = TMD_VERTEX(vertices, face->gt4.vertex0);
            vb = TMD_VERTEX(vertices, face->gt4.vertex1);
            vc = TMD_VERTEX(vertices, face->gt4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->gt4.vertex3);
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt4.clut;
            prim->packed.tpage = (face->gt4.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.xy3 = TMD_XY(vd);
            prim->packed.uv0 = face->gt4.uv0;
            prim->packed.uv1 = face->gt4.uv1;
            prim->packed.uv2 = face->gt4.uv2;
            prim->packed.uv3 = face->gt4.uv3;
            NormalColorDpq3((SVECTOR *)(normals + face->gt4.normal0),
                            (SVECTOR *)(normals + face->gt4.normal1),
                            (SVECTOR *)(normals + face->gt4.normal2),
                            &map_textured_primitive_color, va->depth_cue,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            NormalColorDpq((SVECTOR *)(normals + face->gt4.normal3),
                           &map_textured_primitive_color, va->depth_cue,
                           &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 12;
            prim->sdk.code = 0x3e;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_FT4: {
            POLY_FT4 *prim;

            va = TMD_VERTEX(vertices, face->ft4.vertex0);
            vb = TMD_VERTEX(vertices, face->ft4.vertex1);
            vc = TMD_VERTEX(vertices, face->ft4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->ft4.vertex3);
            prim = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft4.clut;
            prim->tpage = (face->ft4.tpage & 0xff9f) | blend_bits;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(long *)&prim->x3 = TMD_XY(vd);
            *(u16 *)&prim->u0 = face->ft4.uv0;
            *(u16 *)&prim->u1 = face->ft4.uv1;
            *(u16 *)&prim->u2 = face->ft4.uv2;
            *(u16 *)&prim->u3 = face->ft4.uv3;
            NormalColorDpq((SVECTOR *)(normals + face->ft4.normal),
                           &map_textured_primitive_color,
                           (va->depth_cue + vb->depth_cue + vc->depth_cue + vd->depth_cue) >> 2,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 9;
            prim->code = 0x2e;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        }
        packet += TMD_PACKET_BODY_BYTES(header.word);
    }
}

ADDRESS(0x8002e4dc, 0x704)
void render_enqueue_textured_tmd(u16 object_index, s32 depth_bias)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    KfTmdPacketHeader header;
    u32 remaining;

    object = tmd_get_object(object_index);
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
             (object->primitive_offset + KF_TMD_HEADER_BYTES);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
              (object->normal_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
        s32 mode;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;
        s32 depth;

        header.word = *(u32 *)packet;
        mode = header.word >> 24;
        packet += KF_TMD_PACKET_HEADER_BYTES;
        face = (KfTmdPrimitive *)packet;
        switch (mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_FT3: {
            POLY_FT3 *prim;

            va = TMD_VERTEX(vertices, face->ft3.vertex0);
            vb = TMD_VERTEX(vertices, face->ft3.vertex1);
            vc = TMD_VERTEX(vertices, face->ft3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (POLY_FT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft3.clut;
            prim->tpage = face->ft3.tpage;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(u16 *)&prim->u0 = face->ft3.uv0;
            *(u16 *)&prim->u1 = face->ft3.uv1;
            *(u16 *)&prim->u2 = face->ft3.uv2;
            NormalColorDpq((SVECTOR *)(normals + face->ft3.normal),
                           &map_textured_primitive_color,
                           (va->depth_cue + vb->depth_cue + vc->depth_cue) / 3,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 7;
            prim->code = mode;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth >= KF_MAP_OT_DEPTH_LIMIT)
                break;
            AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        case KF_TMD_MODE_GT3: {
            KfGpuGT3 *prim;

            va = TMD_VERTEX(vertices, face->gt3.vertex0);
            vb = TMD_VERTEX(vertices, face->gt3.vertex1);
            vc = TMD_VERTEX(vertices, face->gt3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt3.clut;
            prim->packed.tpage = face->gt3.tpage;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.uv0 = face->gt3.uv0;
            prim->packed.uv1 = face->gt3.uv1;
            prim->packed.uv2 = face->gt3.uv2;
            NormalColorDpq3((SVECTOR *)(normals + face->gt3.normal0),
                            (SVECTOR *)(normals + face->gt3.normal1),
                            (SVECTOR *)(normals + face->gt3.normal2),
                            &map_textured_primitive_color, va->depth_cue,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 9;
            prim->sdk.code = mode;
            depth = (va->sz + vb->sz + vc->sz) / 3;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_GT4: {
            KfGpuGT4 *prim;

            va = TMD_VERTEX(vertices, face->gt4.vertex0);
            vb = TMD_VERTEX(vertices, face->gt4.vertex1);
            vc = TMD_VERTEX(vertices, face->gt4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->gt4.vertex3);
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt4.clut;
            prim->packed.tpage = face->gt4.tpage;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.xy3 = TMD_XY(vd);
            prim->packed.uv0 = face->gt4.uv0;
            prim->packed.uv1 = face->gt4.uv1;
            prim->packed.uv2 = face->gt4.uv2;
            prim->packed.uv3 = face->gt4.uv3;
            NormalColorDpq3((SVECTOR *)(normals + face->gt4.normal0),
                            (SVECTOR *)(normals + face->gt4.normal1),
                            (SVECTOR *)(normals + face->gt4.normal2),
                            &map_textured_primitive_color, va->depth_cue,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            NormalColorDpq((SVECTOR *)(normals + face->gt4.normal3),
                           &map_textured_primitive_color, va->depth_cue,
                           &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 12;
            prim->sdk.code = mode;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], &prim->sdk);
            break;
        }
        case KF_TMD_MODE_FT4: {
            POLY_FT4 *prim;

            va = TMD_VERTEX(vertices, face->ft4.vertex0);
            vb = TMD_VERTEX(vertices, face->ft4.vertex1);
            vc = TMD_VERTEX(vertices, face->ft4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->ft4.vertex3);
            prim = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->clut = face->ft4.clut;
            prim->tpage = face->ft4.tpage;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(long *)&prim->x3 = TMD_XY(vd);
            *(u16 *)&prim->u0 = face->ft4.uv0;
            *(u16 *)&prim->u1 = face->ft4.uv1;
            *(u16 *)&prim->u2 = face->ft4.uv2;
            *(u16 *)&prim->u3 = face->ft4.uv3;
            NormalColorDpq((SVECTOR *)(normals + face->ft4.normal),
                           &map_textured_primitive_color,
                           (va->depth_cue + vb->depth_cue + vc->depth_cue + vd->depth_cue) >> 2,
                           (CVECTOR *)&prim->r0);
            ((u8 *)&prim->tag)[3] = 9;
            prim->code = mode;
            depth = (va->sz + vb->sz + vc->sz + vd->sz) >> 2;
            if (depth <= 0)
                break;
            depth += depth_bias;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT)
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], prim);
            break;
        }
        }
        packet += TMD_PACKET_BODY_BYTES(header.word);
    }
}

ADDRESS(0x8002ebe0, 0x5b4)
void render_enqueue_tmd_fixed_depth(u16 object_index, s32 blend_mode, s16 fixed_depth)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    u32 remaining;
    u32 blend_bits;

    object = tmd_get_object(object_index);
    blend_bits = (u32)blend_mode << 5;
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
             (object->primitive_offset + KF_TMD_HEADER_BYTES);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
              (object->normal_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
        KfTmdPacketHeader header;
        u8 mode;
        KfTmdPrimitive *face;
        KfScreenVertex *va;
        KfScreenVertex *vb;
        KfScreenVertex *vc;
        KfScreenVertex *vd;
        void *enqueue_prim;

        header.word = *(u32 *)packet;
        mode = header.word >> 24;
        packet += KF_TMD_PACKET_HEADER_BYTES;
        face = (KfTmdPrimitive *)packet;
        switch (mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_GT3: {
            KfGpuGT3 *prim;

            va = TMD_VERTEX(vertices, face->gt3.vertex0);
            vb = TMD_VERTEX(vertices, face->gt3.vertex1);
            vc = TMD_VERTEX(vertices, face->gt3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt3.clut;
            prim->packed.tpage = (face->gt3.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.uv0 = face->gt3.uv0;
            prim->packed.uv1 = face->gt3.uv1;
            prim->packed.uv2 = face->gt3.uv2;
            NormalColorCol3((SVECTOR *)(normals + face->gt3.normal0),
                            (SVECTOR *)(normals + face->gt3.normal1),
                            (SVECTOR *)(normals + face->gt3.normal2),
                            &map_textured_primitive_color,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 9;
            prim->sdk.code = (mode & 2) | 0x34;
            enqueue_prim = &prim->sdk;
            goto enqueue;
        }
        case KF_TMD_MODE_GT4: {
            KfGpuGT4 *prim;

            va = TMD_VERTEX(vertices, face->gt4.vertex0);
            vb = TMD_VERTEX(vertices, face->gt4.vertex1);
            vc = TMD_VERTEX(vertices, face->gt4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->gt4.vertex3);
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            prim->packed.clut = face->gt4.clut;
            prim->packed.tpage = (face->gt4.tpage & 0xff9f) | blend_bits;
            prim->packed.xy0 = TMD_XY(va);
            prim->packed.xy1 = TMD_XY(vb);
            prim->packed.xy2 = TMD_XY(vc);
            prim->packed.xy3 = TMD_XY(vd);
            prim->packed.uv0 = face->gt4.uv0;
            prim->packed.uv1 = face->gt4.uv1;
            prim->packed.uv2 = face->gt4.uv2;
            prim->packed.uv3 = face->gt4.uv3;
            NormalColorCol3((SVECTOR *)(normals + face->gt4.normal0),
                            (SVECTOR *)(normals + face->gt4.normal1),
                            (SVECTOR *)(normals + face->gt4.normal2),
                            &map_textured_primitive_color,
                            &prim->packed.color0, &prim->packed.color1,
                            &prim->packed.color2);
            NormalColorCol((SVECTOR *)(normals + face->gt4.normal3),
                           &map_textured_primitive_color,
                           &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 12;
            prim->sdk.code = (mode & 2) | 0x3c;
            enqueue_prim = &prim->sdk;
            goto enqueue;
        }
        case KF_TMD_MODE_G3: {
            POLY_G3 *prim;

            va = TMD_VERTEX(vertices, face->g3.vertex0);
            vb = TMD_VERTEX(vertices, face->g3.vertex1);
            vc = TMD_VERTEX(vertices, face->g3.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            prim = (POLY_G3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_G3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            NormalColorCol3((SVECTOR *)(normals + face->g3.normal0),
                            (SVECTOR *)(normals + face->g3.normal1),
                            (SVECTOR *)(normals + face->g3.normal2),
                            &face->g3.color,
                            (CVECTOR *)&prim->r0, (CVECTOR *)&prim->r1,
                            (CVECTOR *)&prim->r2);
            ((u8 *)&prim->tag)[3] = 6;
            prim->code = 0x30;
            enqueue_prim = prim;
            goto enqueue;
        }
        case KF_TMD_MODE_G4: {
            POLY_G4 *prim;

            va = TMD_VERTEX(vertices, face->g4.vertex0);
            vb = TMD_VERTEX(vertices, face->g4.vertex1);
            vc = TMD_VERTEX(vertices, face->g4.vertex2);
            if (NormalClip(TMD_XY(va), TMD_XY(vb), TMD_XY(vc)) <= 0)
                break;
            vd = TMD_VERTEX(vertices, face->g4.vertex3);
            prim = (POLY_G4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_G4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end)
                return;
            *(long *)&prim->x0 = TMD_XY(va);
            *(long *)&prim->x1 = TMD_XY(vb);
            *(long *)&prim->x2 = TMD_XY(vc);
            *(long *)&prim->x3 = TMD_XY(vd);
            NormalColorCol3((SVECTOR *)(normals + face->g4.normal0),
                            (SVECTOR *)(normals + face->g4.normal1),
                            (SVECTOR *)(normals + face->g4.normal2),
                            &face->g4.color,
                            (CVECTOR *)&prim->r0, (CVECTOR *)&prim->r1,
                            (CVECTOR *)&prim->r2);
            NormalColorCol((SVECTOR *)(normals + face->g4.normal3),
                           &face->g4.color,
                           (CVECTOR *)&prim->r3);
            ((u8 *)&prim->tag)[3] = 8;
            prim->code = 0x38;
            enqueue_prim = prim;
            goto enqueue;
        }
        }
        goto next_packet;
enqueue:
        if (fixed_depth <= 0)
            goto next_packet;
        if ((u32)fixed_depth >= KF_MAP_OT_DEPTH_LIMIT)
            goto next_packet;
        AddPrim(&game_graphics_runtime.display_state.ordering_table[fixed_depth],
                enqueue_prim);
next_packet:
        packet += TMD_PACKET_BODY_BYTES(header.word);
    }
}

/* The prepared indices in an FT packet are byte offsets into this array. */
#define MAP_VERTEX(base, offset) ((KfScreenVertex *)((u8 *)(base) + (offset)))
#define MAP_XY(vertex) (*(long *)(vertex))

ADDRESS(0x8002f194, 0x41c)
void render_enqueue_map(u16 object_index)
{
    KfTmdObject *object;
    KfTmdPacketHeader header;
    u8 *normals;
    u8 *packet;
    u32 remaining;
    CVECTOR shade;
    KfScreenVertex *va;
    KfScreenVertex *vb;
    KfScreenVertex *vc;
    KfScreenVertex *vd;

    object = tmd_get_object(object_index);
    normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
        (object->normal_offset + KF_TMD_HEADER_BYTES);
    tmd_project_vertices_with_fog(object->vertex_count);
    packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
        (object->primitive_offset + KF_TMD_HEADER_BYTES);
    remaining = object->primitive_count;
    while (remaining-- != 0) {
        u8 *vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;

        header.word = *(u32 *)packet;
        packet += KF_TMD_PACKET_HEADER_BYTES;
        switch (header.bytes.mode & KF_TMD_MODE_MASK) {
        case KF_TMD_MODE_FT4: {
            KfTmdFt4 *face = (KfTmdFt4 *)packet;
            KfGpuGT4 *prim;
            s32 depth;

            va = MAP_VERTEX(vertices, face->vertex0);
            vb = MAP_VERTEX(vertices, face->vertex1);
            vc = MAP_VERTEX(vertices, face->vertex2);
            if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                break;
            }
            prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            vd = MAP_VERTEX(vertices, face->vertex3);
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end) {
                return;
            }
            prim->packed.clut = face->clut;
            prim->packed.tpage = face->tpage;
            prim->packed.xy0 = MAP_XY(va);
            prim->packed.xy1 = MAP_XY(vb);
            prim->packed.xy2 = MAP_XY(vc);
            prim->packed.xy3 = MAP_XY(vd);
            prim->packed.uv0 = face->uv0;
            prim->packed.uv1 = face->uv1;
            prim->packed.uv2 = face->uv2;
            prim->packed.uv3 = face->uv3;
            NormalColorCol((SVECTOR *)(normals + face->normal),
                           &map_textured_primitive_color, &shade);
            DpqColor(&shade, va->depth_cue, &prim->packed.color0);
            DpqColor(&shade, vb->depth_cue, &prim->packed.color1);
            DpqColor(&shade, vc->depth_cue, &prim->packed.color2);
            DpqColor(&shade, vd->depth_cue, &prim->packed.color3);
            ((u8 *)&prim->sdk.tag)[3] = 0x0c;
            prim->sdk.code = header.bytes.mode | 0x3c;
            depth = ((va->sz + vb->sz + vc->sz + vd->sz) >> 2) +
                KF_MAP_OT_DEPTH_BIAS;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT) {
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth],
                        &prim->sdk);
            }
            break;
        }
        case KF_TMD_MODE_FT3: {
            KfTmdFt3 *face = (KfTmdFt3 *)packet;
            KfGpuGT3 *prim;
            s32 depth;

            va = MAP_VERTEX(vertices, face->vertex0);
            vb = MAP_VERTEX(vertices, face->vertex1);
            vc = MAP_VERTEX(vertices, face->vertex2);
            if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                break;
            }
            prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
            game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
            if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                game_graphics_runtime.display_state.primitive_buffer->end) {
                return;
            }
            prim->packed.clut = face->clut;
            prim->packed.tpage = face->tpage;
            prim->packed.xy0 = MAP_XY(va);
            prim->packed.xy1 = MAP_XY(vb);
            prim->packed.xy2 = MAP_XY(vc);
            prim->packed.uv0 = face->uv0;
            prim->packed.uv1 = face->uv1;
            prim->packed.uv2 = face->uv2;
            NormalColorCol((SVECTOR *)(normals + face->normal),
                           &map_textured_primitive_color, &shade);
            DpqColor(&shade, va->depth_cue, &prim->packed.color0);
            DpqColor(&shade, vb->depth_cue, &prim->packed.color1);
            DpqColor(&shade, vc->depth_cue, &prim->packed.color2);
            ((u8 *)&prim->sdk.tag)[3] = 0x09;
            prim->sdk.code = header.bytes.mode | 0x34;
            depth = (va->sz + vb->sz + vc->sz) / 3 + KF_MAP_OT_DEPTH_BIAS;
            if ((u32)depth < KF_MAP_OT_DEPTH_LIMIT) {
                AddPrim(&game_graphics_runtime.display_state.ordering_table[depth],
                        &prim->sdk);
            }
            break;
        }
        }
        packet += header.bytes.input_length * KF_TMD_WORD_BYTES;
    }
}

/* The clipping SDK returns EVECTOR pointers; NormalClip takes packed screen XY. */
#define CLIPPED_MAP_VERTEX(index) \
    (game_graphics_runtime.clip_result_vertices[index])
#define CLIPPED_MAP_XY(vertex) (*(long *)&(vertex)->sxy)

ADDRESS(0x8002f5b0, 0x258)
void render_enqueue_clipped_tmd_polygon(s32 vertex_count, SVECTOR *normal, u16 clut, u16 tpage,
                   u32 mode, s32 depth_bias)
{
    EVECTOR *first;
    EVECTOR *second;
    EVECTOR *third;
    CVECTOR shade;
    CVECTOR first_color;
    u32 packet_code;
    EVECTOR **next;
    s32 triangle_count;

    if (NormalClip(CLIPPED_MAP_XY(CLIPPED_MAP_VERTEX(0)),
                   CLIPPED_MAP_XY(CLIPPED_MAP_VERTEX(1)),
                   CLIPPED_MAP_XY(CLIPPED_MAP_VERTEX(2))) <= 0) {
        return;
    }
    NormalColorCol(normal, &map_textured_primitive_color, &shade);
    next = game_graphics_runtime.clip_result_vertices;
    first = *next++;
    packet_code = mode | 0x34;
    DpqColor(&shade, first->sxyz.pad >> 1, &first_color);
    second = *next++;
    DpqColor(&shade, second->sxyz.pad >> 1, &second->rgb);

    triangle_count = vertex_count - 2;
    goto loop_test;
loop_body: {
        KfGpuGT3 *packet;
        s32 depth;
        s32 summed_depth;

        third = *next++;
        DpqColor(&shade, third->sxyz.pad >> 1, &third->rgb);
        packet = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
        game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
        if (game_graphics_runtime.display_state.primitive_buffer->cursor >
            game_graphics_runtime.display_state.primitive_buffer->end) {
            return;
        }
        packet->packed.clut = clut;
        packet->packed.tpage = tpage;
        packet->packed.xy0 = CLIPPED_MAP_XY(first);
        packet->packed.xy1 = CLIPPED_MAP_XY(second);
        packet->packed.xy2 = CLIPPED_MAP_XY(third);
        packet->packed.uv0 = first->txuv;
        packet->packed.uv1 = second->txuv;
        packet->packed.uv2 = third->txuv;
        *(u32 *)&packet->packed.color0 = *(u32 *)&first_color;
        *(u32 *)&packet->packed.color1 = *(u32 *)&second->rgb;
        *(u32 *)&packet->packed.color2 = *(u32 *)&third->rgb;
        ((u8 *)&packet->sdk.tag)[3] = 9;
        packet->sdk.code = packet_code;
        summed_depth = first->sxyz.vz + second->sxyz.vz + third->sxyz.vz;
        depth = summed_depth / 12 + depth_bias;
        if (depth < 16) {
            depth = 16;
        }
        AddPrim(&game_graphics_runtime.display_state.ordering_table[depth & 0x1fff],
                &packet->sdk);

        second = third;
    }
loop_test:
    if (triangle_count-- > 0) {
        goto loop_body;
    }
}

#define MAP_OUTSIDE_Y(delta) ((u32)(delta) + 511u >= 1023u)
#define MAP_OUTSIDE_X(delta) ((u32)(delta) + 1023u >= 2047u)
#define MAP_ORIGINAL_VERTEX(base, offset) ((SVECTOR *)((u8 *)(base) + (offset)))

ADDRESS(0x8002f808, 0x754)
void render_enqueue_tmd_with_clipping(u16 object_index, s32 depth_bias,
                   KfTmdPreparedAsset *prepared_asset)
{
    KfTmdObject *object;
    u8 *normals;
    u8 *packet;
    u8 *vertices;
    SVECTOR *original_vertices;
    u32 remaining;
    KfTmdPacketHeader header;
    CVECTOR shade;

    if (prepared_asset != 0) {
        object = &prepared_asset->object;
        normals = (u8 *)prepared_asset +
            (object->normal_offset + KF_TMD_HEADER_BYTES);
        game_graphics_runtime.current_tmd_vertices =
            (SVECTOR *)((u8 *)prepared_asset +
                        (object->vertex_offset + KF_TMD_HEADER_BYTES));
    } else {
        object = tmd_get_object(object_index);
        normals = (u8 *)game_graphics_runtime.tmd_state.current_asset +
            (object->normal_offset + KF_TMD_HEADER_BYTES);
    }
    original_vertices = game_graphics_runtime.current_tmd_vertices;
    tmd_project_vertices_mark_clipped(object->vertex_count);
    if (prepared_asset != 0) {
        packet = (u8 *)prepared_asset +
            (object->primitive_offset + KF_TMD_HEADER_BYTES);
    } else {
        packet = (u8 *)game_graphics_runtime.tmd_state.current_asset +
            (object->primitive_offset + KF_TMD_HEADER_BYTES);
    }
    remaining = object->primitive_count;
    if (remaining-- != 0) {
        do {
            vertices = (u8 *)game_graphics_runtime.tmd_projected_vertices;
            header.word = *(u32 *)packet;
            packet += KF_TMD_PACKET_HEADER_BYTES;
            switch (header.bytes.mode & KF_TMD_MODE_MASK) {
            case KF_TMD_MODE_FT4: {
                KfTmdFt4 *face = (KfTmdFt4 *)packet;
                KfScreenVertex *va = MAP_VERTEX(vertices, face->vertex0);
                KfScreenVertex *vb = MAP_VERTEX(vertices, face->vertex1);
                KfScreenVertex *vc = MAP_VERTEX(vertices, face->vertex2);
                KfScreenVertex *vd = MAP_VERTEX(vertices, face->vertex3);
                s32 dy01;
                s32 dy13;
                s32 dy32;
                s32 dy20;
                s32 dy12;
                s32 dx01;
                s32 dx13;
                s32 dx32;
                s32 dx20;
                s32 dx12;
                s32 clipped_count;
                s32 depth;
                KfGpuGT4 *prim;

                dy01 = va->y - vb->y;
                dy13 = vb->y - vd->y;
                dy32 = vd->y - vc->y;
                dy20 = vc->y - va->y;
                dy12 = vb->y - vc->y;
                dx01 = va->x - vb->x;
                dx13 = vb->x - vd->x;
                dx32 = vd->x - vc->x;
                dx20 = vc->x - va->x;
                dx12 = vb->x - vc->x;
                if (!((s16)(va->sz | vb->sz | vc->sz | vd->sz) == -1 ||
                    MAP_OUTSIDE_Y(dy01) || MAP_OUTSIDE_Y(dy13) ||
                    MAP_OUTSIDE_Y(dy32) || MAP_OUTSIDE_Y(dy20) ||
                    MAP_OUTSIDE_Y(dy12) || MAP_OUTSIDE_X(dx01) ||
                    MAP_OUTSIDE_X(dx13) || MAP_OUTSIDE_X(dx32) ||
                    MAP_OUTSIDE_X(dx20) || MAP_OUTSIDE_X(dx12))) {
                    if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                        break;
                    }
                    prim = (KfGpuGT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
                    game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT4);
                    if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                        game_graphics_runtime.display_state.primitive_buffer->end) {
                        return;
                    }
                    prim->packed.clut = face->clut;
                    prim->packed.tpage = face->tpage;
                    prim->packed.xy0 = MAP_XY(va);
                    prim->packed.xy1 = MAP_XY(vb);
                    prim->packed.xy2 = MAP_XY(vc);
                    prim->packed.xy3 = MAP_XY(vd);
                    prim->packed.uv0 = face->uv0;
                    prim->packed.uv1 = face->uv1;
                    prim->packed.uv2 = face->uv2;
                    prim->packed.uv3 = face->uv3;
                    NormalColorCol((SVECTOR *)(normals + face->normal),
                                   &map_textured_primitive_color, &shade);
                    DpqColor(&shade, va->depth_cue, &prim->packed.color0);
                    DpqColor(&shade, vb->depth_cue, &prim->packed.color1);
                    DpqColor(&shade, vc->depth_cue, &prim->packed.color2);
                    DpqColor(&shade, vd->depth_cue, &prim->packed.color3);
                    ((u8 *)&prim->sdk.tag)[3] = 12;
                    prim->sdk.code = (header.bytes.mode & 2) | 0x3c;
                    depth = ((va->sz + vb->sz + vc->sz + vd->sz) >> 2) + depth_bias;
                    if (depth < 16) {
                        depth = 16;
                    }
                    AddPrim(&game_graphics_runtime.display_state.ordering_table[depth & 0x1fff],
                            &prim->sdk);
                } else {
                    clipped_count = Clip4FTP(MAP_ORIGINAL_VERTEX(original_vertices, face->vertex0),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex1),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex2),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex3),
                                             (short *)&face->uv0,
                                             (short *)&face->uv1,
                                             (short *)&face->uv2,
                                             (short *)&face->uv3,
                                             game_graphics_runtime.clip_result_vertices);
                    if (clipped_count >= 3) {
                        render_enqueue_clipped_tmd_polygon(clipped_count,
                                       (SVECTOR *)(normals + face->normal),
                                       face->clut, face->tpage,
                                       header.bytes.mode & 2, depth_bias);
                    }
                }
                break;
            }
            case KF_TMD_MODE_FT3: {
                KfTmdFt3 *face = (KfTmdFt3 *)packet;
                KfScreenVertex *va = MAP_VERTEX(vertices, face->vertex0);
                KfScreenVertex *vb = MAP_VERTEX(vertices, face->vertex1);
                KfScreenVertex *vc = MAP_VERTEX(vertices, face->vertex2);
                s32 dy01;
                s32 dy12;
                s32 dy20;
                s32 dx01;
                s32 dx12;
                s32 dx20;
                s32 clipped_count;
                s32 depth;
                KfGpuGT3 *prim;

                dy01 = va->y - vb->y;
                dy12 = vb->y - vc->y;
                dy20 = vc->y - va->y;
                dx01 = va->x - vb->x;
                dx12 = vb->x - vc->x;
                dx20 = vc->x - va->x;
                if (!((s16)(va->sz | vb->sz | vc->sz) == -1 ||
                    MAP_OUTSIDE_Y(dy01) || MAP_OUTSIDE_Y(dy12) ||
                    MAP_OUTSIDE_Y(dy20) || MAP_OUTSIDE_X(dx01) ||
                    MAP_OUTSIDE_X(dx12) || MAP_OUTSIDE_X(dx20))) {
                    if (NormalClip(MAP_XY(va), MAP_XY(vb), MAP_XY(vc)) <= 0) {
                        break;
                    }
                    prim = (KfGpuGT3 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
                    game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_GT3);
                    if (game_graphics_runtime.display_state.primitive_buffer->cursor >
                        game_graphics_runtime.display_state.primitive_buffer->end) {
                        return;
                    }
                    prim->packed.clut = face->clut;
                    prim->packed.tpage = face->tpage;
                    prim->packed.xy0 = MAP_XY(va);
                    prim->packed.xy1 = MAP_XY(vb);
                    prim->packed.xy2 = MAP_XY(vc);
                    prim->packed.uv0 = face->uv0;
                    prim->packed.uv1 = face->uv1;
                    prim->packed.uv2 = face->uv2;
                    NormalColorCol((SVECTOR *)(normals + face->normal),
                                   &map_textured_primitive_color, &shade);
                    DpqColor(&shade, va->depth_cue, &prim->packed.color0);
                    DpqColor(&shade, vb->depth_cue, &prim->packed.color1);
                    DpqColor(&shade, vc->depth_cue, &prim->packed.color2);
                    ((u8 *)&prim->sdk.tag)[3] = 9;
                    prim->sdk.code = (header.bytes.mode & 2) | 0x34;
                    depth = (va->sz + vb->sz + vc->sz) / 3 + depth_bias;
                    if (depth < 16) {
                        depth = 16;
                    }
                    AddPrim(&game_graphics_runtime.display_state.ordering_table[depth & 0x1fff],
                            &prim->sdk);
                } else {
                    clipped_count = Clip3FTP(MAP_ORIGINAL_VERTEX(original_vertices, face->vertex0),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex1),
                                             MAP_ORIGINAL_VERTEX(original_vertices, face->vertex2),
                                             (short *)&face->uv0,
                                             (short *)&face->uv1,
                                             (short *)&face->uv2,
                                             game_graphics_runtime.clip_result_vertices);
                    if (clipped_count >= 3) {
                        render_enqueue_clipped_tmd_polygon(clipped_count,
                                       (SVECTOR *)(normals + face->normal),
                                       face->clut, face->tpage,
                                       header.bytes.mode & 2, depth_bias);
                    }
                }
                break;
            }
            }
            packet += header.bytes.input_length * KF_TMD_WORD_BYTES;
        } while (remaining-- != 0);
    }
}

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
    if (state_8017d118.transition_active == 1 && state_8017d118.tmd_object_limit_active &&
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

ADDRESS(0x800311b0, 0x144)
void render_textured_quad(s32 x, s32 y, s32 right, s32 bottom,
                   u8 texture_u, u8 texture_v, u8 texture_width,
                   u8 texture_height, u8 semitrans, u16 tpage,
                   u16 clut, u8 red, u8 green, u8 blue, s32 depth)
{
    POLY_FT4 *quad = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;

    game_graphics_runtime.display_state.primitive_buffer->cursor += sizeof(POLY_FT4);
    if (game_graphics_runtime.display_state.primitive_buffer->cursor >
        game_graphics_runtime.display_state.primitive_buffer->end) {
        return;
    }

    setPolyFT4(quad);
    if (semitrans != 0xff && semitrans != 0) {
        setSemiTrans(quad, 1);
    }
    setRGB0(quad, red, green, blue);
    quad->tpage = tpage;
    quad->clut = clut;
    quad->u0 = texture_u;
    quad->v0 = texture_v;
    quad->u1 = texture_u + texture_width;
    quad->v1 = texture_v;
    quad->u2 = texture_u;
    quad->v2 = texture_v + texture_height;
    quad->u3 = texture_u + texture_width;
    quad->v3 = texture_v + texture_height;
    quad->x0 = quad->x2 = x;
    quad->x1 = quad->x3 = right;
    quad->y0 = quad->y1 = y;
    quad->y2 = quad->y3 = bottom;

    if (depth > 0 && (u32)depth < KF_GAME_ORDERING_TABLE_LENGTH) {
        AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], quad);
    }
}

ADDRESS(0x800312f4, 0x90)
void render_sliding_panel_primary(void)
{
    s32 y = player_state.collision_lower_clearance;

    if (y < 240) {
        y += 120;
        if (y < 0) {
            y = 0;
        }
        render_textured_quad(0, y, 320, 240, 128, 192, 15, 15, 1, 55,
                             0x7bdc, 60, 60, 60, 64);
    }
}

ADDRESS(0x80031384, 0x90)
void render_sliding_panel_secondary(void)
{
    s32 y = player_state.collision_upper_clearance;

    if (y < 240) {
        y += 120;
        if (y < 0) {
            y = 0;
        }
        render_textured_quad(0, y, 320, 240, 144, 192, 15, 15, 1, 55,
                             0x7bdc, 60, 60, 60, 64);
    }
}

ADDRESS(0x80031414, 0xc0)
void render_color_overlay(void)
{
    if (game_graphics_runtime.color_overlay_control != 0xff) {
        render_textured_quad(0, 0, 0x140, 0xf0,
                             0x80, 0xd0, 0xf, 0xf, 1,
                             ((game_graphics_runtime.color_overlay_control & 3) << 5) | 0x17,
                             0x7bdc,
                             game_graphics_runtime.color_overlay_rgb[0],
                             game_graphics_runtime.color_overlay_rgb[1],
                             game_graphics_runtime.color_overlay_rgb[2],
                             (game_graphics_runtime.color_overlay_control & 0x80) ? 1 : 0x40);
    }
}

ADDRESS(0x800314d4, 0x28)
void render_set_color_overlay(u8 control, u8 red, u8 green, u8 blue)
{
    game_graphics_runtime.color_overlay_control = control;
    game_graphics_runtime.color_overlay_rgb[0] = red;
    game_graphics_runtime.color_overlay_rgb[1] = green;
    game_graphics_runtime.color_overlay_rgb[2] = blue;
}

ADDRESS(0x800314fc, 0x138)
void render_accumulated_color_overlay(void)
{
    if (game_graphics_runtime.color_overlay_sample_count != 0) {
        render_textured_quad(0, 0, 0x140, 0xf0,
                      0x80, 0xd0, 0xf, 0xf, 1, 0x37, 0x7bdc,
                      *(s16 *)&game_graphics_runtime.color_overlay_red_sum /
                          game_graphics_runtime.color_overlay_sample_count,
                      *(s16 *)&game_graphics_runtime.color_overlay_green_sum /
                          game_graphics_runtime.color_overlay_sample_count,
                      *(s16 *)&game_graphics_runtime.color_overlay_blue_sum /
                          game_graphics_runtime.color_overlay_sample_count,
                      0x40);
    }
}

ADDRESS(0x80031634, 0x94)
void accumulate_color_overlay(s32 first, s32 second, s32 third, s32 scale)
{
    game_graphics_runtime.color_overlay_sample_count++;
    game_graphics_runtime.color_overlay_red_sum += (first * scale) >> 12;
    game_graphics_runtime.color_overlay_green_sum += (second * scale) >> 12;
    game_graphics_runtime.color_overlay_blue_sum += (third * scale) >> 12;
}

ADDRESS(0x800316c8, 0x188)
void render_player_weapon(void)
{
    s32 cell_x;
    s32 cell_z;
    u8 *layer;
    KfCollisionRow *row;
    KfWeaponRecordGame *weapon;
    KfTmdObject *object;
    MATRIX model;

    if (player_state.weapon_attack_phase == -1) {
        return;
    }

    cell_z = player_state.camera_position.vz >> 11;
    cell_x = player_state.camera_position.vx >> 11;
    layer = &bss_801c7540.map_cells[0][0].layer[0].lighting_index;
    row = &game_graphics_runtime.collision_rows[
        layer[cell_x * sizeof(KfMapOccupancyCell) +
              cell_z * sizeof(bss_801c7540.map_cells[0]) +
              player_state.map_layer_index] & 0x3f];
    SetColorMatrix((MATRIX *)&row->motion);
    SetLightMatrix((MATRIX *)&row->rotations[0]);
    fog_set_near(row->filter.angle);
    SetBackColor(row->filter.kinds.types[0], row->filter.kinds.types[1],
                 row->filter.kinds.types[2]);

    weapon = player_state.equipped_weapon_record;
    model.t[0] = (s16)weapon->position_offset_x;
    model.t[1] = (s16)weapon->position_offset_y;
    model.t[2] = (s16)weapon->position_offset_z;
    RotMatrix((SVECTOR *)&weapon->rotation_offset_x, &model);
    SetRotMatrix(&model);
    SetTransMatrix(&model);
    asset_registry_select(0x20);
    object = tmd_get_object(0);
    if (animation_prepare_asset_vertices(&player_state.weapon_animation_cache, 0x20,
                      player_state.weapon_attack_mode,
                      player_state.weapon_attack_phase,
                      object->vertex_count) != 0) {
        tmd_project_vertices_with_fog(object->vertex_count);
        render_enqueue_textured_tmd(0, 100);
    }
}

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

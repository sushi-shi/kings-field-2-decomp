#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>
#include <kf/lib/math.h>

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

DATA(0x800fba58, 0x32000)
u8 display_primitive_memory[KF_DISPLAY_BUFFER_COUNT * KF_GAME_PRIMITIVE_BUFFER_BYTES];

DATA(0x8017d140, 0x17cf0)
KfGraphicsRuntimeGame game_graphics_runtime;

DATA(0x801d9610, 0x4)
s32 display_frame_cleared_word;

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
void func_8002d4f4(const VECTOR *position, const SVECTOR *rotation)
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

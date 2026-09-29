#ifndef KF_GAME_GRAPHICS_H
#define KF_GAME_GRAPHICS_H

#include <kf/lib/types.h>
#include <kf/game/asset.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>
#include <psyq/sdk.h>

/* GAME.EXE double-buffered display and the graphics runtime region cleared
 * by game_main_loop (0x5f3c words at 0x8017d140). Members are named where a
 * reconstructed function establishes them; the rest stay opaque. */
enum {
    KF_DISPLAY_BUFFER_COUNT = 2,
    KF_GAME_ORDERING_TABLE_LENGTH = 0x2000,
    KF_GAME_PRIMITIVE_BUFFER_BYTES = 0x19000,
    KF_GAME_TMD_SLOT_COUNT = 4,
    /* Extent up to current_tmd_vertices; entry 0x181 is read directly. The
     * original capacity is unproven. */
    KF_ASSET_REGISTRY_EXTENT = 0x240,
    /* Extent from the projection loops' base; the original capacity is unproven. */
    KF_PROJECTED_VERTEX_EXTENT = 1000,
    KF_NOTIFICATION_CAPACITY = 8,
    KF_CLIP_EDGE_COUNT = 16,
    KF_FLOOR_ITEM_CAPACITY = 8
};

typedef struct KfPrimitiveBuffer {
    u8 *start;
    u8 *end;
    u8 *cursor;
} KfPrimitiveBuffer;

typedef struct KfOrderingTable {
    u32 entries[KF_GAME_ORDERING_TABLE_LENGTH];
} KfOrderingTable;

typedef struct KfDisplayState {
    u8 buffer_index;
    u8 unknown_01[3];
    u8 *asset_load_buffer;
    KfPrimitiveBuffer primitive_buffers[KF_DISPLAY_BUFFER_COUNT];
    KfPrimitiveBuffer *primitive_buffer;
    KfOrderingTable ordering_tables[KF_DISPLAY_BUFFER_COUNT];
    u32 *ordering_table;
} KfDisplayState;

typedef struct KfTmdState {
    KfTmdHeader *slots[KF_GAME_TMD_SLOT_COUNT];
    KfTmdHeader *current_asset;
} KfTmdState;

/* Projected or view-space vertex: screen x/y, a depth and the raw z. */
typedef struct KfScreenVertex {
    s16 x;
    s16 y;
    s16 sz;
    s16 p2;
} KfScreenVertex;

typedef struct KfNotificationControl {
    u8 queue_tail;
    u8 queue_head;
    u8 effect_phase;
    u8 hold_frames;
} KfNotificationControl;

typedef struct KfFloorItem {
    u8 kind;
    u8 unknown_01[23];
} KfFloorItem;

typedef struct KfRenderState {
    MATRIX view_matrix;
    MATRIX pitch_matrix;
    MATRIX unknown_matrix_40;
    VECTOR view_position;
    SVECTOR view_rotation;
    s32 view_cell_x;
    s32 view_cell_z;
    u8 unknown_80[0x10];
    s32 fog_near_distance;
} KfRenderState;

typedef struct KfGraphicsRuntimeGame {
    KfDisplayState display_state;
    DRAWENV display_draw_environments[KF_DISPLAY_BUFFER_COUNT];
    DISPENV display_disp_environments[KF_DISPLAY_BUFFER_COUNT];
    KfTmdState tmd_state;
    KfAssetHeader *asset_registry_entries[KF_ASSET_REGISTRY_EXTENT];
    SVECTOR *current_tmd_vertices;
    KfPoolRecord pool_records[KF_ANIMATION_CACHE_CAPACITY];
    KfScreenVertex tmd_projected_vertices[KF_PROJECTED_VERTEX_EXTENT];
    u8 unknown_12a50[0x1f94];
    EVECTOR clip_edges[KF_CLIP_EDGE_COUNT];
    u8 notification_message_ids[KF_NOTIFICATION_CAPACITY];
    u16 notification_payloads[KF_NOTIFICATION_CAPACITY];
    KfNotificationControl notification_control;
    u8 unknown_14cc0;
    u8 unknown_14cc1;
    u8 unknown_14cc2[0x0a];
    u32 frame_counter_a;
    u32 frame_counter_b;
    KfFloorItem floor_items[KF_FLOOR_ITEM_CAPACITY];
    KfRenderState render_state;
    u8 unknown_14e28[0x22c4];
    s32 unknown_170ec;
    u8 unknown_170f0[0xc00];
} KfGraphicsRuntimeGame;

typedef char kf_display_state_size[sizeof(KfDisplayState) == 0x10028 ? 1 : -1];
typedef char kf_render_state_size[sizeof(KfRenderState) == 0x94 ? 1 : -1];
typedef char kf_graphics_runtime_size[sizeof(KfGraphicsRuntimeGame) == 0x17cf0 ? 1 : -1];

extern KfGraphicsRuntimeGame game_graphics_runtime;
/* Primitive memory the display reset splits into the two primitive buffers. */
extern u8 display_primitive_memory[KF_DISPLAY_BUFFER_COUNT * KF_GAME_PRIMITIVE_BUFFER_BYTES];
/* Cleared with the per-frame counters; no other reference is known yet. */
extern s32 display_frame_cleared_word;

enum {
    KF_NOTIFICATION_NONE = 0xff,
    KF_FLOOR_ITEM_NONE = 0xff,
    KF_DISPLAY_BUFFER_NONE = 0xff
};

void fog_set_near(s32 distance);
void display_initialize(void);
void display_reset(void);
void display_begin_frame(void);
void display_present_frame(void);

#endif

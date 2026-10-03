#ifndef KF_GAME_COLLISION_CACHE_H
#define KF_GAME_COLLISION_CACHE_H

#include <kf/game/player.h>

s32 collision_sample_map_cell_layer(s32 x, s32 y, s32 z);
s32 collision_evaluate_shape_records(s32 x, s32 y, s32 z, s32 radius, s32 height);
s32 collision_probe_floor_height(s32 x, s32 y, s32 z, s32 radius, s32 height);
void collision_cache_load_hit_bounds(void);
s32 collision_query_shapes_with_layer_sample(s32 x, s32 y, s32 z, s32 radius, s32 height);
s32 collision_probe_forward_shape_0x20(const VECTOR *position, const struct KfEulerAngles *angles);
s32 collision_query_world(s32 x, s32 y, s32 z, s32 radius, s32 height, s32 mode);
void interpolate_collision_filter_rows(u8 first, u8 second, u8 third, s32 angle, u16 value);

/* collision_query_world uses the same bit positions to request and report
 * actor and map-object checks. Shape records only report bits below 0x10. */
enum {
    KF_COLLISION_QUERY_SHAPES = 0x01,
    KF_COLLISION_QUERY_ACTORS = 0x10,
    KF_COLLISION_QUERY_MAP_OBJECTS = 0x20,
    KF_COLLISION_HIT_AXIS = 0x01,
    KF_COLLISION_HIT_DIAGONAL = 0x02,
    KF_COLLISION_HIT_ACTOR = 0x10,
    KF_COLLISION_HIT_MAP_OBJECT = 0x20
};

/* Phase-one resource loading copies 0x600 words to this interior BSS range.
 * The variable-length shape records within it remain untyped. */
#define KF_COLLISION_SHAPE_BANK \
    ((u8 *)&bss_801c7540 + 0x10000)
#define KF_COLLISION_SHAPE_BANK_BYTES 0x1800

typedef struct KfCollisionShapeOffsetTable {
    u16 offsets[256];
} KfCollisionShapeOffsetTable;

typedef char kf_collision_shape_offset_table_size[
    sizeof(KfCollisionShapeOffsetTable) == 0x200 ? 1 : -1];

/* Temporary interior view of the complete startup-cleared BSS object. The
 * boundary with the provisional equipment-record view is still unresolved. */
#define KF_COLLISION_CACHE_CELL \
    (*(KfMapOccupancyCell **)((u8 *)&bss_801c7540 + 0x11800))
#define KF_COLLISION_CACHE_SHAPE \
    (*(u8 **)((u8 *)&bss_801c7540 + 0x11804))
#define KF_COLLISION_CACHE_LAYER \
    (*(u16 *)((u8 *)&bss_801c7540 + 0x1180a))
#define KF_COLLISION_CACHE_HEIGHT \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x1180c))
#define KF_COLLISION_CACHE_RESULT \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11810))
#define KF_COLLISION_CACHE_HEIGHT_LIMIT \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11814))
/* The cache's complete layout still overlaps the provisional equipment view. */
#define KF_COLLISION_CACHE_LOWER_BOUND \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11818))
#define KF_COLLISION_CACHE_UPPER_BOUND \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x1181c))
#define KF_COLLISION_CACHE_FLAGS \
    (*(u32 *)((u8 *)&bss_801c7540 + 0x11820))
#define KF_COLLISION_CACHE_ACTOR_INDEX \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11824))
#define KF_COLLISION_CACHE_OBJECT_INDEX \
    (*(s32 *)((u8 *)&bss_801c7540 + 0x11828))
#define KF_COLLISION_CACHE_POSITION \
    (*(VECTOR *)((u8 *)&bss_801c7540 + 0x11830))
#define KF_COLLISION_CACHE_RADIUS \
    (*(u16 *)((u8 *)&bss_801c7540 + 0x11840))
#define KF_COLLISION_CACHE_INTERACTION_HEIGHT \
    (*(u16 *)((u8 *)&bss_801c7540 + 0x11842))

#endif

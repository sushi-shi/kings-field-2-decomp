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
    KF_COLLISION_QUERY_ACTORS_INCLUDE_TYPE3 = 0x40,
    KF_COLLISION_QUERY_PLAYER = 0x80,
    KF_COLLISION_HIT_AXIS = 0x01,
    KF_COLLISION_HIT_DIAGONAL = 0x02,
    KF_COLLISION_HIT_FLOOR = 0x04,
    KF_COLLISION_HIT_HEIGHT_LIMIT = 0x08,
    KF_COLLISION_HIT_ACTOR = 0x10,
    KF_COLLISION_HIT_MAP_OBJECT = 0x20,
    KF_COLLISION_HIT_PLAYER = 0x80
};

/* A layer with this lighting byte flag brings the other cell layer into the
 * camera mask when its own map object is present. */
enum { KF_MAP_CELL_LAYER_REVEALS_OTHER_LAYER = 0x80 };

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

/* Interior state of the startup-cleared BSS owner. Its boundary with the
 * provisional equipment-record view remains unresolved. */
typedef struct KfCollisionCache {
    KfMapOccupancyCell *cell;
    KfMapOccupancyLayer *shape;
    u8 unknown_08[2];
    u16 layer;
    s32 height;
    s32 result;
    s32 height_limit;
    s32 lower_bound;
    s32 upper_bound;
    u32 flags;
    s32 actor_index;
    s32 object_index;
    u8 unknown_2c[4];
    VECTOR position;
    u16 radius;
    u16 interaction_height;
} KfCollisionCache;

typedef char kf_collision_cache_size[sizeof(KfCollisionCache) == 0x44 ? 1 : -1];
typedef char kf_collision_cache_height_offset[
    (u32)&((KfCollisionCache *)0)->height == 0x0c ? 1 : -1];
typedef char kf_collision_cache_position_offset[
    (u32)&((KfCollisionCache *)0)->position == 0x30 ? 1 : -1];
typedef char kf_collision_cache_radius_offset[
    (u32)&((KfCollisionCache *)0)->radius == 0x40 ? 1 : -1];

#define KF_COLLISION_CACHE \
    (*(KfCollisionCache *)((u8 *)&bss_801c7540 + 0x11800))
#define KF_COLLISION_CACHE_CELL KF_COLLISION_CACHE.cell
#define KF_COLLISION_CACHE_SHAPE KF_COLLISION_CACHE.shape
#define KF_COLLISION_CACHE_LAYER KF_COLLISION_CACHE.layer
#define KF_COLLISION_CACHE_HEIGHT KF_COLLISION_CACHE.height
#define KF_COLLISION_CACHE_RESULT KF_COLLISION_CACHE.result
#define KF_COLLISION_CACHE_HEIGHT_LIMIT KF_COLLISION_CACHE.height_limit
#define KF_COLLISION_CACHE_LOWER_BOUND KF_COLLISION_CACHE.lower_bound
#define KF_COLLISION_CACHE_UPPER_BOUND KF_COLLISION_CACHE.upper_bound
#define KF_COLLISION_CACHE_FLAGS KF_COLLISION_CACHE.flags
#define KF_COLLISION_CACHE_ACTOR_INDEX KF_COLLISION_CACHE.actor_index
#define KF_COLLISION_CACHE_OBJECT_INDEX KF_COLLISION_CACHE.object_index
#define KF_COLLISION_CACHE_POSITION KF_COLLISION_CACHE.position
#define KF_COLLISION_CACHE_RADIUS KF_COLLISION_CACHE.radius
#define KF_COLLISION_CACHE_INTERACTION_HEIGHT KF_COLLISION_CACHE.interaction_height

#endif

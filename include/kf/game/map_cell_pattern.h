#ifndef KF_GAME_MAP_CELL_PATTERN_H
#define KF_GAME_MAP_CELL_PATTERN_H

#include <kf/lib/types.h>
#include <kf/game/render_types.h>

typedef struct KfMapCellPatternVariant {
    u8 first_collision_shape_id;
    u8 second_collision_shape_id;
    u8 first_object_index;
    u8 second_object_index;
} KfMapCellPatternVariant;

typedef struct KfMapCellPattern {
    KfMapCellPatternVariant variant[2];
    s8 offset_x;
    s8 offset_z;
} KfMapCellPattern;

typedef char kf_map_cell_pattern_size[sizeof(KfMapCellPattern) == 10 ? 1 : -1];
typedef char kf_map_cell_pattern_offset[
    (u32)&((KfMapCellPattern *)0)->offset_x == 8 ? 1 : -1];

enum {
    KF_MAP_OBJECT_PATTERN_GROUPS = 9,
    KF_MAP_OBJECT_PATTERN_ROWS = 3
};

/* map_cell_copy_rotated_fields copies these occupancy-layer components. */
enum {
    KF_MAP_CELL_COPY_OBJECT_INDEX = 0x01,
    KF_MAP_CELL_COPY_ELEVATION = 0x02,
    KF_MAP_CELL_COPY_ROTATED_ORIENTATION = 0x04,
    KF_MAP_CELL_COPY_COLLISION_SHAPE = 0x08,
    KF_MAP_CELL_COPY_LIGHTING_INDEX = 0x10,
    KF_MAP_CELL_COPY_LIGHTING_BIT_40 = 0x20,
    KF_MAP_CELL_COPY_LIGHTING_BIT_80 = 0x40
};

extern KfMapCellPattern map_object_cell_patterns
    [KF_MAP_OBJECT_PATTERN_GROUPS][KF_MAP_OBJECT_PATTERN_ROWS];

void map_cell_apply_rotated_pattern(KfMapLayerMask mode, s32 world_x, s32 world_z, s32 angle,
                   const KfMapCellPattern *patterns, s32 variant_index,
                   s32 layer_flag);
void map_cell_copy_rotated_fields(KfMapLayerMask layer_select, s32 source_x, s32 source_z,
                   s32 destination_x, s32 destination_z, s32 width,
                   s32 height, s32 rotation, u32 field_mask);

#endif

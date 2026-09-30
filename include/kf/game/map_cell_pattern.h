#ifndef KF_GAME_MAP_CELL_PATTERN_H
#define KF_GAME_MAP_CELL_PATTERN_H

#include <kf/lib/types.h>

typedef struct KfMapCellPatternVariant {
    u8 first_unknown_03;
    u8 second_unknown_03;
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

#endif

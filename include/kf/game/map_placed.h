#ifndef KF_GAME_MAP_PLACED_H
#define KF_GAME_MAP_PLACED_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

enum {
    KF_MAP_PLACED_ENTRY_COUNT = 128,
    KF_MAP_PLACED_NONE = 0xffff,
    KF_MAP_PLACED_ASSET_BASE = 0x28,
    KF_MAP_PLACED_CLIP_BASE = 0x80,
    KF_MAP_PLACED_ANIMATION_DISABLED = 0
};

/* Sixteen-byte rows loaded from a map resource section. */
typedef struct KfMapPlacedSource {
    u16 model_index;
    u8 frame_count;
    u8 frame_period;
    u8 layer_mask;
    u8 region_z;
    u8 region_x;
    u8 unknown_07[3];
    s16 local_z;
    s16 local_x;
    s16 height_offset;
} KfMapPlacedSource;

/* Twenty-four-byte runtime rows at game_graphics_runtime +0x170f0. */
typedef struct KfMapPlacedEntry {
    u16 model_index;
    u8 layer_mask;
    u8 frame_count;
    u8 frame_period;
    u8 frame_index;
    u8 unknown_06[2];
    VECTOR position;
} KfMapPlacedEntry;

typedef char kf_map_placed_source_size[sizeof(KfMapPlacedSource) == 16 ? 1 : -1];
typedef char kf_map_placed_entry_size[sizeof(KfMapPlacedEntry) == 24 ? 1 : -1];
#define KF_MAP_PLACED_OFFSET(type, member) ((unsigned long)&((type *)0)->member)
typedef char kf_map_placed_source_height_offset[
    KF_MAP_PLACED_OFFSET(KfMapPlacedSource, height_offset) == 14 ? 1 : -1];
typedef char kf_map_placed_entry_world_x_offset[
    KF_MAP_PLACED_OFFSET(KfMapPlacedEntry, position.vx) == 8 ? 1 : -1];
typedef char kf_map_placed_entry_world_z_offset[
    KF_MAP_PLACED_OFFSET(KfMapPlacedEntry, position.vz) == 16 ? 1 : -1];
#undef KF_MAP_PLACED_OFFSET

void map_placed_expand_sources(const KfMapPlacedSource *sources);

#endif

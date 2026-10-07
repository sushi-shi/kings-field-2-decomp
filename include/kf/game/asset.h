#ifndef KF_GAME_ASSET_H
#define KF_GAME_ASSET_H

#include <kf/lib/offsetof.h>
#include <kf/lib/types.h>
#include <kf/game/pool.h>
#include <kf/game/tmd.h>

enum {
    KF_ASSET_ARCHIVE_HEADER_BYTES = 4,
    KF_ASSET_MIN_REGISTERED_BYTES = 5
};

typedef struct KfAssetHeader {
    u32 byte_size;
    u32 animation_present;
    u32 tmd_data_offset;
    u32 morph_offsets_offset;
    u32 clip_table_offset;
} KfAssetHeader;

typedef char kf_asset_header_size[sizeof(KfAssetHeader) == 20 ? 1 : -1];
typedef char kf_asset_morph_offsets_offset[
    offsetof(KfAssetHeader, morph_offsets_offset) == 0x0c ? 1 : -1];

#define ASSET_BYTES(asset, offset) ((u8 *)(asset) + (offset))
#define ASSET_TMD(asset) ((KfTmdHeader *)ASSET_BYTES(asset, (asset)->tmd_data_offset))
#define ASSET_MORPH_OFFSETS(asset) \
    ((u32 *)ASSET_BYTES(asset, (asset)->morph_offsets_offset))
#define ASSET_CLIP_TABLE(asset) ((u32 *)ASSET_BYTES(asset, (asset)->clip_table_offset))

void asset_registry_load_tmd_archive(u16 first_asset_id, u8 *archive);
void asset_registry_set(u16 index, KfAssetHeader *asset);
void asset_registry_select(u16 index);
KfAssetHeader *resource_registry_get(u16 index);
u32 asset_vertex_count(s32 asset_index, KfAnimationClip clip);

#endif

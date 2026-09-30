#ifndef KF_GAME_ASSET_H
#define KF_GAME_ASSET_H

#include <kf/lib/types.h>

enum {
    KF_ASSET_ARCHIVE_HEADER_BYTES = 4,
    KF_ASSET_MIN_REGISTERED_BYTES = 5
};

/* The archive walker advances by each embedded record's byte length. */
typedef struct KfAssetHeader {
    u32 byte_size;
    u32 animation_present;
    u32 tmd_data_offset;
    u32 unknown_0c;
    u32 clip_table_offset;
} KfAssetHeader;

typedef char kf_asset_header_size[sizeof(KfAssetHeader) == 20 ? 1 : -1];

void asset_registry_load_tmd_archive(u16 first_asset_id, u8 *archive);
void asset_registry_set(u16 index, KfAssetHeader *asset);
void asset_registry_select(u16 index);
KfAssetHeader *resource_registry_get(u16 index);
u32 asset_vertex_count(s32 asset_index, s32 encoded_object_index);

#endif

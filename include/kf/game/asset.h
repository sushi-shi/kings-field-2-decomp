#ifndef KF_GAME_ASSET_H
#define KF_GAME_ASSET_H

#include <kf/lib/types.h>

/* Registered model asset; only the TMD offset is established so far. */
typedef struct KfAssetHeader {
    u8 unknown_00[8];
    u32 tmd_data_offset;
} KfAssetHeader;

void asset_registry_set(u16 index, KfAssetHeader *asset);
void asset_registry_select(u16 index);

#endif

#include <kf/lib/address.h>
#include <kf/game/asset.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>

ADDRESS(0x800339fc, 0xb8)
void asset_registry_load_tmd_archive(u16 first_asset_id, u8 *archive)
{
    u16 count = *(u16 *)archive;

    archive += KF_ASSET_ARCHIVE_HEADER_BYTES;
    while (count-- != 0) {
        KfAssetHeader *asset = (KfAssetHeader *)archive;
        u32 size = asset->byte_size;

        if (size >= KF_ASSET_MIN_REGISTERED_BYTES) {
            game_graphics_runtime.asset_registry_entries[first_asset_id] = asset;
            asset_registry_select(first_asset_id);
            tmd_prepare_primitive_indices(game_graphics_runtime.tmd_state.current_asset);
        }
        archive += size;
        first_asset_id++;
    }
}

ADDRESS(0x80033ab4, 0x48)
void asset_registry_set(u16 index, KfAssetHeader *asset)
{
    game_graphics_runtime.asset_registry_entries[index] = asset;
    asset_registry_select(index);
    tmd_prepare_primitive_indices(game_graphics_runtime.tmd_state.current_asset);
}

ADDRESS(0x80033afc, 0x38)
void asset_registry_select(u16 index)
{
    KfAssetHeader *asset = game_graphics_runtime.asset_registry_entries[index];

    game_graphics_runtime.tmd_state.current_asset =
        (KfTmdHeader *)((u8 *)asset + asset->tmd_data_offset);
}

#include <kf/lib/address.h>
#include <kf/game/asset.h>
#include <kf/game/graphics.h>
#include <kf/game/tmd.h>

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

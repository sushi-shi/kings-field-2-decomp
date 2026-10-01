#include <kf/lib/address.h>
#include <kf/game/asset.h>
#include <kf/game/audio.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>
#include <kf/game/resources.h>
#include <kf/game/tmd.h>

enum { KF_MAP_CELL_SHIFT = 11 };

ADDRESS(0x80031fa0, 0x68)
KfAssetHeader *resource_registry_get(u16 index)
{
    KfAssetHeader *asset = game_graphics_runtime.asset_registry_entries[index];

    if (index < 104) {
        return asset;
    }
    if (asset != 0 && (u8)(memory_block_kind((u8 *)asset) - 1) < 2) {
        return asset;
    }
    return 0;
}

ADDRESS(0x80032008, 0x38)
void resource_tmd_read_complete(u8 *data)
{
    KfAssetHeader *asset = (KfAssetHeader *)data;

    tmd_prepare_primitive_indices((KfTmdHeader *)(data + asset->tmd_data_offset));
    memory_block_set_kind(data, 2);
}

ADDRESS(0x80032040, 0x70)
u32 map_cell_layer_mask(const VECTOR *position)
{
    s32 z = (position->vz >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_z;
    s32 x;

    if ((u32)z >= KF_MAP_CELL_GRID_SIDE) {
        return 0;
    }
    x = (position->vx >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_x;
    if ((u32)x >= KF_MAP_CELL_GRID_SIDE) {
        return 0;
    }
    return game_graphics_runtime.render_grid.map_cell_layer_masks[z][x];
}

ADDRESS(0x800320b0, 0xc4)
u32 map_cell_layer_mask_radius(const VECTOR *position, s32 radius)
{
    s32 span = radius * 2;
    u8 mask = 0;
    s32 z = (position->vz >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_z - radius;
    s32 x0 = (position->vx >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_x - radius;
    s32 row_offset = z * KF_MAP_CELL_GRID_SIDE;
    const u8 *row = &game_graphics_runtime.render_grid.map_cell_layer_masks[0][0] + row_offset;
    s32 row_count = span;

    do {
        if (row_offset >= 0 && (u32)row_offset < sizeof(game_graphics_runtime.render_grid.map_cell_layer_masks)) {
            s32 x = x0;
            s32 column_count = span;
            const u8 *cell = row + x;

            do {
                if (x >= 0 && (u32)x < KF_MAP_CELL_GRID_SIDE) {
                    mask |= *cell;
                }
                cell++;
                x++;
                column_count--;
            } while (column_count != -1);
        }
        row += KF_MAP_CELL_GRID_SIDE;
        row_offset += KF_MAP_CELL_GRID_SIDE;
        row_count--;
    } while (row_count != -1);

    return mask;
}

ADDRESS(0x80032174, 0x64)
s32 map_cell_visible(const VECTOR *position, s32 radius_x, s32 radius_z)
{
    s32 z = position->vz >> KF_MAP_CELL_SHIFT;
    s32 x;
    s32 visible = 0;

    if (game_graphics_runtime.render_state.view_cell_z < z - radius_z) {
        goto done;
    }
    if (z + radius_z < game_graphics_runtime.render_state.view_cell_z) {
        goto done;
    }

    x = position->vx >> KF_MAP_CELL_SHIFT;
    if (game_graphics_runtime.render_state.view_cell_x < x - radius_x) {
        goto done;
    }
    visible = x + radius_x >= game_graphics_runtime.render_state.view_cell_x;

done:
    return visible;
}

ADDRESS(0x800321d8, 0x9c)
void resource_tmd_queue_read(s32 archive_slot, s32 entry, s32 registry_index)
{
    u32 size = cd_archive_entry_size(archive_slot, entry);
    u8 *block;

    block = memory_arena_allocate_block(KF_GAME_RESOURCE_ARENA_BASE, size,
        (u8 **)&game_graphics_runtime.asset_registry_entries[registry_index]);
    if (block != 0) {
        memory_block_set_kind(block, 3);
        memory_block_set_tag(block, registry_index);
        /* Kind 0x10 completion passes the destination, not the request. */
        cd_archive_queue_read(archive_slot, entry, (u_long *)block,
            (KfCdRequestCallback)resource_tmd_read_complete);
    }
}

ADDRESS(0x80032274, 0xf0)
void resource_vab_update_range(s32 archive_slot, s32 entry, s32 vab_slot,
    s32 count, u8 *flags)
{
    s32 end = count + entry;

    while (entry < end) {
        KfAudioVabStreamSlot *state = audio_state.vab_slots[vab_slot].stream_slot;

        if (*flags++) {
            if (state == 0) {
                audio_queue_vab_stream(archive_slot, entry, vab_slot);
            } else if (state->state == 2) {
                state->state = 1;
            }
        } else if (state != 0 && state->state == 1) {
            state->state = 2;
        }
        entry++;
        vab_slot++;
    }
}

ADDRESS(0x80032364, 0x118)
void resource_tmd_update_range(s32 archive_slot, s32 entry, s32 registry_index,
    s32 count, u8 *flags)
{
    s32 end = count + entry;

    while (entry < end) {
        u8 *block;

        if (*flags++) {
            block = (u8 *)game_graphics_runtime.asset_registry_entries[registry_index];
            if (block == 0) {
                resource_tmd_queue_read(archive_slot, entry, registry_index);
            } else if (memory_block_kind(block) == 1) {
                memory_block_set_kind(block, 2);
            }
        } else {
            block = (u8 *)game_graphics_runtime.asset_registry_entries[registry_index];
            if (block != 0 && memory_block_kind(block) != 3) {
                memory_block_set_kind(block, 1);
            }
        }
        entry++;
        registry_index++;
    }
}

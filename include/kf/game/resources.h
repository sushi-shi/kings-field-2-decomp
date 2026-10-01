#ifndef KF_GAME_RESOURCES_H
#define KF_GAME_RESOURCES_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

u32 map_cell_layer_mask(const VECTOR *position);
u32 map_cell_layer_mask_radius(const VECTOR *position, s32 radius);
s32 map_cell_visible(const VECTOR *position, s32 radius_x, s32 radius_z);

void tim_upload_images(u8 *tim_data);
void resource_tmd_read_complete(u8 *data);
void resource_tmd_queue_read(s32 archive_slot, s32 entry, s32 registry_index);
void resource_vab_update_range(s32 archive_slot, s32 entry, s32 vab_slot,
    s32 count, u8 *flags);
void resource_tmd_update_range(s32 archive_slot, s32 entry, s32 registry_index,
    s32 count, u8 *flags);

#endif

#ifndef KF_GAME_RESOURCES_H
#define KF_GAME_RESOURCES_H

#include <kf/lib/types.h>
#include <psyq/sdk.h>

u32 map_cell_layer_mask(const VECTOR *position);
u32 map_cell_layer_mask_radius(const VECTOR *position, s32 radius);
s32 map_cell_visible(const VECTOR *position, s32 radius_x, s32 radius_z);

void func_800160e8(s32 dx, s32 dy, s32 dz);
void func_80016260(u8 first, u8 second, u8 third, u8 fourth,
    u8 fifth, s8 offset_x, s8 offset_z, s8 offset_y);
void func_80016820(void);
void func_80015d58(void);
void func_80015fd4(void);

void tim_upload_images(u8 *tim_data);
void resource_tmd_read_complete(u8 *data);
void resource_tmd_queue_read(s32 archive_slot, s32 entry, s32 registry_index);
void resource_vab_update_range(s32 archive_slot, s32 entry, s32 vab_slot,
    s32 count, u8 *flags);
void resource_tmd_update_range(s32 archive_slot, s32 entry, s32 registry_index,
    s32 count, u8 *flags);

#endif
